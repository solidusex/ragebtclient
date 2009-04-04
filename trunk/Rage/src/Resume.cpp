#include "Resume.h"
#include "BencodingDecoder.h"
#include "BencodingEncoder.h"

namespace Rage {




void ResumeInfo::ClearTaskInfo()
{
		m_task_map.clear();



}
		
bool ResumeInfo::IsExisted(const Sha1Hash &infohash)const
{
		TaskInfoMap::const_iterator it = m_task_map.find(infohash);
		return it != m_task_map.end();
}

bool ResumeInfo::Add(const ResumeTaskInfo &tinfo)
{
		TaskInfoMap::const_iterator it = m_task_map.find(tinfo.infohash);

		if(it == m_task_map.end())
		{
				m_task_map[tinfo.infohash] = tinfo;
				return true;
		}else
		{
				return false;
		}
}

bool ResumeInfo::Remove(const Sha1Hash &infohash)
{

		TaskInfoMap::iterator it = m_task_map.find(infohash);

		if(it != m_task_map.end())
		{
				m_task_map.erase(it);
				return true;
		}else
		{
				return false;
		}

}


bool ResumeInfo::Update(const ResumeTaskInfo &tinfo)//如果还没增加进来，则返回false
{
		
		TaskInfoMap::iterator it = m_task_map.find(tinfo.infohash);

		if(it != m_task_map.end())
		{
				assert(tinfo.infohash == it->first);
				it->second = tinfo;
				return true;
		}else
		{
				return false;
		}
}













ResumeInfo::CIterator ResumeInfo::Begin()const
{
		return m_task_map.begin();

}

ResumeInfo::CIterator ResumeInfo::End()const
{

		return m_task_map.end();
}



using namespace FileSysSpace;

ResumeInfo::ResumeInfo(const std::wstring &file_path)  : m_info_file_path(file_path)
{
		
		if(!m_file.Open(file_path, F_READ_WRITE |F_OPEN_ALWAYS |F_SHARED_READ))
		{
				throw ExceptionSpace::FileException("not create || open resume file");
		}

		if(m_file.GetLength() > 2*MB)
		{
				m_file.SetLength(0);
		}

		Load();
}


ResumeInfo::~ResumeInfo()
{


}



#define KEY_TASK		"Task"	//对应一个每个项都是dict的链表
		#define			INFOHASH					"Infohash"	//STRING
		#define			OWN_PIECES					"OwnPieces" //STRING
		#define			SAVE_PATH					"SavePath"	//STRING
		#define			TORRENT_FILE_PATH			"TorrentPath"//STRING



#define CHECK_AND_RET(cond) do { if(!(cond)) {return;}  }while(0)


void ResumeInfo::Load()
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
		
		
		try{
				ListType *ptinfo = TypeCast<ListType>(dict.get_value(KEY_TASK));
				
				for(ListType::C_Iterator cit = ptinfo->begin(); cit != ptinfo->end(); ++cit)
				{
						DictType *pdict = TypeCast<DictType>(*cit);
						
						ResumeTaskInfo tinfo;

						std::vector<t_byte> buf;

						TypeCast<StringType>(pdict->get_value(INFOHASH))->get_str(buf);

						if(buf.size() != 20)
						{
								continue;
						}else
						{
								tinfo.infohash.Reset(&buf[0]);
						}
						
						TypeCast<StringType>(pdict->get_value(OWN_PIECES))->get_str(buf);

						if(buf.empty())continue;
						
						tinfo.own_pieces = buf;
						
						std::string utf8_str = TypeCast<StringType>(pdict->get_value(TORRENT_FILE_PATH))->get_str();
						tinfo.torrent_name = FormatSpace::UTF8Converter::ToUnicode(utf8_str);

						utf8_str = TypeCast<StringType>(pdict->get_value(SAVE_PATH))->get_str();
						tinfo.save_path = FormatSpace::UTF8Converter::ToUnicode(utf8_str);

						m_task_map[tinfo.infohash] = tinfo;
				}



		}catch(...)
		{
				DEBUG_PRINT0("Load failed\n");
				CHECK_AND_RET(false);
		}




		
}


bool ResumeInfo::Save()const
{
		assert(m_file.IsOpen());
		
		DictType resume_data;

		DictType *psetting = new DictType();

		

		ListType *ptinfo_list = new ListType();

		for(CIterator cit = m_task_map.begin(); cit != m_task_map.end(); ++cit)
		{
				DictType *ptask = new DictType();
				ptask->set_value(INFOHASH, new StringType(cit->second.infohash.GetRawData(), 20));
				ptask->set_value(OWN_PIECES, new StringType(cit->second.own_pieces));
				
				std::string utf8_str = FormatSpace::UnicodeConverter::ToUTF8(cit->second.save_path);
				ptask->set_value(SAVE_PATH, new StringType(utf8_str));
				
				utf8_str = FormatSpace::UnicodeConverter::ToUTF8(cit->second.torrent_name);
				ptask->set_value(TORRENT_FILE_PATH, new StringType(utf8_str));

				ptinfo_list->insert(ptask);
		}
		
		resume_data.set_value(KEY_TASK, ptinfo_list);

		BencodingEncoder bec;
		
		if(!bec.GenContent(resume_data)) return false;

		m_file.SetLength(0);

		m_file.SeekToBeg();

		m_file.Write(bec.Content(), bec.Size());
		return true;
}













}