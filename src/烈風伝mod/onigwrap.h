#pragma once

#include <string>
#include <map>
#include "../oniguruma/oniguruma.h"

using namespace std;


using Matches = std::map<int, string>;

/*
	Matches matches;
	if ( OnigMatch("能力と表現", "(能)(.+)(現)", &matches) ) {
		OutputDebugStream << "0:" << matches[0] << ",1:" << matches[1] << ",2:" << matches[2] << endl;
	}
 */
int OnigMatch(string str_target, string str_regex, Matches* pmatches = NULL);
