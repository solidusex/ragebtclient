
/*
 * The Rage Library
 * Copyright (c) 2007 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  It is provided "as is" without express
 * or implied warranty.
 */

#include "Setting.h"
#include "BencodingDecoder.h"
#include "BencodingEncoder.h"


namespace Rage {




void GlobalSetting::SetMaxCacheSize(size_t size)
{
		LockT lock(m_mutex, false);
		m_max_cache_size = size;

}

size_t GlobalSetting::GetMaxCacheSize()const
{
		LockT lock(m_mutex, true);
		return m_max_cache_size;

}

t_uint16 GlobalSetting::GetListenPort()const
{
		LockT lock(m_mutex, true);
		return m_listen_port;
}

void GlobalSetting::SetListenPort(t_uint16 port)
{
		LockT lock(m_mutex, false);
		m_listen_port = port;
}


		
t_uint32 GlobalSetting::GetMaxDownSpeed()const
{
		LockT lock(m_mutex, true);
		return m_max_down_speed;
}

void  GlobalSetting::SetMaxDownSpeed(t_uint32 max_down)
{
		LockT lock(m_mutex, false);
		m_max_down_speed = max_down;
}


t_uint32 GlobalSetting::GetMaxUpSpeed()const
{
		LockT lock(m_mutex, true);
		return m_max_up_speed;
}

void  GlobalSetting::SetMaxUpSpeed(t_uint32 max_up)
{
		LockT lock(m_mutex, false);
		m_max_up_speed = max_up;
}

std::wstring GlobalSetting::GetDefaultSavePath()const
{
		LockT lock(m_mutex, true);
		return m_def_save_path;
}

		
void GlobalSetting::SetDefaultSavePath(const std::wstring &path)
{
		LockT lock(m_mutex, false);
		if(FileSysSpace::Path(path).GetPathType() == FileSysSpace::PATH_DIR)
		{
				m_def_save_path = path;
		}
}



t_uint32 GlobalSetting::GetMaxRequestPendingCount()const
{
		LockT lock(m_mutex, true);
		return m_max_request_pending_count;
}

void     GlobalSetting::SetMaxRequestPendingCount(t_uint32 count)
{
		LockT lock(m_mutex, false);
		if(count < 5)
		{
				count = 5;
		}

		m_max_request_pending_count = count;
}


t_uint32 GlobalSetting::GetMaxUploadFromLocalPeerCount()const
{
		LockT lock(m_mutex, true);
		return m_max_upload_from_local_peer_count;

}

void GlobalSetting::SetMaxUploadFromLocalPeerCount(t_uint32 count)
{
		LockT lock(m_mutex, false);
		if(count < 5)
		{
				count = 5;
		}

		m_max_upload_from_local_peer_count = count;
}


t_uint32 GlobalSetting::GetMaxConnectionPerTask()const
{
		LockT lock(m_mutex, true);
		return m_max_connection_count_per_task;
}

void GlobalSetting::SetMaxConnectionPerTask(t_uint32 max_conn)
{
		LockT lock(m_mutex, false);
		if(max_conn < 20)
		{
				max_conn = 20;
		}

		m_max_connection_count_per_task = max_conn;
}




GlobalSetting::GlobalSetting(const std::wstring &path_name) 
				: m_listen_port(0)
				, m_max_down_speed(~0)
				, m_max_up_speed(~0)
				, m_max_request_pending_count(5)
				, m_max_upload_from_local_peer_count(10)
				, m_max_connection_count_per_task(40)
				, m_max_cache_size(8*MB)
				, m_def_save_path(L"")
{
		using namespace FileSysSpace;

		if(!m_file.Open(path_name, F_READ_WRITE |F_OPEN_ALWAYS |F_SHARED_READ))
		{
				throw ExceptionSpace::FileException("not create || open config file");
		}

		if(m_file.GetLength() > 2*MB)
		{
				m_file.SetLength(0);
		}

		Load();
}

GlobalSetting::~GlobalSetting()
{
		try{
				m_file.Close();
		}catch(...)
		{
				
		}

}


/////////////////////////////////////////////serialization//////////////////////



#define KEY_SETTING		"Configuration"	//对应一个dict
		#define			LISTEN_PORT							"ListenPort"		//int
		#define			MAX_DOWN_RATE						"MaxDownRate"		//int
		#define			MAX_UP_RATE							"MaxUpRate"			//int
		#define			MAX_UPLOAD_FROM_LOCAL_PEER_COUNT	"MaxUploadFromLocalPeerCount"//int
		#define			MAX_REQ_PENDING_COUNT				"MaxReqPendingCount"		//int
		#define			MAX_CONNECTION_PER_TASK				"MaxConnectionPerTask"		//int
		#define			MAX_CACHE_SIZE						"MaxCacheSize"				//int
		#define			DEF_SAVE_PATH						"DefSavePath"				//string


#define CHECK_AND_RET(cond) if(!(cond)) return



void GlobalSetting::Load()
{
		assert(m_file.IsOpen());

		

		t_uint64 len = m_file.GetLength();
		if(len == 0)
		{
				return;
		}


		BencodingDecoder bdc;
		
		std::vector<t_byte> buf(len);
		m_file.SeekToBeg();
		t_ulong rn = m_file.Read(&buf[0], buf.size());
		assert(rn == len);
		
		

		CHECK_AND_RET(bdc.SetContent(buf));
		
		


		DictType dict;
		CHECK_AND_RET(bdc.ReadDict(0, dict) > 0);


		t_uint16				listen_port;
		std::wstring			def_save_path;

		t_uint32				max_down_speed;
		t_uint32				max_up_speed;

		t_uint32				max_request_pending_count;
		t_uint32				max_upload_from_local_peer_count;
		t_uint32				max_connection_count_per_task; //最大连接数
		size_t					max_cache_size;
		
		try{
				DictType *pset = TypeCast<DictType>(dict.get_value(KEY_SETTING));

				listen_port = TypeCast<IntegerType>(pset->get_value(LISTEN_PORT))->get_int();
				max_connection_count_per_task = TypeCast<IntegerType>(pset->get_value(MAX_CONNECTION_PER_TASK))->get_int();
				max_down_speed = TypeCast<IntegerType>(pset->get_value(MAX_DOWN_RATE))->get_int();
				max_up_speed =  TypeCast<IntegerType>(pset->get_value(MAX_UP_RATE))->get_int();
				max_request_pending_count = TypeCast<IntegerType>(pset->get_value(MAX_REQ_PENDING_COUNT))->get_int();
				max_upload_from_local_peer_count = TypeCast<IntegerType>(pset->get_value(MAX_UPLOAD_FROM_LOCAL_PEER_COUNT))->get_int();
				max_cache_size = TypeCast<IntegerType>(pset->get_value(MAX_CACHE_SIZE))->get_int();
				def_save_path = FormatSpace::UTF8Converter::ToUnicode(TypeCast<StringType>(pset->get_value(DEF_SAVE_PATH))->get_str());
				
				CHECK_AND_RET(FileSysSpace::Path(def_save_path).GetPathType() == FileSysSpace::PATH_DIR);
	
		}catch(...)
		{

				return;
		}


		LockT lock(m_mutex, false);

		m_listen_port = listen_port;
		m_def_save_path = def_save_path;

		m_max_down_speed = max_down_speed;
		m_max_up_speed = max_up_speed;

		m_max_request_pending_count = max_request_pending_count;
		m_max_upload_from_local_peer_count = max_upload_from_local_peer_count;
		m_max_connection_count_per_task =		max_connection_count_per_task;
		m_max_cache_size			=		max_cache_size;


}




void GlobalSetting::Save()const
{
		assert(m_file.IsOpen());
		
		DictType resume_data;

		{
				LockT lock(m_mutex, true);



				DictType *psetting = new DictType();

				psetting->set_value(LISTEN_PORT, new IntegerType(m_listen_port));
				psetting->set_value(MAX_DOWN_RATE, new IntegerType(this->m_max_down_speed));
				psetting->set_value(MAX_UP_RATE, new IntegerType(m_max_up_speed));
				psetting->set_value(MAX_UPLOAD_FROM_LOCAL_PEER_COUNT, new IntegerType(m_max_upload_from_local_peer_count));
				psetting->set_value(MAX_REQ_PENDING_COUNT, new IntegerType(m_max_request_pending_count));
				psetting->set_value(MAX_CONNECTION_PER_TASK, new IntegerType(m_max_connection_count_per_task));

				psetting->set_value(MAX_CACHE_SIZE, new IntegerType(m_max_cache_size));

				std::string save_path_utf8 = FormatSpace::UnicodeConverter::ToUTF8(m_def_save_path);
				psetting->set_value(DEF_SAVE_PATH, new StringType(save_path_utf8));


				resume_data.set_value(KEY_SETTING, psetting);

		}

		BencodingEncoder bec;
		
		if(!bec.GenContent(resume_data)) return;

		m_file.SetLength(0);

		m_file.SeekToBeg();

		m_file.Write(bec.Content(), bec.Size());
}


}