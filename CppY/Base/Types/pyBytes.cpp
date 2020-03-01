#include "pyBytes.h"
#include "Comparisons.h"
#include "algorithm"
namespace py
{
	pyBytes::pyBytes(std::initializer_list<unsigned char> const& v)
	{
		for (auto elem : v) _impl.push_back(new pyByte(elem));
		for (pyObjPtr& i : _impl) i.IsAssignable = false;

	}
	pyBytes::pyBytes(std::vector<unsigned char> const& v)
	{
		for (auto elem : v) _impl.push_back(new pyByte(elem));
		for (pyObjPtr& i : _impl) i.IsAssignable = false;
	}

	pyBytes::pyBytes(std::string const& v) {
		auto numStrs = pyStr(v).split(R"(\\x)");
		//std::vector<unsigned char> res;
		for (auto singleNumStr : numStrs)
		{
			std::stringstream ss;
			ss << std::hex << singleNumStr;
			int tst;
			ss >> tst;
			//res.push_back(tst);
			_impl.push_back(new pyByte(tst));
		}
		for (pyObjPtr& i : _impl) i.IsAssignable = false;
		//*this=pyBytes(res);
	}

	pyObjPtr pyBytes::Clone() const
	{
		std::shared_ptr<pyBytes> result(new pyBytes());
		for (pyObjPtr oPtr : this->_impl)
			result->append(oPtr);
		return result;
	}

	std::string pyBytes::Type() const
	{
		return "bytes";
	}

	pyBytes::operator std::string() const
	{
		std::string result = "";
		for (auto elem : _impl)
		{
			result += "\\x" + (std::string)*elem;
		}
		return result;
	}

	std::string pyBytes::hex() const
	{
		std::string result = "";
		for (auto elem : _impl)
		{
			result += (std::string) * elem;
		}
		return result;
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
		return (res == _impl.end() - ending._impl.size());
	}

	bool pyBytes::startswith(pyBytes const& open)
	{
		if (open._impl.size() > this->_impl.size())
			return false;

		auto res = std::search(_impl.begin(), _impl.end(), open._impl.begin(), open._impl.end());
		return (res == _impl.begin());
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