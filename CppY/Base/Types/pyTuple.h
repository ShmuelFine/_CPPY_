#pragma once
#include "pyList.h"

namespace py
{
	// Tuple is an unextendable list:
	class pyTuple : public pyList
	{
	public:
		pyTuple();
		pyTuple(std::vector<object> const& values);
		pyTuple(std::pair<object, object> const& values);

		virtual std::string Type() const override;
		virtual pyObjPtr Clone() const override;
		// for using as key in sets, maps, etc:
		virtual bool operator <(pyObj const& other) const override;

		virtual pyObj& operator += (pyObj const& other) override { NOT_SUPPORTED_PAIR(other); }
		virtual pyObjPtr  operator +(pyObj const& other)  const { NOT_SUPPORTED_PAIR(other); }

// TODO:Stopped here - putting functions as attributes and blocking addition to tuple.
	};
}