#include "pyType.h"
#include "FunDefs.h"

namespace py
{

	object pyType::Clone() const
	{
		object resultObj(new pyType(*Template));
		pyType* resultPtr = reinterpret_cast<pyType*>(resultObj.get());
		resultPtr->attributes = attributes;
		return resultObj;
	}
}