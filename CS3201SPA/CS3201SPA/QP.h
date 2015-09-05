#pragma once
#include "QE.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <map>
#include <regex>
#include <fstream>
#include <string>
#include <ctype.h>
#include "PKB.h"

class QP {

public:

	vector<string> assignD;
	vector<string> whileD;
	vector<string> stmtD;
	vector<string> variableD;
	vector<string> constantD;
	vector<string> prog_lineD;

private:

	vector<vector<string>> queriesForQE;
	vector<string> queryUsedSyns;
	string querySyn;
	string relQ;
	string leftQ;
	string rightQ;
	string ansF;
	bool valid; //if at anytime valid flag turns to false, return none

public:
	QP(string fileName, PKB* pkb);
	void startQP(string fileName, PKB pkb);
	string checkSynType(string syn);

private:
	void selectField(vector<string>);
	string trim(const string& str, const string& trimmers);
	vector<string> separateDQ(string& str);
	bool validNoSTPattern(string& str);
	vector<string> tokenize(const string& str, const string& delimiters);
	bool addSynonymsToCorrectType(string type, vector<string> synonyms);
	bool checkIfSynDontExist(string syn);
	bool processingDeclarations(string declarations);
	void declarationPrinter();
	
	bool extractSTSyn(string& query);
	bool extractPatternSyns(string& query);
	string extractPattern(string& queryString);
	string extractST(string& queryString);
	void queryStringHandler(string queryString);
	void queryPrinter();
	void clearMemory();
	bool isQueryLegit(string rawQueryString);
	vector<string> formattedSTQE(string st, string qSyn, string typeOfQuery);
	bool isInt(string input);
};