//
// Created by fine on 12/8/19.
//

#include "os.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <sstream>
#include "ghc/filesystem.hpp"
namespace fs = ghc::filesystem;
using namespace std;

namespace py
{
	os_t os;

	std::string path_t::join(std::string const& p1, std::string const& p2)
	{
		return fs::path(p1).append(p2).string();
	}

	std::string path_t::join(std::string const& p1, std::string const& p2, std::string const& p3)
	{
		return join(join(p1 ,p2),p3);
	}

	bool path_t::exists(std::string const& path)
	{
		//return (std::ifstream(path).is_open());
		return fs::exists(path);
	}

	void path_t::splitext(std::string const& path, std::string& out_filename, std::string& out_extension)
	{
		std::size_t extPos = path.find(".");
		if (extPos == std::string::npos || extPos == 0)
		{
			out_filename = path;
			out_extension = "";
		}
		else
		{
			out_filename = path.substr(0, extPos);
			out_extension = path.substr(extPos, path.length());
		}

	}

	std::string path_t::abspath(std::string const& relPath)
	{
		return fs::absolute(relPath.c_str()).string();
	}

	bool path_t::isfile(std::string const& path)
	{
		return fs::is_regular_file(path);
	}

	std::vector<std::string> splitpath(
		const std::string& str
		, const std::set<char> delimiters)
	{
		std::vector<std::string> result;

		char const* pch = str.c_str();
		char const* start = pch;
		for (; *pch; ++pch)
		{
			if (delimiters.find(*pch) != delimiters.end())
			{
				if (start != pch)
				{
					std::string str(start, pch);
					result.push_back(str);
				}
				else
				{
					result.push_back("");
				}
				start = pch + 1;
			}
		}
		result.push_back(start);

		return result;
	}

	std::string path_t::basename(std::string const& path)
	{
		return splitpath(path, { '\\', '/' }).back();
	}

	void path_t::makedirs(std::string const& path)
	{
		fs::create_directories(path);
	}

	bool path_t::isdir(std::string const& path)
	{
		return fs::is_directory(path);
	}


	void splitext(std::string const& path, std::string& out_filename, std::string& out_extension)
	{
		out_extension = fs::path(path).extension().string();
		out_filename = fs::path(path).stem().string();
	}


	void os_t::remove(std::string const& p)
	{
		fs::remove(p.c_str());
	}

	std::string os_t::devnull()
	{
		return fs::temp_directory_path().string();
	}

	void os_t::rename(std::string const& src, std::string const& dst)
	{
		fs::rename(src.c_str(), dst.c_str());
	}

	//
	//
	std::vector<std::string> ListFilePaths(std::string const& path)
	{
		std::vector<std::string> result;
		for (const auto& entry : fs::directory_iterator(path))
			result.push_back(entry.path().string());
	
		return result;
	}

	std::vector<std::string> ListFileNames(std::string const& path)
	{
		std::vector<std::string> result;
		for (const auto& entry : fs::directory_iterator(path))
			result.push_back(string(entry.path().filename().string())); //stem().string()
	
		return result;
	}
}