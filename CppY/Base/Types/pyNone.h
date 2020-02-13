#pragma once
#include "PyObj.h"
#include "pyExceptions.h"
#include <memory>

namespace py
{
	class pyNone : public pyObj
	{
	public:
		// Inherited via pyObj
		virtual operator std::string() const override;

		virtual operator double() const override;
		virtual operator float() const override;
		virtual operator int() const override;
		virtual operator bool() const override;

		virtual pyObjPtr & operator [](pyObjPtr const & key) {NOT_SUPPORTED;}
		virtual pyObjPtr FetchByIdx(int idx) const { NOT_SUPPORTED; }
		virtual pyObjIterator begin() const {NOT_SUPPORTED;}
		virtual pyObjIterator end() const {NOT_SUPPORTED;}
		virtual pyObjPtr operator()(pyObj& params) const {NOT_SUPPORTED;}

		virtual std::string Type() const override;

		virtual bool operator<(pyObj const& other) const override;

		virtual pyObjPtr Clone() const override;

		virtual pyObjPtr  operator ++(int) {NOT_SUPPORTED;}
		virtual pyObj& operator ++() {NOT_SUPPORTED;}
		virtual pyObj& operator += (pyObj const& other) {NOT_SUPPORTED;}


	};


}
