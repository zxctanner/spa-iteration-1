#pragma once
using namespace std;
#include <vector>

class QueryParser {
public:
	QueryParser();
	string ParseInput(string);
	string ParseField(string);
	string ExtractST(string& str);
	string ExtractPattern(string& str);
	void Tokenize(const string& str, vector<string>& tokens, const string& delimiters);
	string Trim(const string& str, const string& trimmers);
};