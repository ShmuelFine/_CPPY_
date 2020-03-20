#include "pyType.h"
#include "FunDefs.h"

namespace py
{

	pyObjPtr pyType::Clone() const
	{
		auto result = std::make_shared<pyType>(*Template);
		result->attributes = attributes;
		return result;
	}
}