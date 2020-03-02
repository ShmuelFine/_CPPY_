#pragma once

#include "PyObj.h"
#include "pyExceptions.h"
#include "ComparisonUtils.h"
#include <sstream>
#include <iomanip>
namespace secretPy {
	const std::string secretKeyword = "NUMERIC_";
}
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

		virtual std::string Type() const override { return  secretPy::secretKeyword + typeid(T).name(); }
		virtual pyObjPtr Clone() const override { return std::make_shared<pyNumType<T>>(val); }
		virtual bool operator <(pyObj const& other) const override {
			std::string othersType = other.Type();
			if (othersType.compare(0, secretPy::secretKeyword.length(), secretPy::secretKeyword) == 0)
			{
				T othersVal = (T)other;
				return val < othersVal;
			}
			else
				EXPLICIT_COMPARE(Type(), othersType);

			return false;
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

	//typedef pyNumType<int> pyInt;  //has an implementation at line 116
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
		virtual operator uint8_t() const	override { return (uint8_t)val; }

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

		virtual operator std::string() const override;

	};

	class pyInt : public pyNumType<int>
	{
	public:
		pyInt(int const& i) :pyNumType<int>(i) {};
		pyInt() :pyNumType<int>() {};

		std::string bin();
	};
}
