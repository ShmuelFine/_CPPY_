#pragma once

#include "str.h"

struct tempfile_t
{
	str gettempdir();
};

tempfile_t tempfile;
