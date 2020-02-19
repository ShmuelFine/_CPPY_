#include "FunDefs.h"
#include "bytes.h"
#include "pyBytes.h"
#include "list.h"

namespace py
{

	namespace py_bytes {
		FUN_DEF(hex);
		PARAM(self, );
		auto meAsBytes = reinterpret_cast<pyBytes*>(self._ptr.get());
		return meAsBytes->hex();
		END_FUN(hex);

		/*FUN_DEF(decode);
		PARAM(self, );
		PARAM(encoding, 'utf-8');
		PARAM(errors, 'strict');
		auto meAsBytes = reinterpret_cast<pyBytes*>(self._ptr.get());
		meAsBytes->decode(encoding, errors);
		END_FUN(decode);*/


		FUN_DEF(endswith);
		PARAM(self, );
		PARAM(ending, );
		auto meAsBytes = reinterpret_cast<pyBytes*>(self._ptr.get());
		auto end = reinterpret_cast<pyBytes*>(ending._ptr.get());
		return meAsBytes->endswith(*end);
		END_FUN(endswith);

		FUN_DEF(startswith);
		PARAM(self, );
		PARAM(opening, );
		auto meAsBytes = reinterpret_cast<pyBytes*>(self._ptr.get());
		auto open = reinterpret_cast<pyBytes*>(opening._ptr.get());
		return meAsBytes->startswith(*open);
		END_FUN(startswith);

		FUN_DEF(join);
		PARAM(self, );
		PARAM(iterable, );
		auto meAsStr = reinterpret_cast<pyStr*>(self._ptr.get());
		meAsStr->join(iterable);
		END_FUN(join);

		FUN_DEF(replace);
		PARAM(self, );
		PARAM(what, );
		PARAM(withWhat, );
		auto meAsBytes = reinterpret_cast<pyBytes*>(self._ptr.get());
		auto old = reinterpret_cast<pyBytes*>(what._ptr.get());
		auto new1 = reinterpret_cast<pyBytes*>(withWhat._ptr.get());
		return meAsBytes->replace(*old, *new1);
		END_FUN(replace);



		FUN_DEF(partition);
		PARAM(self, );
		PARAM(sep, );
		auto meAsBytes = reinterpret_cast<pyBytes*>(self._ptr.get());
		auto delimitor = reinterpret_cast<pyByte*>(sep._ptr.get());
		return meAsBytes->partition(*delimitor);
		END_FUN(partition);

		FUN_DEF(center);
		PARAM(self, );
		PARAM(width, );
		PARAM(fillByte, 32);
		auto meAsBytes = reinterpret_cast<pyBytes*>(self._ptr.get());
		auto filling = reinterpret_cast<pyByte*>(fillByte._ptr.get());
		return meAsBytes->center(width, *filling);
		END_FUN(center);

		FUN_DEF(rindex);
		PARAM(self, );
		PARAM(what, );
		PARAM(i, None);
		PARAM(j, None);
		auto meAsBytes = reinterpret_cast<pyBytes*>(self._ptr.get());
		auto x = reinterpret_cast<pyBytes*>(what._ptr.get());
		return meAsBytes->rindex(*x, i, j, true);
		END_FUN(rindex);

		FUN_DEF(rfind);
		PARAM(self, );
		PARAM(what, );
		PARAM(i, None);
		PARAM(j, None);
		auto meAsBytes = reinterpret_cast<pyBytes*>(self._ptr.get());
		auto x = reinterpret_cast<pyBytes*>(what._ptr.get());
		return meAsBytes->rindex(*x, i, j, false);
		END_FUN(rfind);
	}

}
namespace py
{
	void bytes::AddAttributes()
	{
		(*this).attr(hex) = py_bytes::hex;

		attr(count) = py_list::count;

		//(*this).attr(decode) = py_bytes::decode;

		(*this).attr(endswith) = py_bytes::endswith;
		(*this).attr(startswith) = py_bytes::startswith;

		//does weird thing in python
		(*this).attr(join) = py_bytes::join;

		attr(find) = py_list::find;

		attr(index) = py_list::index;

		//static method
		//(*this).attr(maketrans) = py_bytes::maketrans;

		//to do
		(*this).attr(partition) = py_bytes::partition;

		//(*this).attr(translate) = py_bytes::translate;

		(*this).attr(center) = py_bytes::center;

		(*this).attr(replace) = py_bytes::replace;
		(*this).attr(rfind) = py_bytes::rfind;
		(*this).attr(rindex) = py_bytes::rindex;
		//(*this).attr(rpartition) = py_bytes::rpartition;

	}

	bytes::bytes() : object(pyBytes())
	{
		AddAttributes();
	}

	bytes::bytes(std::vector<unsigned char> const& v)
		: object(pyBytes(std::vector<unsigned char>(v.begin(), v.end())))
	{
		AddAttributes();
	}

	bytes::bytes(uint8_t singleton)
		: object(pyBytes(std::vector<unsigned char>({ singleton })))
	{
		AddAttributes();
	}
	
	bytes::bytes(object const& other)
		: object(other)
	{
	}

	bytes::bytes(std::string const& v) :
		object(pyBytes(v))
		//object(pyBytes(std::string(v.begin(), v.end())))
	{
		AddAttributes();
	}

	bytes::bytes(const char* v_str)
		:object(pyBytes(v_str))
	{
		AddAttributes();
	}

}
