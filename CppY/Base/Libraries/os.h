//
// Created by fine on 12/8/19.
//

#ifndef HELLO_LIBS_OS_H
#define HELLO_LIBS_OS_H

#include <string>
#include <vector>

namespace py
{
	struct path_t
	{
		std::string join(std::string const& p1, std::string const& p2);
		std::string join(std::string const& p1, std::string const& p2, std::string const& p3);
		bool exists(std::string const& path);
		std::string basename(std::string const& path);
		void makedirs(std::string const& path);
		bool isdir(std::string const& path);
		bool isfile(std::string const& path);
		void splitext(std::string const& path, std::string& out_filename, std::string& out_extension);
		std::string abspath(std::string const& relPath);
	};

	struct os_t
	{
		path_t path;
		void makedirs(std::string const& p, bool exist_ok = true)
		{
			path.makedirs(p);
		}
		void remove(std::string const& p);
		std::string devnull();
		void rename(std::string const& src, std::string const& dst);
	};

	extern os_t os;


	void splitext(std::string const& path, std::string& out_filename, std::string& out_extension);

	std::vector<std::string> ListFilePaths(std::string const& folder);
	std::vector<std::string> ListFileNames(std::string const& folder);

}

#endif //HELLO_LIBS_OS_H
