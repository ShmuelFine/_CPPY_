#include "pyBytes.h"
#include "Comparisons.h"
#include "algorithm"
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
		if (ending._impl.size() > this->_impl.size()) 
			return false;
		
		auto res = std::search(_impl.begin(), _impl.end(), ending._impl.begin(), ending._impl.end());
		return res != _impl.end();
	}

	bool pyBytes::startswith(pyBytes const& open)
	{
		if (open._impl.size() > this->_impl.size()) return false;
		return std::equal(open._impl.begin(), open._impl.end(), _impl.begin());

	}

	pyBytes pyBytes::replace(pyBytes const& what, pyBytes const& withWhat)
	{
		//auto newObj = _impl.begin();
		//auto itPos = _impl.begin();
		//while (itPos != _impl.end()){
		//itPos = std::search(itPos,_impl.end(),what.begin(),what.end());
		//newObj = std::copy(withWhat.begin(), withWhat.end(), itPos);
		//}
		//auto asBytes = reinterpret_cast<pyBytes*>(newObj._Ptr->get());
		//return asBytes->_impl;
		throw;
	}

	pyBytes pyBytes::center(int width, pyByte fillByte)
	{
		// width must be even:
		width = width - width % 2;
		std::vector<unsigned char> result_vec(width + _impl.size(), fillByte);
		for (int i = 0; i < _impl.size(); i++)
			result_vec[width / 2 + i] = *_impl[i];
		
		return pyBytes(result_vec);
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

	int pyBytes::rindex(pyBytes o, pyInt i, pyInt j, bool throwEx)
	{
		//if (is_ofType(i, None))
		//	i = 0;
		//if (is_ofType(j, None))
		//	j = this->_impl.size();
		//auto it = std::find_end(
		//	_impl.begin() + i,
		//	_impl.end() - (_impl.size() - j), o.begin(),o.end());
		//if (it != _impl.end() - (_impl.size() - j))
		//	if (throwEx)
		//		THROW("Not found element");
		//	else
		//		return -1;
		//return (int)std::distance(_impl.begin() + i, it);
		//
		///*std::reverse(_impl.begin(), _impl.end());
		//auto iObjPtr = reinterpret_cast<pyObj*>(&i);
		//auto jObjPtr = reinterpret_cast<pyObj*>(&j);
		//int pos = this->index(o, iObjPtr, jObjPtr, throwEx);
		//return _impl.size() - pos;*/
		throw;
	}

}