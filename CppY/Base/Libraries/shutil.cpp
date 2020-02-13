#include "shutil.h"
#ifndef _WIN_
#include "ghc/filesystem.hpp"
namespace fs = ghc::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif
using namespace std;

namespace py
{

	void shutil_t::rmtree(str path, object ignore_errors, object onerror)
	{
		error_code err;
		fs::remove_all(path.c_str(), err);

		if (!ignore_errors)
		{
			if (onerror == None)
			{
				throw PyException((str("Couldn't delete ") + path).c_str());
			}
			else
			{
				NOT_IMPL;
			}
		}
	}

	void shutil_t::copy(str src, str dst)
	{
		fs::copy(src.c_str(), dst.c_str());
	}

	void shutil_t::copyfile(str src, str dst)
	{
		fs::copy_file(src.c_str(), dst.c_str());
	}

	shutil_t shutil;
}