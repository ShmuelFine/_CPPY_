//
// Created by fine on 12/8/19.
//

#ifndef HELLO_LIBS_SUBPROCESS_H__
#define HELLO_LIBS_SUBPROCESS_H__

#include "CppY.h"

#include <fstream>

namespace py
{
	struct Popen_dummy
	{
		void communicate() {};
	};

	struct subprocess_t
	{
		void call(list const& args, str const& stdoutFile = "", str const& stderrFile = "");
		Popen_dummy Popen(list const& args, str const& stdoutFile = "", str const& stderrFile = "");
	};

	extern subprocess_t subprocess;



}

#define PIPE (R"(.\PIPE)")
#define PIPE_2_ (R"(.\PIPE_2_)")

#endif //HELLO_LIBS_PYSTR_H


