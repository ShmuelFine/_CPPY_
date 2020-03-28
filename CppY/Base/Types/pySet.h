#pragma once

#include "PyObj.h"
#include "pyExceptions.h"
#include "pyList.h"

#include <vector>
#include <memory>
#include <set>

namespace py {

	struct pyObjPtrCompare
	{
		bool operator()(const object& _Left, const object& _Right) const {
			return _Left < _Right;
		}
	};
	class pySet;
	typedef std::shared_ptr<pySet> pySetPtr;

	class pySet :  public pyObj
	{

	public: 
		std::set<object, pyObjPtrCompare> _impl;

	public:
		pySet();
		pySet(std::vector<object> const& v);
		pySet(object v);
		
		pySet(pySet const& other);

		// pyObj: //////////////////////////////////////////////////
		virtual std::string Type() const override;
		virtual operator std::string() const override;
		virtual operator double() const override;
		virtual operator float() const override;
		virtual operator int() const override;
		virtual operator bool() const override;

		virtual object& operator [](object const& key) { NOT_SUPPORTED; }
		virtual object operator()(pyObj const& params) const { NOT_SUPPORTED; }

		virtual object FetchByIdx(int idx) const;

		virtual pyObjIterator begin() const
		{
			return pyObjIterator((pyObj*)this, 0);
		}

		virtual pyObjIterator end() const
		{
			return pyObjIterator((pyObj*)this, (int)_impl.size());
		}

		virtual object Clone() const override;
		virtual bool operator <(pyObj const& other) const override;

		virtual object  operator ++(int) { NOT_SUPPORTED; };
		virtual pyObj& operator ++() { NOT_SUPPORTED; };
		virtual pyObj& operator += (pyObj const& other) { NOT_SUPPORTED; };

		/*virtual bool operator ==(pyObj const& other) const;*/

		/////////////////////////////////////////////////////////////

		void update(pyList const& l);
		//void update(std::vector<std::string> const& v);

		void add(object o);
		pySet operator -(pySet const& other) const;

		pyList ToList() const;

		pySetPtr difference(pyObj const& others) const;

		pySetPtr intersection(pyObj const& others) const;

		void difference_update(pyObj const& others);

		void intersection_update(pyObj const& others);

		bool issuperset(pySet const& others) const;

		bool issubset(pySet const& others) const;

		bool isdisjoint(pySet const& others) const;

		pySet symmetric_difference(pySet const& others) const;

		void symmetric_difference_update(pySet const& others);

		pySet setUnion(pyObj const& others) const;

		void remove(object o);
	};

	//object sum(pyList const& l);

}

