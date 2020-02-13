#pragma once

#include "CppY.h"


namespace py
{
	struct shutil_t
	{
		void rmtree(str path, object ignore_errors = False, object onerror = None);
		void copy(str src, str dst);
		void copyfile(str src, str dst);
	};

	extern shutil_t shutil;
}