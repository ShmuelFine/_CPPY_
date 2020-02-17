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

#define PARAM_GENERIC(paramName, isByKeyAllowed, isByPositionAllowed, ...)\
object paramName;\
{ \
	object _key__ ##paramName(#paramName);\
	bool isNamedArgExist__ ##paramName = isByKeyAllowed && params._impl.count(_key__ ##paramName);\
	bool isUnNamedArgExist__ ##paramName = isByPositionAllowed && params._impl.count(object(__param_counter__));\
	bool hasDefaultValue__ ##paramName = strlen(#__VA_ARGS__) > 0;\
	THROW_UNLESS(isNamedArgExist__ ##paramName || isUnNamedArgExist__ ##paramName || hasDefaultValue__ ##paramName,\
				"Not enough parameters given - can't fetch param " #paramName); \
	if (isNamedArgExist__ ##paramName)			{paramName = params[_key__ ##paramName];} \
	else if (isUnNamedArgExist__ ##paramName)	{paramName = params[object(__param_counter__++)];}\
	else if (hasDefaultValue__ ##paramName)		{paramName = object(__VA_ARGS__); }\
}

#define PARAM(paramName, ...) PARAM_GENERIC(paramName, true, true, __VA_ARGS__);
#define PARAM_BY_KEY(paramName, ...) PARAM_GENERIC(paramName, true, false, __VA_ARGS__);
#define PARAM_BY_POS(paramName, ...) PARAM_GENERIC(paramName, false, true, __VA_ARGS__);

#define HAS_MORE_POS_PARAMS() params._impl.count(object(__param_counter__ + 1))

#define END_FUN(funName) return None; } };\
 func_ ##funName __func_ ##funName ##__instance;\
 object funName(__func_ ##funName ##__instance);