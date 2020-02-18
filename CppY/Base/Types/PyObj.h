#pragma once
#include <string>
#include <memory>
#include <map>
#include "ComparisonUtils.h"
#include "booleanOperators.h"

#ifndef __FUNCTION_NAME__
#ifdef WIN32   //WINDOWS
#define __FUNCTION_NAME__   __FUNCTION__  
#else          //*NIX
#define __FUNCTION_NAME__   __func__ 
#endif
#endif

namespace py
{
	class pyObj;
	class pyObjPtr : public std::shared_ptr<pyObj>
	{
	public:
		bool IsAssignable;

		std::map<std::string, pyObjPtr> attributes;

	public:
		pyObjPtr() : std::shared_ptr<pyObj>() { IsAssignable = true; }
		pyObjPtr(pyObj* px) : std::shared_ptr<pyObj>(px) { IsAssignable = true; }
		template <typename T>
		pyObjPtr(std::shared_ptr<T> const& other) : std::shared_ptr<pyObj>(other) { IsAssignable = true; }

		pyObjPtr& operator =(pyObjPtr const& other);
		bool operator == (pyObjPtr const& other) const;
	};
};

namespace py
{

	class pyObjIterator
	{
	protected:
		int currIdx;
		pyObj* iterable;

	public:
		pyObjIterator(pyObj* container, int startIdx);
		pyObjIterator& operator ++();
		bool operator != (pyObjIterator const& other);
		bool operator == (pyObjIterator const& other);
		pyObjPtr operator *();
	};


	class pyObj
	{
	public:
		virtual operator std::string() const = 0;
		virtual operator double() const = 0;
		virtual operator float() const = 0;
		virtual operator int() const = 0;
		virtual operator bool() const = 0;
		virtual operator uint8_t() const;

		virtual pyObjPtr& operator [](pyObjPtr const& key) = 0;

		virtual pyObjPtr FetchByIdx(int idx) const = 0;
		virtual pyObjIterator begin() const = 0;
		virtual pyObjIterator end() const = 0;

		virtual pyObjPtr operator()(pyObj& params) const = 0;

		virtual std::string Type() const = 0;

		virtual pyObjPtr Clone() const = 0;

		// for using as key in sets, maps, etc:
		virtual bool operator <(pyObj const& other) const = 0;
		virtual bool operator == (pyObj const& other) const { return !(*this < other || other < *this); }
		virtual bool operator != (pyObj const& other) const { return (*this < other || other < *this); }

		virtual pyObjPtr  operator ++(int) = 0;
		virtual pyObj& operator ++() = 0;
		virtual pyObj& operator += (pyObj const& other) = 0;


		virtual pyObjPtr  operator +(pyObj const& other)  const;
		virtual pyObjPtr  operator -(pyObj const& other)  const;
		virtual pyObjPtr  operator *(pyObj const& other)  const;
		virtual pyObjPtr  operator /(pyObj const& other) const;
	};

}

