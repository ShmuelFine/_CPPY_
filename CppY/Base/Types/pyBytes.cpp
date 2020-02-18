#include "pyBytes.h"
namespace py
{

	std::string pyBytes::Type() const
	{
		return "bytes";
	}

	std::string pyBytes::hex()
	{
		std::string hex1, hex2;
		for (auto elem : *this)
		{
			auto myByte = reinterpret_cast<pyByte*>(elem.get());
			hex1 = myByte->GetHexDigit(0);
			hex2 = myByte->GetHexDigit(1);
		}
		return hex1 + hex2;
	}


	std::string pyBytes::decode(std::string encoding, std::string errors)
	{
		NOT_IMPL;
	}

	bool pyBytes::endswith(pyBytes const& ending)
	{
		if (ending._impl.size() > this->_impl.size()) return false;
		return std::equal(ending._impl.rbegin(), ending._impl.rend()-1, _impl.rbegin());
	}

	bool pyBytes::startswith(pyBytes const& open)
	{
		if (open._impl.size() > this->_impl.size()) return false;
		return std::equal(open._impl.begin(), open._impl.end(), _impl.begin());

	}

	pyList pyBytes::replace(pyBytes const& what, pyBytes const& withWhat)
	{
		auto b = _impl;
		//std::replace(b.begin(), b.end(), what, withWhat);
		return b;
	}

	pyBytes pyBytes::center(int width, pyByte fillByte)
	{
		std::vector<unsigned char> newB((width / 2), fillByte);
		for (auto elem : *this) {
			auto myByte = reinterpret_cast<pyByte*>(elem.get());
			newB.push_back(*myByte);
		}
		newB.insert(newB.end(), (width / 2), fillByte);
		return newB;
	}

	pyTuple pyBytes::partition(pyByte const& delimitor)
	{
		/*auto meAsList = reinterpret_cast<pyList*>(this);
		auto len= reinterpret_cast<pyObj*>(meAsList->_impl.size());
		auto sep = delimitor.Clone();
		int sepInd = meAsList->index(sep, 0, len, true);
		std::vector<unsigned char> vec;
		auto it = meAsList->begin();
		std::vector<unsigned char> before(it, (it) + sepInd);
		vec.push_back(before);
		vec.push_back(delimitor);
		std::vector<unsigned char> after(it + sepInd+1, meAsList->end());
		vec.push_back(after);*/
		throw;
	}

}