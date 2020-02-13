#pragma once
#include "object.h"
#include <string>
namespace py
{
	class ARGS : private pyDict
	{
	public:
		ARGS(object const& key, object const& value);
		ARGS(object const& value);

		ARGS & A(object const& key, object const& value);
		ARGS & A(object const& value);

		bool IsNamed;
		int ArgCounter;

		operator object();
	};

};

#define MEM_FUN(obj, fun) obj.attr(fun)(ARGS(obj)

#define FUN_DEF(funName) class func_ ##funName : public pyFunc { \
public:\
virtual std::string Type() const override { return std::string("_func_") + #funName; }\
virtual pyObjPtr Clone() const override {return pyObjPtr(new func_ ##funName());}\
virtual pyObjPtr operator()(pyObj& paramsObj) const override { return CALL(paramsObj); }\
virtual object CALL(pyObj& paramsObj) const { \
pyDict * params_ptr = reinterpret_cast<pyDict *>(&paramsObj);\
pyDict & params = *params_ptr;\
int __param_counter__ = 0;

#define PARAM(paramName, ...)\
object _key__ ##paramName(#paramName);\
bool isNamedArgExist__ ##paramName = params._impl.count(_key__ ##paramName);\
bool isUnNamedArgExist__ ##paramName = params._impl.count(object(__param_counter__));\
bool hasDefaultValue__ ##paramName = strlen(#__VA_ARGS__) > 0;\
THROW_UNLESS(isNamedArgExist__ ##paramName || isUnNamedArgExist__ ##paramName || hasDefaultValue__ ##paramName,\
			"Not enough parameters given - can't fetch param " #paramName); \
object paramName;\
if (isNamedArgExist__ ##paramName)			{paramName = params[_key__ ##paramName];} \
else if (isUnNamedArgExist__ ##paramName)	{paramName = params[object(__param_counter__++)];}\
else if (hasDefaultValue__ ##paramName)		{paramName = object(__VA_ARGS__); }\
else THROW_IF(true, "Error - could not retrieve param.");


#define END_FUN(funName) return None; } };\
 func_ ##funName __func_ ##funName ##__instance;\
 object funName(__func_ ##funName ##__instance);