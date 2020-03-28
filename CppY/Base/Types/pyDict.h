#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <utility>

#include "PyObj.h"
#include "pyStr.h"
#include "pyList.h"
#include "pyTuple.h"
#include "pyExceptions.h"
#include "pyNums.h"
#include "pyLambda.h"
namespace py
{
	
struct compareObjPtrs {
	bool operator()(const object& a, const object& b) const;
};


class pyDict : public pyObj
{

public:
	std::map<object, object, compareObjPtrs> _impl;

public:
	pyDict();
	pyDict(std::map<object, object> const& v);
	pyDict(std::initializer_list<std::pair<object, object> > const& v);
	pyDict(std::vector<std::pair<object, object>> const& v);
	pyDict(object const& other) = delete;

public:
	/////// pyObj: //////////////////////////////
	virtual operator std::string() const override;

	virtual operator double() const override { NOT_IMPL; }
	virtual operator float() const override { NOT_IMPL; }
	virtual operator int() const override { NOT_IMPL; }
	virtual operator bool() const override { return !this->_impl.empty(); }

	virtual object  operator ++(int) { NOT_SUPPORTED; };
	virtual pyObj& operator ++() { NOT_SUPPORTED; };
	virtual pyObj& operator += (pyObj const& other) { NOT_SUPPORTED; };

	virtual pyObjIterator begin() const
	{
		return pyObjIterator((pyObj*)this, 0);
	}

	virtual pyObjIterator end() const
	{
		return pyObjIterator((pyObj*)this, (int)_impl.size());
	}

	virtual object operator()(pyObj const& params) const { NOT_SUPPORTED; };
	virtual object FetchByIdx(int idx) const;
	virtual object & operator [](object const & key) override;
	object const& at(object const& key) const;
	virtual object Clone() const override;	

	// for using as key in sets, maps, etc:
	virtual bool operator <(pyObj const& other) const override { NOT_IMPL; }
	
	virtual std::string Type() const override { return  typeid(pyDict).name(); }

};

class pyDefaultDict : public pyDict
{
public:
	pyDefaultDict();
	pyDefaultDict(std::map<object, object> const& v);
	pyDefaultDict(std::initializer_list<std::pair<object, object> > const& v);
	pyDefaultDict(std::vector<std::pair<object, object>> const& v);
	pyDefaultDict(object const& other) = delete;

	virtual object & operator [](object const& key) override;
	virtual object Clone() const override;

};

	/*
	class Counter : public pyDefaultDict
	{
	public:
		Counter();
		Counter(pyList const& words);

		void update(pyList const& words);
	};
	*/

}