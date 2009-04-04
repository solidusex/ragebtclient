
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
#pragma once
#include "../Common/ComnHdr.h"

#include "../Utility/UtilityHdr.h"


namespace ThreadSpace {

class EventImpl;

//ע��,���ʵ�ֱ�֤����,���κ�ƽ̨��,��is_auto== false ʱ��,�κε�һ��setevent����֮��,���
//������reset����,��ô֮���wait����,������������ʾ�ȴ��ɹ�,�����Ҫ����Ϊpthread����������
//��win32�е�event���岻��ͬ,pthread_cond_signal���û�з������˵ȴ�,��ֱ�ӷ���,�´εĵȴ�
//��������ص�bug,Ӱ�쵽����ܶಿ��,���,���汾��ʵ�ֱ��뱣֤signal��wait�Ĵ����޹���

class  FoundationAPI  Event : private NonCopyable
{
private:
		EventImpl	*m_pimpl;
public:
		Event(bool is_auto = false);
		~Event();
		void Wait();
		bool Wait(t_uint64 timeout);
		bool TryWait();
		void Set();
		void Reset();
};



}
