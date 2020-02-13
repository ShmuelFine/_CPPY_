#pragma once
#pragma once

#include "PyObj.h"
#include "pyExceptions.h"
#include "ComparisonUtils.h"
#include <sstream>
#include <iomanip>
namespace py
{

	template<typename T>
	class pyNumType : public pyObj
	{
	protected:
		T val;

	public:
		pyNumType(T const& i) :val(i) {};
		pyNumType() :val(T(0)) {};

		virtual operator std::string() const { return std::to_string(val); }
		virtual operator double() const override { return (double)val; }
		virtual operator float() const	override { return (float)val; }
		virtual operator int() const 	override { return (int)val; }
		virtual operator bool() const	override { return (bool)val; }
		virtual operator uint8_t() const	override { return (uint8_t)val; }

		virtual pyObjPtr& operator [](pyObjPtr const& key) { NOT_SUPPORTED; }
		virtual pyObjPtr FetchByIdx(int idx) const { NOT_SUPPORTED; }
		virtual pyObjIterator begin() const { NOT_SUPPORTED; }
		virtual pyObjIterator end() const { NOT_SUPPORTED; }
		virtual pyObjPtr operator()(pyObj& params) const { NOT_SUPPORTED; }

		virtual std::string Type() const override { return typeid(T).name(); }
		virtual pyObjPtr Clone() const override { return std::make_shared<pyNumType<T>>(val); }
		virtual bool operator <(pyObj const& other) const override {
			EXPLICIT_COMPARE(Type(), other.Type());
			return val < (reinterpret_cast<pyNumType<T> const*>(&other))->val;
		}

		virtual pyObjPtr operator ++(int) override { auto result = *this; val++; return result.Clone(); }
		virtual pyObj& operator ++() override { val++; return *this; }
		virtual pyObj& operator += (pyObj const& other) override { val += (T)other; return *this; }

		bool operator == (T const& other) { return val == other; }

		operator T() { return val; }

		virtual pyObjPtr  operator +(pyObj const& other) const override { if (Type() == other.Type()) return decltype(*this)(val + (reinterpret_cast<pyNumType<T> const*>(&other))->val).Clone(); else NOT_SUPPORTED_PAIR(other); }
		virtual pyObjPtr  operator -(pyObj const& other) const override { if (Type() == other.Type()) return decltype(*this)(val - (reinterpret_cast<pyNumType<T> const*>(&other))->val).Clone(); else NOT_SUPPORTED_PAIR(other); }
		virtual pyObjPtr  operator *(pyObj const& other) const override { if (Type() == other.Type()) return decltype(*this)(val * (reinterpret_cast<pyNumType<T> const*>(&other))->val).Clone(); else NOT_SUPPORTED_PAIR(other); }
		virtual pyObjPtr  operator /(pyObj const& other) const override { if (Type() == other.Type()) return decltype(*this)(val / (reinterpret_cast<pyNumType<T> const*>(&other))->val).Clone(); else NOT_SUPPORTED_PAIR(other); }


	};

	typedef pyNumType<int> pyInt;
	typedef pyNumType<float> Float;
	typedef pyNumType<double> Double;

	class Bool : public pyObj
	{
	private:
		bool val;

	public:
		Bool(bool const& i) :val(i) {};
		Bool() :val(false) {};

		virtual operator std::string() const { return (val ? "True" : "False"); }
		virtual operator double() const override { return (double)val; }
		virtual operator float() const	override { return (float)val; }
		virtual operator int() const 	override { return (int)val; }
		virtual operator bool() const	override { return (bool)val; }

		virtual pyObjPtr& operator [](pyObjPtr const& key) { NOT_SUPPORTED; }
		virtual pyObjPtr FetchByIdx(int idx) const { NOT_SUPPORTED; }
		virtual pyObjIterator begin() const { NOT_SUPPORTED; }
		virtual pyObjIterator end() const { NOT_SUPPORTED; }
		virtual pyObjPtr operator()(pyObj& params) const { NOT_SUPPORTED; }

		virtual std::string Type() const override { return typeid(bool).name(); }
		virtual pyObjPtr Clone() const override { return std::make_shared<Bool>(val); }
		virtual bool operator <(pyObj const& other) const override {
			EXPLICIT_COMPARE(Type(), other.Type());
			return val < (reinterpret_cast<Bool const*>(&other))->val;
		}

		virtual pyObjPtr  operator ++(int) { NOT_SUPPORTED; }
		virtual pyObj& operator ++() { NOT_SUPPORTED; }
		virtual pyObj& operator += (pyObj const& other) { NOT_SUPPORTED; }

	};

	class pyByte : public pyNumType<uint8_t>
	{
	public:
		pyByte(unsigned char in) : pyNumType<uint8_t>(in) {};
		// @pos can be 0 or 1.
		// e.g. pyByte(250).GetHexDigit(0) -> F
		// e.g. pyByte(250).GetHexDigit(1) -> A
		char GetHexDigit(int pos) {
			//std::istringstream is(this[pos]);
			//char hexDigit;
			//is >> std::hex >> hexDigit;
			//return hexDigit;
			std::stringstream ss;
			ss << std::hex << val;
			return ss.str()[0];
		};

		//virtual operator std::string() const
		//{
		//	// TODO: Return "\xFA" = "\x" + GetHexDigit(0) + GetHexDigit(1);
		//	//val;
		//	//return std::to_string(val); 
		//	//throw;
		//}

	};
}
