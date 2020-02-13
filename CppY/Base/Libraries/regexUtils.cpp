#include "regexUtils.h"

#include <regex>
using namespace std;

int MatchesAmount(string const & s, string const & re)
{
	regex words_regex(re);
	auto words_begin = sregex_iterator(s.begin(), s.end(), words_regex);
	auto words_end = sregex_iterator();
	return (int)distance(words_begin, words_end);
}