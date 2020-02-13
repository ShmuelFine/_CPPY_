#pragma once


#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

//#include "os.h"
//#include "range.hpp"

#include "pyDefs.h"


int OPEN_MODE(std::string const& s);
//
//class FSTREAM : public std::fstream
//{
//public:
//	FSTREAM(std::string const& path, int mode);
//};
//
#define WITH_OPEN(fileObj, filePath, open_type, ...) for (auto fileObj = std::fstream(filePath, OPEN_MODE(open_type)); fileObj.is_open(); fileObj.close())

#define pass int __nothing ##__LINE__ = __LINE__

//#define setattr(instance, field, value) (instance).field = (value)

#define DoDictEntry std::make_pair<object, object>

//#define None py::object(new py::pyNone())

namespace py
{

	bool IS_IN(object const& a, list const& l);

	bool IS_IN(object const& a, set const& s);

	template<typename T>
	bool IS_IN(object const& a, defaultdict<T> const& d)
	{
		return IS_IN(a, d.keys());
	}

	bool IS_IN(object const& a, object const& s);


	template<typename Iterable>
	object max(Iterable const& l)
	{
		if (l.empty())
			return None;

		object res = *l.begin();
		for (auto& o : l)
		{
			if (res < o)
				res = o;
		}
		return res;
	}

	template<typename Iterable>
	object min(Iterable const& l)
	{
		if (l.empty())
			return None;

		object res = *l.begin();
		for (auto & o : l)
		{
			if (o < res)
				res = o;
		}
		return res;
	}

	void print(object const& o);

	int len(list const& l);
	int len(dict const& l);

	int len(set const& s);
	int len(str const& s);
	template<typename Pr>
	list sorted(list v, Pr p)
	{
		std::sort(v.begin(), v.end(), 
			[&](object const& o1, object const& o2) {return p(o1) < p(o2); });
		return v;
	}

	list sorted(list const& orig);
	list sorted(set s);
}

