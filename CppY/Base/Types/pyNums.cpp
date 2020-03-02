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

}