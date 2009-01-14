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
 */#pragma once

#include "TypeDef.h"

#include "TorrentFile.h"


namespace Rage {


class Storage;

struct SliceInfo
{
		t_uint32 piece_idx;
		t_uint32 start_pos;
		t_uint32 slice_len;
};


class RageAPI DownloadingPiece : NonCopyable
{
private:
		enum SliceState
		{
				FREE			= 0x400A,
				REQUESTED		= 0x400B,
				COMPLETED		= 0x400C
		};

		struct RequestInfo
		{
				SliceState		state;
				t_uint32		start_pos;
				t_uint32		slice_len;
		};
private:
		//typedef std::list<RequestInfo>			SliceRequestList;
		typedef std::vector<RequestInfo>			SliceRequestList;
private:
		t_uint32								m_piece_idx;
		t_uint32								m_piece_len;
		t_uint32								m_slice_len;
		SliceRequestList						m_req_list;
private:
		void reset(t_uint32 piece_idx, t_uint32 piece_len, t_uint32 slice_len);
public:
		DownloadingPiece(t_uint32 piece_idx, t_uint32 piece_len, t_uint32 slice_len);
		
		~DownloadingPiece();
public:
		t_uint32 GetPieceIdx()const { return m_piece_idx; }
		
		t_uint32 GetPieceLength()const { return m_piece_len; }
		
		t_uint32 GetSliceLength()const { return m_slice_len; }
public:
		bool GetRequest(SliceInfo &slice_info, bool is_endgame);

		
		
		bool RequestCompleted(const SliceInfo &slice_info);
		
		bool AbortRequest(const SliceInfo &slice_info);

		bool IsCompleted()const;

		bool HasFreeSlice(bool is_endgame)const;

		bool NotRequested()const;
};



#define NONE_POS  (~0)

//#define END_GAME_LINE	(0.98500f)

#define END_GAME_LINE	(3)

class RageAPI PieceManager : NonCopyable
{
private:
		typedef std::map<t_uint32, DownloadingPiece*>		StartedPiecesMap;
		
		enum PieceState
		{
				P_FREE			= 0x500A,
				P_DOWNLOADING	= 0x500B,
				P_DONE			= 0x500C,

		};

		struct PieceInfo
		{
				PieceState		state;
				t_uint32		have;
				
				PieceInfo() : state(P_FREE), have(0)
				{

				}

				PieceInfo(PieceState _state, t_uint32 _have) : state(_state), have(_have)
				{

				}
		};
private:
		const TorrentFile				&m_torrent_file;
		Storage							&m_storage;
private:
		BitField						m_bitfield;
		StartedPiecesMap				m_started_pieces;
		bool							m_is_endgame;
private:

		//此数组和片段数一样，每个值记录每个片有多少个have及其状态
		std::vector<PieceInfo>						m_pieces_info;

		//存储所有未完成下载的片，数据索引为该片拥有的have个数
		std::vector<std::vector<t_uint32> >			m_interests;
		
		//存储未完成的piece在m_interests中的位置
		std::vector<t_uint32>						m_pos_in_interests;

public:
		PieceManager(const BitField &bit_field, const TorrentFile &tf, Storage &storage);

		~PieceManager();

private:
		void shift_over(t_uint32 piece_idx, std::vector<t_uint32> &pieces_a, std::vector<t_uint32> &pieces_b);
		
		bool get_req_in_downloading(SliceInfo &slice_info, const BitField &remote_own_pieces); 

public:
		bool get_req_piece(t_uint32 &piece_idx, const BitField &remote_own_pieces);

		bool get_req_piece_rarely(t_uint32 &piece_idx, const BitField &remote_own_pieces);
		
		bool get_req_piece_mostly(t_uint32 &piece_idx, const BitField &remote_own_pieces);
public:
				
		bool need_enter_endgame();
public:
		void IncreaseHave(t_uint32 piece_idx);

		void DecreaseHave(t_uint32 piece_idx);

		void IncreaseHave(const BitField &remote_bitf);

		void DecreaseHave(const BitField &remote_bitf);
		
		void ResetPiecesInfo();//将所有未获得的片段的have数清0
public:
		//根据我自己有的和remote有的选出某一片的子片来
		//选出来一个子片，如果返回false，则为无可请求的子片了
		//bool GetRequest(SliceInfo &slice_info, const BitField &remote_own_pieces); 

		//此函数当某一子片完成时，会由peerlink调用，告诉此manager此子片完成了
		bool SliceCompleted(const SliceInfo &slice_info, const t_byte *pbuf);
		
		//此函数在当我请求了某一片之后，此链接中断了，则将未完成的请求重新放回来
		//会交给有可能交给其它peer继续下载
		bool AbortRequest(const SliceInfo &slice_info);

		//这个函数应该是当某一子片request得到后，再检测此子片所在的piece是否已经OK，如果是
		//由peerlink调用onpiece_completed函数，通知所有peer已经得到某一片
		bool PieceIsCompleted(t_uint32 piece_idx)const;

private:
		DownloadingPiece* get_best_downloading(const BitField &remote_own_pieces);

public:
		bool GetRequest(std::list<SliceInfo> &slice_info, t_uint32 want_num, const BitField &remote_own_pieces);
public:
		bool WriteData(const t_byte *buf, t_uint32 len, t_uint32 piece_idx, t_uint32 offset);
		
		bool ReadData(t_byte *buf, t_uint32 len, t_uint32 piece_idx, t_uint32 offset)const;
		
		bool WritePiece(const t_byte *buf, t_uint32 piece_idx);

		bool ReadPiece(t_byte *buf, t_uint32 piece_idx)const;

		bool CheckPiece(t_uint32 piece_idx)const;
public:

		bool IsEndGame()const;

		bool IsFinished()const;

		const BitField& GetBitField()const;

		//Storage& GetStorage();

		t_uint32 GetPieceLength(t_uint32 piece_idx)const;

		t_uint64 GetFinishedSize()const;

		t_uint64 GetLeftSize()const;

		float GetFinishedDegree()const;
};






#if(0)

class RageAPI PieceManager : NonCopyable
{
private:
		typedef std::map<t_uint32, DownloadingPiece*>		StartedPiecesMap;
		
		enum PieceState
		{
				P_FREE			= 0x500A,
				P_DOWNLOADING	= 0x500B,
				P_DONE			= 0x500C,

		};
		struct PieceInfo
		{
				PieceState		state;
				t_uint32		have;
				
				PieceInfo() : state(P_FREE), have(0)
				{

				}

				PieceInfo(PieceState _state, t_uint32 _have) : state(_state), have(_have)
				{

				}
		};

		
		struct LargerThan
		{
				bool operator()(const PieceInfo &a, const PieceInfo &b)const
				{
						return (a.have > b.have);
				}

		};

private:
		const TorrentFile				&m_torrent_file;
		Storage							&m_storage;
private:
		BitField						m_bitfield;
		StartedPiecesMap				m_started_pieces;
private:
		//此数组和片段数一样，每个值记录每个片有多少个have及其状态
		std::vector<PieceInfo>						m_pieces_info;
public:
		PieceManager(const BitField &bit_field, const TorrentFile &tf, Storage &storage);

		~PieceManager();

private:
		bool get_req_in_downloading(SliceInfo &slice_info, const BitField &remote_own_pieces); 

		bool get_req_piece(t_uint32 &piece_idx, const BitField &remote_own_pieces);
public:
		void IncreaseHave(t_uint32 piece_idx);

		void DecreaseHave(t_uint32 piece_idx);
		
		void IncreaseHave(const BitField &remote_bitf);
		void DecreaseHave(const BitField &remote_bitf);

		void ResetPiecesInfo();//将所有未获得的片段的have数清0
public:
		//根据我自己有的和remote有的选出某一片的子片来
		//选出来一个子片，如果返回false，则为无可请求的子片了
		bool GetRequest(SliceInfo &slice_info, const BitField &remote_own_pieces); 

		//此函数当某一子片完成时，会由peerlink调用，告诉此manager此子片完成了
		bool SliceCompleted(const SliceInfo &slice_info);
		
		//此函数在当我请求了某一片之后，此链接中断了，则将未完成的请求重新放回来
		//会交给有可能交给其它peer继续下载
		bool AbortRequest(const SliceInfo &slice_info);

		//这个函数应该是当某一子片request得到后，再检测此子片所在的piece是否已经OK，如果是
		//由peerlink调用onpiece_completed函数，通知所有peer已经得到某一片
		bool PieceIsCompleted(t_uint32 piece_idx)const;

public:
		bool IsFinished()const;

		const BitField& GetBitField()const;

		Storage& GetStorage();
};




#endif





}

