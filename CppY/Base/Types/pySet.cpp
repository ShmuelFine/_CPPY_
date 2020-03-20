#include "pySet.h"
#include "pyStr.h"
#include "pyNums.h"
#include "pyExceptions.h"
#include "ComparisonUtils.h"
#include "pyLambda.h"
//#include "Comparisons.h"
#include <algorithm>
#include <vector>

namespace py {

	using namespace std;

	pySet::pySet()
	{
	}

	pySet::pySet(std::vector<pyObjPtr> const& v)
	{
		for (auto& element : v)
			_impl.insert(element);
	}

	pySet::pySet(pyObjPtr v)
	{
		for (auto elem : *v)
		{
			_impl.insert(elem);
		}
	}

	pySet::pySet(pySet const& other)
	{
		for (auto& elem : other._impl)
			_impl.insert(elem);
	}


	std::string pySet::Type() const
	{
		return "set";
	}

	pySet::operator std::string() const
	{
		pyStr values = pyStr(",").join(LAMBDA_BASE(pyStr(*x).Clone(), x, (*this)));
		pyStr result = pyStr("{ ") + values + pyStr(" }");
		return result;
	}

	pySet::operator double() const
	{
		NOT_IMPL;
	}

	pySet::operator float() const
	{
		NOT_IMPL;
	}

	pySet::operator int() const
	{
		NOT_IMPL;
	}

	pySet::operator bool() const
	{
		return !_impl.empty();
	}

	bool pySet::operator<(pyObj const& other) const
	{
		EXPLICIT_COMPARE(Type(), other.Type());

		pySet const* otherPtr = dynamic_cast<pySet const*> (&other);
		return ToList() < otherPtr->ToList();
	}

	pyObjPtr pySet::FetchByIdx(int index) const
	{
		int idx = index;
		auto it = _impl.begin();
		for (int i = 0; i < idx; i++)
			it++;
		return pyObjPtr(*it);
	}


	pyObjPtr pySet::Clone() const
	{
		shared_ptr<pySet> result(new pySet());
		for (pyObjPtr const& oPtr : *this)
			result->add(oPtr);
		return result;
	}


	void pySet::update(pyList const& v)
	{
		for (pyObjPtr element : v)
			_impl.insert(element);
	}


	void pySet::add(pyObjPtr o)
	{
		_impl.insert(o);
	}

	pySet pySet::operator-(pySet const& other) const
	{
		pySet result;
		for (auto mine : *this)
			if (!other._impl.count(mine))
				result._impl.insert(mine);
		return result;
	}

	pyList pySet::ToList() const
	{
		pyList result;
		for (auto v : *this)
			result.append(v);
		return result;
	}

	bool HasElement(pyObj& iterable, pyObjPtr toFind)
	{
		for (auto elem : iterable)
			if (*toFind == *elem)
				return true;

		return false;
	}

	pySetPtr pySet::difference(pyObj const& others) const
	{
		pySetPtr result(new pySet(*this));
		pyList toDelete;
		for (auto iterable : others)
		{
			toDelete._impl.clear();

			for (auto const& elem : *result)
			{
				if (HasElement(*iterable, elem))
				{
					toDelete.append(elem);
				}
			}
			for (auto elem : toDelete)
			{
				result->_impl.erase(elem);
			}
		}

		return result;
	}

	void pySet::difference_update(pyObj const& others)
	{
		pySet difference;
		difference = *this->difference(others);
		_impl.clear();
		*this = difference;
	}




	pySetPtr pySet::intersection(pyObj const& others) const
	{
		pySetPtr result(new pySet(*this));
		pyList toDelete;
		for (auto iterable : others)
		{
			toDelete._impl.clear();

			for (auto const& elem : *result)
			{
				if (not HasElement(*iterable, elem))
				{
					toDelete.append(elem);
				}
			}
			for (auto elem : toDelete)
			{
				result->_impl.erase(elem);
			}
		}

		return result;
	}

	void pySet::intersection_update(pyObj const& others)
	{
		pySet intersect;
		intersect = *this->intersection(others);
		_impl.clear();
		*this = pySet(intersect);
	}



	bool pySet::issuperset(pySet const& others) const
	{
		return others.issubset(*this);
	}

	bool pySet::issubset(pySet const& others) const
	{
		return others.difference(*this)->_impl.empty();
	}


	bool pySet::isdisjoint(pySet const& others) const
	{
		return others.intersection(*this)->_impl.empty();
	}



	pySetPtr pySet::setUnion(pyObj const& others) const
	{
		pySet result = *this;
		for (auto iterable : others)
		{
			for (auto elem : *iterable)
			{
				result.add(elem);
			}
		}
		return std::make_shared<pySet>(result);
	}


	pySetPtr pySet::symmetric_difference(pySet const& others) const
	{

		auto differ1 = (*this).difference(others);
		auto differ2 = others.difference(*this);

		return differ1->setUnion(*differ2);
	}

	void pySet::symmetric_difference_update(pySet const& others)
	{
		auto differ1 = this->difference(others);
		auto differ2 = others.difference(*this);

		*this = *differ1->setUnion(*differ2);
	}

	void pySet::remove(pyObjPtr o)
	{
		if (0 == this->_impl.erase(o))
			THROW("element doesnt exist in the set");
	}

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
