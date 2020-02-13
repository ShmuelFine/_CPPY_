#pragma once

#include "PyObj.h"
#include <string>
#include <map>

namespace py
{
	// Exceptions are special in the sense that they can be thrown from the backend
	// and should be translated to object in the frontend when caught.

	enum class ExceptionTypes 
	{	
		BaseException
		,Exception
		,ArithmeticError	
		,BufferError		
		,LookupError		
		
		//Concrete exceptions
		
		,AssertionError	   
		,AttributeError	   
		,EOFError		   
		,FloatingPointError 
		,GeneratorExit	   
		,ImportError		   
		,ModuleNotFoundError
		,IndexError		   
		,KeyError		   
		,KeyboardInterrupt  
		,MemoryError		   
		,NameError		
		,NotImplementedError
		,OSError
		,OverflowError
		,RecursionError
		,ReferenceError
		,RuntimeError
		,StopIteration
		,StopAsyncIteration
		,SyntaxError
		,IndentationError
		,TabError
		,SystemError
		,SystemExit
		,TypeError
		,UnboundLocalError
		,UnicodeError
		,UnicodeEncodeError
		,UnicodeDecodeError
		,UnicodeTranslateError
		,ValueError
		,ZeroDivisionError
		,EnvironmentError
		,IOError
		,WindowsError

		// OS exceptions
		
		,BlockingIOError
		,ChildProcessError
		,ConnectionError
		,BrokenPipeError
		,ConnectionAbortedError
		,ConnectionRefusedError
		,ConnectionResetError
		,FileExistsError
		,FileNotFoundError
		,InterruptedError
		,IsADirectoryError
		,NotADirectoryError
		,PermissionError
		,ProcessLookupError
		,TimeoutError

		// Warnings

		,Warning
		,UserWarning
		,DeprecationWarning
		,PendingDeprecationWarning
		,SyntaxWarning
		,RuntimeWarning
		,FutureWarning
		,ImportWarning
		,UnicodeWarning
		,BytesWarning
		,ResourceWarning
	};

	class PyBaseException : public pyObj
	{
	public:
		std::string What;
		ExceptionTypes ErrType;
		std::map<std::string, std::string> Props;

	public:
		PyBaseException(std::string const& err, 
			ExceptionTypes type = ExceptionTypes::Exception,
			std::map<std::string, std::string> const & props = std::map<std::string, std::string>())
			: What(err)
			, ErrType(type)
			, Props(props)
		{};

		// Inherited via pyObj
		virtual operator std::string() const override { return What; }
		virtual operator double() const override;
		virtual operator float() const override;
		virtual operator int() const override;
		virtual operator bool() const override;
		virtual pyObjPtr& operator[](pyObjPtr const& key) override;
		virtual pyObjPtr FetchByIdx(int idx) const override;
		virtual pyObjIterator begin() const override;
		virtual pyObjIterator end() const override;
		virtual pyObjPtr operator()(pyObj& params) const override;
		virtual std::string Type() const override;
		virtual pyObjPtr Clone() const override;
		virtual bool operator<(pyObj const& other) const override;
		virtual pyObjPtr operator++(int) override;
		virtual pyObj& operator++() override;
		virtual pyObj& operator+=(pyObj const& other) override;
	};



}

#define NOT_SUPPORTED \
	throw PyBaseException(\
		std::string("The operation ") + __FUNCTION_NAME__  + " is not supported for " + Type()\
		, ExceptionTypes::NotImplementedError)

#define NOT_SUPPORTED_PAIR(other) {\
throw PyBaseException(\
	std::string("The operation ") + __FUNCTION_NAME__  + " is not supported for " + Type() + " and " + other.Type()\
	, ExceptionTypes::NotImplementedError); }

#define NOT_IMPL throw PyBaseException("Not Implemented");

#define THROW(msg) throw PyBaseException(msg)
#define THROW_IF(x, msg) if (x) THROW(msg)
#define THROW_UNLESS(x, msg) if (!(x)) THROW(msg)

