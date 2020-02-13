#include "subprocess.h"
#include "os.h"


namespace py
{
	void subprocess_t::call(list const& args, str const & stdoutFile, str const & stderrFile)
	{
		std::string command = str(" ").join(args);
		if (!stdoutFile.empty())
			command += " > " + stdoutFile;
		if (!stderrFile.empty())
			command += " 2> " + stderrFile;
		system(command.c_str());
	}
	
	// TODO, HACK
	Popen_dummy subprocess_t::Popen(list const& args, str const& stdoutFile, str const& stderrFile)
	{
		call(args, stdoutFile, stderrFile);
		return Popen_dummy();
	}

	subprocess_t subprocess;

}
