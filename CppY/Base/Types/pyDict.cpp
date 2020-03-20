#include "pyDict.h"

#include "pyLambda.h"
#include "pyStr.h"
#include <memory>
#include <vector>

namespace py
{
	using namespace std;

	
	bool compareObjPtrs::operator()(const pyObjPtr & a, const pyObjPtr & b) const {
		return (*a) < (*b);
	}

	pyDict::pyDict() : _impl() {};
	
	pyDict::pyDict(std::map<pyObjPtr, pyObjPtr> const& v)
	{
		_impl.insert(v.begin(), v.end());
	}

	pyDict::pyDict(std::initializer_list<std::pair<pyObjPtr, pyObjPtr> > const& v)
	{	
		_impl.insert(v.begin(), v.end());
	}

	pyDict::pyDict(std::vector<std::pair<pyObjPtr, pyObjPtr>> const& v)
	{
		_impl.insert(v.begin(), v.end());
	}

	pyDict::operator std::string() const 
	{
		pyStr result = pyStr("{ ") + pyStr(" , ").join(LAMBDA_BASE(pyStr("{ " + (std::string) * (kvp.first) + " : " + (std::string) * (kvp.second) + " }").Clone(), kvp, _impl)) + pyStr(" }");
		return result;
	}

	pyObjPtr & pyDict::operator [](pyObjPtr const & params)
	{
		THROW_UNLESS(_impl.count(params), "KeyError: " + params->operator std::string());
		return _impl[params];
	}
	
	pyObjPtr const& pyDict::at(pyObjPtr const& params) const
	{
		THROW_UNLESS(_impl.count(params), "KeyError: " + params->operator std::string());
		return _impl.at(params);
	}


	pyObjPtr pyDict::FetchByIdx(int index) const
	{
		int idx = index;
		auto it = _impl.begin();
		for (int i = 0; i < idx; i++)
			it++;
		return pyObjPtr(new pyTuple(*it));
	}


	pyObjPtr pyDict::Clone() const
	{
		std::shared_ptr<pyDict> result(new pyDict());
		for (auto const currPair : this->_impl)
		{
			result->_impl[currPair.first->Clone()] = currPair.second->Clone();
		}
		return result;
	}

	////////////////// default dict: //////////////////

	pyDefaultDict::pyDefaultDict() : pyDict() {}

	pyDefaultDict::pyDefaultDict(std::map<pyObjPtr, pyObjPtr> const& v) : pyDict(v) {}

	pyDefaultDict::pyDefaultDict(std::initializer_list<std::pair<pyObjPtr, pyObjPtr>> const& v) : pyDict(v) {}

	pyDefaultDict::pyDefaultDict(std::vector<std::pair<pyObjPtr, pyObjPtr>> const& v) : pyDict(v) {}

	pyObjPtr & pyDefaultDict::operator[](pyObjPtr const& key)
	{
		return _impl[key];
	}

	pyObjPtr pyDefaultDict::Clone() const
	{
		std::shared_ptr<pyDefaultDict> result(new pyDefaultDict());
		for (auto const currPair : this->_impl)
		{
			result->_impl[currPair.first->Clone()] = currPair.second->Clone();
		}
		return result;
	}



	//pyObjPtr const & pyDict::get(pyObjPtr const & key, pyObjPtr const & defaultValue) const
	//{
	//	if (this->_impl.count(key))
	//		return this->_impl.at(key);
	//	else
	//		return defaultValue;
	//}

	//pyDict const& pyDict::update(pyDict const& other)
	//{
	//	for (auto& kvp : other._impl)
	//		((*this)._impl)[kvp.first] = kvp.second;

	//	return *this;
	//}

	///*
	//Counter::Counter() {}

	//Counter::Counter(pyList const& words)
	//{
	//	update(words);
	//}

	//
	//void Counter::update(pyList const& words)
	//{
	//	for (auto& w : words)
	//		(*this)[w]++;
	//}
	//*/
}
