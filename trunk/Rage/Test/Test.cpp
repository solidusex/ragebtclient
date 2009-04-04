

#if !defined(_TEST_USE_LIB)

#define RAGE_SHARED_LIB

#endif

#include "Rage.h"
#pragma comment(lib, "Rage.lib")
#pragma comment(lib, "Foundation.lib")
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

const std::wstring TORRENT_PATH = L"D:\\Programming\\Solidus\\Importance\\Rage\\torrent\\1.torrent";






Rage::DownloaderManager d_m;


int main()
{
#if(0)

		bool res = d_m.Init(L"D:\\Programming\\Solidus\\Importance\\Rage\\resume.dat",
							L"D:\\Programming\\Solidus\\Importance\\Rage\\config.dat", 
							L"D:\\Programming\\Solidus\\Importance\\Rage\\torrent_save\\"
							);
		assert(res);
		
		printf("INIT dcount == %d\n", d_m.GetDownloaderList().size());

		
		::MessageBoxW(0, d_m.GetGlobalSetting().GetDefaultSavePath().c_str(), 0, 0);
		
		
		

				


		
		Rage::Downloader *pd = (*d_m.GetDownloaderList().begin());

		assert(pd != NULL);
		
		::MessageBoxW(0, pd->GetSavePath().c_str(), 0, 0);
		::MessageBoxW(0, pd->GetTorrentFile().GetTorrentName().c_str(), 0, 0);
		
		
		


		d_m.UnInit();
#endif

		Rage::TorrentFile tf;
		bool res = tf.Load(TORRENT_PATH);
		
		assert(res);

		Rage::BitField tmp(tf.GetPieceCount());
		
		srand(time(0));
		for(size_t i = 0; i < tmp.NBits(); ++i)
				tmp.Set(::rand() % 2);
		
		assert(!tmp.IsAllSet());

		size_t nfiles = tf.GetFileCount();
		
		printf("Nfiles == %d\n", nfiles);

		Rage::FileStatusList files_status;

		files_status.reserve(nfiles);

		for(size_t idx = 0; idx < nfiles; ++idx)
		{
				t_uint32 beg = 0, end = 0;
				bool res = tf.MapFileToPieces(idx, beg, end);
				assert(res);
				
				size_t own_count = 0;

				for(size_t i = beg; i < end + 1; ++i)
				{
						if(tmp.IsSet(i))
						{
								own_count++;
						}
				}
				
				printf("own count == %d\n", own_count);
				printf("real count == %d\n",  end + 1 - beg);
				
				
				Rage::FileStatus fstatus;
				fstatus.fidx = idx;
				fstatus.finished_degree = (float)own_count / (end + 1 - beg);
				
				files_status.push_back(fstatus);
		}

		for(Rage::FileStatusList::iterator it = files_status.begin(); it != files_status.end(); ++it)
		{
				cout << it->finished_degree << endl;
		}
		
		
		

		printf("done\n");
		cin.get();
		return 0;
}


#if(0)

int  main()
{
		Rage::ResumeInfo rinfo(L".\\resume.dat");

		rinfo.Load();

		rinfo.Save();

		
		printf("done\n");
		cin.get();
		return 0;
}

#endif










#if(0)

Rage::ResourceManager	res_m;
Rage::ServiceManager	sm;
Rage::GlobalSetting		global_set;
Rage::PeerAcceptor		peer_acceptor(sm, res_m);





int main()
{
		bool res = sm.Start();
		global_set.SetMaxUploadFromLocalPeerCount(20);
		global_set.SetMaxRequestPendingCount(5);
		global_set.SetMaxConnectionPerTask(200);
		global_set.SetDefaultSavePath(L"I:\\1\\");
		assert(res);
		
		res = peer_acceptor.Start(9512);
		
		global_set.SetListenPort(peer_acceptor.GetListenPort());
		assert(res);
		
		
		

		
		Rage::TorrentFile tf;

		res = tf.Load(TORRENT_PATH);

		assert(res);
		

		
		
		Rage::BitField bitf(tf.GetPieceCount());

		Rage::Downloader downloader(global_set, sm, res_m);

		res = downloader.Open(TORRENT_PATH, L"L:\\1\\");
		assert(res);
		
		res = downloader.StartHashChecing();
		assert(res);



		while(downloader.GetDownloaderState() == Rage::DS_HASH_CHECKING)
		{
				Rage::HashingStatus status = downloader.GetHashStatus();
				
				printf("hash degree == %f\n", status.finished_degree);
				
				cin.get();

				if(status.is_done)
				{
						break;
				}

				
		}

		if(downloader.GetDownloaderState() == Rage::DS_ABORT)
		{
				::MessageBoxA(0, "Abort", 0, 0);
		}

		downloader.StopHashChecking();
		
		downloader.Close();








		peer_acceptor.Stop();
		sm.Stop();
		
		
		
		printf("done\n");
		cin.get();
		cin.get();
		return 0;

}





int main()
{
		bool res = sm.Start();
		global_set.SetMaxUploadFromLocalPeerCount(20);
		global_set.SetMaxRequestPendingCount(5);
		global_set.SetMaxConnectionPerTask(200);
		global_set.SetDefaultSavePath(L"I:\\1\\");
		assert(res);
		
		res = peer_acceptor.Start(8512);
		
		global_set.SetListenPort(peer_acceptor.GetListenPort());
		assert(res);
		
		
		

		
		Rage::TorrentFile tf;

		res = tf.Load(TORRENT_PATH);

		assert(res);
		

		
		
		Rage::BitField bitf(tf.GetPieceCount());

		Rage::Downloader downloader(global_set, sm, res_m);

		res = downloader.Open(TORRENT_PATH, L"i:\\1\\");
		assert(res);
		
		
		res = downloader.StartDownload();
		assert(res);

		while(downloader.GetDownloaderState() == Rage::DS_RUNNING)
		{
				Rage::DownStatus dstatus = downloader.GetDownStatus();

				printf("unused peers num == %d\n",dstatus.num_unused_peer);
				printf("peer num == %d\n", dstatus.num_peer);
				printf("seed num == %d\n", dstatus.num_seed);
				printf("down rate == %f\n", dstatus.down_rate);
				printf("up rate == %f\n", dstatus.up_rate);
				printf("finished degree == %f\n", dstatus.finished_degree);
				printf("remain time == %I64u\n", dstatus.remain_time);

				cin.get();
		}


		if(downloader.GetDownloaderState() == Rage::DS_ABORT)
		{
				MessageBoxA(0, "DS_ABORT", 0, 0);
		}

		downloader.StopDownload();




		downloader.Close();








		peer_acceptor.Stop();
		sm.Stop();
		
		
		
		printf("done\n");
		cin.get();
		cin.get();
		return 0;

}



#endif











