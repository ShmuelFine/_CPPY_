#include "Int.h"
#include "pyNums.h"
#include "FunDefs.h"
#include "pyBytes.h"

namespace py
{
	namespace py_int
	{

		int flooredLog(int x)
		{
			x = abs(x);
			int log2x = 0;
			while (x > 0)
			{
				log2x++;
				x /= 2;
			}
			return log2x;
		}

		//////////////////////

		FUN_DEF(bit_length);
		PARAM(self, );
		int x = (int)self;
		if (0 == x)
			return 0;

		return flooredLog(x);
		END_FUN(bit_length);

		//////////////////////

		//FUN_DEF(to_bytes);
		//PARAM(self, );
		//PARAM(outputSize, );

		//// TODO: Only a very partial solution
		//int param = self;
		//param = abs(param);
		//std::vector<pyObjPtr> outArray(outputSize + 1);
		//for (size_t i = 0; i < (size_t)outputSize; i++)
		//	outArray[outputSize - i].reset(new pyInt(param >> (i * 8)));

		//return pyBytes(outArray);

		//END_FUN(to_bytes);

		//////////////////////

		FUN_DEF(bin);
		PARAM(self, );
		throw "Not yet tested";
		auto meAsInt = reinterpret_cast<pyInt*>(self._ptr.get());
		return meAsInt->bin();
		END_FUN(bin);


	}
}
namespace py
{

	void Int::AddAttributes()
	{
		(*this).attr(bit_length) = py_int::bit_length;
		(*this).attr(bin) = py_int::bin;
		//(*this).attr(to_bytes) = py_int::to_bytes;

	}

	Int::Int(object const& o) : object(pyInt((int)o))
	{
		AddAttributes();
	}

	Int::Int(int i) : object(pyInt(i))
	{
		AddAttributes();
	}

}