#include "pch.h"
#include "..\Base\Types\dict.h"
using namespace py;

TEST(dict, Initialization)
{
	//std::map<object, object> initVec_objs = { {"hello","2"}, {"World","4"} };
	dict({ {"hello","2"}, {"World", 321} , });
}