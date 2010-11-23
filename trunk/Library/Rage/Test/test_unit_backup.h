#if(0)


#if(0)
Rage::TaskID			task_id = INVALID_TASK_ID;
Rage::TorrentFile		torrent_file;
Rage::PeerID			peer_id;
Rage::ResourceManager	res_m;
Rage::ServiceManager	sm;
Rage::PeerAcceptor		peer_acceptor(sm, res_m);


class ConnInValidatorTest
{
private:
		typedef ThreadSpace::Thread		ThreadT;
		//typedef NetSpace::Acceptor		AcceptorT;
		//typedef NetSpace::SockStream	SockStreamT;
private:
		bool					m_is_started;
private:
		//AcceptorT				m_acceptor;
		ThreadT					*m_res_check_thread;
		//ThreadT					*m_listen_thread;
private:
		void result_check_thread()
		{
				using namespace Rage;

				using namespace MsgSpace;
				using namespace NetSpace;
				
				task_id = ThreadT::GetCurrentThreadId();

				MsgContext context;
				
				
				
				while(m_is_started)
				{
						Rage::NotifyEvent *pnotify  = (NotifyEvent*)context.GetMessageEx();
				
						switch(pnotify->GetType())
						{
						case NET_EVENT:
						{
								NetEvent *pnet_event = (NetEvent*)pnotify;
								switch(pnet_event->GetNetEventType())
								{
								case NET_IO_EVENT:
								{
										NetIOEvent *pio = (NetIOEvent*)pnet_event;
										printf("recv data size == %d\n", pio->GetBufferLength());
										break;

								}
								case NET_CONN_ESTABLISHED:
								{
										printf("validator ok\n");
										NetConnEstablished *pconn_estab = (NetConnEstablished*)pnet_event;
										InetAddress addr;
										addr.SetAddr(pconn_estab->GetPeerEntry().addr);
										addr.SetPort(pconn_estab->GetPeerEntry().port);
										
										t_byte buf[20];
										memset(buf, 11, 20);
										PeerID pid(buf);

										assert(pid == pconn_estab->GetPeerID());

										printf("peer entry == %s:%d\n", addr.Str().c_str(), addr.Port());
										NetSpace::NetHandle  sock_hdl = NetSpace::NetSock::Duplicate(pconn_estab->GetNetHandle());
										
										Rage::PeerIO *pio = new Rage::PeerIO(sock_hdl, task_id, pconn_estab->GetPeerEntry());
										sm.GetNetService().RegisterHandler(pio);
										pio->Release();
										break;
								}
								case NET_CONN_ABORT:
								{
										printf("NET_CONN_ABORT connection abort\n");
										break;
								}
								case NET_CONNECT_REAMOTE_FAILED:
								{
										printf("NET_CONNECT_REAMOTE_FAILED connect failed\n");
										break;
								}
								default:
										assert(0);
								}
								break;

						}
						case TRACKER_EVENT:
						{
								assert(0);
								break;
						}
						case TIMER_EVENT:
						{
								assert(0);
								break;
						}
						case USER_EVENT:
						{
								printf("user event\n");
								break;
						}
						default:
								assert(0);
						}
				}

		}
public:
		bool Start()
		{
				m_res_check_thread = new ThreadT(this, &ConnInValidatorTest::result_check_thread);
				bool res = peer_acceptor.Start(8512);
				printf("listen port == %d\n", peer_acceptor.GetListenPort());
				return res;
		}

		void Stop()
		{
				assert(peer_acceptor.IsStarted());
				peer_acceptor.Stop();
		}
};



static const t_byte PROTOCOL_LENGTH = 19;
static const  char* PROTOCOL_STRING = "BitTorrent protocol";
static const t_byte RESERVED_8BYTES[8] = {0,0,0,0,0,0,0,0 };

using namespace Rage;

//Peer_id 为返回对方peer_id;

bool parse_handshake_info(const t_byte* pbuf, Sha1Hash &info_hash, PeerID &peer_id)
{
		assert(pbuf != 0);

		if(pbuf[0] != PROTOCOL_LENGTH)
		{
				DEBUG_PRINT0("invalid protocol length\n");
				return false;
		}

		if(strcmp((const char*)&pbuf[1], PROTOCOL_STRING) != 0)
		{
				DEBUG_PRINT0("invalid protocol string\n");
				return false;
		}
		
		info_hash.Reset(&pbuf[28]);
		
		peer_id.Reset(&pbuf[48]);
		
		return true;
}


std::vector<t_byte> build_hand_shake(const Sha1Hash &info_hash, const PeerID &local_peer_id)
{
		std::vector<t_byte>  buf(68, 0);
		buf[0] = PROTOCOL_LENGTH;
		memcpy(&buf[1], PROTOCOL_STRING, (t_uint32)PROTOCOL_LENGTH);
		memcpy(&buf[28], info_hash.GetRawData(), 20);
		memcpy(&buf[48], local_peer_id.GetRawData(), 20);
		return buf;
}

void conn_in_test(const NetSpace::InetAddress &addr)
{
		NetSpace::SockStream sock;

		bool res = sock.Open();
		assert(res);
		int rn = ::connect(sock.GetHandle(), addr.Saddr(), addr.Length());

		assert(rn == 0);
		
		t_byte buf[68];
		memset(buf, 11, 20);

		PeerID pid(buf);
		
		//parse_handshake_info(buf, torrent_file.GetInfoHash(), pid);
		
		SimpleBuffer h_buf;

		h_buf.Insert(&build_hand_shake(torrent_file.GetInfoHash(), pid)[0], 68);
		//h_buf.Insert(&build_hand_shake(pid, pid)[0], 68);
		assert(h_buf.Size() == 68);
		//size_t wn = sock.Send();
		while(h_buf.Size() > 0)
		{
				int sn = sock.Send(h_buf.Data(), 1);
				assert(sn == 1);
				h_buf.Erase(1);
				//cin.get();

		}
		
		t_uint32 send_len = 0;
		h_buf.Clear();
		{
				t_uint32 s = htonl(send_len);

				h_buf.Insert((t_byte*)&s, 4);
				h_buf.Allocate(send_len);
				sock.Send(h_buf.Data(), h_buf.Size());
		}

		while(std::cin >> send_len)
		{
				h_buf.Clear();
				if(send_len == 8000)
				{
						break;
				}
				t_uint32 s = htonl(send_len);

				h_buf.Insert((t_byte*)&s, 4);
				h_buf.Allocate(send_len);
				while(!h_buf.IsEmpty())
				{
						sock.Send(h_buf.Data(), 1);
						h_buf.Erase(1);
						::Sleep(0);
				}
				
		}

		sock.Close();

		printf("done\n");
}

int main()
{
		printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
		t_byte buf[20];
		buf[10] = 15;

		peer_id.Reset(buf);

		bool res = torrent_file.Load(TORRENT_PATH);
		cout << torrent_file.GetInfoHash().ToHexString() << endl;
		assert(res);
		sm.Start();
		
		printf("xxxxxxxxx\n");
		ConnInValidatorTest cit;
		res = cit.Start();
		assert(res);
		cin.get();

		res = res_m.GetDownloaderRecorder().RegisterRecord(torrent_file.GetInfoHash(), peer_id, task_id);

		cin.get();

		
		
		NetSpace::InetAddress addr("192.168.2.2", 8512);
		
		conn_in_test(addr);
		cin.get();

		printf("done\n");
		cin.get();
		return 0;

}
#endif

#if(0)
int main()
{
		Rage::TorrentFile tf;

		tf.Load(TORRENT_PATH);
		
		cout << "Total size == " << tf.GetTotalSize()  << endl;

		
		Rage::Storage storage(L"D:\\1\\", tf);
		
		Rage::Storage storage_dest(L"i:\\1\\test\\", tf);
		
		Rage::BitField bitf(tf.GetPieceCount());

		bitf.Flip();

		bool res = storage.Open(bitf);
		if(!res)
		{
				printf("open storage failed\n");
				exit(-1);
		}

		assert(res);
		
		bitf.Flip();
		res = storage_dest.Open(bitf);
		assert(res);
		if(!res)
		{
				printf("open storage dest failed\n");
				exit(-1);
		}
		
		std::vector<t_uint32> v_p_set(tf.GetPieceCount());

		printf("xxxxxxxxx\n");
		for(size_t i = 0; i < tf.GetPieceCount(); ++i)
		{
				res = storage_dest.GetNewPieceTask(storage.GetBitfield(), v_p_set[i]);
				if(!res)
				{
						printf("failed\n");
						exit(0);
				}
		}
		
		printf("xxxxxxxxx\n");

		Rage::PieceRequest preq;
		
		printf("is beg read\n");
		cin.get();
		for(size_t i = 0; i < v_p_set.size(); ++i)
		{
				cout << v_p_set[i] << endl;
				
				preq.Reset(v_p_set[i], tf.GetPieceLength(v_p_set[i]), SLICE_LENGTH);
		
				while(!preq.IsCompleted())
				{
						t_uint32 offset;
						t_uint32 len;
						bool res = preq.GetRequest(offset, len);
						
						if(!res){ printf("get request failed\n"); exit(-1); }

						std::vector<t_byte> buf(len);
						printf("read slice\n");
						if(!storage.ReadData(&buf[0], len, preq.GetPieceIndex(), offset))
						{
								printf(" read file failed\n");
								exit(-1);
						}
						printf("wirte slice\n");
						storage_dest.WriteData(&buf[0], len, preq.GetPieceIndex(), offset);

						preq.RequestCompleted(offset, len);
				}

				if(storage_dest.CheckPiece(preq.GetPieceIndex()))
				{

						printf("piece idx == %d completed\n", preq.GetPieceIndex());
						storage_dest.MarkPieceFinished(preq.GetPieceIndex());
				}else
				{
						printf("piece idx == %d failed\n", preq.GetPieceIndex());
						exit(-1);
				}
		}
		
		if(storage_dest.IsFinished())
		{
				printf("storage_dest is finished\n");
		}else
		{
				printf("storage dest failed\n");
				exit(-1);
		}
		

		
		printf("done\n");
		cin.get();

		return 0;
}


#endif





// Test.cpp : Defines the entry point for the console application.
//

Rage::TorrentFile tf;

		bool res = tf.Load(TORRENT_PATH);

		Rage::DownloadingPiece pd(0, tf.GetPieceLength(0), 16*KB);
		
		Rage::SliceInfo si;

		res = pd.GetRequest(si);
		assert(res);

		si.piece_idx = pd.GetPieceIdx();
		cout << "pd == " << pd.GetPieceIdx() << endl;
		pd.AbortRequest(si);

		assert(pd.NotRequested());

		cin.get();

		assert(res);

		//printf("1105 len == %d\n", tf.GetPieceLength(1105));

		Rage::Storage dest(L"D:\\", tf);
		

		
		res == dest.Open();
		assert(res);
		Rage::BitField bitf(tf.GetPieceCount());
		
		Rage::PieceManager piece_m(bitf, tf, dest);

		piece_m.ResetPiecesInfo();

		
		Rage::BitField remote_bitf1(tf.GetPieceCount());

		Rage::BitField remote_bitf2(tf.GetPieceCount());
		

		for(size_t i = 0;  i < tf.GetPieceCount(); ++i)
		{
				if((i % 2) != 0)
				{
						remote_bitf1.Set(i);
				}else
				{
						remote_bitf2.Set(i);
				}
		}

		Rage::BitField test = remote_bitf1 | remote_bitf2;

		assert(test.IsAllSet());
		assert((remote_bitf1&remote_bitf2).IsEmpty());

		DWORD beg = ::GetTickCount();

		piece_m.IncreaseHave(remote_bitf1);
		piece_m.IncreaseHave(remote_bitf1);
		piece_m.IncreaseHave(remote_bitf2);
		
		piece_m.IncreaseHave(1105);
		DWORD end = ::GetTickCount();
		
		printf("elapsed time == %d\n", end - beg);
		
		//printf("piece_m. == %d\n", piece_m.m_interests[3].size());
		//printf("piece_m. == %d\n", piece_m.m_interests[3][1]);
		//cin.get();
		
		Rage::Storage sour(L"D:\\1\\", tf);
		res = sour.Open();
		assert(res);
		Rage::Storage sour2(L"D:\\2\\", tf);
		res = sour2.Open();
		assert(res);
		
		Rage::SliceInfo slice_info;
		
		while(piece_m.GetRequest(slice_info, remote_bitf1))
		{
				
				//printf("slice len == %d\n", slice_info.slice_len);
				//printf("slice pos == %d\n", slice_info.start_pos);
				//cin.get();


				std::vector<t_byte> buf(slice_info.slice_len);
				sour.ReadData(&buf[0], slice_info.slice_len, slice_info.piece_idx, slice_info.start_pos);
				piece_m.WriteData(&buf[0], slice_info.slice_len, slice_info.piece_idx, slice_info.start_pos);
				res = piece_m.SliceCompleted(slice_info);
				assert(res);

				if(piece_m.PieceIsCompleted(slice_info.piece_idx))
				{
						printf("piece idx == %d done\n", slice_info.piece_idx);
				}
		}

		while(piece_m.GetRequest(slice_info, remote_bitf2))
		{
				std::vector<t_byte> buf(slice_info.slice_len);
				sour2.ReadData(&buf[0], slice_info.slice_len, slice_info.piece_idx, slice_info.start_pos);
				piece_m.WriteData(&buf[0], slice_info.slice_len, slice_info.piece_idx, slice_info.start_pos);
				res = piece_m.SliceCompleted(slice_info);
				assert(res);

				if(piece_m.PieceIsCompleted(slice_info.piece_idx))
				{
						printf("piece idx == %d done\n", slice_info.piece_idx);
				}
		}



		assert(piece_m.IsFinished());


#define RAGE_SHARED_LIB
#include "Rage.h"
#pragma comment(lib, "Rage.lib")

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

const std::wstring TORRENT_PATH 
		= L"H:\\Programming\\Importance\\Rage\\torrent\\10.torrent";


int main()
{
		::setlocale(LC_ALL, "chs");

		Rage::TorrentFile  tf;
		
		DWORD beg = ::GetTickCount();
		
		bool res = tf.Load(TORRENT_PATH);
		
		DWORD end = ::GetTickCount();

		printf("elapsed == %d\n", end - beg);
		
		for(size_t i = 0; i < tf.GetFileCount(); ++i)
		{
				wprintf(L"%s\n", tf.GetFileInfo(i).path.c_str());
		}

		for(size_t i = 0; i < tf.GetAnnounceCount(); ++i)
		{
				printf("%s\n", tf.GetAnnounce(i).c_str());
		}

		for(size_t i = 0; i < tf.GetNodeCount(); ++i)
		{
				NetSpace::InetAddress addr;
				addr.SetAddr(tf.GetNode(i).addr);
				addr.SetPort(tf.GetNode(i).port);

				printf("%s : %d\n", addr.Str().c_str(), addr.Port());
		}

		/*for(size_t i = 0; i < tf.GetPieceCount(); ++i)
		{
				Rage::Sha1Hash hash = tf.GetPieceHash(i);
				cout << hash.ToHexString() << endl;
		}*/


		cout << "piece count == " << tf.GetPieceCount() << endl;
		cout << "piece length == " << tf.GetPieceLength() << endl;
		cout << "total length == " << tf.GetTotalSize() << endl;
		
		wprintf(L"comment == %s\n", tf.GetComment().c_str());
		wprintf(L"creator == %s\n", tf.GetCreator().c_str());
		cout << "creation date == " << tf.GetCreationDate() << endl;
		
		return 0;
}




#endif