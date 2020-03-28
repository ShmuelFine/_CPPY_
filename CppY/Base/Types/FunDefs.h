#pragma once
#include "object.h"
#include <string>
namespace py
{
	class ARGS
	{
	private:
		object _;
		
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

#define FUN_DEF_BODY(funName)\
public:\
virtual std::string Type() const override { return std::string("_func_") + #funName; }\
virtual object Clone() const override {return object(new func_ ##funName());}\
virtual object operator()(pyObj const& paramsObj) const override { return CALL(paramsObj); }\
\
virtual object CALL(pyObj const & paramsObj) const \
{ \
	pyDict const * params_ptr = reinterpret_cast<pyDict const *>(&paramsObj);\
	pyDict const & params = *params_ptr;\
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
		if (isNamedArgExist__ ##paramName)			{paramName = params.at(_key__ ##paramName);} \
		else if (isUnNamedArgExist__ ##paramName)	{paramName = params.at(object(__param_counter__++));}\
		else if (hasDefaultValue__ ##paramName)		{paramName = object(__VA_ARGS__); }\
	}

#define PARAM(paramName, ...) PARAM_GENERIC(paramName, true, true, __VA_ARGS__);
#define PARAM_BY_KEY(paramName, ...) PARAM_GENERIC(paramName, true, false, __VA_ARGS__);
#define PARAM_BY_POS(paramName, ...) PARAM_GENERIC(paramName, false, true, __VA_ARGS__);

#define HAS_MORE_POS_PARAMS() params._impl.count(object(__param_counter__))

#define FUN_DEF(funName)\
class func_ ##funName : public pyFunc { \
FUN_DEF_BODY(funName)

//#define FUN_DEF_WITH_DOC(funName, docStr)\
//class func_ ##funName : public pyFunc { \
//func_ ##funName()\
//{\
//	attributes["__doc__"] = docStr\
//}\
//FUN_DEF_BODY(funName)

#define END_FUN(funName)\
	 return None; \
	} \
};\
func_ ##funName __func_ ##funName ##__instance;\
object funName(__func_ ##funName ##__instance);

#define END_FUN_WITH_DOC_STR(funName, DocStr)\
	 return None; \
	} \
};\
func_ ##funName __func_ ##funName ##__instance;\
object funName(__func_ ##funName ##__instance, DocStr);


