
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


template<class D, class B>
class IsDerivedFrom
{
		class No { };
		class Yes { No no[2]; };
		static Yes Test(B*);
		static No  Test(...);
		static void Constraints(D *pd) { B *pb = pd; pb = pd; }
		
public:
		enum { Is = sizeof(Test(static_cast<D*>(0))) == sizeof(Yes) };
		IsDerivedFrom() { void (*test)(D*) = Constraints; }
};





template<class Type>
class MustBePod
{
private:
		union Impl
		{
				Type			type;
				unsigned char	data[1];
		};
public:
		MustBePod()
		{
				Impl();
		}

		~MustBePod()
		{

		}
};



