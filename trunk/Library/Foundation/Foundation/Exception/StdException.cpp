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
#include "StdException.h"

namespace ExceptionSpace {



___IMPL_EXPT_CLASS(LogicException, Exception)

___IMPL_EXPT_CLASS(RuntimeException, Exception)


___IMPL_EXPT_CLASS(BugCheckException, LogicException)
___IMPL_EXPT_CLASS(IllegalStateException, LogicException)
___IMPL_EXPT_CLASS(InvalidAccessException, LogicException)
___IMPL_EXPT_CLASS(InvalidArgumentException, LogicException)
___IMPL_EXPT_CLASS(NotImplementedException, LogicException)
___IMPL_EXPT_CLASS(AssertionViolationException, LogicException)
___IMPL_EXPT_CLASS(NullPointerException, LogicException)
___IMPL_EXPT_CLASS(RangeException, LogicException)

___IMPL_EXPT_CLASS(NotFoundException, RuntimeException)
___IMPL_EXPT_CLASS(ExistsException, RuntimeException)
___IMPL_EXPT_CLASS(SystemException, RuntimeException)
___IMPL_EXPT_CLASS(IOException, RuntimeException)
___IMPL_EXPT_CLASS(NetException, IOException)
___IMPL_EXPT_CLASS(DataException, RuntimeException)

___IMPL_EXPT_CLASS(FileException, IOException)

___IMPL_EXPT_CLASS(SyntaxException, DataException)
___IMPL_EXPT_CLASS(PathSyntaxException, SyntaxException)
#if(0)

___IMPL_EXPT_CLASS(AssertionViolationException, LogicException)
___IMPL_EXPT_CLASS(NullPointerException, LogicException)
___IMPL_EXPT_CLASS(BugcheckException, LogicException)
___IMPL_EXPT_CLASS(InvalidArgumentException, LogicException)
___IMPL_EXPT_CLASS(NotImplementedException, LogicException)
___IMPL_EXPT_CLASS(RangeException, LogicException)
___IMPL_EXPT_CLASS(IllegalStateException, LogicException)
___IMPL_EXPT_CLASS(InvalidAccessException, LogicException)
___IMPL_EXPT_CLASS(SignalException, LogicException)
___IMPL_EXPT_CLASS(UnhandledException, LogicException)
___IMPL_EXPT_CLASS(TimeoutException, RuntimeException)
___IMPL_EXPT_CLASS(NotFoundException, RuntimeException)
___IMPL_EXPT_CLASS(ExistsException, RuntimeException)
___IMPL_EXPT_CLASS(TimeoutException, RuntimeException)
___IMPL_EXPT_CLASS(SystemException, RuntimeException)
___IMPL_EXPT_CLASS(RegularExpressionException, RuntimeException)
___IMPL_EXPT_CLASS(LibraryLoadException, RuntimeException)
___IMPL_EXPT_CLASS(LibraryAlreadyLoadedException, RuntimeException)
___IMPL_EXPT_CLASS(NoThreadAvailableException, RuntimeException)
___IMPL_EXPT_CLASS(PropertyNotSupportedException, RuntimeException)
___IMPL_EXPT_CLASS(PoolOverflowException, RuntimeException)
___IMPL_EXPT_CLASS(NoPermissionException, RuntimeException)
___IMPL_EXPT_CLASS(OutOfMemoryException, RuntimeException)
___IMPL_EXPT_CLASS(DataException, RuntimeException)

___IMPL_EXPT_CLASS(DataFormatException, DataException)
___IMPL_EXPT_CLASS(SyntaxException, DataException)
___IMPL_EXPT_CLASS(CircularReferenceException, DataException)
___IMPL_EXPT_CLASS(PathSyntaxException, SyntaxException)
___IMPL_EXPT_CLASS(IOException, RuntimeException)
___IMPL_EXPT_CLASS(FileException, IOException)
___IMPL_EXPT_CLASS(FileExistsException, FileException)
___IMPL_EXPT_CLASS(FileNotFoundException, FileException)
___IMPL_EXPT_CLASS(PathNotFoundException, FileException)
___IMPL_EXPT_CLASS(FileReadOnlyException, FileException)
___IMPL_EXPT_CLASS(FileAccessDeniedException, FileException)
___IMPL_EXPT_CLASS(CreateFileException, FileException)
___IMPL_EXPT_CLASS(OpenFileException, FileException)
___IMPL_EXPT_CLASS(WriteFileException, FileException)
___IMPL_EXPT_CLASS(ReadFileException, FileException)
___IMPL_EXPT_CLASS(UnknownURISchemeException, RuntimeException)

___IMPL_EXPT_CLASS(ApplicationException, Exception)
___IMPL_EXPT_CLASS(BadCastException, RuntimeException)


#endif







}