#pragma once
#include <map>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

class QP {

public:

	QP QueryParser();
	void ParseField(string field);
	string trim(const string& str, const string& trimmers);
	vector<string> separateDQ(string& str);
	bool validNoSTPattern(string& str);
	bool isInt(string input);
};