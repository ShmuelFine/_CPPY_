#include "pyFunc.h"
#include "pyExceptions.h"

namespace py
{
	pyFunc::operator std::string() const{NOT_SUPPORTED;}
	pyFunc::operator double() const{NOT_SUPPORTED;}
	pyFunc::operator float() const{NOT_SUPPORTED;}
	pyFunc::operator int() const{NOT_SUPPORTED;}
	pyFunc::operator bool() const{NOT_SUPPORTED;}
	pyObjPtr & pyFunc::operator [](pyObjPtr const & key) { NOT_SUPPORTED; }
	pyObjPtr pyFunc::FetchByIdx(int idx) const { NOT_SUPPORTED; }
	pyObjIterator pyFunc::begin() const { NOT_SUPPORTED; }
	pyObjIterator pyFunc::end() const { NOT_SUPPORTED; }
	
	bool pyFunc::operator<(pyObj const& other) const { NOT_SUPPORTED; }
	pyObjPtr pyFunc::operator++(int) { NOT_SUPPORTED; }
	pyObj& pyFunc::operator++() { NOT_SUPPORTED; }
	pyObj& pyFunc::operator+=(pyObj const& other) { NOT_SUPPORTED; }
}