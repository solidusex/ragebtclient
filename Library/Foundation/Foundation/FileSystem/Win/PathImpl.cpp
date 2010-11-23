
/*
 * The Foundation Library
 * Copyright (c) 2007 by Solidus
 * 
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  It is provided "as is" without express
 * or implied warranty.
 */
#include "PathImpl.h"

#include "FileImpl.h"
#include "../../Format/FormatHdr.h"
#include "../../Regex/Regex.h"


#include <windows.h>




namespace FileSysSpace {


namespace {

void remove_dir(const std::wstring &path_name);

void delete_file_or_dir(const std::wstring &parent_path, const WIN32_FIND_DATAW &fd)
{
		if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
				if(wcscmp(fd.cFileName, L".") != 0 && wcscmp(fd.cFileName, L"..") != 0)
				{
						std::wstring new_path = parent_path + fd.cFileName + L"\\";
						remove_dir(new_path);
				}
		}else
		{
				std::wstring pn = parent_path + fd.cFileName;
				::DeleteFileW(pn.c_str());
		}
}


void remove_dir(const std::wstring &path_name)
{
		::WIN32_FIND_DATAW fd;
		std::wstring path_n = path_name + L"*";
		HANDLE hdl = ::FindFirstFileW(path_n.c_str(), &fd);
		
		if(hdl != INVALID_HANDLE_VALUE)
		{
				delete_file_or_dir(path_name, fd);
				
				while(::FindNextFileW(hdl, &fd))
				{
						delete_file_or_dir(path_name, fd);
				}
				::FindClose(hdl);
		}
		
		if(!::RemoveDirectoryW(path_name.c_str()))
		{
				DEBUG_PRINT1("::RemoveDirectoryW Failed == %ld\n", ::GetLastError());
		}
}


inline bool is_existed(const std::wstring &path)
{
		HANDLE hdl = CreateFileW(path.c_str(),
					0, 
					FILE_SHARE_READ|FILE_SHARE_WRITE,
					0,
					OPEN_EXISTING,
					FILE_FLAG_BACKUP_SEMANTICS,//此标志意味着可以打开一个文件夹
					0);

		bool res = (hdl != INVALID_HANDLE_VALUE);
		if(res)
		{
				::CloseHandle(hdl);
		}
		return res;
}



inline std::vector<std::wstring> separate_path_to_list(const std::wstring &path)
{
		std::vector<std::wstring>  res;
		
		size_t idx = 0;
		
		size_t curr_idx = 0;
 
		while((curr_idx = path.find(L'\\', idx)) != std::wstring::npos)
		{
				res.push_back(path.substr(idx, curr_idx - idx));
				idx = curr_idx + 1;
		}

		std::wstring last_f = path.substr(idx);
		if(last_f != L"")
		{
			res.push_back(last_f);
		}
		return res;
}


__forceinline bool make_sure_dir_path_exists(const std::wstring &path)
{
		if(path == L"")
		{
				return false;
		}else
		{
				if(path[path.size() - 1] != L'\\')
				{
						return false;
				}
		}

		std::vector<std::wstring> res = separate_path_to_list(path);
		if(res.size() <= 1) return false;

		std::wstring curr_path = L"";
		for(size_t idx = 0; idx < res.size(); ++idx)
		{
				curr_path += res[idx];
				curr_path += L"\\";
				if(!is_existed(curr_path))
				{
						if(CreateDirectoryW(curr_path.c_str(), 0) == FALSE)
						{
								return false;
						}
				}
		}
		return true;

}

}





bool PathImpl::operator==(const PathImpl &other)
{
		if(m_type == INVALID_PATH && other.m_type == INVALID_PATH) return true;
		
		return (m_path == other.m_path);

}



PathImpl::PathImpl()
{
		m_type = INVALID_PATH;
		m_parent = L"";
		m_name = L"";
		m_path = L"";
}

PathImpl::PathImpl(const std::wstring &path)
{
		Reset(path);
}

PathImpl::PathImpl(const PathImpl &other)
{
		*this  = other;
}

PathImpl& PathImpl::operator=(const PathImpl &other)
{
		if(this != &other)
		{
				m_parent = other.m_parent;
				m_name   = other.m_name;
				m_path   = other.m_path;
				m_type   = other.m_type;
		}
		return *this;
}



bool PathImpl::Rename(const std::wstring &name)
{
		if(!IsExisted()) return false;
		if(m_type == INVALID_PATH) return false;

		std::wstring new_path;

		switch(m_type)
		{
		case PATH_DIR:
				new_path = m_parent + name + L"\\";
				if(::MoveFileW(m_path.c_str(), new_path.c_str()) == FALSE) return false;
				break;
		case PATH_FILE:
				new_path = m_parent + name;
				if(::MoveFileW(m_path.c_str(), new_path.c_str()) == FALSE) return false;
				break;
		}

		Reset(new_path);
		return IsExisted();
}


bool PathImpl::GetParent(std::wstring &parent)const
{
		if(m_type == INVALID_PATH) return false;
		parent = m_parent;
		return true;
}

bool PathImpl::GetName(std::wstring &name)const
{
		if(m_type == INVALID_PATH) return false;
		name = m_name;
		return true;
}

bool PathImpl::GetPath(std::wstring &path) const
{
		if(m_type == INVALID_PATH) return false;

		path = m_path;
		return true;
}



void PathImpl::separate_path(const std::wstring &path)
{
		
		size_t mark;
		switch(m_type)
		{
		case PATH_DIR:
				mark = path.find_last_of(L'\\');
				mark = path.substr(0, mark).find_last_of(L'\\');
				m_parent = path.substr(0, mark + 1);
				m_name = path.substr(mark + 1);
				m_name.erase(m_name.find(L'\\'));
				break;
		case PATH_FILE:
				mark = path.find_last_of(L'\\');
				m_parent = path.substr(0, mark + 1);
				m_name = path.substr(mark + 1);
				break;
		default:
				m_parent = L"";
				m_name = L"";
				break;
		}
}

void PathImpl::Reset(const std::wstring &path)
{
		m_path = L"";
		m_parent = L"";
		m_name = L"";
		m_type = detect_path_type(path);
		if(m_type == INVALID_PATH) return;
		
		m_path = path;
		separate_path(path);




		
}

PathType PathImpl::GetPathType()const 
{
		return m_type; 
}














PathImpl::PathImpl(const std::string &path)
{
		Reset(path);
}

bool PathImpl::GetParent(std::string &parent)const
{
		std::wstring wstr;
		if(!GetParent(wstr)) return false;

		parent = FormatSpace::UnicodeConverter::ToAnsi(wstr);
		return true;
}
		
bool PathImpl::GetName(std::string &name)const
{
		std::wstring wstr;
		if(!GetName(wstr)) return false;
		
		name = FormatSpace::UnicodeConverter::ToAnsi(wstr);
		return true;

}

bool PathImpl::GetPath(std::string &path) const
{
		std::wstring wstr;
		if(!GetPath(wstr)) return false;

		path = FormatSpace::UnicodeConverter::ToAnsi(wstr);
		return true;

}

bool PathImpl::Rename(const std::string &name)
{
		return Rename(FormatSpace::AnsiConverter::ToUnicode(name));

}

void PathImpl::Reset(const std::string &path)
{
		Reset(FormatSpace::AnsiConverter::ToUnicode(path));

}




void PathImpl::GetCurrentPath(std::wstring &path)
{
		wchar_t buf[MAX_LINE];
		DWORD len = ::GetCurrentDirectoryW(MAX_LINE, buf);
		buf[len + 1] = L'\0';
		path = buf;
}


void PathImpl::GetCurrentPath(std::string &path)
{
		std::wstring wstr;
		GetCurrentPath(wstr);
		path = FormatSpace::UnicodeConverter::ToAnsi(wstr);
}


bool PathImpl::SetCurrentPath(std::wstring &path)
{
		return (::SetCurrentDirectoryW(path.c_str()) == TRUE);
}

		
bool PathImpl::SetCurrentPath(std::string &path)
{
		std::wstring wstr = FormatSpace::AnsiConverter::ToUnicode(path);
		return SetCurrentPath(wstr);
}
		



#define PATH_ROOT_A "^[a-z.A-Z]+:\\\\$"

#define PATH_DIR_A	"^[a-z.A-Z]+:\\\\([^\\\\/\\?\\*\\|<>:\\t\\v\\n]+\\\\[ ]*)+$"

#define PATH_FILE_A   "^[a-z.A-Z]+:\\\\([^\\\\/\\?\\*\\|<>:\\t\\v\\n]+\\\\[ ]*)+[^\\\\/\\?\\*\\|<>:\\t\\v\\n]+$|^[a-z.A-Z]+:\\\\[^\\\\/\\?\\*\\|<>:\\t\\v\\n]+$"


#define PATH_ROOT_W L"^[a-z.A-Z]+:\\\\$"

#define PATH_DIR_W  L"^[a-z.A-Z]+:\\\\([^\\\\/\\?\\*\\|<>:\\t\\v\\n]+\\\\[ ]*)+$"

#define PATH_FILE_W L"^[a-z.A-Z]+:\\\\([^\\\\/\\?\\*\\|<>:\\t\\v\\n]+\\\\[ ]*)+[^\\\\/\\?\\*\\|<>:\\t\\v\\n]+$|^[a-z.A-Z]+:\\\\[^\\\\/\\?\\*\\|<>:\\t\\v\\n]+$"


#define PATH_RELATIVE_DIR_W  L"^(^\\.{1,2})+\\\\([^\\\\/\\?\\*\\|<>:\\t\\v\\n]+\\\\[ ]*)+$"

#define PATH_RELATIVE_FILE_W L"^(^\\.{1,2})+\\\\([^\\\\/\\?\\*\\|<>:\\t\\v\\n]+\\\\[ ]*)+[^\\\\/\\?\\*\\|<>:\\t\\v\\n]+$|^(^\\.{1,2})+\\\\[^\\\\/\\?\\*\\|<>:\\t\\v\\n]+$"




bool PathImpl::is_valid_path(const wchar_t *pattern, const std::wstring &path)
{
		RegexSpace::CRegexpT<wchar_t>  reg(pattern);
		
		RegexSpace::MatchResult res = reg.Match(path.c_str());

		return (res.GetStart() != -1);
}

PathType PathImpl::detect_path_type(const std::wstring &path)
{
		if(is_valid_path(PATH_ROOT_W, path))
		{
				return PATH_ROOT;
		}

		if(is_valid_path(PATH_DIR_W, path))
		{
				return PATH_DIR;
		}

		if(is_valid_path(PATH_FILE_W, path))
		{
				return PATH_FILE;
		}

		if(is_valid_path(PATH_RELATIVE_DIR_W, path))
		{
				return PATH_DIR;
		}

		if(is_valid_path(PATH_RELATIVE_FILE_W, path))
		{
				return PATH_FILE;
		}

		return INVALID_PATH;
}


bool PathImpl::IsExisted() const
{
		return is_existed(m_path.c_str());
}





bool PathImpl::Make()
{
		if(m_type == INVALID_PATH) return false;

		if(IsExisted()) return true;
		
		FileSysSpace::FileImpl  file;

		switch(m_type)
		{
		case PATH_DIR:
				make_sure_dir_path_exists(m_path.c_str());
				break;
		case PATH_FILE:
				PathImpl tmp(m_parent);
				if(!tmp.IsExisted())
				{
						if(!tmp.Make()) return false;
				}

				file.Open(m_path, F_READ_WRITE|F_CREATE_NEW|F_NOSHARED);
				file.SetLength(0);
				break;
		}
		return IsExisted();
}










bool PathImpl::Remove()
{
		if(!IsExisted()) return false;
		
		if(m_type == INVALID_PATH) return false;
		
		if(m_type == PATH_FILE)
		{
				DeleteFileW(m_path.c_str());
		}else if(m_type == PATH_DIR)
		{
				remove_dir(m_path.c_str());
		}

		return (!IsExisted());
}




}
























#if(0)
/*
#define PATH_ROOT_A "^[a-z.A-Z]+:\\\\$"

#define PATH_DIR_A	"^[a-z.A-Z]+:\\\\([^\\\\/\\?\\*\\|<>:\\t\\v\\n]+\\\\[ ]*)+$"

#define PATH_FILE_A   "^[a-z.A-Z]+:\\\\[^\\\\/\\?\\*\\|<>:\\t\\v\\n]+\\\\+[^\\\\/\\?\\*\\|<>:\\t\\v\\n]+$|^[a-z.A-Z]+:\\\\[^\\\\/\\?\\*\\|<>:\\t\\v\\n]+$"


#define PATH_ROOT_W L"^[a-z.A-Z]+:\\\\$"

#define PATH_DIR_W  L"^[a-z.A-Z]+:\\\\([^\\\\/\\?\\*\\|<>:\\t\\v\\n]+\\\\[ ]*)+$"

#define PATH_FILE_W L"^[a-z.A-Z]+:\\\\[^\\\\/\\?\\*\\|<>:\\t\\v\\n]+\\\\+[^\\\\/\\?\\*\\|<>:\\t\\v\\n]+$|^[a-z.A-Z]+:\\\\[^\\\\/\\?\\*\\|<>:\\t\\v\\n]+$"
*/

/*SHFILEOPSTRUCTW   fo; 
				memset((void*)&fo, 0, sizeof(fo));
				fo.wFunc=FO_DELETE;

				wchar_t buf[MAX_LINE];

				memset((void*)buf, 0, sizeof(buf));
				std::wstring tmp = m_path + L"*";
				wcscpy(buf, tmp.c_str());
				buf[tmp.size() + 1] = L'\0';

				fo.pFrom= buf;
				fo.pTo=NULL;
				fo.fFlags= FOF_NOCONFIRMATION | FOF_SILENT|FOF_ALLOWUNDO|FOF_NOERRORUI;
				SHFileOperationW(&fo);
				::RemoveDirectoryW(m_path.c_str());*/

#endif

