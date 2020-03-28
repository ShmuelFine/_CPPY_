#pragma once

#include "PyObj.h"
#include "pyExceptions.h"
#include <vector>
#include <set>

namespace py
{
	class pyList : public pyObj
	{
	public:
		std::vector<object> _impl;

	public:
		pyList();
		pyList(std::initializer_list<object> const& v);
		pyList(std::vector<object> const& v);

		// copy ctors
		pyList(pyList const& other);

		// pyObj: //////////////////////////////////////////////////
		virtual operator std::string() const override;


		virtual operator double() const override { NOT_IMPL; }
		virtual operator float() const override { NOT_IMPL; }
		virtual operator int() const override { NOT_IMPL; }
		virtual operator bool() const override { return not _impl.empty(); }
		virtual object& operator [](object const& key);

		virtual object FetchByIdx(int idx) const;
		virtual pyObjIterator begin() const;
		virtual pyObjIterator end() const;
		virtual object operator()(pyObj const& params) const { NOT_SUPPORTED; }
		virtual std::string Type() const override;

		virtual object Clone() const override;
		virtual bool operator <(pyObj const& other) const override;

		virtual object  operator ++(int) { NOT_SUPPORTED; };
		virtual pyObj& operator ++() { NOT_SUPPORTED; };
		virtual pyObj& operator += (pyObj const& other);

		virtual object  operator +(pyObj const& other)  const override;

		/////////////////////////////////////////////////////////////

		void append(object o);
		object pop(int idx = -1);
		operator bool() { return !_impl.empty(); }

		int count(object whatToCount, object start, object end);
		int index(object whoToSearch, object startIdx, object endIdx, bool isToThrow) const;

		pyList SUB(int start, int end);
		int CorrectIdx(int idx) const;

		void remove(object o);
		void insert(object idx, object o);
		void reverse();

		pyList copy();

	public:
		// Shouldn't be used. if the other is a list, use As<list>
		pyList(object const& other) = delete;
	};


	pyList ToList(std::string const& s);

	//class pySet : private std::set<object>, public pyObj
	//{
	//public:
	//	pySet();
	//	pySet(std::vector<object> const& v);
	//	pySet(object v);

	//	// pyObj: //////////////////////////////////////////////////
	//	virtual std::string Type() const override;
	//	virtual operator std::string() const override;
	//	virtual operator double() const override;
	//	virtual operator float() const override;
	//	virtual operator int() const override;
	//	virtual operator bool() const override;

	//	virtual object& operator [](object const& key) { NOT_SUPPORTED; }
	//	virtual object FetchByIdx(int idx) const { NOT_SUPPORTED; }
	//	virtual pyObjIterator begin() const { NOT_SUPPORTED; }
	//	virtual pyObjIterator end() const { NOT_SUPPORTED; }
	//	virtual object operator()(pyObj const& params) const { NOT_SUPPORTED; }

	//	virtual object Clone() const override;
	//	virtual bool operator <(pyObj const& other) const override;

	//	virtual object  operator ++(int) { NOT_SUPPORTED; };
	//	virtual pyObj& operator ++() { NOT_SUPPORTED; };
	//	virtual pyObj& operator += (pyObj const& other) { NOT_SUPPORTED; };

	//	/////////////////////////////////////////////////////////////

	//	void update(pyList const& l);
	//	//void update(std::vector<std::string> const& v);

	//	void add(object o);
	//	pySet operator -(pySet const& other) const;

	//	pyList ToList() const;
	//};

	//object sum(pyList const& l);


	template<typename IterableA, typename IterableB>
	pyList intersect(IterableA const& list_a, IterableB const& list_b)
	{
		pyList result;
		for (auto& a : list_a)
		{
			for (auto& b : list_b)
			{
				if (a == b)
				{
					result.append(a);
					break;
				}
			}
		}
		return result;
	}
}
