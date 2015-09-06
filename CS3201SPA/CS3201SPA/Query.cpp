#include "Query.h"

using namespace std;

Query::Query(){}

Query::Query(vector<string> stmtd, vector<string> assignd, vector<string> variabled, vector<string> constantd, vector<string> whiled, vector<string> prog_lined, vector<string> queryfield)
{
	stmtD = stmtd;
	assignD = assignd;
	variableD = variabled;
	constantD = constantd;
	whileD = whiled;
	prog_lineD = prog_lined;
	queryFields = queryfield;
}

stmtType Query::checkSynType(string syn) {
	/*if (std::find(stmtD.begin(), stmtD.end(), syn) != stmtD.end()) {
		type = "stmt";
	}*/
	stmtType type;
	if (std::find(assignD.begin(), assignD.end(), syn) != assignD.end()) {
		type = ASSIGN;
	}
	if (std::find(whileD.begin(), whileD.end(), syn) != whileD.end()) {
		type = WHILE;
	}/*
	if (std::find(variableD.begin(), variableD.end(), syn) != variableD.end()) {
		type = "variable";
	}
	if (std::find(prog_lineD.begin(), prog_lineD.end(), syn) != prog_lineD.end()) {
		type = "prog_line";
	}
	if (std::find(constantD.begin(), constantD.end(), syn) != constantD.end()) {
		type = "constant";
	}*/
	return type;
}

vector<string> Query::getQueryFields() {
	return queryFields;
}


Query::~Query()
{
}
