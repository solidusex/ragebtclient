
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

#include "Exception.h"




namespace ExceptionSpace {

//___DECL_EXPT_CLASS
//___IMPL_EXPT_CLASS


#if defined(___DECL_EXPT_CLASS) || defined(___IMPL_EXPT_CLASS)

#undef ___DECL_EXPT_CLASS
#undef ___IMPL_EXPT_CLASS

#endif

#define ___DECL_EXPT_CLASS(_CALL_API, _CLS, _BASE)		\
class _CALL_API _CLS : public _BASE						\
{														\
public:													\
		_CLS();											\
		_CLS(const std::string& msg);					\
		_CLS(const std::string& msg, t_ulong code);		\
		_CLS(const _CLS &other);						\
		virtual ~_CLS() throw();						\
		_CLS& operator= (const _CLS& other);			\
public:													\
		virtual const char* ClassName() const throw();	\
		virtual const char* what() const throw();		\
		virtual _CLS* Clone() const;					\
		virtual void ReThrow() const;					\
};



#define ___IMPL_EXPT_CLASS(_CLS, _BASE)									\
_CLS::_CLS()															\
{																		\
}																		\
_CLS::_CLS(const std::string& msg) : _BASE(msg)							\
{																		\
}																		\
																		\
_CLS::_CLS(const std::string& msg, t_ulong code) : _BASE(msg, code)		\
{																		\
}																		\
																		\
																		\
_CLS::~_CLS() throw()													\
{																		\
}																		\
																		\
_CLS& _CLS::operator=(const _CLS &other)								\
{																		\
																		\
		if(this != &other)												\
		{																\
				_BASE::operator=(other);								\
		}																\
		return *this;													\
}																		\
_CLS::_CLS(const _CLS &other)											\
{																		\
		*this = other;													\
}																		\
																		\
const char* _CLS::ClassName() const throw()								\
{																		\
		return typeid(*this).name();									\
																		\
}																		\
																		\
_CLS* _CLS::Clone() const												\
{																		\
		return (new _CLS(*this));										\
}																		\
																		\
void _CLS::ReThrow() const												\
{																		\
		throw *this;													\
}																		\
																		\
const char* _CLS::what() const throw()									\
{																		\
		return _BASE::what();											\
}




___DECL_EXPT_CLASS(FoundationAPI, LogicException, Exception)
___DECL_EXPT_CLASS(FoundationAPI, RuntimeException, Exception)


___DECL_EXPT_CLASS(FoundationAPI, BugCheckException, LogicException)
___DECL_EXPT_CLASS(FoundationAPI, IllegalStateException, LogicException)
___DECL_EXPT_CLASS(FoundationAPI, InvalidAccessException, LogicException)
___DECL_EXPT_CLASS(FoundationAPI, InvalidArgumentException, LogicException)
___DECL_EXPT_CLASS(FoundationAPI, NotImplementedException, LogicException)
___DECL_EXPT_CLASS(FoundationAPI, AssertionViolationException, LogicException)
___DECL_EXPT_CLASS(FoundationAPI, NullPointerException, LogicException)
___DECL_EXPT_CLASS(FoundationAPI, RangeException, LogicException)

___DECL_EXPT_CLASS(FoundationAPI, NotFoundException, RuntimeException)
___DECL_EXPT_CLASS(FoundationAPI, ExistsException, RuntimeException)
___DECL_EXPT_CLASS(FoundationAPI, SystemException, RuntimeException)
___DECL_EXPT_CLASS(FoundationAPI, IOException, RuntimeException)
___DECL_EXPT_CLASS(FoundationAPI, FileException, IOException)
___DECL_EXPT_CLASS(FoundationAPI, NetException, IOException)

___DECL_EXPT_CLASS(FoundationAPI, DataException, RuntimeException)
___DECL_EXPT_CLASS(FoundationAPI, SyntaxException, DataException)
___DECL_EXPT_CLASS(FoundationAPI, PathSyntaxException, SyntaxException)

#if(0)

___DECL_EXPT_CLASS(AssertionViolationException, LogicException)
___DECL_EXPT_CLASS(NullPointerException, LogicException)
___DECL_EXPT_CLASS(RangeException, LogicException)
___DECL_EXPT_CLASS(RangeException, LogicException)

___DECL_EXPT_CLASS(SignalException, LogicException)
___DECL_EXPT_CLASS(UnhandledException, LogicException)

___DECL_EXPT_CLASS(RegularExpressionException, RuntimeException)
___DECL_EXPT_CLASS(LibraryLoadException, RuntimeException)
___DECL_EXPT_CLASS(LibraryAlreadyLoadedException, RuntimeException)
___DECL_EXPT_CLASS(PropertyNotSupportedException, RuntimeException)
___DECL_EXPT_CLASS(PoolOverflowException, RuntimeException)
___DECL_EXPT_CLASS(NoPermissionException, RuntimeException)
___DECL_EXPT_CLASS(OutOfMemoryException, RuntimeException)
___DECL_EXPT_CLASS(TimeoutException, RuntimeException)

___DECL_EXPT_CLASS(DataFormatException, DataException)
___DECL_EXPT_CLASS(CircularReferenceException, DataException)



___DECL_EXPT_CLASS(FileExistsException, FileException)
___DECL_EXPT_CLASS(FileNotFoundException, FileException)
___DECL_EXPT_CLASS(PathNotFoundException, FileException)
___DECL_EXPT_CLASS(FileReadOnlyException, FileException)
___DECL_EXPT_CLASS(FileAccessDeniedException, FileException)
___DECL_EXPT_CLASS(CreateFileException, FileException)
___DECL_EXPT_CLASS(OpenFileException, FileException)
___DECL_EXPT_CLASS(WriteFileException, FileException)
___DECL_EXPT_CLASS(ReadFileException, FileException)
___DECL_EXPT_CLASS(UnknownURISchemeException, RuntimeException)

___DECL_EXPT_CLASS(ApplicationException, Exception)
___DECL_EXPT_CLASS(BadCastException, RuntimeException)

#endif








}