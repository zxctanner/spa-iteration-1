#pragma once
#include "QE.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <map>
#include <regex>
#include <string>
#include <ctype.h>

class QP {

public:
	
	vector<string> assignD;
	vector<string> whileD;
	vector<string> stmtD;
	vector<string> variableD;
	vector<string> constantD;
	vector<string> prog_lineD;
	vector<string> queryUsedSyns;
	vector<string> queriesForQE;
	string querySyn;

public:

	QP QueryParser();
	void selectField(vector<string>);
	string trim(const string& str, const string& trimmers);
	vector<string> separateDQ(string& str);
	bool validNoSTPattern(string& str);
	vector<string> tokenize(const string& str, const string& delimiters);
	void addSynonymsToCorrectType(string type, vector<string> synonyms);
	bool checkIfSynDontExist(string syn);
	void processingDeclarations(vector<string> declarationTokens);
	void declarationPrinter();
	bool extractSTSyn(string& query);
	bool extractPatternSyns(string& query);
	string extractPattern(string& queryString);
	string extractST(string& queryString);
	void queryStringHandler(string queryString);
	void queryPrinter();
	void clearMemory();
	bool isInt(string input)
};