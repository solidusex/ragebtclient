#pragma once


#include <string>
#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <stack>
#include <cassert>

#include <stdio.h>





enum EXPR_NODE_T { CHAR_SET, BRANCH, SEQUENCE, LOOP, FUNCTION };

struct RegexExprNodeBase
{
private:
		size_t			m_ref;
		EXPR_NODE_T		m_type;
public:
		RegexExprNodeBase(EXPR_NODE_T t) : m_type(t), m_ref(1)
		{

		}

		virtual ~RegexExprNodeBase() = 0 {   }

		void AddRef() { m_ref++;}
		void Release()
		{
				if(--m_ref == 0)
				{
						delete this;
				}
		}

		
public:
		virtual void TestNode() = 0;
private:
		RegexExprNodeBase& operator=(const RegexExprNodeBase &other);
		RegexExprNodeBase(const RegexExprNodeBase &other);
};


/*******************************************************************************/

class CharSet : public RegexExprNodeBase
{

private:
		CharSet& operator=(const CharSet &other);
		CharSet(const CharSet &other);
public:
		enum MATCH_T { BEGIN, END, NORMAL};

		struct Range
		{
				wchar_t			beg;
				wchar_t			end;
		
				Range(wchar_t b, wchar_t e) : beg(b), end(e) { }
				~Range() { }

				void Reset(wchar_t b, wchar_t e)
				{
						beg = b; end = e;
						assert(b <= e);
				}
		};

		typedef std::list<Range>		RangeList;
private:
		MATCH_T			m_type;
		RangeList		m_list;
		bool			m_is_nega;
public:
		CharSet(MATCH_T			type, bool is_nega) 
				: RegexExprNodeBase(CHAR_SET)
				, m_type(type)
				, m_is_nega(is_nega)
		{

		}
		virtual ~CharSet() {   }

		
private:
		bool	__is_include(wchar_t  c)const
		{
				for(RangeList::const_iterator cit = m_list.begin(); cit != m_list.end(); ++cit)
				{
						if(cit->beg <= c && cit->end >= c)return true;
				}
				return false;
		}
public:
		void Insert(const Range &range);
		
		bool	IsInclude(wchar_t  c)const
		{
				bool res = __is_include(c);
				
				return m_is_nega ? !res : res;
		}

		MATCH_T	GetMatchType()const { return m_type;}

public:
		virtual void TestNode()
		{
				for(RangeList::iterator it = m_list.begin(); it != m_list.end(); ++it)
				{
						printf("beg == %d : end == %d\n", it->beg, it->end);
				}
		}

};


/*****************************************************************************/

#if(0)
Expr			-> Branch{1,}
Branch			-> (Sequence '|' Sequence)
Sequence		-> Repeat|Simple 
Repeat			-> Simple | Simple ['?', '*', '+'] | Simple '{' __RepeatCount '}'
__RepeatCount	-> digit |digit ','| digit ',' digit
Simple			-> token |  '(' Expr ')'| '(''?' FunctionExpr ')' | '^' | '$'
FunctionExpr	-> __Pattern | :'__Pattern | '='__Pattern | '!'__Pattern
__Pattern		-> Expr | '<' name '>' | '<''#'name'>'Expr | '<''$'name'>'Expr

digit			->[0-9]
token			->[^ digit, ^,$, {,},[,],<,>, (,),?,+,-,*,|,\\, }


#endif


typedef std::list<RegexExprNodeBase*>	ExprList;
typedef ExprList::iterator				Iterator;
typedef ExprList::const_iterator		CIterator;

class RegexExprSection : public RegexExprNodeBase
{
public:
		
private:
		ExprList		m_list;
public:
		Iterator	Begin();
		Iterator	End();

		CIterator	Begin()const;
		CIterator	End()const;
public:
		RegexExprSection(EXPR_NODE_T type) : RegexExprNodeBase(type)
		{


		}

		virtual ~RegexExprSection() = 0
		{
				for(Iterator it = m_list.begin(); it != m_list.end(); ++it)
				{
						(*it)->Release();
				}

				m_list.clear();

		}

public:
		void Insert(RegexExprNodeBase *node)
		{
				m_list.push_back(node);
		}
		
		size_t	Count()const { return m_list.size(); }

		bool	Empty()const { return Count() == 0;}

private:
		RegexExprSection& operator=(const RegexExprSection &other);
		RegexExprSection(const RegexExprSection &other);
};

class RegexExprBranch : public RegexExprSection 
{
public:
		RegexExprBranch() : RegexExprSection(BRANCH) { }
		virtual ~RegexExprBranch() {  }

};

class RegexExprSequence : public RegexExprSection 
{
public:
		RegexExprSequence() : RegexExprSection(SEQUENCE) { }
		virtual ~RegexExprSequence() {  }

};



/*****************************************************************************/

class RegexLoop : public RegexExprNodeBase
{
private:
		RegexExprNodeBase		*m_expr;
		size_t					m_min;
		size_t					m_max;
		bool					m_finite;
		bool					m_greedy;
public:
		RegexLoop(RegexExprNodeBase	*expr, size_t min, size_t max, bool finite = false , bool greedy = true)
				: RegexExprNodeBase(LOOP)
				, m_expr(expr)
				, m_min(min)
				, m_max(max)
				, m_finite(finite)
				, m_greedy(greedy)
		{
				assert(m_expr != NULL);
				assert(m_min <= m_max);

		}

		virtual ~RegexLoop()
		{
				m_expr->Release();
		}

public:
		bool IsFinite()const { return m_finite;}
		bool IsGreedy()const { return m_greedy;}
		size_t GetMin()const { return m_min;}
		size_t GetMax()const { return m_max; }
		RegexExprNodeBase* GetExpr() { return m_expr; }
};



/************************************************************************************/





class FunctionExpr : public RegexExprNodeBase
{
public:
		enum FUNC_T { CAPTURE, UNCAPTURE, POSITIVE, NEGATIVE };

		enum CONTENT_T { NORMAL_EXPR,  NAMED_EXPR, REF_EXPR, STORE_MATCHED, COMPARE_MATCHED };

private:
		FUNC_T					m_func_type;
		CONTENT_T				m_content_type;
		std::wstring			m_name;
		RegexExprNodeBase		*m_ref;
		RegexExprNodeBase		*m_expr;
public:
		FunctionExpr( FUNC_T f
				, CONTENT_T c
				, const std::wstring &name
				, RegexExprNodeBase	*ref
				, RegexExprNodeBase	*expr
				) 
				: RegexExprNodeBase(FUNCTION)
				, m_func_type(f)
				, m_content_type(c)
				, m_name(name)
				, m_ref(ref)
				, m_expr(expr)
		{


		}

		virtual ~FunctionExpr()
		{
				if(m_ref != NULL)m_ref->Release();
				if(m_expr != NULL)m_expr->Release();
		}

public:
		FUNC_T			GetFuncType()const { return m_func_type;}
		CONTENT_T		GetContentType()const { return m_content_type;}
		const std::wstring& GetName()const { return m_name;}
		
		RegexExprNodeBase* GetRefExpr(){return m_ref;}
		RegexExprNodeBase* GetExpr(){return m_expr;}

};		








#if(0)


class RegexExprNode
{
private:
		RegexExprNodeBase		*m_impl;

public:
		RegexExprNode(RegexExprNodeBase		*_impl) : m_impl(_impl)
		{


		}

		RegexExprNode(const RegexExprNode &other) : m_impl(other.m_impl)
		{
				
		}

		virtual ~RegexExprNode() = 0
		{
				m_impl->Release();
		}

		RegexExprNode& operator=(const RegexExprNode &other)
		{
				if(this != &other)
				{
						m_impl->Release();
						other.m_impl->AddRef();
						m_impl = other.m_impl;
				}
				return *this;
		}
public:
		RegexExprNodeBase*	GetNode() { return m_impl;}


};

#endif





