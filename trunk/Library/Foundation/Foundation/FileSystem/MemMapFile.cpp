
/*
 * The Foundation Library
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
 */
#include "MemMapFile.h"


#if defined(OS_FAMILY_WINDOWS)
		#include "Win/MemMapFileImpl.h"
#else
		#error NOT_SUPPORT_PLATFORM
#endif

namespace {

inline t_uint64 calc_index_base(t_uint64 idx)
{
		if(idx < MEM_BLOCK_SIZE)
		{
				return 0;
		}
		else
		{
				return ((idx + MEM_BLOCK_SIZE ) & (~(MEM_BLOCK_SIZE  - 1))) - MEM_BLOCK_SIZE;
						
		}
}





}

namespace FileSysSpace {


MemMapFileView::MemMapFileView(t_byte *baddr, t_byte *mapped_addr, size_t mapped_len)
{
		m_base_addr = baddr;
		m_mapped_addr = mapped_addr;
		m_mapped_len = mapped_len;
		assert(m_base_addr != 0);
		assert(m_mapped_addr != 0);
		assert(m_mapped_len != 0);
}

MemMapFileView::~MemMapFileView()
{


}


t_byte* MemMapFileView::GetAddr()
{
		return m_mapped_addr;
}

size_t MemMapFileView::GetLength()const
{
		return m_mapped_len;
}


//-------------------------------------------------------------------------/

//const t_ulong MemMapFile::MEM_BLOCK_SIZE = IMPL_MEM_BLOCK_SIZE;



MemMapFile::MemMapFile() : m_pimpl(0)
{

}
		
MemMapFile::~MemMapFile()
{
		Close();
}




bool MemMapFile::Open(void *fhandle, t_uint64 fsize, t_ulong mode)
{
		if(m_pimpl != 0)
		{
				return false;
		}else
		{
				try{
						m_pimpl = new MemMapFileImpl(fhandle, fsize, mode);
						assert(m_pimpl != 0);
						return true;
				}catch(const std::exception &expt)
				{
						DEBUG_PRINT1("MemMapFile::Open : %s\n", expt.what());
						return false;
				}
				
		}
}

void MemMapFile::Close()
{
		if(m_pimpl)
		{
				delete m_pimpl;
				m_pimpl = 0;
		}
}

bool MemMapFile::IsOpen()const
{
		return (m_pimpl != 0);
}


MemMapFileView* MemMapFile::MapViewOfFile(t_uint64 offset, size_t len, t_ulong mode)
{
		assert(m_pimpl != 0);
		t_uint64 new_offset = calc_index_base(offset);//得到对齐后的offset;
		
		t_uint64 off = offset - new_offset;		//得到新的offset和原来的差距;

		//len + off为当前要映射的长度;
		
		t_byte *ptr = m_pimpl->MapViewOfFile(new_offset, (size_t)(len + off), mode);
		
		//mapped_addr 为用户所需求的地址begin
		t_byte *mapped_addr = ptr + off;
		
		if(ptr == 0)
		{
				return 0;
		}else
		{
				return (new MemMapFileView(ptr, mapped_addr, len));
		}

}

void MemMapFile::UnmapViewOfFile(MemMapFileView* pmv)
{
		assert(m_pimpl != 0);
		assert(pmv != 0);
		assert(pmv->m_base_addr != 0);
		m_pimpl->UnmapViewOfFile(pmv->m_base_addr);
		delete pmv;
}

bool MemMapFile::Flush(MemMapFileView* pmv)
{
		assert(m_pimpl != 0);
		assert(pmv->m_base_addr != 0);
		t_uint64 len = pmv->m_mapped_len + (pmv->m_mapped_addr - pmv->m_base_addr);
		return m_pimpl->Flush(pmv->m_base_addr, (t_ulong)len);
}


}