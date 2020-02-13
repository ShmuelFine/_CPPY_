#include "PyBinary.h"
#include "pyBinary.h"
namespace py
{

	std::string pyBinary::Type() const
	{
		return "bytes";
	}
	std::string pyBinary::hex()
	{
		std::string hex1;
		std::string hex2;
		for (auto elem : *this)
		{
			auto myByte = reinterpret_cast<pyByte*>(elem.get());
			hex1=myByte->GetHexDigit(0);
			hex2=myByte->GetHexDigit(1);
		}
		return hex1 + hex2;
	}

	std::string pyBinary::decode(std::string encoding, std::string errors)
	{
		NOT_IMPL;
	}

}