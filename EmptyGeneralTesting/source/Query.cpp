#include "Query.h"

using namespace std;

Query::Query() {}

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

string Query::checkSynType(string syn) {
	string type;
	if (std::find(stmtD.begin(), stmtD.end(), syn) != stmtD.end()) {
		type = "STATEMENT";
	}

	if (std::find(assignD.begin(), assignD.end(), syn) != assignD.end()) {
		type = "ASSIGN";
	}
	if (std::find(whileD.begin(), whileD.end(), syn) != whileD.end()) {
		type = "WHILE";
	}
	if (std::find(variableD.begin(), variableD.end(), syn) != variableD.end()) {
		type = "VARIABLE";
	}
	if (std::find(prog_lineD.begin(), prog_lineD.end(), syn) != prog_lineD.end()) {
		type = "PROG_LINE";
	}
	if (std::find(constantD.begin(), constantD.end(), syn) != constantD.end()) {
		type = "CONSTANT";
	}
	return type;
}

vector<string> Query::getQueryFields() {
	return queryFields;
}

void Query::printQuery() {
	vector<string> fields = queryFields;
	if (fields.size() == 0) {
		cout << "Empty query!" << endl;
	}
	else {
		cout << "Queried synonym is: " + fields.at(0) << endl;
		cout << "Relationship is: " + fields.at(1) << endl;
		cout << "First operand is: " + fields.at(2) << endl;
		cout << "Second is: " + fields.at(3) << endl;
		if (fields.size() == 8) {
			cout << endl;
			cout << "2nd Queried synonym is: " + fields.at(4) << endl;
			cout << "2nd Relationship is: " + fields.at(5) << endl;
			cout << "First operand is: " + fields.at(6) << endl;
			cout << "Second is: " + fields.at(7) << endl;
		}
	}
}

//converts the query components to a continuous string, for testing
string Query::toString() {
	string str;
	if (queryFields.size() == 0) {
		str.append("Empty query");
	}
	else {
		for (int i = 0; i < stmtD.size(); ++i) {
			str.append("stmt:" + stmtD[i] + " ");
		}
		for (int i = 0; i < assignD.size(); ++i) {
			str.append("assign:" + assignD[i] + " ");
		}
		for (int i = 0; i < variableD.size(); ++i) {
			str.append("var:" + variableD[i] + " ");
		}
		for (int i = 0; i < constantD.size(); ++i) {
			str.append("const:" + constantD[i] + " ");
		}
		for (int i = 0; i < whileD.size(); ++i) {
			str.append("while:" + whileD[i] + " ");
		}
		for (int i = 0; i < prog_lineD.size(); ++i) {
			str.append("progL:" + prog_lineD[i] + " ");
		}
		str.append(". Query fields:");
		for (int i = 0; i < queryFields.size(); ++i) {
			str.append(queryFields[i] + " ");
		}
	}
	return str;
}

Query::~Query()
{
}