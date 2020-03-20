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
		virtual pyObjPtr& operator [](pyObjPtr const& key);

		virtual pyObjPtr FetchByIdx(int idx) const;
		virtual pyObjIterator begin() const;
		virtual pyObjIterator end() const;
		virtual pyObjPtr operator()(pyObj const& params) const { NOT_SUPPORTED; }
		virtual std::string Type() const override;

		virtual pyObjPtr Clone() const override;
		virtual bool operator <(pyObj const& other) const override;

		virtual pyObjPtr  operator ++(int) { NOT_SUPPORTED; };
		virtual pyObj& operator ++() { NOT_SUPPORTED; };
		virtual pyObj& operator += (pyObj const& other);

		virtual pyObjPtr  operator +(pyObj const& other)  const override;

		/////////////////////////////////////////////////////////////

		void append(pyObjPtr o);
		pyObjPtr pop(int idx = -1);
		operator bool() { return !_impl.empty(); }

		int count(pyObjPtr whatToCount, pyObjPtr start, pyObjPtr end);
		int index(pyObjPtr whoToSearch, pyObjPtr startIdx, pyObjPtr endIdx, bool isToThrow) const;

		pyList SUB(int start, int end);
		int CorrectIdx(int idx) const;

		void remove(pyObjPtr o);
		void insert(pyObjPtr idx, pyObjPtr o);
		void reverse();

		pyList copy();

	public:
		// Shouldn't be used. if the other is a list, use As<list>
		pyList(pyObjPtr const& other) = delete;
	};


	pyList ToList(std::string const& s);

	//class pySet : private std::set<pyObjPtr>, public pyObj
	//{
	//public:
	//	pySet();
	//	pySet(std::vector<pyObjPtr> const& v);
	//	pySet(pyObjPtr v);

	//	// pyObj: //////////////////////////////////////////////////
	//	virtual std::string Type() const override;
	//	virtual operator std::string() const override;
	//	virtual operator double() const override;
	//	virtual operator float() const override;
	//	virtual operator int() const override;
	//	virtual operator bool() const override;

	//	virtual pyObjPtr& operator [](pyObjPtr const& key) { NOT_SUPPORTED; }
	//	virtual pyObjPtr FetchByIdx(int idx) const { NOT_SUPPORTED; }
	//	virtual pyObjIterator begin() const { NOT_SUPPORTED; }
	//	virtual pyObjIterator end() const { NOT_SUPPORTED; }
	//	virtual pyObjPtr operator()(pyObj const& params) const { NOT_SUPPORTED; }

	//	virtual pyObjPtr Clone() const override;
	//	virtual bool operator <(pyObj const& other) const override;

	//	virtual pyObjPtr  operator ++(int) { NOT_SUPPORTED; };
	//	virtual pyObj& operator ++() { NOT_SUPPORTED; };
	//	virtual pyObj& operator += (pyObj const& other) { NOT_SUPPORTED; };

	//	/////////////////////////////////////////////////////////////

	//	void update(pyList const& l);
	//	//void update(std::vector<std::string> const& v);

	//	void add(pyObjPtr o);
	//	pySet operator -(pySet const& other) const;

	//	pyList ToList() const;
	//};

	//pyObjPtr sum(pyList const& l);


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
