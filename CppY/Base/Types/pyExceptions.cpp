#include "pyExceptions.h"

#include "PyObj.h"
#include <string>

namespace py
{
	PyBaseException::operator double() const { NOT_IMPL; }
	PyBaseException::operator float() const { NOT_IMPL; }
	PyBaseException::operator int() const { NOT_IMPL; }
	PyBaseException::operator bool() const { NOT_IMPL; }
	object& PyBaseException::operator[](object const& key) { NOT_IMPL; }
	object PyBaseException::FetchByIdx(int idx) const { NOT_IMPL; }
	pyObjIterator PyBaseException::begin() const { NOT_IMPL; }
	pyObjIterator PyBaseException::end() const { NOT_IMPL; }
	object PyBaseException::operator()(pyObj const& params) const { NOT_IMPL; }

	//PyBaseException::operator std::string() const 
	std::string PyBaseException::Type() const { return "BaseException"; }
	object PyBaseException::Clone() const { return pyObjPtr(new PyBaseException(*this)); }

	bool PyBaseException::operator<(pyObj const& other) const { NOT_IMPL; }
	object PyBaseException::operator++(int) { NOT_IMPL; }
	pyObj& PyBaseException::operator++() { NOT_IMPL; }
	pyObj& PyBaseException::operator+=(pyObj const& other) { NOT_IMPL; }
}