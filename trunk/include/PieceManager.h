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

		//�������Ƭ����һ����ÿ��ֵ��¼ÿ��Ƭ�ж��ٸ�have����״̬
		std::vector<PieceInfo>						m_pieces_info;

		//�洢����δ������ص�Ƭ����������Ϊ��Ƭӵ�е�have����
		std::vector<std::vector<t_uint32> >			m_interests;
		
		//�洢δ��ɵ�piece��m_interests�е�λ��
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
		
		void ResetPiecesInfo();//������δ��õ�Ƭ�ε�have����0
public:
		//�������Լ��еĺ�remote�е�ѡ��ĳһƬ����Ƭ��
		//ѡ����һ����Ƭ���������false����Ϊ�޿��������Ƭ��
		//bool GetRequest(SliceInfo &slice_info, const BitField &remote_own_pieces); 

		//�˺�����ĳһ��Ƭ���ʱ������peerlink���ã����ߴ�manager����Ƭ�����
		bool SliceCompleted(const SliceInfo &slice_info, const t_byte *pbuf);
		
		//�˺����ڵ���������ĳһƬ֮�󣬴������ж��ˣ���δ��ɵ��������·Ż���
		//�ύ���п��ܽ�������peer��������
		bool AbortRequest(const SliceInfo &slice_info);

		//�������Ӧ���ǵ�ĳһ��Ƭrequest�õ����ټ�����Ƭ���ڵ�piece�Ƿ��Ѿ�OK�������
		//��peerlink����onpiece_completed������֪ͨ����peer�Ѿ��õ�ĳһƬ
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
		//�������Ƭ����һ����ÿ��ֵ��¼ÿ��Ƭ�ж��ٸ�have����״̬
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

		void ResetPiecesInfo();//������δ��õ�Ƭ�ε�have����0
public:
		//�������Լ��еĺ�remote�е�ѡ��ĳһƬ����Ƭ��
		//ѡ����һ����Ƭ���������false����Ϊ�޿��������Ƭ��
		bool GetRequest(SliceInfo &slice_info, const BitField &remote_own_pieces); 

		//�˺�����ĳһ��Ƭ���ʱ������peerlink���ã����ߴ�manager����Ƭ�����
		bool SliceCompleted(const SliceInfo &slice_info);
		
		//�˺����ڵ���������ĳһƬ֮�󣬴������ж��ˣ���δ��ɵ��������·Ż���
		//�ύ���п��ܽ�������peer��������
		bool AbortRequest(const SliceInfo &slice_info);

		//�������Ӧ���ǵ�ĳһ��Ƭrequest�õ����ټ�����Ƭ���ڵ�piece�Ƿ��Ѿ�OK�������
		//��peerlink����onpiece_completed������֪ͨ����peer�Ѿ��õ�ĳһƬ
		bool PieceIsCompleted(t_uint32 piece_idx)const;

public:
		bool IsFinished()const;

		const BitField& GetBitField()const;

		Storage& GetStorage();
};




#endif





}

