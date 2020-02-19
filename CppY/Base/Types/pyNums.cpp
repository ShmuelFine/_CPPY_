#include "pyNums.h"
namespace py
{
	pyByte::operator std::string() const
	{
		std::stringstream ss;
		ss << std::setw(2) << std::setfill('0') << std::hex << int(val);
		return ss.str();
	}

}