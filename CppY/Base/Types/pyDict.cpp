#include "pyDict.h"

#include "pyLambda.h"
#include "pyStr.h"
#include <memory>
#include <vector>

namespace py
{
	using namespace std;

	
	bool compareObjPtrs::operator()(const object & a, const object & b) const {
		return (*a.get()) < (*b.get());
	}

	pyDict::pyDict() : _impl() {};
	
	pyDict::pyDict(std::map<object, object> const& v)
	{
		_impl.insert(v.begin(), v.end());
	}

	pyDict::pyDict(std::initializer_list<std::pair<object, object> > const& v)
	{	
		_impl.insert(v.begin(), v.end());
	}

	pyDict::pyDict(std::vector<std::pair<object, object>> const& v)
	{
		_impl.insert(v.begin(), v.end());
	}

	pyDict::operator std::string() const 
	{
		pyStr result = pyStr("{ ") + pyStr(" , ").join(LAMBDA_BASE(pyStr("{ " + (std::string) (kvp.first) + " : " + (std::string) (kvp.second) + " }").Clone(), kvp, _impl)) + pyStr(" }");
		return result;
	}

	object & pyDict::operator [](object const & params)
	{
		THROW_UNLESS(_impl.count(params), "KeyError: " + params->operator std::string());
		return _impl[params];
	}
	
	object const& pyDict::at(object const& params) const
	{
		THROW_UNLESS(_impl.count(params), "KeyError: " + params->operator std::string());
		return _impl.at(params);
	}


	object pyDict::FetchByIdx(int index) const
	{
		int idx = index;
		auto it = _impl.begin();
		for (int i = 0; i < idx; i++)
			it++;
		return new pyTuple(*it);
	}


	object pyDict::Clone() const
	{
		object resultObj(new pyDict());
		pyDict* resultPtr = reinterpret_cast<pyDict*>(resultObj.get());
		pyDict& result = *resultPtr;
		for (auto const currPair : this->_impl)
		{
			result._impl[currPair.first->Clone()] = currPair.second->Clone();
		}

		return resultObj;
	}

	////////////////// default dict: //////////////////

	pyDefaultDict::pyDefaultDict() : pyDict() {}

	pyDefaultDict::pyDefaultDict(std::map<object, object> const& v) : pyDict(v) {}

	pyDefaultDict::pyDefaultDict(std::initializer_list<std::pair<object, object>> const& v) : pyDict(v) {}

	pyDefaultDict::pyDefaultDict(std::vector<std::pair<object, object>> const& v) : pyDict(v) {}

	object & pyDefaultDict::operator[](object const& key)
	{
		return _impl[key];
	}

	object pyDefaultDict::Clone() const
	{
		object resultObj(new pyDict());
		pyDict* resultPtr = reinterpret_cast<pyDict*>(resultObj.get());
		pyDict& result = *resultPtr;
		for (auto const currPair : this->_impl)
		{
			result._impl[currPair.first->Clone()] = currPair.second->Clone();
		}

		return resultObj;
	}



	//object const & pyDict::get(object const & key, object const & defaultValue) const
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
