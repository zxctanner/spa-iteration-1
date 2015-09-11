#pragma once
#ifndef QP_H
#define QP_H
#include "QE.h"
#include "Query.h"
#include "PKB.h"
#include <ctype.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <map>
#include <regex>
#include <fstream>
#include <string>



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
	Query queryForQE;
	string querySyn;
	string inputFileName;
	string queryStr;
	bool valid; //if at anytime valid flag turns to false, return none

public:
	QP();
	QP(string fileName);
	void process();
	void processSingleQuery(string query);
	vector<Query> getVectorQuery();
	Query getQuery();

private:

	string checkSynType(string syn);
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
	void queryStringHandler(string queryString);
	void singleQueryStringHandler(string queryString);
	void clearMemory();
	bool isQueryLegit(string rawQueryString);
	bool checkValidQuery(Query query);
	vector<string> formattedSTQE(string st, string qSyn, string typeOfQuery);

};

#endif