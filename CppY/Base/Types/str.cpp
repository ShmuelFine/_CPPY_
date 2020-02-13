#include "str.h"
#include "FunDefs.h"
#include "Comparisons.h"
#include "Int.h"
namespace py
{

	namespace py_str
	{
	//	FUN_DEF(capitalize);
	//	PARAM(self, );
	//	auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
	//	std::string data = meAsStr->_impl;
	//	std::transform(std::begin(data), std::end(data), std::begin(data), std::tolower);
	//	if (!data.empty())
	//		data[0] = std::toupper(data[0]);
	//	return data;
	//	END_FUN(capitalize);

	//	FUN_DEF(casefold);
	//	PARAM(self, );
	//	auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
	//	return meAsStr->lower();
	//	END_FUN(casefold);

	//	FUN_DEF(center);
	//	PARAM(self, );
	//	PARAM(width, );
	//	PARAM(fillchar, ' ');
	//	if (not is_ofType(width, Int(1)))
	//		THROW("integer value expected");
	//	int wid = width;
	//	auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
	//	std::string data = meAsStr->_impl;
	//	int dataLen = data.length();
	//	int fillspaces = wid - data.length();

	//	while (fillspaces > 1)
	//	{
	//		data = fillchar + data + fillchar;
	//		fillspaces -= 2;
	//	}

	//	if (fillspaces == 1)
	//		return data += fillchar;
	//	else
	//		return data;
	//	END_FUN(center);

	///*	S.count(sub[, start[, end]]) -> int
	//   Return the number of non - overlapping occurrences of substring sub in
	//   string S[start:end].Optional arguments start and end are
	//   interpreted as in slice notation.*/
	//	FUN_DEF(count);
	//	PARAM(self, );

	//	END_FUN(count);

	//	FUN_DEF(encode);
	//	PARAM(self, );
	//	PARAM(encoding, "utf-8");
	//	PARAM(errors, "strict");

	//	END_FUN(encode);

	//	FUN_DEF(endswith);
	//	PARAM(self, );
	//	PARAM(ending, );
	//	PARAM(start, None);
	//	PARAM(end, None);
	//	auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
	//	std::string data = meAsStr->_impl;
	//	if (is_ofType(start, None))
	//		start = 0;
	//	if (is_ofType(end, None))
	//		end = data.length - 1;
	//	data = data.substr(start, (end - start) + 1);
	//	return meAsStr->endswith(data);
	//	END_FUN(endswith);

	//	FUN_DEF(expandtabs);
	//	PARAM(self, );
	//	PARAM(tabSize, 8);
	//	if (not is_ofType(tabSize, Int(tabSize)))
	//		THROW("integer value expected");
	//	int tabNum = tabSize;
	//	auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
	//	std::string data = meAsStr->_impl;
	//	std::string tabStr = "";
	//	for (int i = 0; i < tabNum; i++)
	//		tabStr += " ";

	//	END_FUN();

	//	FUN_DEF(lower);
	//	PARAM(self, );

	//	END_FUN(lower);

	//	

	//	FUN_DEF(join);
	//	PARAM(self, );

	//	END_FUN(join);

	//	FUN_DEF(format);
	//	PARAM(self, );

	//	END_FUN(format);

	//	FUN_DEF(strip);
	//	PARAM(self, );

	//	END_FUN(strip);

	//	FUN_DEF(split);
	//	PARAM(self, );

	//	END_FUN(split);

	//	FUN_DEF(replace);
	//	PARAM(self, );

	//	END_FUN(replace);
	//	

	//	FUN_DEF();
	//	PARAM(self, );

	//	END_FUN();
	}
}

namespace py
{
	void str::AddAttributes()
	{
		//(*this).attr(capitalize) = py_str::capitalize;
		//(*this).attr(casefold) = py_str::casefold;
		//(*this).attr(center) = py_str::center;
		//(*this).attr(count) = py_str::count;
		//(*this).attr(encode) = py_str::encode;
		//(*this).attr(endswith) = py_str::endswith;
		//(*this).attr(expandtabs) = py_str::expandtabs;
	}

	str::str() : object(pyStr())
	{
		AddAttributes();
	}

	str::str(const char* cstr) : object(pyStr(cstr))
	{
		AddAttributes();
	}
	str::str(std::string const& stdStr) : object(pyStr(stdStr))
	{
		AddAttributes();
	}
}