#pragma once

#include "pyList.h"
#include <memory>
namespace py
{
	template<typename Deduction, typename Container>
	pyList CreateListWithLambda(Deduction what, Container const & inWhere)
	{
		std::vector<pyObjPtr> result;
		for (auto x : inWhere)
		{
			result.push_back(what(x));
		}
		return result;
	}

#define LAMBDA_BASE(what, whom, inWhere) CreateListWithLambda([&](auto const & whom) {return what;}, inWhere)

	template<typename Deduction, typename Container>
	pyList CreateListWithConditionalLambda(Deduction condition, Container const & inWhere)
	{
		pyList result;
		for (auto const & x : inWhere)
		{
			if (condition(x))
				result.append(x);
		}
		return result;
	}

#define LAMBDA_IF_BASE(condition, whom, inWhere) CreateListWithConditionalLambda([&](object const & whom) {return condition;}, inWhere)

//	template<typename Action>
//	pyList map(Action what, pyList const & inWhere)
//	{
//		pyList result;
//		for (auto& x : inWhere)
//		{
//			if (std::dynamic_pointer_cast<pyList>(x))
//			{
//				result.extend(map(what, As<pyList>(x)));
//			}
//			else
//				result.append(what(x));
//		}
//		return result;
//	}
//
//#define MAP(what, l) map([&](auto & x){return what(x);}, l)

	//#define STRize [](object const& x) {return str(x); }
}
