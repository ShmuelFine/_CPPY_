#include "FunDefs.h"
#include "binary.h"
#include "pyBytes.h"
//#include "list.cpp"

namespace py
{

	namespace py_bytes{
	FUN_DEF(hex);
	PARAM(self, );
	auto meAsBytes = reinterpret_cast<pyBytes*>(self._ptr.get());
	meAsBytes->hex();
	END_FUN(hex);
	}
	namespace py_binary
	{

		/*FUN_DEF(decode);
		PARAM(self, );
		PARAM(encoding, 'utf-8');
		PARAM(errors, 'strict');
		auto meAsBytes = reinterpret_cast<pyBinary*>(self._ptr.get());
		meAsBytes->decode(encoding, errors);
		END_FUN(decode);*/


		FUN_DEF(endswith);
		PARAM(self, );
		PARAM(ending, );
		auto meAsBytes = reinterpret_cast<pyStr*>(self._ptr.get());
		meAsBytes->endswith(ending);
		END_FUN(endswith);

		FUN_DEF(join);
		PARAM(self, );
		PARAM(iterable, );
		auto meAsBytes = reinterpret_cast<pyStr*>(self._ptr.get());
		meAsBytes->join(iterable);
		END_FUN(join);

	}

}
namespace py
{
	void binary::AddAttributes()
	{
		(*this).attr(hex) = py_bytes::hex;

		//attr(count) = py_list::count;

		//(*this).attr(decode) = py_binary::decode;

		(*this).attr(endswith) = py_binary::endswith;

		//pystr commented out
		(*this).attr(join) = py_binary::join;
		
		//attr(find) = py_list::find;

		//attr(index) = py_list::index;

	}

	binary::binary() : object(pyBinary())
	{
		AddAttributes();
	}

	//Binary::Binary(unsigned char const& o) : object(pyBinary(o))
	//{
	//	AddAttributes();
	//}
	binary::binary(std::vector<unsigned char> const& v)
		: object(pyBinary(std::vector<unsigned char>(v.begin(), v.end())))
	{
		AddAttributes();
	}
}
