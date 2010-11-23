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

#pragma once

#include "TypeDef.h"
#include <vector>


namespace DSSpace{


template<class Type>
class Array2D
{
private:
		typedef std::vector<Type>  ContainerType;
private:
		size_t			m_rows;
		size_t			m_columns;
		ContainerType	m_container;
private:
		inline size_t row_based_map(size_t row, size_t column)const
		{
				assert(row < m_rows && column < m_columns);
				return row * m_columns + column;
		}
public:
		Array2D(size_t rows, size_t columns)
		{
				assert(rows > 0 && columns > 0);
				Reset(rows, columns);
		}

		~Array2D()
		{


		}
public:
		Array2D(const Array2D &other)
		{
				*this = other;
		}
		
		Array2D& operator=(const Array2D &other)
		{
				if(this != &other)
				{
						Reset(other.m_rows, other.m_columns);
						m_container = other.m_container;
				}
				return *this;
		}
		
		
public:
		const Type& Value(size_t row, size_t column)const
		{
				if(row >= m_rows || column >= m_columns)
				{
						assert(0);
						throw ExceptionSpace::RangeException("Out of range : const Type& Get(size_t row, size_t column)const");
				}

				size_t elem_idx = row_based_map(row, column);
				return m_container[elem_idx];
		}
		
		Type&	Value(size_t row, size_t column)
		{
				if(row >= m_rows || column >= m_columns)
				{
						assert(0);
						throw ExceptionSpace::RangeException("Out of range : Type&	Get(size_t row, size_t column)");
				}
				
				size_t elem_idx = row_based_map(row, column);
				return m_container[elem_idx];

		}
public:

		void Reset(size_t rows, size_t columns)
		{
				if(rows == 0 || columns == 0)
				{
						assert(0);
						throw ExceptionSpace::InvalidArgumentException("Invalid array2 init arg : void Reset(size_t rows, size_t columns)");
				}
				m_rows = rows;
				m_columns = columns;
				
				m_container.clear();
				m_container.resize(rows * columns);

		}

		size_t Rows()const
		{
				return m_rows;
		}

		size_t Columns()const
		{
				return m_columns;
		}
};


////////////////////////////Array2D<bool>//////////////////
template<>
class Array2D<bool>
{
private:
		//typedef ReverseBitSet	ContainerType;
		typedef std::vector<bool>		ContainerType;
private:
		size_t			m_rows;
		size_t			m_columns;
		ContainerType	m_container;
private:
		inline size_t row_based_map(size_t row, size_t column)const
		{
				assert(row < m_rows && column < m_columns);
				return row * m_columns + column;
		}
public:
		Array2D(size_t rows, size_t columns)
		{
				assert(rows > 0 && columns > 0);
				Reset(rows, columns);
		}

		~Array2D()
		{


		}
public:
		Array2D(const Array2D &other)
		{
				*this = other;
		}
		
		Array2D& operator=(const Array2D &other)
		{
				if(this != &other)
				{
						Reset(other.m_rows, other.m_columns);
						m_container = other.m_container;
				}
				return *this;
		}
public:
		
		bool IsSet(size_t row, size_t column)const
		{
				//return m_container.IsSet(row_based_map(row, column));
				return m_container[row_based_map(row, column)];
		}

		bool IsAllSet()const
		{
				for(size_t i = 0; i < m_container.size(); ++i)
				{
						if(!m_container[i]) return false;
				}
				return true;
		}
		
		bool IsEmpty()const//所有bit都没有被set;
		{
				for(size_t i = 0; i < m_container.size(); ++i)
				{
						if(m_container[i]) return false;
				}
				return true;
		}
		
		void Set(size_t row, size_t column)
		{
				m_container[row_based_map(row, column)] = true;
		}

		void UnSet(size_t row, size_t column)
		{
				m_container[row_based_map(row, column)] = false;
		}
		
		void Flip()
		{
				m_container.flip();
		}

		void ClearALL()
		{
				m_container = std::vector<bool>(false, m_rows * m_columns);
		}

public:

		void Reset(size_t rows, size_t columns)
		{
				if(rows == 0 || columns == 0)
				{
						assert(0);
						throw ExceptionSpace::InvalidArgumentException("Invalid array2 init arg : void Reset(size_t rows, size_t columns)");
				}
				m_rows = rows;
				m_columns = columns;
				m_container = std::vector<bool>(false, rows * columns);
		}

		size_t Rows()const
		{
				return m_rows;
		}

		size_t Columns()const
		{
				return m_columns;
		}
};














}