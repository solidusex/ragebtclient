/*
* The Rage Library
* Copyright (c) 2007 by Solidus
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* provided that the above copyright notice appear in all copies and
* representations about the suitability of this software for any
* purpose.  It is provided "as is" without express or implied warranty.
*
*/
#include "TorrentFile.h"
#include "BencodingDecoder.h"

#include <iostream>

namespace Rage {


#define			KEY_ANNOUNCE			"announce"
#define			KEY_ANNOUNCE_LIST		"announce-list"
#define			KEY_INFO				"info"


#define			KEY_PIECE_LENGTH		"piece length"
#define			KEY_PIECES				"pieces"


#define			KEY_NODES				"nodes"
#define			KEY_FILES				"files"
;
#define			KEY_PATH				"path"
#define			KEY_PATH_UTF8			"path.utf-8"
#define			KEY_LENGTH				"length"

#define			KEY_NAME				"name"
#define			KEY_NAME_UTF8			"name.utf-8"


#define			KEY_CREATION_DATE		"creation date"

#define			KEY_COMMENT				"comment"
#define			KEY_COMMENT_UTF			"comment.utf-8"

#define			KEY_CREATE_BY			"created by"
#define			KEY_CREATE_BY_UTF		"created by.utf8"




#define			KEY_PUBLISHER			"publisher"
#define			KEY_PUBLISHER_UTF		"publisher.utf-8"

#define			KEY_PUBLISHER_URL       "publisher-url"
#define			KEY_PUBLISHER_URL_UTF	"publisher-url.utf8"



#define CHECK_RESULT(res) if(!(res)) return false

bool TorrentFile::Load(const std::wstring &file_name)
{
		using namespace FileSysSpace;

		File		file;

		if(!file.Open(file_name, F_READ_ONLY|F_OPEN_EXISTED|F_SHARED_READ))
		{
				return false;
		}

		t_uint64 len = file.GetLength();

		if(len > (5*MB)) return false;

		std::vector<t_byte> buf(len);

		if(file.Read(&buf[0], buf.size()) != buf.size())
		{
				return false;
		}
		
		file.Close();

		BencodingDecoder bd;

		if(!bd.SetContent(buf)) return false;

		

		TorrentFile tf;

		try{
				using namespace FormatSpace;

				DictType b_dict;
				
				CHECK_RESULT(bd.ReadDict(0, b_dict) != 0);
				
				ListType *p_urls = (ListType*)b_dict.get_value(KEY_ANNOUNCE_LIST);

				if(p_urls != 0 && p_urls->type() == LIST_TYPE)
				{
						for(ListType::C_Iterator it = p_urls->begin(); it != p_urls->end(); ++it)
						{
								CHECK_RESULT((*it)->type() == LIST_TYPE);
								ListType *in_plist = (ListType*)(*it);

								for(ListType::C_Iterator inner_it = in_plist->begin(); inner_it != in_plist->end(); ++inner_it)
								{
										std::string url = TypeCast<StringType>(*inner_it)->get_str();
										tf.m_announces.push_back(url);
								}
						}
				}else if(StringType *p_url = (StringType*)b_dict.get_value(KEY_ANNOUNCE))
				{
						tf.m_announces.push_back(p_url->get_str());
				}

				using namespace NetSpace;

				if(ListType *pnodes = (ListType*)b_dict.get_value(KEY_NODES))
				{
						for(ListType::C_Iterator it = pnodes->begin(); it != pnodes->end(); ++it)
						{
								ListType *pnl = TypeCast<ListType>(*it);
								CHECK_RESULT(pnl->size() == 2);

								ListType::C_Iterator ip_iter = pnl->begin();
								std::string ip = TypeCast<StringType>(*ip_iter)->get_str();
								++ip_iter;
								t_uint16 port = (t_uint16)TypeCast<IntegerType>(*ip_iter)->get_int();
								
								InetAddress addr(ip, port);

								tf.m_nodes.push_back(PeerEntry(addr.IP(), addr.Port()));
						}
				}
				

				if(IntegerType *pint = (IntegerType*)b_dict.get_value(KEY_CREATION_DATE))
				{
						CHECK_RESULT(pint->type() == INTEGER_TYPE);

						tf.m_creation_date = pint->get_int();
				}

				if(StringType *pstr = (StringType*)b_dict.get_value(KEY_COMMENT_UTF))
				{
						tf.m_comment = UTF8Converter::ToUnicode(pstr->get_str());

				}else if(StringType *pstr = (StringType*)b_dict.get_value(KEY_COMMENT))
				{
						tf.m_comment = AnsiConverter::ToUnicode(pstr->get_str());
				}


				if(StringType *pstr = (StringType*)b_dict.get_value(KEY_CREATE_BY_UTF))
				{
						tf.m_creator = UTF8Converter::ToUnicode(pstr->get_str());

				}else if(StringType *pstr = (StringType*)b_dict.get_value(KEY_CREATE_BY))
				{
						tf.m_creator = AnsiConverter::ToUnicode(pstr->get_str());
				}
		
				
				//////////////////////////////////////////////////////
				std::string encoding = "";
				StringType *pencoding = (StringType*)b_dict.get_value("encoding");
				
				if(pencoding != 0)
				{
						encoding = pencoding->get_str();
				}
				DEBUG_PRINT1("encoding == %s\n", encoding.c_str());
				/////////////////////////////////////////////////
				
				size_t info_pos = bd.LocateKey(0, KEY_INFO);
				CHECK_RESULT(info_pos != 0);

				DictType info_dict;
				size_t info_len = bd.ReadDict(info_pos, info_dict);
				CHECK_RESULT(len != 0);

				const t_byte *p_data = bd.GetContent(info_pos);
				CHECK_RESULT(p_data != 0);

				CryptSpace::ShaHash sha1_dec(CryptSpace::ShaHash::SHA160);

				sha1_dec.Begin();
				sha1_dec.Update(p_data, info_len);
				t_byte result[20];
				sha1_dec.End(result);

				tf.m_infohash.Reset(result);
				tf.m_piece_length = TypeCast<IntegerType>(info_dict.get_value(KEY_PIECE_LENGTH))->get_int();

				TypeCast<StringType>(info_dict.get_value(KEY_PIECES))
						->get_str(tf.m_pieces_hash_val);
				
				CHECK_RESULT(tf.m_pieces_hash_val.size() >= 20);
				CHECK_RESULT((tf.m_pieces_hash_val.size() % 20) == 0);

				tf.m_piece_count = tf.m_pieces_hash_val.size() / 20;
				
				
				
				
				

				TypeBase* pfiles = info_dict.get_value(KEY_FILES);
				if(pfiles == 0)
				{
						FileInfo		f_info;
						
						StringType *pname = (StringType*)info_dict.get_value(KEY_NAME_UTF8);

						if((pname != 0) && (pname->type() == STRING_TYPE))
						{
								f_info.path = UTF8Converter::ToUnicode(pname->get_str());
						}else
						{
								pname = (StringType*)info_dict.get_value(KEY_NAME);
								
								CHECK_RESULT(pname != 0 && pname->type() == STRING_TYPE);
								
								if(encoding != "UTF-8")
								{
										f_info.path = AnsiConverter::ToUnicode(pname->get_str());
								}else
								{
										f_info.path = UTF8Converter::ToUnicode(pname->get_str());
								}
						}

						CHECK_RESULT(!f_info.path.empty());
						f_info.length = TypeCast<IntegerType>(info_dict.get_value(KEY_LENGTH))->get_int();
						tf.m_topdir = L"";						
						tf.m_files.push_back(f_info);

				}else	
				{
						
						StringType *pname = (StringType*)info_dict.get_value(KEY_NAME_UTF8);
						
						if(pname != 0 && pname->type() == STRING_TYPE)
						{
								tf.m_topdir = UTF8Converter::ToUnicode(pname->get_str());
										
						}else
						{
								pname = (StringType*)info_dict.get_value(KEY_NAME);
								
								CHECK_RESULT(pname != 0 && pname->type() == STRING_TYPE);

								if(encoding != "UTF-8")
								{
										tf.m_topdir = AnsiConverter::ToUnicode(pname->get_str());
								}else
								{
										tf.m_topdir = UTF8Converter::ToUnicode(pname->get_str());
								}
						}
						
						CHECK_RESULT(!tf.m_topdir.empty());
						
						ListType *p_flist = TypeCast<ListType>(pfiles);
						
						size_t file_idx = 0;
						
						tf.m_files.clear();

						for(ListType::C_Iterator list_iter = p_flist->begin(); list_iter != p_flist->end(); list_iter++)
						{
								DictType *pf_dict = TypeCast<DictType>(*list_iter);

								FileInfo f_info;
								
								f_info.length = TypeCast<IntegerType>(pf_dict->get_value(KEY_LENGTH))->get_int();
								
								ListType *p_path_list = (ListType*)(pf_dict->get_value(KEY_PATH_UTF8));
								
								if(p_path_list != 0 && p_path_list->type() == LIST_TYPE)
								{
								
										for(ListType::C_Iterator path_iter = p_path_list->begin(); 
												path_iter != p_path_list->end();
												path_iter++)
										{
												f_info.path += UTF8Converter::ToUnicode(TypeCast<StringType>(*path_iter)->get_str());
												f_info.path += Path::PATH_SP_W;
										}
										
										CHECK_RESULT(f_info.path.size() >= 2);
										f_info.path.erase(f_info.path.size() - 1);
								}else
								{
										p_path_list = (ListType*)(pf_dict->get_value(KEY_PATH));
										
										CHECK_RESULT(p_path_list != 0 && p_path_list->type() == LIST_TYPE);
										std::string relative_path = "";

										for(ListType::C_Iterator path_iter = p_path_list->begin(); 
												path_iter != p_path_list->end();
												path_iter++)
										{
												
												relative_path += TypeCast<StringType>(*path_iter)->get_str();
												relative_path += Path::PATH_SP_A;

										}
										
										CHECK_RESULT(relative_path.size() >= 2);
										relative_path.erase(relative_path.size() - 1);

										if(encoding != "UTF-8")
										{
												f_info.path = AnsiConverter::ToUnicode(relative_path);
										}else
										{
												f_info.path = UTF8Converter::ToUnicode(relative_path);
										}
								}
								tf.m_files.push_back(f_info);
						}
				}
				
				tf.m_total_length = 0;

				for(size_t fidx = 0; fidx < tf.m_files.size(); ++fidx)
				{
						tf.m_total_length += tf.m_files[fidx].length;
				}



				

		}catch(...)
		{
				Clear();
				return false;
		}

		*this = tf;

		return true;
}



using namespace ExceptionSpace;


TorrentFile::TorrentFile()
{
		Clear();

}


TorrentFile::TorrentFile(const TorrentFile &other)
{
		*this = other;
}


TorrentFile& TorrentFile::operator=(const TorrentFile &other)
{
		if(this != &other)
		{
				m_topdir = other.m_topdir;
				m_files = other.m_files;

				m_total_length = other.m_total_length;
				m_announces = other.m_announces;
				m_piece_length = other.m_piece_length;
				m_piece_count = other.m_piece_count;

				m_pieces_hash_val = other.m_pieces_hash_val;
				m_infohash = other.m_infohash;
				m_nodes = other.m_nodes;
				m_creation_date = other.m_creation_date;
				m_comment = other.m_comment;
				m_creator = other.m_creator;
				//m_publisher_url = other.m_publisher_url;
				//m_publisher = other.m_publisher;
		}
		return *this;
}


TorrentFile::~TorrentFile()
{


}


std::wstring TorrentFile::GetTopDir()const
{
		return m_topdir;

}

t_uint32 TorrentFile::GetFileCount()const
{

		return m_files.size();

}

FileInfo TorrentFile::GetFileInfo(t_uint32 idx)const
{
		if(idx < m_files.size())
		{
				return m_files[idx];
		}else
		{
				throw InvalidArgumentException("TorrentFile::GetFileInfo : invalid idx");
		}
}

t_uint64 TorrentFile::GetTotalSize()const
{

		return m_total_length;
}


Sha1Hash TorrentFile::GetInfoHash()const
{
		return m_infohash;

}

t_uint32 TorrentFile::GetPieceLength(size_t idx)const
{
		if(idx < m_piece_count)
		{
				if(idx == (m_piece_count - 1))
				{
						t_uint64 pieces = m_piece_count -1;
						t_uint64 piece_len = m_piece_length;

						return (m_total_length - pieces * piece_len);
				}else
				{
						return m_piece_length;
				}
		}else
		{
				throw InvalidArgumentException("TorrentFile::GetPieceLength : invalid idx");
		}


}

t_uint32 TorrentFile::GetPieceLength()const
{
		return m_piece_length;

}


t_uint32 TorrentFile::GetPieceCount()const
{
		return m_piece_count;
}

Sha1Hash TorrentFile::GetPieceHash(t_uint32 idx)const
{
		assert(m_pieces_hash_val.size() == (m_piece_count * 20));

		if(idx < m_piece_count)
		{
				return Sha1Hash(&m_pieces_hash_val[idx * 20]);
		}else
		{
				throw InvalidArgumentException("TorrentFile::GetPieceHash : invalid idx");
		}

}


t_uint32 TorrentFile::GetAnnounceCount()const
{
		return m_announces.size();
}


std::string TorrentFile::GetAnnounce(size_t idx)const
{
		if(idx < m_announces.size())
		{
				return m_announces[idx];
		}else
		{
				throw InvalidArgumentException("TorrentFile::GetAnnounce : invalid idx");
		}
}

t_uint32 TorrentFile::GetNodeCount()const
{
		return m_nodes.size();
}


PeerEntry TorrentFile::GetNode(size_t idx)const
{

		if(idx < m_nodes.size())
		{
				return m_nodes[idx];
		}else
		{
				throw InvalidArgumentException("TorrentFile::GetNode : invalid idx");
		}
}



void TorrentFile::Clear()
{
		m_topdir = L"";
		m_files.swap(FileInfoArray(0));
		m_total_length = 0;
		m_announces.swap(AnnounceArray(0));
		m_piece_length = 0;
		m_piece_count = 0;
		m_pieces_hash_val.swap(std::vector<t_byte>(0));
		m_infohash.Clear();
		m_nodes.swap(NodeArray(0));
		m_creation_date = 0;
		m_comment = L"";
		m_creator = L"";
//		m_publisher_url = L"";
//		m_publisher = L"";

}

std::wstring TorrentFile::GetTorrentName()const
{
		assert(m_files.size() != 0);

		std::wstring res = GetTopDir();
		if(res != L"")
		{
				return res;
		}else
		{
				return m_files[0].path;
		}
}






t_uint64 TorrentFile::GetCreationDate()const
{

		return m_creation_date;

}

std::wstring TorrentFile::GetComment()const
{
		return m_comment;

}

std::wstring TorrentFile::GetCreator()const
{
		return m_creator;
}


bool TorrentFile::MapFileToPieces(size_t file_idx, t_uint32 &piece_beg, t_uint32 &piece_end)const
{
		try{
				t_uint64 start_pos = 0;

				for(size_t fidx = 0; fidx < file_idx; ++fidx)
				{
						start_pos += GetFileInfo(fidx).length;
				}

				piece_beg = start_pos / GetPieceLength();

				piece_end = (start_pos + GetFileInfo(file_idx).length ) / GetPieceLength();
		}catch(...)
		{
				return false;
		}
		return true;
}


/*
std::wstring TorrentFile::GetPublisher()const
{
		return m_publisher;
}

std::wstring TorrentFile::GetPublisherUrl()const
{
		return m_publisher_url;
}
*/





















}