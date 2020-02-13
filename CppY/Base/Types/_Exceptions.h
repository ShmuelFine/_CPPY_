#pragma once

#include "object.h"
#include "pyExceptions.h"

#define DefException(ExceptionName, BaseClass)\
	class ExceptionName : public BaseClass\
	{																		\
	public:																	\
		static const ExceptionTypes Type = ExceptionTypes:: ##ExceptionName;	\
																			\
	public:																	\
		ExceptionName(PyBaseException const& backendType);					\
	};


namespace py
{
	DefException(BaseException, object);
	/**/DefException(SystemExit			, BaseException);
	/**/DefException(KeyboardInterrupt	, BaseException);
	/**/DefException(GeneratorExit		, BaseException);
	/**/DefException(Exception			, BaseException);
	/**//**/DefException(StopIteration		,		Exception);
	/**//**/DefException(StopAsyncIteration	,		Exception);
	/**//**/DefException(ArithmeticError	,		Exception);
	/**//**//**/DefException(FloatingPointError, ArithmeticError);
	/**//**//**/DefException(OverflowError		, ArithmeticError);
	/**//**//**/DefException(ZeroDivisionError	, ArithmeticError);
	/**//**/DefException(AssertionError,		Exception);
	/**//**/DefException(AttributeError,		Exception);
	/**//**/DefException(BufferError   ,		Exception);
	/**//**/DefException(EOFError	   ,		Exception);
	/**//**/DefException(ImportError   ,		Exception);
	/**//**//**/DefException(ModuleNotFoundError, ImportError);
	/**//**/ DefException(LookupError	,		Exception);
	/**//**//**/DefException(IndexError			, ImportError);
	/**//**//**/DefException(KeyError			, ImportError);
	/**//**/ DefException(MemoryError,		Exception);
	/**//**/ DefException(NameError	 ,		Exception);
	/**//**//**/DefException(UnboundLocalError, NameError);
	/**//**/ DefException(OSError, Exception);
	/**//**//**/DefException(BlockingIOError	, OSError);
	/**//**//**/DefException(ChildProcessError	, OSError);
	/**//**//**/DefException(ConnectionError	, OSError);
	/**//**//**//**/DefException(BrokenPipeError		,ConnectionError);
	/**//**//**//**/DefException(ConnectionAbortedError	,ConnectionError);
	/**//**//**//**/DefException(ConnectionRefusedError	,ConnectionError);
	/**//**//**//**/DefException(ConnectionResetError	,ConnectionError);
	/**//**//**/DefException(FileExistsError	, OSError);
	/**//**//**/DefException(FileNotFoundError	, OSError);
	/**//**//**/DefException(InterruptedError	, OSError);
	/**//**//**/DefException(IsADirectoryError	, OSError);
	/**//**//**/DefException(NotADirectoryError	, OSError);
	/**//**//**/DefException(PermissionError	, OSError);
	/**//**//**/DefException(ProcessLookupError	, OSError);
	/**//**//**/DefException(TimeoutError		, OSError);
	/**//**/ DefException(ReferenceError, Exception);
	/**//**/ DefException(RuntimeError	, Exception);
	/**//**//**/DefException(NotImplementedError, RuntimeError)
	/**//**//**/DefException(RecursionError		, RuntimeError)
	/**//**/ DefException(SyntaxError, Exception);
	/**//**//**/DefException(IndentationError, SyntaxError)
	/**//**//**//**/DefException(TabError, IndentationError)
	/**//**/ DefException(SystemError, Exception);
	/**//**/ DefException(TypeError	 , Exception);
	/**//**/ DefException(ValueError , Exception);
	/**//**//**/DefException(UnicodeError, ValueError);
	/**//**//**//**/ DefException(UnicodeDecodeError, UnicodeError);
	/**//**//**//**/ DefException(UnicodeEncodeError, UnicodeError);
	/**//**//**//**/ DefException(UnicodeTranslateError, UnicodeError);
	/**//**/ DefException(Warning, Exception);
	/**//**//**/ DefException(DeprecationWarning, Warning);
	/**//**//**/ DefException(PendingDeprecationWarning, Warning);
	/**//**//**/ DefException(RuntimeWarning		   , Warning);
	/**//**//**/ DefException(SyntaxWarning			   , Warning);
	/**//**//**/ DefException(UserWarning			   , Warning);
	/**//**//**/ DefException(FutureWarning			   , Warning);
	/**//**//**/ DefException(ImportWarning			   , Warning);
	/**//**//**/ DefException(UnicodeWarning		   , Warning);
	/**//**//**/ DefException(BytesWarning			   , Warning);
	/**//**//**/ DefException(ResourceWarning		   , Warning);
}