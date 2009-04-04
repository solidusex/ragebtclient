
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


#include <typeinfo>
#include <stdexcept>




class AnyType
{
private:
		class PlaceHolder
		{
		public:
				virtual ~PlaceHolder() {   }

				virtual const std::type_info& Type()const = 0;

				virtual PlaceHolder* Clone()const = 0;

		};

		template<class ValueType>
		class Holder : public PlaceHolder
		{
		public:
				ValueType		m_val;
		public:
				Holder(ValueType val) : m_val(val)
				{


				}

				~Holder()
				{

				}

				virtual const std::type_info& Type()const
				{
						return typeid(ValueType);
				}

				virtual PlaceHolder* Clone()const
				{
						return new Holder(m_val);
				}
		};
private:
		PlaceHolder			*m_content;
public:
		AnyType() : m_content(0)
		{

		}

		template<class ValueType>
		AnyType(ValueType val) : m_content(new Holder<ValueType>(val))
		{


		}

		AnyType(const AnyType &other) : m_content(other.m_content->Clone())
		{
				

		}

		
		~AnyType()
		{
				delete m_content;
		}

		bool IsEmpty()const
		{
				return !m_content;
		}

		AnyType& Swap(AnyType &other)
		{
				std::swap(m_content, other.m_content);
				return *this;
		}

		AnyType& operator=(const AnyType &other)
		{
				AnyType(other).Swap(*this);
				return *this;
		}

		template<class ValueType>
		AnyType& operator=(ValueType val)
		{
				SetVal<ValueType>(val);
				return *this;
		}

		template<class ValueType>
		AnyType& SetVal(ValueType val)
		{
				delete m_content;
				m_content = new Holder<ValueType>(val);
				return *this;
		}

		const std::type_info& Type()const
		{
				return m_content ? m_content->Type() : typeid(void);
		}

		template<class ValueType>
		ValueType GetVal()const
		{
				if(!IsEmpty())
				{
						if(typeid(ValueType) == m_content->Type())
						{
								return reinterpret_cast<Holder<ValueType>*>(m_content)->m_val;
						}else
						{
								throw std::logic_error("Bad Type Cast!");
						}
				}else
				{
						throw std::logic_error("AnyType Is Empty!");
				}
		}
};

