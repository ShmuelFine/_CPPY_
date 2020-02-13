#pragma once

#include "pyObj.h"

namespace py
{

	class pyFunc : public pyObj
	{
	public:

		// Inherited via pyObj
		virtual operator std::string() const override;
		virtual operator double() const override;
		virtual operator float() const override;
		virtual operator int() const override;
		virtual operator bool() const override;

		virtual pyObjPtr & operator [](pyObjPtr const & key) override;
		virtual pyObjPtr FetchByIdx(int idx) const;
		virtual pyObjIterator begin() const override;
		virtual pyObjIterator end() const override;
		
		virtual pyObjPtr operator()(pyObj& params) const = 0;
		virtual std::string Type() const = 0;
		virtual pyObjPtr Clone() const  = 0;

		virtual bool operator<(pyObj const& other) const override;
		virtual pyObjPtr operator++(int) override;
		virtual pyObj& operator++() override;
		virtual pyObj& operator+=(pyObj const& other) override;

	};





}