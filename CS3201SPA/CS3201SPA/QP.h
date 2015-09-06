#pragma once
#ifndef QP_H
#define QP_H
#include "QE.h"
#include "Query.h"
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

private:

	vector<string> assignD;
	vector<string> whileD;
	vector<string> stmtD;
	vector<string> variableD;
	vector<string> constantD;
	vector<string> prog_lineD;
	vector<Query> queriesForQE;
	vector<string> queryUsedSyns;
	string querySyn;
	string inputFileName;
	bool valid; //if at anytime valid flag turns to false, return none

public:
	QP(string fileName);
	void process();
	vector<Query> getVectorQuery();

private:

	string trim(const string& str, const string& trimmers);
	vector<string> separateDQ(string& str);
	bool validNoSTPattern(string& str);
	vector<string> tokenize(const string& str, const string& delimiters);
	bool addSynonymsToCorrectType(string type, vector<string> synonyms);
	bool checkIfSynDontExist(string syn);
	bool processingDeclarations(string declarations);
	vector<string> combineVector(vector<string> first, vector<string> second);
	void declarationPrinter();
	bool extractSTSyn(string& query);
	bool extractPatternSyns(string& query);
	string extractPattern(string& queryString);
	string extractST(string& queryString);
	bool queryStringHandler(string queryString);
	void clearMemory();
	bool isQueryLegit(string rawQueryString);
	vector<string> formattedSTQE(string st, string qSyn, string typeOfQuery);

};

#endif