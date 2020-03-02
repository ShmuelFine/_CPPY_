#include "pyNums.h"
#include <vector>
#include <iterator>
namespace py
{
	pyByte::operator std::string() const
	{
		std::stringstream ss;
		ss << std::setw(2) << std::setfill('0') << std::hex << int(val);
		return ss.str();
	}


	std::string pyInt::bin()
	{
		std::vector<int> binary = std::vector<int>();
		std::stringstream result;
		if (val == 0)
			return "0b0";
		while (val > 0)
		{
			result << val % 2;
			val /= 2;
		}
		result << "b0";
		std::string s;
		result >> s;
		std::reverse(s.begin(), s.end());
		return s;
	}


	pyTuple pyInt::divmod(pyObjPtr const& x, pyObjPtr const& y)
	{ 
		std::pair<pyInt, pyInt> res(x->operator int / y->operator int, x->operator int% y->operator int);
		pyTuple result;
		result.append(&res.first);
		result.append(&res.second);
		//pySetPtr result1 = pySetPtr(new pySet());
		//std::set<int> res(x->operator int / y->operator int, x->operator int% y->operator int);
		//result->_impl.insert(res.begin(), res.end());
		return result;
	}

	pyTuple Float::divmod(pyObjPtr const& x, pyObjPtr const& y)
	{
		double div;
		double vx = x->operator double, wx = y->operator double;
		auto mod = fmod(vx, wx);
		div = (vx - mod) / wx;
		if (mod) {
			if ((wx < 0) != (mod < 0)) {
				mod += wx;
				div -= 1.0;
			}
		}
		else {
			mod = copysign(0.0, wx);
		}
		if (div) {
			auto floordiv = floor(div);
			if (div - floordiv > 0.5) {
				floordiv += 1.0;
			}
		}
		else {
			auto floordiv = copysign(0.0, vx / wx);
		}

		std::pair<Float, Float> res(div, mod);
		pyTuple result;
		result.append(&res.first);
		result.append(&res.second);
		//pySetPtr result = pySetPtr(new pySet());
		//std::set<float> res(div, mod);
		//result->_impl.insert(res.begin(), res.end());
		return result;
	}

}