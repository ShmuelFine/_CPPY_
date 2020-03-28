#pragma once

#include "PyObj.h"
#include "pyStr.h"
#include "pyNums.h"

namespace py
{
	class pyRange : public pyObj
	{
	public:
		int _start;
		int _stop;
		int _step;

	public:
		pyRange(int start, int stop, int step)
			:_start(start), _stop(stop)
		{
			//If step is zero, ValueError is raised.
			if (step == 0)
			{
				THROW("ValueError");
			}
			else
				_step = step;
		}

		// pyObj: //////////////////////////////////////////////////
		virtual operator std::string() const override { return pyStr("range({},{},{})").format(_start, _stop, _step); };

		virtual operator double() const override { NOT_IMPL; }
		virtual operator float() const override { NOT_IMPL; }
		virtual operator int() const override { NOT_IMPL; }
		virtual operator bool() const override { NOT_IMPL; }

		virtual object& operator [](object const& key) { NOT_IMPL; }

		virtual object FetchByIdx(int idx) const
		{
			int result = _start + _step * (idx);
			THROW_IF(result > _stop, "range object index out of range");
			return pyInt(result).Clone();
		}

		virtual pyObjIterator begin() const { return pyObjIterator((pyObj*)this, 0); }
		virtual pyObjIterator end() const { return pyObjIterator((pyObj*)this, (_stop - _start) / _step); }
		virtual object operator()(pyObj const& params) const { NOT_SUPPORTED; }
		virtual std::string Type() const override { return "range"; }

		virtual object Clone() const override { return object(new pyRange(_start, _stop, _step)); }
		virtual bool operator <(pyObj const& other) const override
		{
			EXPLICIT_COMPARE(Type(), other.Type());
			pyRange const* otherPtr = dynamic_cast<pyRange const*>(&other);
			EXPLICIT_COMPARE(_start, otherPtr->_start);
			EXPLICIT_COMPARE(_stop, otherPtr->_stop);
			EXPLICIT_COMPARE(_step, otherPtr->_step);

			return false;
		}

		virtual object operator ++(int) { NOT_SUPPORTED; };
		virtual pyObj& operator ++() { NOT_SUPPORTED; };
		virtual pyObj& operator += (pyObj const& other) { NOT_SUPPORTED_PAIR(other); };
		virtual object  operator +(pyObj const& other)  const override { NOT_SUPPORTED_PAIR(other); };

	};
}