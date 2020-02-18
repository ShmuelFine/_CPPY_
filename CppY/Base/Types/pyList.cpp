#include "pyList.h"
#include "pyStr.h"
#include "pyNums.h"
#include "pyExceptions.h"
#include "ComparisonUtils.h"
#include "pyLambda.h"
//#include "Comparisons.h"
#include <algorithm>

namespace py
{
	using namespace std;

	std::string pyList::Type() const
	{
		return "list";
	}

	pyList::operator std::string() const
	{
		return pyStr("[ ") + pyStr(",").join(LAMBDA_BASE(pyStr(*x).Clone(), x, (_impl))) + pyStr(" ]");
	}

	int pyList::CorrectIdx(int idx) const
	{
		int minIdx = 0;
		int maxIdx = (int)_impl.size();
		int theRange = maxIdx - minIdx;
		while (idx < minIdx)
			idx += theRange;
		while (idx > maxIdx)
			idx -= theRange;
		return idx;
	}

	pyObjPtr& pyList::operator [](pyObjPtr const& key)
	{
		return _impl[CorrectIdx(key->operator int())];
	}

	pyObjPtr pyList::FetchByIdx(int idx) const
	{
		return _impl[CorrectIdx(idx)];
	}

	pyObjIterator pyList::begin() const
	{
		return pyObjIterator((pyObj*)this, 0);
	}

	pyObjIterator pyList::end() const
	{
		return pyObjIterator((pyObj*)this, (int)_impl.size());
	}

	bool pyList::operator<(pyObj const& other) const
	{
		EXPLICIT_COMPARE(Type(), other.Type());

		pyList const* otherPtr = reinterpret_cast<pyList const*>(&other);
		if (_impl.empty() && !otherPtr->_impl.empty())
			return true;

		for (size_t i = 0; i < min(_impl.size(), otherPtr->_impl.size()); i++)
		{
			EXPLICIT_COMPARE(*_impl[i], *(*otherPtr)._impl[i]);
		}

		EXPLICIT_COMPARE(_impl.size(), otherPtr->_impl.size());

		return false;
	}

	pyList::pyList()
	{}

	pyList::pyList(std::initializer_list<pyObjPtr> const& v)
	{
		_impl.insert(_impl.end(), v.begin(), v.end());
	}

	pyList::pyList(std::vector<pyObjPtr> const& v)
	{
		_impl.insert(_impl.end(), v.begin(), v.end());
	}

	pyList::pyList(pyList const& other)
	{
		for (auto& elem : other._impl)
			_impl.push_back(elem);
	}

	pyObjPtr pyList::Clone() const
	{
		shared_ptr<pyList> result(new pyList());
		for (pyObjPtr oPtr : this->_impl)
			result->append(oPtr);
		return result;
	}

	void pyList::append(pyObjPtr o)
	{
		_impl.push_back(o);
	}

	pyObjPtr pyList::pop(int idx)
	{
		while (idx < 0)
			idx += (int)_impl.size();
		while (idx >= _impl.size())
			idx -= (int)_impl.size();

		pyObjPtr ret = (*this)._impl[idx];
		this->_impl.erase(_impl.begin() + idx);

		return ret;
	}


	pyObjPtr  pyList::operator +(pyObj const& other)  const
	{
		shared_ptr<pyList> result(new pyList(*this));
		(*result) += other;
		return result;
	}

	pyObj& pyList::operator += (pyObj const& other)
	{
		pyList const* otherPtr = reinterpret_cast<pyList const*>(&other);
		if (!otherPtr)
			NOT_SUPPORTED_PAIR(other);

		_impl.insert(_impl.end(), otherPtr->_impl.begin(), otherPtr->_impl.end());
		return *this;
	}
	int pyList::count(pyObjPtr whatToCount, pyObjPtr start, pyObjPtr end)
	{
		int startdx = *start, enddx = *end;

		int count = 0;
		for (auto it = _impl.begin() + startdx; it < _impl.end() - (_impl.size() - enddx); it++)
			if (*it == whatToCount)
				count++;
		return count;
	}

	int pyList::index(pyObjPtr o, pyObjPtr i, pyObjPtr j,bool throwEx) const
	{
		int idx = *i, jdx = *j;
		auto it = std::find_if(
			_impl.begin() + idx,
			_impl.end() - (_impl.size() - jdx),
			[&o](pyObjPtr const& a) {return (*a) == (*o); });


		if (it == _impl.end() - (_impl.size() - jdx))
			if (throwEx)
				THROW("Not found element");
			else
				return -1;

		return (int)std::distance(_impl.begin() + idx, it);
	}

	pyList pyList::SUB(int start, int end)
	{
		int sz = (int)_impl.size();
		while (start < 0) start += sz;
		while (start >= sz) start -= sz;
		while (end < 0) end += sz;
		while (end >= sz) end -= sz;
		vector<pyObjPtr> subVec(_impl.begin() + start, _impl.begin() + end);
		pyList result;
		for (auto& v : subVec)
			result._impl.push_back(v);
		return result;
	}

	void pyList::remove(pyObjPtr o)
	{
		auto it = std::find_if(
			_impl.begin(),
			_impl.end(),
			[&o](pyObjPtr const& a) {return (*a) == (*o); });
		THROW_IF(it == _impl.end(), "Not found element");
		_impl.erase(it);
	}

	void pyList::insert(pyObjPtr idx, pyObjPtr o)
	{
		int index = *idx;
		THROW_IF(_impl.begin() + index == _impl.end(), "Index out of range");
		_impl.insert(_impl.begin() + index, o);
	}

	void pyList::reverse()
	{
		std::reverse(_impl.begin(), _impl.end());
	}

	pyList pyList::copy()
	{
		pyList newList = pyList(_impl);
		return newList;
	}

	//////////////////////////////////////////////////////

	//pySet::pySet()
	//	:std::set<pyObjPtr>()
	//{}

	//pySet::pySet(std::vector<pyObjPtr> const& v)
	//{
	//	for (auto& element : v)
	//		insert(element);
	//}

	//pySet::pySet(pyObjPtr v)
	//{
	//	pyList dummyList;
	//	if (v->Type() == dummyList.Type())
	//	{
	//		pyList const* l = reinterpret_cast<pyList const*>(v.get());
	//		if (l)
	//		{
	//			for (auto element : *l)
	//				insert(element);
	//		}
	//	}
	//}


	//std::string pySet::Type() const
	//{
	//	return "set";
	//}

	//pySet::operator std::string() const
	//{
	//	return "{ " + pyStr(",").join(LAMBDA_BASE(pyStr(*x).Clone(), x, (*this))) + pyStr(" }");
	//}

	//pySet::operator double() const
	//{
	//	NOT_IMPL;
	//}

	//pySet::operator float() const
	//{
	//	NOT_IMPL;
	//}

	//pySet::operator int() const
	//{
	//	NOT_IMPL;
	//}

	//pySet::operator bool() const
	//{
	//	return !empty();
	//}

	//bool pySet::operator<(pyObj const& other) const
	//{
	//	EXPLICIT_COMPARE(Type(), other.Type());

	//	pySet const* otherPtr = reinterpret_cast<pySet const*> (&other);
	//	return ToList() < otherPtr->ToList();
	//}

	//pyObjPtr pySet::Clone() const
	//{
	//	shared_ptr<pySet> result(new pySet());
	//	for (pyObjPtr const& oPtr : *this)
	//		result->add(oPtr);
	//	return result;
	//}



	////void pySet::update(pyList const& v)
	////{
	////	for (auto& element : v)
	////		insert(element);
	////}

	//void pySet::update(pyList const& v)
	//{
	//	for (pyObjPtr element : v)
	//		insert(element);
	//}

	////void pySet::update(std::vector<std::string> const& v)
	////{
	////	for (pyObjPtr element : v)
	////		insert(element);
	////}

	//void pySet::add(pyObjPtr o)
	//{
	//	insert(o);
	//}

	//pySet pySet::operator-(pySet const& other) const
	//{
	//	pySet result;
	//	for (auto mine : *this)
	//		if (!other.count(mine))
	//			result.insert(mine);
	//	return result;
	//}

	//pyList pySet::ToList() const
	//{
	//	pyList result;
	//	for (auto v : *this)
	//		result.append(v);
	//	return result;
	//}


	//pyList ToList(std::string const& s)
	//{
	//	pyList result;
	//	result.FromStr(s);
	//	return result;
	//}

	//pyObjPtr sum(pyList const& l)
	//{
	//	double sum = 0;
	//	for (auto elem : l)
	//		sum += (double)(*elem);
	//	return Double(sum);
	//}

}