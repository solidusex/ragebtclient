
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
#ifndef FUNCTOR_H
#define FUNCTOR_H

#include <memory>
#include "Typelist.h"

/***************************************************************************************
��Ϊ����Handler�Ļ���,ʵ�����ṩ�˱�Ҫ�Ľӿں͸�auto_ptr�������handlerָ���ṩ�˿���
����Partial Specilization����Ϊģ���Ƶ�����������Ӧ�õ�,Ҳ����˵Handler�̳��������,������
�������������Ͳ���Ѱ���ʵ���Partial Specilization��instantiation operator,ʵ�ʱ���ʱֻ�е�һ
�ʺϵ�operator()��ʵ����,���Բ�����ֱ������

***************************************************************************************/

template<class R, class TList> class Implement;		

template<class R>
class Implement<R, NULLTYPE>
{
public:
		virtual R operator()() = 0;
		virtual Implement* clone() const = 0;
		virtual ~Implement(){}
};

template<class R, class P1>
class Implement<R, TYPELIST_1(P1)>
{
public:
		virtual R operator()(P1) = 0;
		virtual Implement* clone()const = 0;
		virtual ~Implement(){}
};
		


template<class R, class P1, class P2>
class Implement<R, TYPELIST_2(P1, P2)>
{
public:
		virtual R operator()(P1, P2) = 0;
		virtual Implement* clone()const = 0;
		virtual ~Implement(){}
};




template<class R, class P1, class P2, class P3>
class Implement<R, TYPELIST_3(P1, P2, P3)>
{
public:
		virtual R operator()(P1, P2, P3) = 0;
		virtual Implement* clone()const = 0;
		virtual ~Implement(){}
};



template<class R, class P1, class P2, class P3, class P4>
class Implement<R, TYPELIST_4(P1, P2, P3, P4)>
{
public:
		virtual R operator()(P1, P2, P3, P4) = 0;
		virtual Implement* clone()const = 0;
		virtual ~Implement(){}
};


template<class R, class P1, class P2, class P3, class P4, class P5>
class Implement<R, TYPELIST_5(P1, P2, P3, P4, P5)>
{
public:
		virtual R operator()(P1, P2, P3, P4, P5) = 0;
		virtual Implement* clone()const = 0;
		virtual ~Implement(){}
};

/*************************************************************************************
����º�����handler,����һ���º�����copy,������Implement,������Ϣ����Functor(Parent)


*************************************************************************************/

template<class Parent, class FUN>
class Functor_Handle : public Parent::Imp
{
private:
		FUN 		fun;
public:
		typedef 	typename Parent::R	Result;
		
		Result		operator()() { return (*fun)(); }
		Result		operator()(typename Parent::P1 p1)
		{ return (*fun)(p1); }
		
		Result	operator()(typename Parent::P1 p1, typename Parent::P2 p2)
		{ return (*fun)(p1, p2); }
		
		Result	operator()(		typename Parent::P1 p1, 
								typename Parent::P2 p2,
								typename Parent::P3 p3
						)
		{ return (*fun)(p1, p2, p3); }
		
		Result	operator()(		typename Parent::P1 p1, 
								typename Parent::P2 p2,
								typename Parent::P3 p3,
								typename Parent::P4 p4
						)
		{ return (*fun)(p1, p2, p3, p4); }
		
		Result	operator()(		typename Parent::P1 p1, 
								typename Parent::P2 p2,
								typename Parent::P3 p3,
								typename Parent::P4 p4,
								typename Parent::P5 p5
						)
		{ return (*fun)(p1, p2, p3, p4, p5); }
		
		Functor_Handle(const FUN item) : fun(item) {}
		Functor_Handle* clone() const { return new Functor_Handle(*this); }
};


template<class Parent, class OBJPointerType, class P_MF>		//parent��ʵ����Functor,P_OBJ���Ƶ���������ָ�����ͣ�P_MF���Ƶ������ĳ�Ա����ָ������
class Mem_F_Handle : public Parent::Imp			//��Functor��Implement<Result, TList>���̳�
{
private:
		OBJPointerType	pobj;
		P_MF			mf;
public:
		typedef typename Parent::R Result;			//��ʵҲ��Implement<Result, TList>��Result
		Mem_F_Handle(const OBJPointerType &p_obj, const P_MF p_mf) : 
				pobj(p_obj),  mf(p_mf) 
				{


				}
						

		Mem_F_Handle* clone()const { return new Mem_F_Handle(*this); }//clone
		
		Result		operator()() { return (pobj->*mf)(); }		//ͨ����ָ��������Ա����ָ��
		
		Result		operator()(typename Parent::P1 p1)
		{ return (pobj->*mf)(p1); }
		
		Result	operator()(typename Parent::P1 p1, typename Parent::P2 p2)
		{ return (pobj->*mf)(p1, p2); }
		
		Result	operator()(		typename Parent::P1 p1, 
								typename Parent::P2 p2,
								typename Parent::P3 p3
						)
		{ return (pobj->*mf)(p1, p2, p3); }
		
		Result	operator()(		typename Parent::P1 p1, 
								typename Parent::P2 p2,
								typename Parent::P3 p3,
								typename Parent::P4 p4
						)
		{ return (pobj->*mf)(p1, p2, p3, p4); }
		
		Result	operator()(		typename Parent::P1 p1, 
								typename Parent::P2 p2,
								typename Parent::P3 p3,
								typename Parent::P4 p4,
								typename Parent::P5 p5
						)
		{ return (pobj->*mf)(p1, p2, p3, p4, p5); }
		
};

/*********************************************************************************

һ���൱��Ҫ�������ǲ��ڹ�ע����,����ע��Ľӿ�,ֻҪ�ṩ��ͬ�ĽӿھͿ��Ա�����������
һ��ͨ������ʱ�ľ�̬�Ƶ�,����Ľ��������������ڴ����ӵ���Ϣ����ʱ����������������
����vtable�������

**********************************************************************************/


template<class Result, class TList>
class Functor
{
public:
		typedef Implement<Result, TList> Imp;				
		typedef Result 	R;										//��Ҫ�ǹ��������typedef Parent::R Result��
		typedef typename Type_At<TList, 0>::TYPE 		P1;		//ͨ��typelist�ڱ���ʱ�Ƶ���������
		typedef typename Type_At<TList, 1>::TYPE 		P2;		//��Щ��Ҫ�ṩ�������Handle
		typedef typename Type_At<TList, 2>::TYPE			P3;
		typedef typename Type_At<TList, 3>::TYPE			P4;
		typedef typename Type_At<TList, 4>::TYPE			P5;
public:
		Functor()	{ }
		
		Functor& operator=(const Functor &item)
		{
				imp.release();					//���ͷ�auto_ptr��
				imp.reset(item.imp->clone());	//Ȼ�����item��copy
				return *this;
		}
		
		explicit Functor(const std::auto_ptr<Imp> &item)  
		{ 
				imp.release();					//ͬ��
				imp.reset(item.imp->clone());
		}
		
		//���ݺ���ģ����Զ��Ƶ����ܳ�ʼ��Functor_Handle,����ָ��洢Ϊauto_ptr,��Ա������ָ��,ע��
		template<class T>
		Functor(const T item) : imp(new Functor_Handle<Functor, T>(item)) {}
		
		
		template<class OBJPointerType, class MF>
		Functor(const OBJPointerType &pobj, const MF mf) 
				: imp(new Mem_F_Handle<Functor, OBJPointerType, MF>(pobj, mf)) {}

public:
		
		R operator()() 				//ͨ��auto_ptr���ú���	
		{ return (*imp)(); }
		
		R operator()(P1 p1) 			
		{ return (*imp)(p1); }
		
		R operator()(P1 p1, P2 p2) 	
		{ return (*imp)(p1, p2); }
		
		R operator()(P1 p1, P2 p2, P3 p3)
		{ return (*imp)(p1, p2, p3); }
		
		R operator()(P1 p1, P2 p2, P3 p3, P4 p4)
		{ return (*imp)(p1, p2, p3, p4); }
		
		R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
		{ return (*imp)(p1, p2, p3, p4, p5); }
private:
		std::auto_ptr<Imp> imp;

};



#endif
