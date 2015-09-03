#pragma once

#include "QP.h"
#include "QE.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <map>
#include <regex>
#include <string>
#include <ctype.h>

using namespace std;

regex declaration_rgx("^((stmt|assign|while|variable|constant|prog_line)\\s+[a-zA-Z][a-zA-Z0-9#]*\\s*(,\\s*[a-zA-Z][a-zA-Z0-9#]*\\s*)*;\\s*)+$");
regex query_rgx("^Select\\s+[a-zA-Z][a-zA-Z0-9#]*(\\s+(((such that)\\s+(((Parent|Parent[*]|Follows|Follows[*])\\s*[(]\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*,\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*[)]\\s*)|((Modifies|Uses)\\s*[(]\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*,\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|\"[a-zA-Z][a-zA-Z0-9#]*\")\\s*[)]\\s*)))|((pattern)\\s+(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*[(]\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|(\"[a-zA-Z][a-zA-Z0-9#]*\"))\\s*,\\s*((_\\s*\"(([a-zA-Z][a-zA-Z0-9#]*)|([0-9]+))\"\\s*_)|_)\\s*[)]))){1,2}$");
regex synonym_rgx("^[a-zA-Z][a-zA-Z0-9#]*$");

//safe: regex query_rgx("^Select\\s+[a-zA-Z][a-zA-Z0-9#]*\\s+(such that)\\s+(((Parent|Parent[*]|Follows|Follows[*])\\s*[(]\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*,\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*[)]\\s*)|((Modifies|Uses)\\s*[(]\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*,\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|\"[a-zA-Z][a-zA-Z0-9#]*\")\\s*[)]\\s*))$");

//Functions:


/*
General flow of events in parser:
1. Receive query string
2. Remove leading and trailing whitespaces from query string - DONE
3. Splitting of string into 2 parts: Declarations string & Actual query string - DONE
4. Syntatic Validation of:
	a. Declarations string - DONE
	b. Actual query string - DONE
5. Semantic Validation of Actual query string (i.e. does the query make sense?)
6. Storing of declared types into table (i.e. assign a, a1; variable v; => assign type: a, a1; variable type: v)
7. Extracting exact queries from actual query string
8. Formatting queries (remove any unneccessary whitespaces)
9. Storing of formatted queries (should be storing each query as a string, so vector<string> queries of maximum size 2)


Validation will be conducted upon receiving query string, taking place in two separate phases:
1. Syntatic Validation (i.e. does query match the grammar rules?)
2. Semantic Validation (i.e. are the synonyms repeated, etc.)
*/

regex declaration_rgx("^((stmt|assign|while|variable|constant|prog_line)\\s+[a-zA-Z][a-zA-Z0-9#]*\\s*(,\\s*[a-zA-Z][a-zA-Z0-9#]*\\s*)*;\\s*)+$");
regex query_rgx("^Select\\s+[a-zA-Z][a-zA-Z0-9#]*(\\s+(((such that)\\s+(((Parent|Parent[*]|Follows|Follows[*])\\s*[(]\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*,\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*[)]\\s*)|((Modifies|Uses)\\s*[(]\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*,\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|\"[a-zA-Z][a-zA-Z0-9#]*\")\\s*[)]\\s*)))|((pattern)\\s+(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*[(]\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|(\"[a-zA-Z][a-zA-Z0-9#]*\"))\\s*,\\s*((_\\s*\"(([a-zA-Z][a-zA-Z0-9#]*)|([0-9]+))\"\\s*_)|_)\\s*[)]))){1,2}$");

QP QP::QueryParser() {

}

string QP::trim(const string& str, const string& trimmers) {
	const auto strBegin = str.find_first_not_of(trimmers);
	if (strBegin == string::npos) {
		return "";
	}
	const auto strEnd = str.find_last_not_of(trimmers);
	const auto strRange = strEnd - strBegin + 1;
	return str.substr(strBegin, strRange);
}

bool QP::validNoSTPattern(string& str) {
	int indexFirstST = str.find("such that");
	int indexLastST = str.rfind("such that");
	int indexFirstPattern = str.find("pattern");
	int indexLastPattern = str.rfind("pattern");
	if ((indexFirstST != indexLastST) || (indexFirstPattern != indexLastPattern)) {
		return false;
	}
	else {
		return true;
	}
}

vector<string> QP::separateDQ(string& str) {
	vector<string> processedStr;
	string declarations, queryClause, declarationsS, queryClauseS;
	int lastSemi = str.find_last_of(";");
	int selectIndex = str.find("Select");
	if (lastSemi == string::npos || selectIndex == string::npos) {
		cout << "Invalid query!" << endl;
		return processedStr;
	}
	//	cout << selectIndex << endl;
	declarations = str.substr(0, lastSemi + 1);
	declarationsS = str.substr(0, selectIndex - 1);
	queryClause = str.substr(lastSemi + 2);
	queryClauseS = str.substr(selectIndex);
	//	cout << "Dec using ; : " + declarations << endl;
	//	cout << "Dec using Select : " + declarationsS << endl;
	//	cout << "QC using ; : " + queryClause << endl;
	//	cout << "QC using Select : " + queryClauseS << endl;	
	if (declarations != declarationsS || queryClause != queryClauseS) {
		cout << "invalid syntax for query" << endl;
	}
	else {
		processedStr.push_back(declarations);
		processedStr.push_back(queryClause);
	}
	return processedStr;
}

vector<string> QP::tokenize(const string& str, const string& delimiters) {
	vector<string> tokens;
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	string::size_type pos = str.find_first_of(delimiters, lastPos);
	while (string::npos != pos || string::npos != lastPos) {
		tokens.push_back(QP::trim(str.substr(lastPos, pos - lastPos), " "));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastPos);
	}
	return tokens;
}

void QP::processingDeclarations(vector<string> declarationTokens) {
	string type;
	string currentToken;
	string decSynonyms;
	vector<string> tokenDecSynonyms;
	int indexFirstWS;
	for (auto currentToken : declarationTokens) {
		indexFirstWS = currentToken.find(" ");
		type = currentToken.substr(0, indexFirstWS);
		//cout << type << endl;
		decSynonyms = currentToken.substr(indexFirstWS);
		decSynonyms.erase(remove_if(decSynonyms.begin(), decSynonyms.end(), isspace), decSynonyms.end());
		tokenDecSynonyms = QP::tokenize(decSynonyms, ",");
		addSynonymsToCorrectType(type, tokenDecSynonyms);
	}
}

void QP::addSynonymsToCorrectType(string type, vector<string> synonyms) { //if synonyms already exists, can throw out error and return none (implement later)
	if (type == "stmt") {
		for (auto i : synonyms) {
			if (checkIfSynDontExist(i)) {
				QP::stmtD.push_back(i);
			}
		}
	}
	if (type == "assign") {
		for (auto i : synonyms) {
			if (checkIfSynDontExist(i)) {
				QP::assignD.push_back(i);
			}
		}
	}
	if (type == "while") {
		for (auto i : synonyms) {
			if (checkIfSynDontExist(i)) {
				QP::whileD.push_back(i);
			}
		}
	}
	if (type == "variable") {
		for (auto i : synonyms) {
			if (checkIfSynDontExist(i)) {
				QP::variableD.push_back(i);
			}
		}
	}
	if (type == "constant") {
		for (auto i : synonyms) {
			if (checkIfSynDontExist(i)) {
				QP::constantD.push_back(i);
			}
		}
	}
	if (type == "prog_line") {
		for (auto i : synonyms) {
			if (checkIfSynDontExist(i)) {
				QP::prog_lineD.push_back(i);
			}
		}
	}
}

bool QP::checkIfSynDontExist(string syn) {
	if ((std::find(QP::stmtD.begin(), QP::stmtD.end(), syn) == QP::stmtD.end()) &&
		(std::find(QP::assignD.begin(), QP::assignD.end(), syn) == QP::assignD.end()) &&
		(std::find(QP::whileD.begin(), QP::whileD.end(), syn) == QP::whileD.end()) &&
		(std::find(QP::variableD.begin(), QP::variableD.end(), syn) == QP::variableD.end()) &&
		(std::find(QP::constantD.begin(), QP::constantD.end(), syn) == QP::constantD.end()) &&
		(std::find(QP::prog_lineD.begin(), QP::prog_lineD.end(), syn) == QP::prog_lineD.end())) {
		return true;
	}
	else {
		return false;
	}
}

void QP::clearMemory() {
	QP::assignD.clear();
	QP::whileD.clear();
	QP::stmtD.clear();
	QP::variableD.clear();
	QP::constantD.clear();
	QP::prog_lineD.clear();
	QP::queryUsedSyns.clear();
	QP::queriesForQE.clear();
	QP::querySyn = "";
}

void QP::declarationPrinter() {
	cout << "STMT synonyms: " << endl;
	copy(QP::stmtD.begin(), QP::stmtD.end(), ostream_iterator<string>(cout, " "));
	cout << endl;
	cout << "ASSIGN synonyms: " << endl;
	copy(QP::assignD.begin(), QP::assignD.end(), ostream_iterator<string>(cout, " "));
	cout << endl;
	cout << "WHILE synonyms: " << endl;
	copy(QP::whileD.begin(), QP::whileD.end(), ostream_iterator<string>(cout, " "));
	cout << endl;
	cout << "VARIABLE synonyms: " << endl;
	copy(QP::variableD.begin(), QP::variableD.end(), ostream_iterator<string>(cout, " "));
	cout << endl;
	cout << "CONSTANT synonyms: " << endl;
	copy(QP::constantD.begin(), QP::constantD.end(), ostream_iterator<string>(cout, " "));
	cout << endl;
	cout << "PROG_LINE synonyms: " << endl;
	copy(QP::prog_lineD.begin(), QP::prog_lineD.end(), ostream_iterator<string>(cout, " "));
	cout << endl;
}

bool QP::extractSTSyn(string& query) { //Expected input: Modifies|Uses|Parent*|Parent|Follows|Follows*(blah,blah)
	string first;
	string second;
	int indexOfSep = query.find(",");
	int indexOfFirst = query.find("(");
	int indexOfSecond = query.find(")");
	first = query.substr(indexOfFirst + 1, indexOfSep - indexOfFirst - 1);
	second = query.substr(indexOfSep + 1, indexOfSecond - indexOfSep - 1);
	if (regex_match(first, synonym_rgx) && regex_match(second, synonym_rgx)) {
		if (!QP::checkIfSynDontExist(first) && !QP::checkIfSynDontExist(second)) {
			QP::queryUsedSyns.push_back(first);
			QP::queryUsedSyns.push_back(second);
			return true;
		}
		else {
			cout << "One or more of the synonyms used in query was not declared!" << endl;
			return false;
		}
	}
	else if (regex_match(first, synonym_rgx)) {
		if (!QP::checkIfSynDontExist(first)) {
			QP::queryUsedSyns.push_back(first);
			return true;
		}
		else {
			cout << "One or more of the synonyms used in query was not declared!" << endl;
			return false;
		}
	}
	else if (regex_match(second, synonym_rgx)) {
		if (!QP::checkIfSynDontExist(second)) {
			QP::queryUsedSyns.push_back(second);
			return true;
		}
		else {
			cout << "One or more of the synonyms used in query was not declared!" << endl;
			return false;
		}
	}
}

bool QP::extractPatternSyns(string& query) { //Expect input: patternblah(blah,blah)
	string first;
	string second;
	int count = 0;
	int indexOfN = query.find_first_of("n");
	int indexOfFB = query.find_first_of("(");
	int indexOfSep = query.find_first_of(",");
	first = query.substr(indexOfN + 1, indexOfFB - indexOfN - 1);
	second = query.substr(indexOfFB + 1, indexOfSep - indexOfFB - 1);
	if (regex_match(first, synonym_rgx) && regex_match(second, synonym_rgx)) {
		if (!QP::checkIfSynDontExist(first) && !QP::checkIfSynDontExist(second)) {
			if ((std::find(QP::queryUsedSyns.begin(), QP::queryUsedSyns.end(), first) != QP::queryUsedSyns.end())) {
				count += 1;
			}
			QP::queryUsedSyns.push_back(first);
			if ((std::find(QP::queryUsedSyns.begin(), QP::queryUsedSyns.end(), second) != QP::queryUsedSyns.end()) && count == 1) {
				cout << "Repeated Synonym detected in query!" << endl;
				return false;
			}
			else {
				QP::queryUsedSyns.push_back(second);
				return true;
			}
		}
		else {
			cout << "One or more of the synonyms used in query was not declared!" << endl;
			return false;
		}
	}
	else if (regex_match(first, synonym_rgx)) {
		if (!QP::checkIfSynDontExist(first)) {
			QP::queryUsedSyns.push_back(first);
			return true;
		}
		else {
			cout << "One or more of the synonyms used in query was not declared!" << endl;
			return false;
		}
	}
	else if (regex_match(second, synonym_rgx)) {
		if (!QP::checkIfSynDontExist(second)) {
			QP::queryUsedSyns.push_back(second);
			return true;
		}
		else {
			cout << "One or more of the synonyms used in query was not declared!" << endl;
			return false;
		}
	}
}

string QP::extractPattern(string& queryString) {
	//cout << queryString << endl;
	string pattern;
	int startOfPattern;
	int endOfPattern;
	string truncated;
	startOfPattern = queryString.find("pattern");
	truncated = queryString.substr(startOfPattern);
	endOfPattern = truncated.find_first_of(")");
	pattern = truncated.substr(0, endOfPattern + 1);
	cout << "Pattern is: " + pattern << endl;
	return pattern;
}

string QP::extractST(string& queryString) {
	string query;
	int startOfQuery;
	int endOfQuery;
	string truncated;
	//cout << queryString << endl;

	if (queryString.find("Modifies") != string::npos) {
		startOfQuery = queryString.find("Modifies");
		truncated = queryString.substr(startOfQuery);
		endOfQuery = truncated.find_first_of(")");
		query = truncated.substr(0, endOfQuery + 1);
	}
	if (queryString.find("Uses") != string::npos) {
		startOfQuery = queryString.find("Uses");
		truncated = queryString.substr(startOfQuery);
		endOfQuery = truncated.find_first_of(")");
		query = truncated.substr(0, endOfQuery + 1);
	}
	if (queryString.find("Parent") != string::npos) {
		startOfQuery = queryString.find("Parent");
		truncated = queryString.substr(startOfQuery);
		endOfQuery = truncated.find_first_of(")");
		query = truncated.substr(0, endOfQuery + 1);
	}
	if (queryString.find("Parent*") != string::npos) {
		startOfQuery = queryString.find("Parent*");
		truncated = queryString.substr(startOfQuery);
		endOfQuery = truncated.find_first_of(")");
		query = truncated.substr(0, endOfQuery + 1);
	}
	if (queryString.find("Follows") != string::npos) {
		startOfQuery = queryString.find("Follows");
		truncated = queryString.substr(startOfQuery);
		endOfQuery = truncated.find_first_of(")");
		query = truncated.substr(0, endOfQuery + 1);
	}
	if (queryString.find("Follows*") != string::npos) {
		startOfQuery = queryString.find("Follows*");
		truncated = queryString.substr(startOfQuery);
		endOfQuery = truncated.find_first_of(")");
		query = truncated.substr(0, endOfQuery + 1);
	}
	cout << "Such that: " + query << endl;
	return query;
}

void QP::queryStringHandler(string queryString) {
	queryString.erase(remove_if(queryString.begin(), queryString.end(), isspace), queryString.end());
	int indexOfST = queryString.find("such");
	int indexOfPattern = queryString.find("pattern");
	bool errorSyn;
	string pattern;
	string suchthat;
	//4 cases here
	//a. only have pattern
	if (indexOfST == string::npos) {
		querySyn = queryString.substr(6, indexOfPattern - 6);
		if (!QP::checkIfSynDontExist(querySyn)) {
			pattern = QP::extractPattern(queryString);
			errorSyn = QP::extractPatternSyns(pattern);
			if (errorSyn) {
				QP::queriesForQE.push_back(pattern);
			}
		}
		else {
			cout << ">>>>ERROR: QUERY SYN NOT DECLARED" << endl;
		}
	}
	//b. only have such that
	else if (indexOfPattern == string::npos) {
		querySyn = queryString.substr(6, indexOfST - 6);
		if (!QP::checkIfSynDontExist(querySyn)) {
			suchthat = QP::extractST(queryString);
			errorSyn = QP::extractSTSyn(suchthat);
			if (errorSyn) {
				QP::queriesForQE.push_back(suchthat);
			}
		}
		else {
			cout << ">>>>ERROR: QUERY SYN NOT DECLARED" << endl;
		}
	}
	//c. such that before pattern
	else if (indexOfST < indexOfPattern) {
		querySyn = queryString.substr(6, indexOfST - 6);
		if (!QP::checkIfSynDontExist(querySyn)) {
			suchthat = QP::extractST(queryString);
			errorSyn = QP::extractSTSyn(suchthat);
			if (errorSyn) {
				pattern = extractPattern(queryString);
				errorSyn = extractPatternSyns(pattern);
				if (errorSyn) {
					QP::queriesForQE.push_back(suchthat);
					QP::queriesForQE.push_back(pattern);
				}
			}
		}
		else {
			cout << ">>>>ERROR: QUERY SYN NOT DECLARED" << endl;
		}
	}
	//d. pattern before such that
	else if (indexOfPattern < indexOfST) {
		querySyn = queryString.substr(6, indexOfPattern - 6);
		if (!QP::checkIfSynDontExist(querySyn)) {
			suchthat = QP::extractST(queryString);
			errorSyn = QP::extractSTSyn(suchthat);
			if (errorSyn) {
				pattern = extractPattern(queryString);
				errorSyn = extractPatternSyns(pattern);
				if (errorSyn) {
					QP::queriesForQE.push_back(suchthat);
					QP::queriesForQE.push_back(pattern);
				}
			}
		}
		else {
			cout << ">>>>ERROR: QUERY SYN NOT DECLARED" << endl;
		}
	}
}

void QP::queryPrinter() {
	cout << "Queries to be passed to QE: " << endl;
	copy(QP::queriesForQE.begin(), QP::queriesForQE.end(), ostream_iterator<string>(cout, " "));
	cout << endl;
	cout << "Queried synonym: " << endl;
	cout << QP::querySyn << endl;
	cout << "Synonyms used in query: " << endl;
	copy(QP::queryUsedSyns.begin(), QP::queryUsedSyns.end(), ostream_iterator<string>(cout, " "));
}

/* Work in progress
multimap<string,string> QueryParser::createQTable(vector<string>& str) {
string designEntity;
vector<string> tokens;
for (auto i = str.begin(); i != str.end(); i++) {
designEntity = QueryParser::ExtractFirstWord(*i);

}
}*/


//JEREMY'S FUNCTIONS:
void QP::ParseField(string field) {
	string split1[] = field.split("(");
	string command = split1[0];
	string split2[] = split1[1].split(",");
	string one = split2[0].erase(remove(split2[0].begin(), split2[0].end(), '\"'), split2[0].end());
	String two = split[1].erase(remove(split2[1].begin(), split2[1].end(), '\"'), split2[1].end());
	two = two.erase(remove(two.begin(), two.end(), ')'), two.end());
	bool isNum1 = QP::isInt(one);
	bool isNum2 = QP::isInt(two);
	if (isNum1) {
		int a = atoi(one.c_str());
	}
	if (isNum2) {
		int b = atoi(two.c_str());
	}

	if (command.compare("ModifiesS") == 0) {
		if (!isNum1 && !isNum2) {
			vector<int> ans1 = QE::ModifiesS(one, two);
		}
		else{
			vector<string> ans1 = QE::ModifiesS(one, b);
		}
	}
	else if (command.compare("UsesS") == 0) {
		if (isNum1 && !isNum2) {
			QE::UsesS(a, two);
		}
		else{
			QE::UsesS(one, b);
		}
	}
	else if (command.compare("Parent") == 0) {
		if (isNum1 && !isNum2) {
			QE::Parent(a, two);
		}
		else if (!isNum1 && isNum2) {
			QE::Parent(one, b);
		}
		else {
			QE::Parent(one, two);
		}
	}
	else if (command.compare("ParentT") == 0) {
		if (isNum1 && !isNum2) {
			QE::ParentT(a, two);
		}
		else if (!isNum1 && isNum2) {
			QE::ParentT(one, b);
		}
		else {
			QE::ParentT(one, two);
		}
	}
	else if (command.compare("Follows") == 0) {
		if (isNum1 && !isNum2) {
			QE::Follows(a, two);
		}
		else if (!isNum1 && isNum2) {
			QE::Follows(one, b);
		}
		else {
			QE::Follows(one, two);
		}
	}
	else if (command.compare("FollowsT") == 0) {
		if (isNum1 && !isNum2) {
			QE::FollowsT(a, two);
		}
		else if (!isNum1 && isNum2) {
			QE::FollowsT(one, b);
		}
		else {
			QE::FollowsT(one, two);
		}
	}
	else{
		QE::pattern(one, two);
	}
}

bool QP::isInt(string input) {
	bool isNumber = true;
	for (string::const_iterator k = input.begin(); k != input.end(); ++k) {
		isNumber && = isdigit(*k);
	}
	return isNumber;
}