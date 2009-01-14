/*
 * The Rage Library
 * Copyright (c) 2007 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */#include "PieceManager.h"
#include <functional>

//#include <iostream>
#include "Storage.h"
namespace Rage {


bool DownloadingPiece::IsCompleted()const
{
		for(SliceRequestList::const_iterator it = m_req_list.begin(); it != m_req_list.end(); ++it)
		{
				if(it->state != COMPLETED) return false;
		}

		return true;

}


bool DownloadingPiece::AbortRequest(const SliceInfo &slice_info)
{
		assert(slice_info.piece_idx == m_piece_idx);

		for(SliceRequestList::iterator it = m_req_list.begin(); it != m_req_list.end(); ++it)
		{
				if(it->state == REQUESTED
						&& it->start_pos == slice_info.start_pos 
						&& it->slice_len == slice_info.slice_len)
						
				{
						
						it->state = FREE;
						return true;
				}
		}
		return false;
}



bool DownloadingPiece::RequestCompleted(const SliceInfo &slice_info)
{
		assert(slice_info.piece_idx == m_piece_idx);

		for(SliceRequestList::iterator it = m_req_list.begin(); it != m_req_list.end(); ++it)
		{
				if(it->state == REQUESTED || it->state == FREE)
				{
					if(it->start_pos == slice_info.start_pos && it->slice_len == slice_info.slice_len)
					{
							it->state = COMPLETED;
								return true;
					}
				}
		}

		return false;
}


bool DownloadingPiece::HasFreeSlice(bool is_endgame)const
{
		for(SliceRequestList::const_iterator it = m_req_list.begin(); it != m_req_list.end(); ++it)
		{
				if(!is_endgame)
				{
						if(it->state == FREE) return true;
				}else
				{
						if(it->state == FREE || it->state == REQUESTED)
						{
								return true;
						}
				}
		}
		return false;
}

bool DownloadingPiece::GetRequest(SliceInfo &slice_info, bool is_endgame)
{
		for(SliceRequestList::iterator it = m_req_list.begin(); it != m_req_list.end(); ++it)
		{
				if(!is_endgame)
				{
						if(it->state == FREE)
						{

								it->state = REQUESTED;
								slice_info.piece_idx = m_piece_idx;
								slice_info.start_pos = it->start_pos;
								slice_info.slice_len = it->slice_len;
								return true;
						}
				}else
				{
						if(it->state == FREE || it->state == REQUESTED)
						{
								it->state = REQUESTED;
								slice_info.piece_idx = m_piece_idx;
								slice_info.start_pos = it->start_pos;
								slice_info.slice_len = it->slice_len;
								return true;
						}
				}
		}

		return false;

}


DownloadingPiece::DownloadingPiece(t_uint32 piece_idx, t_uint32 piece_len, t_uint32 slice_len)
{
		assert(piece_idx != NONE_PIECE_INDEX);
		//assert(piece_len >= (32*KB));
		assert(slice_len == SLICE_LENGTH);
		reset(piece_idx, piece_len, slice_len);
}


DownloadingPiece::~DownloadingPiece()
{


}

void DownloadingPiece::reset(t_uint32 piece_idx, t_uint32 piece_len, t_uint32 slice_len)
{
		m_piece_idx = piece_idx;
		m_piece_len = piece_len;
		m_slice_len = slice_len;
		m_req_list.clear();
		
		t_uint32 left_len = piece_len;

		while(left_len > 0)
		{
				t_uint32 request_len;
				if(left_len > slice_len)
				{
						request_len = slice_len;
				}else
				{
						request_len = left_len;
				}
				RequestInfo req;

				req.state = FREE;
				req.start_pos = piece_len - left_len;
				req.slice_len = request_len;

				m_req_list.push_back(req);
				left_len -= request_len;
		}
}




bool DownloadingPiece::NotRequested()const
{
		for(SliceRequestList::const_iterator it = m_req_list.begin(); it != m_req_list.end(); ++it)
		{
				if(it->state != FREE) return false;
		}
		return true;
}


////////////////////////////PieceManager///////////////////////////////////////



PieceManager::PieceManager(const BitField &bit_field, const TorrentFile &tf, Storage &storage)
				:m_bitfield(bit_field)
				, m_torrent_file(tf)
				, m_storage(storage)
				, m_is_endgame(false)
{
		assert(m_storage.IsOpen());
		assert(bit_field.NBits() == tf.GetPieceCount());
}

PieceManager::~PieceManager()
{


}




void PieceManager::ResetPiecesInfo()
{
		m_is_endgame = false;
		assert(m_storage.IsOpen());
		for(StartedPiecesMap::iterator it = m_started_pieces.begin(); it != m_started_pieces.end(); ++it)
		{
				delete it->second;
		}

		m_started_pieces.clear();


		assert(m_torrent_file.GetPieceCount() == m_bitfield.NBits());
		assert(m_torrent_file.GetPieceCount() > 0);
		
		m_pieces_info.clear();
		m_pieces_info.reserve(m_torrent_file.GetPieceCount());

		m_pos_in_interests.clear();
		m_pos_in_interests.reserve(m_torrent_file.GetPieceCount());


		for(size_t i = 0; i < m_torrent_file.GetPieceCount(); ++i)
		{
				m_pieces_info.push_back(PieceInfo(P_FREE, 0));
				m_pos_in_interests.push_back(NONE_POS);
		}
		
		
		assert(m_pieces_info.size() == m_bitfield.NBits());
		

		m_interests.clear();
		m_interests.push_back(std::vector<t_uint32>(0));
		std::vector<t_uint32> &pieces_list = m_interests[0];

		for(size_t i = 0; i < m_pieces_info.size(); ++i)
		{
				if(m_bitfield.IsSet(i))
				{
						m_pieces_info[i].state = P_DONE;
				}else
				{
						pieces_list.push_back(i);
				}
		}

		srand(TimeSpace::CycleTimer::GetCycleCount());
		std::random_shuffle(pieces_list.begin(), pieces_list.end());
		
		for(size_t i = 0; i < pieces_list.size(); ++i)
		{
				m_pos_in_interests[pieces_list[i]] = i;
		}

		if(need_enter_endgame()) 
		{
				m_is_endgame = true;
		}


}


void PieceManager::IncreaseHave(const BitField &remote_bitf)
{
		if(remote_bitf.NBits() != m_bitfield.NBits())
		{
				assert(0);
				return;
		}
		
		for(size_t i = 0; i < remote_bitf.NBits(); ++i)
		{
				if(remote_bitf.IsSet(i))
				{
						IncreaseHave(i);
				}
		}
}

void PieceManager::DecreaseHave(const BitField &remote_bitf)
{

		for(size_t i = 0; i < remote_bitf.NBits(); ++i)
		{
				if(remote_bitf.IsSet(i))
				{
						DecreaseHave(i);
				}
		}

}

void PieceManager::DecreaseHave(t_uint32 piece_idx)
{
		if(piece_idx >= m_torrent_file.GetPieceCount())
		{
				assert(0);
				return;
		}
		
		t_uint32 num_have = m_pieces_info[piece_idx].have;
		if(num_have == 0) return;
		m_pieces_info[piece_idx].have -= 1;
		
		if(m_pieces_info[piece_idx].state != P_DONE)
		{
				shift_over(piece_idx, m_interests[num_have], m_interests[num_have - 1]);
		}

}


void PieceManager::IncreaseHave(t_uint32 piece_idx)
{
		//printf("in PieceManager::IncreaseHave\n");
		if(piece_idx >= m_torrent_file.GetPieceCount())
		{
				assert(0);
				return;
		}
		
		t_uint32 num_have = m_pieces_info[piece_idx].have;
		
		m_pieces_info[piece_idx].have += 1;
		
		if(m_pieces_info[piece_idx].state != P_DONE)
		{
				if(num_have == (m_interests.size() - 1))
				{
						m_interests.push_back(std::vector<t_uint32>(0));
				}
				shift_over(piece_idx, m_interests[num_have], m_interests[num_have+1]);
		}

		//printf("m_interests[0].size() == %d\n", m_interests[0].size());
}

void PieceManager::shift_over(t_uint32 piece_idx, std::vector<t_uint32> &sour, std::vector<t_uint32> &dest)
{
		t_uint32 pos = m_pos_in_interests[piece_idx];
		//printf("pos == %d    sour[pos] == %d\n", pos, sour[pos]);
		assert(pos != NONE_POS);
		assert(sour[pos] == piece_idx);
		
		if(pos == sour.size() - 1)
		{
				sour.erase(sour.end() - 1);
		}else
		{
				sour[pos] = sour[sour.size() - 1];
				m_pos_in_interests[sour[pos]] = pos;
				sour.erase(sour.end() - 1);
		}

		dest.push_back(piece_idx);

		m_pos_in_interests[piece_idx] = (dest.size() - 1);
}



#if(0)

bool PieceManager::get_req_piece(t_uint32 &piece_idx, const BitField &remote_own_pieces)
{
		for(size_t i = 1; i < m_interests.size(); ++i)
		{
				for(size_t j = 0; j < m_interests[i].size(); ++j)
				{
						t_uint32 pidx = m_interests[i][j];
						
						if(remote_own_pieces.IsSet(pidx) && m_pieces_info[pidx].state == P_FREE)
						{
								piece_idx = pidx;
								return true;
						}
				}
		}
		
		return false;
}

#endif


#if(0)
bool PieceManager::get_req_in_downloading(SliceInfo &slice_info, const BitField &remote_own_pieces)
{
		t_uint32 best_pidx = NONE_PIECE_INDEX;
		t_uint32 best_num = 0;
		
		DownloadingPiece *pdown = 0;

		for(StartedPiecesMap::iterator it = m_started_pieces.begin(); it != m_started_pieces.end(); ++it)
		{
				t_uint32 pidx = it->second->GetPieceIdx();
				assert(pidx < m_bitfield.NBits());
				assert(m_pieces_info[pidx].state == P_DOWNLOADING);
				
				if(remote_own_pieces.IsSet(pidx) && it->second->HasFreeSlice())
				{
						if(m_pieces_info[pidx].have > best_num)
						{
								best_pidx = pidx;
								best_num = m_pieces_info[pidx].have;
								pdown = it->second;
						}
				}
		}

		if(pdown != 0)
		{
				
				assert(best_num > 0);
				bool res = pdown->GetRequest(slice_info);
				assert(res);
				return true;
		}else
		{
				return false;
		}
}


bool PieceManager::GetRequest(SliceInfo &slice_info, const BitField &remote_own_pieces)
{
		//printf("PieceManager::GetRequest\n");
		if(get_req_in_downloading(slice_info, remote_own_pieces))
		{
				return true;
		}
		
		t_uint32 piece_idx = NONE_PIECE_INDEX;
		//printf("get_req_in_downloading faled\n");

		if(!get_req_piece(piece_idx, remote_own_pieces))
		{
				//printf("get_req_piece faled\n");
				return false;
		}else
		{
				//printf("get_req_piece success get piece idx == %d\n", piece_idx);
				assert(m_pieces_info[piece_idx].state == P_FREE);
				m_pieces_info[piece_idx].state = P_DOWNLOADING;
				DownloadingPiece *pdown = new DownloadingPiece(piece_idx, m_torrent_file.GetPieceLength(piece_idx), SLICE_LENGTH);
				assert(m_started_pieces.count(piece_idx) == 0);
				m_started_pieces[piece_idx] = pdown;
				bool res = pdown->GetRequest(slice_info);
				assert(res);
				return true;
		}
}
#endif

/*
bool PieceManager::get_req_piece(t_uint32 &piece_idx, const BitField &remote_own_pieces)
{
		for(size_t i = m_interests.size(); i > 1; --i)
		{
				for(size_t j = 0; j < m_interests[i-1].size(); ++j)
				{
						t_uint32 pidx = m_interests[i-1][j];
						
						if(remote_own_pieces.IsSet(pidx) && m_pieces_info[pidx].state == P_FREE)
						{
								piece_idx = pidx;
								return true;
						}
				}
		}
		
		return false;
}
*/

bool PieceManager::get_req_piece_mostly(t_uint32 &piece_idx, const BitField &remote_own_pieces)
{
		for(size_t i = m_interests.size(); i > 1; --i)
		{
				for(size_t j = 0; j < m_interests[i-1].size(); ++j)
				{
						t_uint32 pidx = m_interests[i-1][j];
						
						if(remote_own_pieces.IsSet(pidx) && m_pieces_info[pidx].state == P_FREE)
						{
								piece_idx = pidx;
								return true;
						}
				}
		}
		
		return false;
}

bool PieceManager::get_req_piece_rarely(t_uint32 &piece_idx, const BitField &remote_own_pieces)
{
		for(size_t i = 1; i < m_interests.size(); ++i)
		{
				for(size_t j = 0; j < m_interests[i].size(); ++j)
				{
						t_uint32 pidx = m_interests[i][j];
						
						if(remote_own_pieces.IsSet(pidx) && m_pieces_info[pidx].state == P_FREE)
						{
								piece_idx = pidx;
								return true;
						}
				}
		}
		
		return false;
}

bool PieceManager::get_req_piece(t_uint32 &piece_idx, const BitField &remote_own_pieces)
{
		if(GetFinishedDegree() < 0.05f)
		{
				return get_req_piece_mostly(piece_idx, remote_own_pieces);
		}else
		{
				return get_req_piece_rarely(piece_idx, remote_own_pieces);
		}
}

////////////////////////////////new/////////////////////////////////////////////
DownloadingPiece* PieceManager::get_best_downloading(const BitField &remote_own_pieces)
{
		t_uint32 best_pidx = NONE_PIECE_INDEX;
		t_uint32 best_num = 0;
		
		DownloadingPiece *pdown = 0;

		for(StartedPiecesMap::iterator it = m_started_pieces.begin(); it != m_started_pieces.end(); ++it)
		{
				t_uint32 pidx = it->second->GetPieceIdx();
				assert(pidx < m_bitfield.NBits());
				assert(m_pieces_info[pidx].state == P_DOWNLOADING);
				
				if(remote_own_pieces.IsSet(pidx) && it->second->HasFreeSlice(IsEndGame()))
				{
						if(m_pieces_info[pidx].have > best_num)
						{
								best_pidx = pidx;
								best_num = m_pieces_info[pidx].have;
								pdown = it->second;
						}
				}
		}
		
		if(pdown != 0)
		{
				assert(best_num > 0);
				return pdown;
		}
		
		t_uint32 piece_idx = NONE_PIECE_INDEX;
		if(!get_req_piece(piece_idx, remote_own_pieces))
		{
				return 0;
		}else
		{
				assert(m_pieces_info[piece_idx].state == P_FREE);
				m_pieces_info[piece_idx].state = P_DOWNLOADING;
				DownloadingPiece *pd = new DownloadingPiece(piece_idx, m_torrent_file.GetPieceLength(piece_idx), SLICE_LENGTH);
				assert(m_started_pieces.count(piece_idx) == 0);
				m_started_pieces[piece_idx] = pd;
				return pd;
		}
}

bool PieceManager::GetRequest(std::list<SliceInfo> &slice_list, t_uint32 want_num, const BitField &remote_own_pieces)
{
		assert(want_num > 0);
		slice_list.clear();
		DownloadingPiece* pdown = get_best_downloading(remote_own_pieces);
		if(pdown == 0)
		{
				DEBUG_PRINT0("get_best_downloading failed\n");
				return false;
		}else
		{
				for(size_t i = 0; i < want_num; ++i)
				{		
						SliceInfo si;
						if(!pdown->GetRequest(si, IsEndGame()))
						{
								DEBUG_PRINT1("pdown->GetRequest(si) failed slist.size() == %d\n", slice_list.size());
								break;
						}else
						{
								DEBUG_PRINT1("slice_list.size() == %d\n", slice_list.size());
								slice_list.push_back(si);
						}
				}
				return (slice_list.size() > 0);
		}
}



///////////////////////////////////////////////////////////////////////////////

#if(0)
bool PieceManager::need_enter_endgame()
{

		double finished_degree = (double)GetFinishedSize() / (double)m_torrent_file.GetTotalSize();

		if(finished_degree >= END_GAME_LINE)
		{
				return true;
		}else
		{
				return false;
		}
}
#endif

bool PieceManager::need_enter_endgame()
{
		if(IsEndGame())return false;

		size_t un_finished = 0;

		for(size_t idx = 0; idx < m_bitfield.NBits(); ++idx)
		{
				if(!m_bitfield.IsSet(idx))
				{
						if(++un_finished >= END_GAME_LINE)
						{
								return false;
						}
				}
		}

		return true;
}


bool PieceManager::SliceCompleted(const SliceInfo &slice_info, const t_byte *pbuf)
{
		StartedPiecesMap::iterator it = m_started_pieces.find(slice_info.piece_idx);

		if(it == m_started_pieces.end()) return false;

		DownloadingPiece *pdown = it->second;
		if(!pdown->RequestCompleted(slice_info))
		{
				return false;
		}else
		{
				WriteData(pbuf, slice_info.slice_len, slice_info.piece_idx, slice_info.start_pos);
		}

		if(pdown->IsCompleted())
		{
				if(CheckPiece(slice_info.piece_idx))
				{
						t_uint32 pidx = slice_info.piece_idx;
						assert(m_pieces_info[pidx].state == P_DOWNLOADING);
						m_pieces_info[pidx].state = P_DONE;

						t_uint32 pos = m_pos_in_interests[pidx];

						assert(pos != NONE_POS);
						m_pos_in_interests[pidx] = NONE_POS;
						std::vector<t_uint32> &piece_list = m_interests[m_pieces_info[pidx].have];
						assert(!piece_list.empty());

						if(pos == piece_list.size() - 1)
						{
								piece_list.erase(piece_list.end() - 1);
						}else
						{
								piece_list[pos] = piece_list[piece_list.size() - 1];
								m_pos_in_interests[piece_list[pos]] = pos;
								piece_list.erase(piece_list.end() - 1);
						}
						m_bitfield.Set(pidx);

						//确认此片写入磁盘中(因为此片也许会在cache中)
						m_storage.FlushPiece(pidx);


						if(!IsEndGame() && need_enter_endgame()) 
						{
								m_is_endgame = true;
								DEBUG_PRINT0("ENTER END GAME MODE!!!!!!!!!!!!!!!\n");
								
						}
				}else
				{
						DEBUG_PRINT1("Piece index == %d hash check failed\n", slice_info.piece_idx);
						t_uint32 pidx = slice_info.piece_idx;
						assert(m_pieces_info[pidx].state == P_DOWNLOADING);
						m_pieces_info[pidx].state = P_FREE;
				}

				m_started_pieces.erase(it);
				delete pdown;

		}
		return true;




}

#if(0)

bool PieceManager::SliceCompleted(const SliceInfo &slice_info)
{
		StartedPiecesMap::iterator it = m_started_pieces.find(slice_info.piece_idx);
		
		if(it == m_started_pieces.end()) return false;

		DownloadingPiece *pdown = it->second;
		if(!pdown->RequestCompleted(slice_info)) return false;

		if(pdown->IsCompleted())
		{
				if(CheckPiece(slice_info.piece_idx))
				{
						t_uint32 pidx = slice_info.piece_idx;
						assert(m_pieces_info[pidx].state == P_DOWNLOADING);
						m_pieces_info[pidx].state = P_DONE;

						t_uint32 pos = m_pos_in_interests[pidx];
						
						assert(pos != NONE_POS);
						m_pos_in_interests[pidx] = NONE_POS;
						std::vector<t_uint32> &piece_list = m_interests[m_pieces_info[pidx].have];
						assert(!piece_list.empty());

						if(pos == piece_list.size() - 1)
						{
								piece_list.erase(piece_list.end() - 1);
						}else
						{
								piece_list[pos] = piece_list[piece_list.size() - 1];
								m_pos_in_interests[piece_list[pos]] = pos;
								piece_list.erase(piece_list.end() - 1);
						}
						m_bitfield.Set(pidx);
						
						//确认此片写入磁盘中(因为此片也许会在cache中)
						m_storage.FlushPiece(pidx);

						if(!m_is_endgame && need_enter_endgame()) 
						{
								m_is_endgame = true;
								DEBUG_PRINT0("ENTER END GAME MODE!!!!!!!!!!!!!!!\n");
						}
				}else
				{
						DEBUG_PRINT1("Piece index == %d hash check failed\n", slice_info.piece_idx);
						::MessageBox(0, TEXT("Piece index == hash check failed\n"), 0,0);
						t_uint32 pidx = slice_info.piece_idx;
						assert(m_pieces_info[pidx].state == P_DOWNLOADING);
						m_pieces_info[pidx].state = P_FREE;
				}

				m_started_pieces.erase(it);
				delete pdown;
						
		}
		return true;
}
#endif



bool PieceManager::AbortRequest(const SliceInfo &slice_info)
{
		StartedPiecesMap::iterator it = m_started_pieces.find(slice_info.piece_idx);
		
		if(it == m_started_pieces.end()) return false;

		if(!it->second->AbortRequest(slice_info)) return false;

		if(it->second->NotRequested())
		{
				t_uint32 pidx = slice_info.piece_idx;
				assert(m_pieces_info[pidx].state == P_DOWNLOADING);
				m_pieces_info[pidx].state = P_FREE;
				delete it->second;
				m_started_pieces.erase(it);
		}

		return true;
}




bool PieceManager::PieceIsCompleted(t_uint32 piece_idx)const
{

		if(piece_idx >= m_bitfield.NBits())
		{
				assert(0); 
				return false; 
		}

		if(m_pieces_info[piece_idx].state == P_DONE)
		{
				assert(m_bitfield.IsSet(piece_idx));
				return true;
		}else
		{
				assert(!m_bitfield.IsSet(piece_idx));
				return false;
		}
}



bool PieceManager::IsFinished()const
{
		return m_bitfield.IsAllSet();
}

const BitField& PieceManager::GetBitField()const
{
		return m_bitfield;

}



bool PieceManager::WriteData(const t_byte *buf, t_uint32 len, t_uint32 piece_idx, t_uint32 offset)
{
		return m_storage.WriteData(buf, len, piece_idx, offset);
}
		
bool PieceManager::ReadData(t_byte *buf, t_uint32 len, t_uint32 piece_idx, t_uint32 offset)const
{
		return m_storage.ReadData(buf, len, piece_idx, offset);

}


bool PieceManager::WritePiece(const t_byte *buf, t_uint32 piece_idx)
{
		return m_storage.WritePiece(buf, piece_idx);
}

bool PieceManager::ReadPiece(t_byte *buf, t_uint32 piece_idx)const
{
		return m_storage.ReadPiece(buf, piece_idx);
}

bool PieceManager::CheckPiece(t_uint32 piece_idx)const
{
		return m_storage.CheckPiece(piece_idx);

}


t_uint32 PieceManager::GetPieceLength(t_uint32 piece_idx)const
{

		return m_torrent_file.GetPieceLength(piece_idx);


}



t_uint64 PieceManager::GetFinishedSize()const
{
		size_t piece_count = m_bitfield.NBits();
		t_uint64 fin_len = 0;
		
		for(size_t i = 0; i < piece_count; ++i)
		{
				if(m_bitfield.IsSet(i))
				{
						fin_len += m_torrent_file.GetPieceLength(i);
				}
		}

		return fin_len;

}

t_uint64 PieceManager::GetLeftSize()const
{

		return m_torrent_file.GetTotalSize() - GetFinishedSize();


}


bool PieceManager::IsEndGame()const
{
		return m_is_endgame;
}



float PieceManager::GetFinishedDegree()const
{

		return (float)GetFinishedSize() / (float)m_torrent_file.GetTotalSize();

}







}

