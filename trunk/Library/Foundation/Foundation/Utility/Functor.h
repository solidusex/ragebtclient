
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
作为各种Handler的基类,实际上提供了必要的接口和给auto_ptr保存各种handler指针提供了可能
各类Partial Specilization是作为模板推导数据类型所应用的,也就是说Handler继承自这个类,编译器
根据这个类的类型参数寻找适当的Partial Specilization来instantiation operator,实际编译时只有单一
适合的operator()被实例化,所以不会出现编译错误

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
处理仿函数的handler,保存一个仿函数的copy,派生自Implement,类型信息来自Functor(Parent)


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


template<class Parent, class OBJPointerType, class P_MF>		//parent其实就是Functor,P_OBJ是推导出来的类指针类型，P_MF是推导出来的成员函数指针类型
class Mem_F_Handle : public Parent::Imp			//从Functor的Implement<Result, TList>来继承
{
private:
		OBJPointerType	pobj;
		P_MF			mf;
public:
		typedef typename Parent::R Result;			//其实也是Implement<Result, TList>的Result
		Mem_F_Handle(const OBJPointerType &p_obj, const P_MF p_mf) : 
				pobj(p_obj),  mf(p_mf) 
				{


				}
						

		Mem_F_Handle* clone()const { return new Mem_F_Handle(*this); }//clone
		
		Result		operator()() { return (pobj->*mf)(); }		//通过类指针调用类成员函数指针
		
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

一个相当重要的特性是不在关注类型,而关注类的接口,只要提供相同的接口就可以被编译器算作
一类通过编译时的静态推导,极大的降低了组件耦合性在处理复杂的消息机制时降低了派生类所担
负的vtable性能损耗

**********************************************************************************/


template<class Result, class TList>
class Functor
{
public:
		typedef Implement<Result, TList> Imp;				
		typedef Result 	R;										//主要是供给后面的typedef Parent::R Result用
		typedef typename Type_At<TList, 0>::TYPE 		P1;		//通过typelist在编译时推导参数类型
		typedef typename Type_At<TList, 1>::TYPE 		P2;		//这些主要提供给上面的Handle
		typedef typename Type_At<TList, 2>::TYPE			P3;
		typedef typename Type_At<TList, 3>::TYPE			P4;
		typedef typename Type_At<TList, 4>::TYPE			P5;
public:
		Functor()	{ }
		
		Functor& operator=(const Functor &item)
		{
				imp.release();					//先释放auto_ptr，
				imp.reset(item.imp->clone());	//然后加入item的copy
				return *this;
		}
		
		explicit Functor(const std::auto_ptr<Imp> &item)  
		{ 
				imp.release();					//同上
				imp.reset(item.imp->clone());
		}
		
		//根据函数模板的自动推导功能初始化Functor_Handle,并将指针存储为auto_ptr,成员必须是指针,注意
		template<class T>
		Functor(const T item) : imp(new Functor_Handle<Functor, T>(item)) {}
		
		
		template<class OBJPointerType, class MF>
		Functor(const OBJPointerType &pobj, const MF mf) 
				: imp(new Mem_F_Handle<Functor, OBJPointerType, MF>(pobj, mf)) {}

public:
		
		R operator()() 				//通过auto_ptr调用函数	
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
