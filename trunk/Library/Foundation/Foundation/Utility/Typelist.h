
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
#ifndef TYPELIST_H
#define TYPELIST_H

class NULLTYPE;												//�������
class EMPTY {};												//����

#define TYPELIST_0()				NULLTYPE

#define TYPELIST_1(P1)				TypeList<P1, NULLTYPE>

#define TYPELIST_2(P1, P2)			TypeList<P1, TYPELIST_1(P2) >

#define TYPELIST_3(P1, P2, P3)		TypeList<P1, TYPELIST_2(P2, P3) >

#define TYPELIST_4(P1, P2, P3, P4)		TypeList<P1, TYPELIST_3(P2, P3, P4) >

#define TYPELIST_5(P1, P2, P3, P4, P5)		TypeList<P1, TYPELIST_4(P2, P3, P4, P5) >

#define TYPELIST_6(P1, P2, P3, P4, P5, P6)		TypeList<P1, TYPELIST_5(P2, P3, P4, P5, P6) >

#define TYPELIST_7(P1, P2, P3, P4, P5, P6, P7)		TypeList<P1, TYPELIST_6(P2, P3, P4, P5, P6, P7) >

#define TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8)		TypeList<P1, TYPELIST_7(P2, P3, P4, P5, P6, P7, P8) >

#define TYPELIST_9(P1, P2, P3, P4, P5, P6, P7, P8, P9)		TypeList<P1, TYPELIST_8(P2, P3, P4, P5, P6, P7, P8, P9) >

#define TYPELIST_10(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)		TypeList<P1, TYPELIST_9(P2, P3, P4, P5, P6, P7, P8, P9, P10) >

#define TYPELIST_11(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)		TypeList<P1, TYPELIST_10(P2, P3, P4, P5, P6, P7, P8, P9, P10, P11) >

template<class T, class U>									
struct TypeList
{
		typedef T 		Head;
		typedef U		Rear;
};

template<class TList> struct Length;						//�������������

template<> 
struct Length<NULLTYPE> {
		enum { value = 0 };
};


template<class H, class R>
struct Length<TypeList<H, R> >
{
		enum { value = 1 + Length<R>::value };
};




template<class TList, unsigned int index> struct Type_At;			//�õ�ָ�������������ڵ�����

template<class H, class R>
struct Type_At<TypeList<H, R>, 0 >						//ָ����ֵ�Ե�,���Ƶ�ǰ���͸�TYPE;
{
		typedef H TYPE;
};

template<unsigned int index>						//�����������ֵ���ԣ�������typelistβ,����EMPTY����
struct Type_At<NULLTYPE, index>
{
		typedef EMPTY TYPE;
};

template<class H, class R, unsigned int index>			//���ݹ�,ֱ��index��Ϊ0����ʱ��indexΪ0��struct����������Ϣֱ��������ɵ����е�TYPE
struct Type_At<TypeList<H, R>, index>
{
		typedef typename Type_At<R, index-1>::TYPE TYPE;
};


template<class T, class TList> struct Get_Index;

template<class T>
struct Get_Index<T, NULLTYPE>
{
		enum { Index = -1 };
};

template<class T, class R>
struct Get_Index<T, TypeList<T, R> >
{
		enum { Index = 0 };
};

template<class T, class H, class R>
struct Get_Index<T, TypeList<H, R> >
{
private:
		enum { temp = Get_Index<T, R>::Index };
public:
		enum { Index = ( (temp != -1) ? 1 + temp : -1) };
};


#endif



























/*
#define TYPELIST_1(P1)				TypeList<P1, NULLTYPE>

#define TYPELIST_2(P1, P2)			TypeList<P1, TypeList<P2, NULLTYPE> >

#define TYPELIST_3(P1, P2, P3)		TypeList<P1, TypeList<P2, TypeList<P3, NULLTYPE> > >

#define TYPELIST_4(P1, P2, P3, P4)		TypeList<P1, TypeList<P2, TypeList<P3, TypeList<P4, NULLTYPE> > > >

#define TYPELIST_5(P1, P2, P3, P4, P5)		TypeList<P1, TypeList<P2, TypeList<P3, TypeList<P4, TypeList<P5, NULLTYPE> > > > >

#define TYPELIST_6(P1, P2, P3, P4, P5, P6)		TypeList<P1, TypeList<P2, TypeList<P3, TypeList<P4, TypeList<P5, TypeList<P6, NULLTYPE> > > > > >


#define TYPELIST_11(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)\
		TypeList<P1, \
				TypeList<P2, \
						TypeList<P3, \
								TypeList<P4, \
										TypeList<P5, \
												TypeList<P6, \
														TypeList<P7,\
																TypeList<P8,\
																		TypeList<P9,\
																				TypeList<P10,\
																						TypeList< P11, NULLTYPE> > > > > > > > > > >

*/
