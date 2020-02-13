//
// Created by fine on 12/8/19.
//

#include "CppY.h"
#include <iostream>
#include <algorithm>
//#include "ghc/filesystem.hpp"
//namespace fs = std::filesystem;

namespace py
{
	bool IsEqual(object const& a, object const& b)
	{
		return !((a < b) || (b < a));
	}

	bool IS_IN(object const& a, list const& l)
	{
		for (auto& o : l)
		{
			if (a == o)
				return true;
		}
		return false;
	}

	bool IS_IN(object const& a, set const& s)
	{
		for (auto& o : s)
		{
			if (a == o)
				return true;
		}
		return false;
	}

	bool IS_IN(object const& a, object const& s)
	{
		if (isinstance<dict>(s))
			return IS_IN(a, As<dict>(s));
		else if (isinstance<list>(s))
			return IS_IN(a, As<list>(s));
		else if (isinstance<set>(s))
			return IS_IN(a, As<set>(s));
		else
			NOT_IMPL;
	}

	void print(object const& o)
	{
		std::cout << str(o) << std::endl;
	}

	int len(list const& l)
	{
		return (int)l.size();
	}

	int len(dict const& d)
	{
		return len(d.keys());
	}

	int len(set const& s)
	{
		return (int)s.size();
	}

	int len(str const& s)
	{
		return (int)s.size();
	}


	list sorted(list const & orig)
	{
		list v = orig;
		std::sort(v.begin(), v.end());
		return v;
	}

	list sorted(pySet s)
	{
		list l; l.FromSet(s);
		return sorted(l);
	}
}

int OPEN_MODE(std::string const& s)
{

	if (s == "r") return std::ios_base::in;
	else if (s == "w") return std::ios_base::out;

	else if (s == "rb") return std::ios_base::in | std::ios_base::binary;
	else if (s == "wb") return std::ios_base::out | std::ios_base::binary;
	else if (s == "a") return std::ios_base::app;
	else if (s == "ab") return std::ios_base::app | std::ios_base::binary;
	else throw std::runtime_error("Unknown open mode");
}
//
//FSTREAM::FSTREAM(std::string const& path, int mode)
//	:std::fstream(fs::path(path.c_str()), mode)
//{
//	THROW_UNLESS(*this, ("Can't open " + path).c_str());
//}
