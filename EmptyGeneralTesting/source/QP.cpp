#pragma once

#include "QP.h"
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <map>
#include <regex>
#include <string>

//General flow of events in parser:
//1. Receive query string
//2. Remove leading and trailing whitespaces from query string - DONE
//3. Splitting of string into 2 parts: Declarations string & Actual query string - DONE
//4. Syntatic Validation of:
//a. Declarations string - DONE
//b. Actual query string - DONE
//5. Semantic Validation of Actual query string (i.e. does the query make sense?)
//6. Storing of declared types into table (i.e. assign a, a1; variable v; => assign type: a, a1; variable type: v)
//7. Extracting exact queries from actual query string
//8. Formatting queries (remove any unneccessary whitespaces)
//9. Storing of formatted queries (should be storing each query as a string, so vector<string> queries of maximum size 2)

//Validation will be conducted upon receiving query string, taking place in two separate phases:
//1. Syntatic Validation (i.e. does query match the grammar rules?)
//2. Semantic Validation (i.e. are the synonyms repeated, etc.)

using namespace std;

regex queryString_rgx("^\\s*((stmt|assign|while|variable|constant|prog_line)\\s+[a-zA-Z][a-zA-Z0-9#]*\\s*(,\\s*[a-zA-Z][a-zA-Z0-9#]*\\s*)*;\\s*)+\\sSelect\\s+[a-zA-Z][a-zA-Z0-9#]*(\\s+(((such that)\\s+(((Parent|Parent[*]|Follows|Follows[*])\\s*[(]\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*,\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*[)]\\s*)|((Modifies|Uses)\\s*[(]\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*,\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|\"[a-zA-Z][a-zA-Z0-9#]*\")\\s*[)]\\s*)))|((pattern)\\s+(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*[(]\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|(\"[a-zA-Z][a-zA-Z0-9#]*\"))\\s*,\\s*((_\\s*\"(([a-zA-Z][a-zA-Z0-9#]*)|([0-9]+))\"\\s*_)|_)\\s*[)]))){1,2}\\s*$");
regex declaration_rgx("^((stmt|assign|while|variable|constant|prog_line)\\s+[a-zA-Z][a-zA-Z0-9#]*\\s*(,\\s*[a-zA-Z][a-zA-Z0-9#]*\\s*)*;\\s*)+$");
regex query_rgx("^Select\\s+[a-zA-Z][a-zA-Z0-9#]*(\\s+(((such that)\\s+(((Parent|Parent[*]|Follows|Follows[*])\\s*[(]\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*,\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*[)]\\s*)|((Modifies|Uses)\\s*[(]\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*,\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|\"[a-zA-Z][a-zA-Z0-9#]*\")\\s*[)]\\s*)))|((pattern)\\s+(([a-zA-Z][a-zA-Z0-9#]*)|_|[0-9]+)\\s*[(]\\s*(([a-zA-Z][a-zA-Z0-9#]*)|_|(\"[a-zA-Z][a-zA-Z0-9#]*\"))\\s*,\\s*((_\\s*\"(([a-zA-Z][a-zA-Z0-9#]*)|([0-9]+))\"\\s*_)|_)\\s*[)]))){1,2}$");
regex synonym_rgx("^[a-zA-Z][a-zA-Z0-9#]*$");
regex integer_rgx("^[0-9]+$");
regex string_rgx("^\"[a-zA-Z][a-zA-Z0-9#]*\"$");
regex factor_rgx("^[_]\"[a-zA-Z][a-zA-Z0-9]*\"[_]$");
regex factorInt_rgx("^[_]\"[0-9]+\"[_]$");

QP::QP(string fileName) {
	inputFileName = fileName;
}

QP::QP() {
}

void QP::process() {
	ifstream qfile;
	qfile.open(this->inputFileName);
	string currentLine, declarations, queryString;
	vector<string> separatedLine;
	while (getline(qfile, currentLine)) {
		if (isQueryLegit(currentLine)) {
			separatedLine = separateDQ(trim(currentLine, " "));
			if (separatedLine.empty()) {
				Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
				queriesForQE.push_back(empty);
			}
			else {
				declarations = trim(separatedLine.at(0), " ");
				queryString = trim(separatedLine.at(1), " ");
				if (validNoSTPattern(queryString)) { //checks if number of ST and Pattern -> 1 each at most
					if (regex_match(declarations, declaration_rgx) && regex_match(queryString, query_rgx)) {
						valid = processingDeclarations(declarations); //HANDLING DECLARATIONS
						if (!valid) {
							Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
							queriesForQE.push_back(empty);
						}
						queryStringHandler(queryString); //HANDLING QUERY STRING
					}
					else {
						Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
						queriesForQE.push_back(empty);
					}
				}
				else {
					Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
					queriesForQE.push_back(empty);
				}
			}
			clearMemory(); //CLEAR MEMORY
		}
		else {
			Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
			queriesForQE.push_back(empty);
		}
	}
}

void QP::processSingleQuery(string query) {
	string declarations, queryString;
	vector<string> separatedLine;
	queryStr = query;
	if (isQueryLegit(queryStr)) {
		separatedLine = separateDQ(trim(queryStr, " "));
		if (separatedLine.empty()) {
			queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
		}
		else {
			declarations = trim(separatedLine.at(0), " ");
			queryString = trim(separatedLine.at(1), " ");
			if (validNoSTPattern(queryString)) { //checks if number of ST and Pattern -> 1 each at most
				if (regex_match(declarations, declaration_rgx) && regex_match(queryString, query_rgx)) {
					valid = processingDeclarations(declarations); //HANDLING DECLARATIONS
					if (!valid) {
						queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
					}
					singleQueryStringHandler(queryString); //HANDLING QUERY STRING
				}
				else {
					queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
				}
			}
			else {
				queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
			}
		}
		clearMemory(); //CLEAR MEMORY
	}
	else {
		queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
	}
}

bool QP::isQueryLegit(string rawQueryString) { //checks if the query string follows format: <declarationsstring><SINGLE SPACE><querystring>
	if (!regex_match(rawQueryString, queryString_rgx)) {
		return false;
	}
	else {
		return true;
	}
}

bool QP::validNoSTPattern(string& str) { //checks if there are multiple STs || patterns
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

vector<string> QP::separateDQ(string& str) { //separates declaration string from query string
	vector<string> processedStr;
	string declarations, queryClause, declarationsS, queryClauseS;
	int lastSemi = str.find_last_of(";");
	int selectIndex = str.find("Select");
	if (lastSemi == string::npos || selectIndex == string::npos) {
		//cout << "Invalid query!" << endl;
		return processedStr;
	}
	declarations = str.substr(0, lastSemi + 1);
	declarationsS = str.substr(0, selectIndex - 1);
	queryClause = str.substr(lastSemi + 2);
	queryClauseS = str.substr(selectIndex);
	if (declarations != declarationsS || queryClause != queryClauseS) {
		cout << "Invalid syntax for query" << endl;
	}
	else {
		processedStr.push_back(declarations);
		processedStr.push_back(queryClause);
	}
	return processedStr;
}

bool QP::processingDeclarations(string declarations) { //splits declaration string into sub-declarations of the various legal types
	string type;
	string currentToken;
	string decSynonyms;
	vector<string> declarationTokens = tokenize(declarations, ";");
	vector<string> tokenDecSynonyms;
	bool illegalFlag;
	int indexFirstWS;
	for (auto currentToken : declarationTokens) {
		indexFirstWS = currentToken.find(" ");
		type = currentToken.substr(0, indexFirstWS);
		decSynonyms = currentToken.substr(indexFirstWS);
		decSynonyms.erase(remove_if(decSynonyms.begin(), decSynonyms.end(), isspace), decSynonyms.end());
		tokenDecSynonyms = tokenize(decSynonyms, ",");
		illegalFlag = addSynonymsToCorrectType(type, tokenDecSynonyms);
		if (!illegalFlag) {
			return false;
		}
	}
	return true;
}

bool QP::addSynonymsToCorrectType(string type, vector<string> synonyms) { //if synonyms already exists, can throw out error and return none (implement later)
	if (type == "stmt") {
		for (auto i : synonyms) {
			if (checkIfSynDontExist(i)) {
				this->stmtD.push_back(i);
			}
			else {
				return false;
				break;
			}
		}
	}
	if (type == "assign") {
		for (auto i : synonyms) {
			if (checkIfSynDontExist(i)) {
				this->assignD.push_back(i);
			}
			else {
				return false;
				break;
			}
		}
	}
	if (type == "while") {
		for (auto i : synonyms) {
			if (checkIfSynDontExist(i)) {
				this->whileD.push_back(i);
			}
			else {
				return false;
				break;
			}
		}
	}
	if (type == "variable") {
		for (auto i : synonyms) {
			if (checkIfSynDontExist(i)) {
				this->variableD.push_back(i);
			}
			else {
				return false;
				break;
			}
		}
	}
	if (type == "constant") {
		for (auto i : synonyms) {
			if (checkIfSynDontExist(i)) {
				this->constantD.push_back(i);
			}
			else {
				return false;
				break;
			}
		}
	}
	if (type == "prog_line") {
		for (auto i : synonyms) {
			if (checkIfSynDontExist(i)) {
				this->prog_lineD.push_back(i);
			}
			else {
				return false;
				break;
			}
		}
	}
	return true;
}

bool QP::checkIfSynDontExist(string syn) { //checks through the declared design entity vectors if syn DOES NOT exist 
	if ((std::find(this->stmtD.begin(), this->stmtD.end(), syn) == this->stmtD.end()) &&
		(std::find(this->assignD.begin(), this->assignD.end(), syn) == this->assignD.end()) &&
		(std::find(this->whileD.begin(), this->whileD.end(), syn) == this->whileD.end()) &&
		(std::find(this->variableD.begin(), this->variableD.end(), syn) == this->variableD.end()) &&
		(std::find(this->constantD.begin(), this->constantD.end(), syn) == this->constantD.end()) &&
		(std::find(this->prog_lineD.begin(), this->prog_lineD.end(), syn) == this->prog_lineD.end())) {
		return true;
	}
	else {
		return false;
	}
}

void QP::clearMemory() { //erases all stored values in tables
	assignD.clear();
	whileD.clear();
	stmtD.clear();
	variableD.clear();
	constantD.clear();
	prog_lineD.clear();
	queryUsedSyns.clear();
	querySyn = "";
}

void QP::declarationPrinter() { //Printer method for the design entity vectors (for debugging)
	cout << "STMT synonyms: " << endl;
	copy(stmtD.begin(), stmtD.end(), ostream_iterator<string>(cout, " "));
	cout << endl;
	cout << "ASSIGN synonyms: " << endl;
	copy(assignD.begin(), assignD.end(), ostream_iterator<string>(cout, " "));
	cout << endl;
	cout << "WHILE synonyms: " << endl;
	copy(whileD.begin(), whileD.end(), ostream_iterator<string>(cout, " "));
	cout << endl;
	cout << "VARIABLE synonyms: " << endl;
	copy(variableD.begin(), variableD.end(), ostream_iterator<string>(cout, " "));
	cout << endl;
	cout << "CONSTANT synonyms: " << endl;
	copy(constantD.begin(), constantD.end(), ostream_iterator<string>(cout, " "));
	cout << endl;
	cout << "PROG_LINE synonyms: " << endl;
	copy(prog_lineD.begin(), prog_lineD.end(), ostream_iterator<string>(cout, " "));
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
		if (!checkIfSynDontExist(first) && !checkIfSynDontExist(second)) {
			queryUsedSyns.push_back(first);
			if (first != second) {
				queryUsedSyns.push_back(second);
			}
			return true;
		}
		else {
			//cout << "One or more of the synonyms used in query was not declared!" << endl;
			return false;
		}
	}
	else if (regex_match(first, synonym_rgx)) {
		if (!checkIfSynDontExist(first)) {
			queryUsedSyns.push_back(first);
			return true;
		}
		else {
			//cout << "One or more of the synonyms used in query was not declared!" << endl;
			return false;
		}
	}
	else if (regex_match(second, synonym_rgx)) {
		if (!checkIfSynDontExist(second)) {
			queryUsedSyns.push_back(second);
			return true;
		}
		else {
			//cout << "One or more of the synonyms used in query was not declared!" << endl;
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
		if (!checkIfSynDontExist(first) && !checkIfSynDontExist(second)) {
			if ((std::find(queryUsedSyns.begin(), queryUsedSyns.end(), first) != queryUsedSyns.end())) {
				count += 1;
			}
			queryUsedSyns.push_back(first);
			if ((std::find(queryUsedSyns.begin(), queryUsedSyns.end(), second) != queryUsedSyns.end()) && count == 1) {
				//cout << "Repeated Synonym detected in query!" << endl;
				return false;
			}
			else {
				queryUsedSyns.push_back(second);
				return true;
			}
		}
		else {
			//cout << "One or more of the synonyms used in query was not declared!" << endl;
			return false;
		}
	}
	else if (regex_match(first, synonym_rgx)) {
		if (!checkIfSynDontExist(first)) {
			queryUsedSyns.push_back(first);
			return true;
		}
		else {
			//cout << "One or more of the synonyms used in query was not declared!" << endl;
			return false;
		}
	}
	else if (regex_match(second, synonym_rgx)) {
		if (!checkIfSynDontExist(second)) {
			queryUsedSyns.push_back(second);
			return true;
		}
		else {
			//cout << "One or more of the synonyms used in query was not declared!" << endl;
			return false;
		}
	}
}

vector<Query> QP::getVectorQuery() {
	return queriesForQE;
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
	vector<string> query;
	vector<string> suchthatVS;
	vector<string> patternVS;
	bool errorSyn;
	string pattern;
	string suchthat;
	//4 cases here
	//a. only have pattern
	if (indexOfST == string::npos) {
		querySyn = queryString.substr(6, indexOfPattern - 6);
		if (!checkIfSynDontExist(querySyn)) {
			pattern = extractPattern(queryString);
			errorSyn = extractPatternSyns(pattern);
			if (errorSyn) {
				query = formattedSTQE(pattern, querySyn, "pattern");
				Query currentQuery(stmtD, assignD, variableD, constantD, whileD, prog_lineD, query);
				if (!checkValidQuery(currentQuery)) {
					Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
					queriesForQE.push_back(empty);
				}
				else {
					queriesForQE.push_back(currentQuery);
				}
			}
			else {
				Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
				queriesForQE.push_back(empty);
			}
		}
		else {
			Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
			queriesForQE.push_back(empty);
			//cout << ">>>>ERROR: QUERY SYN NOT DECLARED" << endl;
		}
	}
	//b. only have such that
	else if (indexOfPattern == string::npos) {
		querySyn = queryString.substr(6, indexOfST - 6);
		if (!checkIfSynDontExist(querySyn)) {
			suchthat = extractST(queryString);
			errorSyn = extractSTSyn(suchthat);
			if (errorSyn) {
				query = formattedSTQE(suchthat, querySyn, "suchthat");
				Query currentQuery(stmtD, assignD, variableD, constantD, whileD, prog_lineD, query);
				if (!checkValidQuery(currentQuery)) {
					Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
					queriesForQE.push_back(empty);
				}
				else {
					queriesForQE.push_back(currentQuery);
				}
			}
			else {
				Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
				queriesForQE.push_back(empty);
			}
		}
		else {
			Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
			queriesForQE.push_back(empty);
			//cout << ">>>>ERROR: QUERY SYN NOT DECLARED" << endl;
		}
	}
	//c. such that before pattern
	else if (indexOfST < indexOfPattern) {
		querySyn = queryString.substr(6, indexOfST - 6);
		if (!checkIfSynDontExist(querySyn)) {
			suchthat = extractST(queryString);
			errorSyn = extractSTSyn(suchthat);
			if (errorSyn) {
				pattern = extractPattern(queryString);
				errorSyn = extractPatternSyns(pattern);
				if (errorSyn) {
					suchthatVS = formattedSTQE(suchthat, querySyn, "suchthat");
					Query st(stmtD, assignD, variableD, constantD, whileD, prog_lineD, suchthatVS);
					patternVS = formattedSTQE(pattern, querySyn, "pattern");
					Query p(stmtD, assignD, variableD, constantD, whileD, prog_lineD, patternVS);
					if (!checkValidQuery(st) || !checkValidQuery(p)) {
						Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
						queriesForQE.push_back(empty);
					}
					else {
						query = combineVector(suchthatVS, patternVS);
						Query currentQuery(stmtD, assignD, variableD, constantD, whileD, prog_lineD, query);
						queriesForQE.push_back(currentQuery);
					}
				}
				else {
					Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
					queriesForQE.push_back(empty);
				}
			}
			else {
				Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
				queriesForQE.push_back(empty);
			}
		}
		else {
			Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
			queriesForQE.push_back(empty);
			//cout << ">>>>ERROR: QUERY SYN NOT DECLARED" << endl;
		}
	}
	//d. pattern before such that
	else if (indexOfPattern < indexOfST) {
		querySyn = queryString.substr(6, indexOfPattern - 6);
		if (!checkIfSynDontExist(querySyn)) {
			suchthat = extractST(queryString);
			errorSyn = extractSTSyn(suchthat);
			if (errorSyn) {
				pattern = extractPattern(queryString);
				errorSyn = extractPatternSyns(pattern);
				if (errorSyn) {
					suchthatVS = formattedSTQE(suchthat, querySyn, "suchthat");
					Query st(stmtD, assignD, variableD, constantD, whileD, prog_lineD, suchthatVS);
					patternVS = formattedSTQE(pattern, querySyn, "pattern");
					Query p(stmtD, assignD, variableD, constantD, whileD, prog_lineD, patternVS);
					if (!checkValidQuery(st) || !checkValidQuery(p)) {
						Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
						queriesForQE.push_back(empty);
					}
					query = combineVector(suchthatVS, patternVS);
					Query currentQuery(stmtD, assignD, variableD, constantD, whileD, prog_lineD, query);
					queriesForQE.push_back(currentQuery);
				}
				else {
					Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
					queriesForQE.push_back(empty);
				}
			}
			else {
				Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
				queriesForQE.push_back(empty);
			}
		}
		else {
			Query empty(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
			queriesForQE.push_back(empty);
		}
	}
}

void QP::singleQueryStringHandler(string queryString) {
	queryString.erase(remove_if(queryString.begin(), queryString.end(), isspace), queryString.end());
	int indexOfST = queryString.find("such");
	int indexOfPattern = queryString.find("pattern");
	vector<string> query;
	vector<string> suchthatVS;
	vector<string> patternVS;
	bool errorSyn;
	string pattern;
	string suchthat;
	//4 cases here
	//a. only have pattern
	if (indexOfST == string::npos) {
		querySyn = queryString.substr(6, indexOfPattern - 6);
		if (!checkIfSynDontExist(querySyn)) {
			pattern = extractPattern(queryString);
			errorSyn = extractPatternSyns(pattern);
			if (errorSyn) {
				query = formattedSTQE(pattern, querySyn, "pattern");
				Query currentQuery(stmtD, assignD, variableD, constantD, whileD, prog_lineD, query);
				if (!checkValidQuery(currentQuery)) {
					queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
				}
				else {
					queryForQE = currentQuery;
				}
			}
			else {
				queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
			}
		}
		else {
			queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
			//cout << ">>>>ERROR: QUERY SYN NOT DECLARED" << endl;
		}
	}
	//b. only have such that
	else if (indexOfPattern == string::npos) {
		querySyn = queryString.substr(6, indexOfST - 6);
		if (!checkIfSynDontExist(querySyn)) {
			suchthat = extractST(queryString);
			errorSyn = extractSTSyn(suchthat);
			if (errorSyn) {
				query = formattedSTQE(suchthat, querySyn, "suchthat");
				Query currentQuery(stmtD, assignD, variableD, constantD, whileD, prog_lineD, query);
				if (!checkValidQuery(currentQuery)) {
					queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
				}
				else {
					queryForQE = currentQuery;
				}
			}
			else {
				queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
			}
		}
		else {
			queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
			//cout << ">>>>ERROR: QUERY SYN NOT DECLARED" << endl;
		}
	}
	//c. such that before pattern
	else if (indexOfST < indexOfPattern) {
		querySyn = queryString.substr(6, indexOfST - 6);
		if (!checkIfSynDontExist(querySyn)) {
			suchthat = extractST(queryString);
			errorSyn = extractSTSyn(suchthat);
			if (errorSyn) {
				pattern = extractPattern(queryString);
				errorSyn = extractPatternSyns(pattern);
				if (errorSyn) {
					suchthatVS = formattedSTQE(suchthat, querySyn, "suchthat");
					Query st(stmtD, assignD, variableD, constantD, whileD, prog_lineD, suchthatVS);
					patternVS = formattedSTQE(pattern, querySyn, "pattern");
					Query p(stmtD, assignD, variableD, constantD, whileD, prog_lineD, patternVS);
					if (!checkValidQuery(st) || !checkValidQuery(p)) {
						queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
					}
					else {
						query = combineVector(suchthatVS, patternVS);
						Query currentQuery(stmtD, assignD, variableD, constantD, whileD, prog_lineD, query);
						queryForQE = currentQuery;
					}
				}
				else {
					queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
				}
			}
			else {
				queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
			}
		}
		else {
			queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
			//cout << ">>>>ERROR: QUERY SYN NOT DECLARED" << endl;
		}
	}
	//d. pattern before such that
	else if (indexOfPattern < indexOfST) {
		querySyn = queryString.substr(6, indexOfPattern - 6);
		if (!checkIfSynDontExist(querySyn)) {
			suchthat = extractST(queryString);
			errorSyn = extractSTSyn(suchthat);
			if (errorSyn) {
				pattern = extractPattern(queryString);
				errorSyn = extractPatternSyns(pattern);
				if (errorSyn) {
					suchthatVS = formattedSTQE(suchthat, querySyn, "suchthat");
					Query st(stmtD, assignD, variableD, constantD, whileD, prog_lineD, suchthatVS);
					patternVS = formattedSTQE(pattern, querySyn, "pattern");
					Query p(stmtD, assignD, variableD, constantD, whileD, prog_lineD, patternVS);
					if (!checkValidQuery(st) || !checkValidQuery(p)) {
						queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
					}
					query = combineVector(suchthatVS, patternVS);
					Query currentQuery(stmtD, assignD, variableD, constantD, whileD, prog_lineD, query);
					queryForQE = currentQuery;
				}
				else {
					queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
				}
			}
			else {
				queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
			}
		}
		else {
			queryForQE = Query(stmtD, assignD, variableD, constantD, whileD, prog_lineD, vector<string>());
		}
	}
}

vector<string> QP::combineVector(vector<string> first, vector<string> second) {
	vector<string> temp;
	for (auto i : first) {
		temp.push_back(i);
	}
	for (auto i : second) {
		temp.push_back(i);
	}
	return temp;
}

vector<string> QP::formattedSTQE(string st, string qSyn, string typeOfQuery) { //formatting ST query && pattern specifically for QE
	string rel;
	string first;
	string second;
	vector<string> query;
	int indexOfFirstB = st.find("(");
	int indexOfSep = st.find(",");
	int indexOfSecondB = st.find(")");
	if (typeOfQuery == "suchthat") {
		rel = st.substr(0, indexOfFirstB);
	}
	else {
		rel = st.substr(0, indexOfFirstB);
	}
	first = st.substr(indexOfFirstB + 1, indexOfSep - indexOfFirstB - 1);
	second = st.substr(indexOfSep + 1, indexOfSecondB - indexOfSep - 1);
	query.push_back(qSyn);
	query.push_back(rel);
	query.push_back(first);
	query.push_back(second);
	return query;
}

vector<string> QP::tokenize(const string& str, const string& delimiters) { //general tokenizer function with customized delimiters allowed
	vector<string> tokens;
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	string::size_type pos = str.find_first_of(delimiters, lastPos);
	while (string::npos != pos || string::npos != lastPos) {
		tokens.push_back(trim(str.substr(lastPos, pos - lastPos), " "));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastPos);
	}
	return tokens;
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

string QP::checkSynType(string syn) {
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

bool QP::checkValidQuery(Query query) {
	vector<string> queryFields = query.getQueryFields();
	string qSyn = queryFields.at(0);
	string rel = queryFields.at(1);
	string left = queryFields.at(2);
	string right = queryFields.at(3);
	if (rel.compare("Modifies") == 0) {
		if (regex_match(left, synonym_rgx)) {
			if (checkSynType(left).compare("VARIABLE") == 0 || checkSynType(left).compare("CONSTANT") == 0) {
				return false;
			}
			else { //LHS OF MODIFIES OKAY!
				if (regex_match(right, synonym_rgx)) {
					if (checkSynType(right).compare("ASSIGN") == 0 || checkSynType(right).compare("STMT") == 0 || checkSynType(right).compare("PROG_LINE") == 0 || checkSynType(right).compare("CONSTANT") == 0 || checkSynType(right).compare("WHILE") == 0) {
						return false;
					}
				}
				else if (regex_match(right, integer_rgx)) {
					return false;
				}
			}
			return true;
		}
		else if (regex_match(left, string_rgx)) { // specified variable i.e. "x"
			return false;
		}
		else { //LHS OF MODIFIES OKAY!
			if (regex_match(right, synonym_rgx)) {
				if (checkSynType(right).compare("ASSIGN") == 0 || checkSynType(right).compare("STMT") == 0 || checkSynType(right).compare("PROG_LINE") == 0 || checkSynType(right).compare("CONSTANT") == 0 || checkSynType(right).compare("WHILE") == 0) {
					return false;
				}
			}
			else if (regex_match(right, integer_rgx)) {
				return false;
			}
			return true;
		}
	}
	else if (rel.compare("Uses") == 0) {
		if (regex_match(left, synonym_rgx)) {
			if (checkSynType(left).compare("VARIABLE") == 0 || checkSynType(left).compare("CONSTANT") == 0) {
				return false;
			}
		}
		else if (regex_match(left, string_rgx)) { // specified variable i.e. "x"
			return false;
		}
		else { //LHS of USES OKAY!
			if (regex_match(right, synonym_rgx)) { //RHS IS A SYN
				if (checkSynType(right).compare("ASSIGN") == 0 || checkSynType(right).compare("STATEMENT") == 0 || checkSynType(right).compare("WHILE") == 0 || checkSynType(right).compare("PROG_LINE") == 0) {
					return false;
				}
			}
		}
		return true;
	}
	else if (rel.compare("Follows") == 0 || rel.compare("Follows*") == 0) {
		if (regex_match(left, synonym_rgx)) {
			if (checkSynType(left).compare("VARIABLE") == 0 || checkSynType(left).compare("CONSTANT") == 0) {
				return false;
			}
			else { //LHS of Follows/Follows* OKAY
				if (regex_match(right, synonym_rgx)) {
					if (checkSynType(right).compare("VARIABLE") == 0 || checkSynType(right).compare("CONSTANT") == 0) {
						return false;
					}
				}
				else if (regex_match(right, string_rgx)) {
					return false;
				}
			}
			return true;
		}
		else if (regex_match(left, string_rgx)) {
			return false;
		}
		else { //LHS of Follows/Follows* OKAY
			if (regex_match(right, synonym_rgx)) {
				if (checkSynType(right).compare("VARIABLE") == 0 || checkSynType(right).compare("CONSTANT") == 0) {
					return false;
				}
			}
			else if (regex_match(right, string_rgx)) {
				return false;
			}
			return true;
		}
	}
	else if (rel.compare("Parent") == 0 || rel.compare("Parent*") == 0) {
		if (regex_match(left, synonym_rgx)) {
			if (checkSynType(left).compare("ASSIGN") == 0 || checkSynType(left).compare("VARIABLE") == 0 || checkSynType(left).compare("CONSTANT") == 0) {
				return false;
			}
			else {
				if (regex_match(right, synonym_rgx)) {
					if (checkSynType(right).compare("VARIABLE") == 0 || checkSynType(right).compare("CONSTANT") == 0) {
						return false;
					}
				}
				else if (regex_match(right, string_rgx)) {
					return false;
				}
			}
			return true;
		}
		else if (regex_match(left, string_rgx)) {
			return false;
		}
		else {
			if (regex_match(right, synonym_rgx)) {
				if (checkSynType(right).compare("VARIABLE") == 0 || checkSynType(right).compare("CONSTANT") == 0) {
					return false;
				}
			}
			else if (regex_match(right, string_rgx)) {
				return false;
			}
			return true;
		}
	}
	else { //patterns
		if (regex_match(left, synonym_rgx)) {
			if (checkSynType(left).compare("ASSIGN") == 0 || checkSynType(left).compare("STATEMENT") == 0 || checkSynType(left).compare("CONSTANT") == 0 || checkSynType(left).compare("PROG_LINE") == 0 || checkSynType(left).compare("WHILE") == 0) {
				return false;
			}
			else {
				if (!regex_match(right, factor_rgx) && right.compare("_") != 0 && !regex_match(right, factorInt_rgx)) {
					return false;
				}
				return true;
			}
		}
		else if (!regex_match(left, string_rgx) && left.compare("_") != 0) {
			return false;
		}
		else {
			if (!regex_match(right, factor_rgx) && right.compare("_") != 0 && !regex_match(right, factorInt_rgx)) {
				return false;
			}
			return true;
		}
	}
}

Query QP::getQuery() {
	return queryForQE;
}