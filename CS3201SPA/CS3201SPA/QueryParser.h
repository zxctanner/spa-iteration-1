#pragma once
using namespace std;
#include <vector>
#include <map>

class QueryParser {

private:
	multimap<string, string> queryTable;

public:
	QueryParser();
	string ParseInput(string);
	string ParseField(string);
	//multimap<string, string> createQTable(vector<string>&);
	string extractST(string& str);
	string extractPattern(string& str);
	string extractFirstWord(string& str);
	vector<string> Tokenize(const string& str, const string& delimiters);
	string trim(const string& str, const string& trimmers);
	vector<string> extractSynonyms(string& str);
	string removeCharsFromString(string &str, char* charsToRemove);
};