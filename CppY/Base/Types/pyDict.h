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
	bool operator()(const pyObjPtr& a, const pyObjPtr& b) const;
};


class pyDict : public pyObj
{

public:
	std::map<pyObjPtr, pyObjPtr, compareObjPtrs> _impl;

public:
	pyDict();
	pyDict(std::map<pyObjPtr, pyObjPtr> const& v);
	pyDict(std::initializer_list<std::pair<pyObjPtr, pyObjPtr> > const& v);
	pyDict(std::vector<std::pair<pyObjPtr, pyObjPtr>> const& v);
	pyDict(pyObjPtr const& other) = delete;

public:
	/////// pyObj: //////////////////////////////
	virtual operator std::string() const override;

	virtual operator double() const override { NOT_IMPL; }
	virtual operator float() const override { NOT_IMPL; }
	virtual operator int() const override { NOT_IMPL; }
	virtual operator bool() const override { return !this->_impl.empty(); }

	virtual pyObjPtr  operator ++(int) { NOT_SUPPORTED; };
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

	virtual pyObjPtr operator()(pyObj const& params) const { NOT_SUPPORTED; };
	virtual pyObjPtr FetchByIdx(int idx) const;
	virtual pyObjPtr & operator [](pyObjPtr const & key) override;
	pyObjPtr const& at(pyObjPtr const& key) const;
	virtual pyObjPtr Clone() const override;	

	// for using as key in sets, maps, etc:
	virtual bool operator <(pyObj const& other) const override { NOT_IMPL; }
	
	virtual std::string Type() const override { return  typeid(pyDict).name(); }

};

class pyDefaultDict : public pyDict
{
public:
	pyDefaultDict();
	pyDefaultDict(std::map<pyObjPtr, pyObjPtr> const& v);
	pyDefaultDict(std::initializer_list<std::pair<pyObjPtr, pyObjPtr> > const& v);
	pyDefaultDict(std::vector<std::pair<pyObjPtr, pyObjPtr>> const& v);
	pyDefaultDict(pyObjPtr const& other) = delete;

	virtual pyObjPtr & operator [](pyObjPtr const& key) override;
	virtual pyObjPtr Clone() const override;

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