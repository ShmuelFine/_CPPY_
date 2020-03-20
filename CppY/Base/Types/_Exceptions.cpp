#include "_Exceptions.h"
#include "FunDefs.h"

#define SIMPLE_IMPL(ExceptionName, base) \
ExceptionName::ExceptionName(PyBaseException const& backendType) : base(backendType) {}

namespace py
{
	FUN_DEF(with_traceback);
	PARAM(tb,);
	// TODO. Look in CPython.
	END_FUN(with_traceback)

	// TODO: Where is this used? Where is the translation happens?
	BaseException::BaseException(PyBaseException const& backendType)
	{
		attr(args) = object(pyList());
		attr(with_traceback) = with_traceback;
		get()->attributes = backendType.attributes;
	}

	/**/SIMPLE_IMPL(SystemExit			, BaseException);
	/**/SIMPLE_IMPL(KeyboardInterrupt	, BaseException);
	/**/SIMPLE_IMPL(GeneratorExit		, BaseException);
	/**/SIMPLE_IMPL(Exception			, BaseException);
	/**//**/SIMPLE_IMPL(StopIteration		,		Exception);
	/**//**/SIMPLE_IMPL(StopAsyncIteration	,		Exception);
	/**//**/SIMPLE_IMPL(ArithmeticError	,		Exception);
	/**//**//**/SIMPLE_IMPL(FloatingPointError, ArithmeticError);
	/**//**//**/SIMPLE_IMPL(OverflowError		, ArithmeticError);
	/**//**//**/SIMPLE_IMPL(ZeroDivisionError	, ArithmeticError);
	/**//**/SIMPLE_IMPL(AssertionError,		Exception);
	/**//**/SIMPLE_IMPL(AttributeError,		Exception);
	/**//**/SIMPLE_IMPL(BufferError   ,		Exception);
	/**//**/SIMPLE_IMPL(EOFError	   ,		Exception);
	/**//**/SIMPLE_IMPL(ImportError   ,		Exception);
	/**//**//**/SIMPLE_IMPL(ModuleNotFoundError, ImportError);
	/**//**/ SIMPLE_IMPL(LookupError	,		Exception);
	/**//**//**/SIMPLE_IMPL(IndexError			, ImportError);
	/**//**//**/SIMPLE_IMPL(KeyError			, ImportError);
	/**//**/ SIMPLE_IMPL(MemoryError,		Exception);
	/**//**/ SIMPLE_IMPL(NameError	 ,		Exception);
	/**//**//**/SIMPLE_IMPL(UnboundLocalError, NameError);
	/**//**/ SIMPLE_IMPL(OSError, Exception)
	/**//**//**/SIMPLE_IMPL(BlockingIOError	, OSError);
	/**//**//**/SIMPLE_IMPL(ChildProcessError	, OSError);
	/**//**//**/SIMPLE_IMPL(ConnectionError	, OSError);
	/**//**//**//**/SIMPLE_IMPL(BrokenPipeError		,ConnectionError);
	/**//**//**//**/SIMPLE_IMPL(ConnectionAbortedError	,ConnectionError);
	/**//**//**//**/SIMPLE_IMPL(ConnectionRefusedError	,ConnectionError);
	/**//**//**//**/SIMPLE_IMPL(ConnectionResetError	,ConnectionError);
	/**//**//**/SIMPLE_IMPL(FileExistsError	, OSError);
	/**//**//**/SIMPLE_IMPL(FileNotFoundError	, OSError);
	/**//**//**/SIMPLE_IMPL(InterruptedError	, OSError);
	/**//**//**/SIMPLE_IMPL(IsADirectoryError	, OSError);
	/**//**//**/SIMPLE_IMPL(NotADirectoryError	, OSError);
	/**//**//**/SIMPLE_IMPL(PermissionError	, OSError);
	/**//**//**/SIMPLE_IMPL(ProcessLookupError	, OSError);
	/**//**//**/SIMPLE_IMPL(TimeoutError		, OSError);
	/**//**/ SIMPLE_IMPL(ReferenceError, Exception);
	/**//**/ SIMPLE_IMPL(RuntimeError	, Exception);
	/**//**//**/SIMPLE_IMPL(NotImplementedError, RuntimeError)
	/**//**//**/SIMPLE_IMPL(RecursionError		, RuntimeError)
	/**//**/ SIMPLE_IMPL(SyntaxError, Exception);
	/**//**//**/SIMPLE_IMPL(IndentationError, SyntaxError)
	/**//**//**//**/SIMPLE_IMPL(TabError, IndentationError)
	/**//**/ SIMPLE_IMPL(SystemError, Exception);
	/**//**/ SIMPLE_IMPL(TypeError	 , Exception);
	/**//**/ SIMPLE_IMPL(ValueError , Exception);
	/**//**//**/SIMPLE_IMPL(UnicodeError, ValueError);
	/**//**//**//**/ SIMPLE_IMPL(UnicodeDecodeError, UnicodeError);
	/**//**//**//**/ SIMPLE_IMPL(UnicodeEncodeError, UnicodeError);
	/**//**//**//**/ SIMPLE_IMPL(UnicodeTranslateError, UnicodeError);
	/**//**/ SIMPLE_IMPL(Warning, Exception);
	/**//**//**/ SIMPLE_IMPL(DeprecationWarning, Warning);
	/**//**//**/ SIMPLE_IMPL(PendingDeprecationWarning, Warning);
	/**//**//**/ SIMPLE_IMPL(RuntimeWarning		   , Warning);
	/**//**//**/ SIMPLE_IMPL(SyntaxWarning			   , Warning);
	/**//**//**/ SIMPLE_IMPL(UserWarning			   , Warning);
	/**//**//**/ SIMPLE_IMPL(FutureWarning			   , Warning);
	/**//**//**/ SIMPLE_IMPL(ImportWarning			   , Warning);
	/**//**//**/ SIMPLE_IMPL(UnicodeWarning		   , Warning);
	/**//**//**/ SIMPLE_IMPL(BytesWarning			   , Warning);
	/**//**//**/ SIMPLE_IMPL(ResourceWarning		   , Warning);

}
