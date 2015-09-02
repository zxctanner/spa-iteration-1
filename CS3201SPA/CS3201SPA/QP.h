#pragma once
#include <map>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

class QP {

public:

	QP QueryParser();
	string ParseInput(string);
	string ParseField(string);
	string trim(const string& str, const string& trimmers);
	vector<string> separateDQ(string& str);
	bool validNoSTPattern(string& str);
};