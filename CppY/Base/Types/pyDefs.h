#pragma once

#include "PyObj.h"
#include "pyStr.h"
#include "pyList.h"
#include "pyDict.h"
#include "pyAutoType.h"
#include "pyNone.h"
#include "pyExceptions.h"
#include "pyLambda.h"

namespace py
{
	typedef pyDict dict;
	template<typename T> using defaultdict = pyDefaultDict<T>;


	template<typename T> bool isinstance(object const& o) { return o->Type() == typeid(T).name();}
}