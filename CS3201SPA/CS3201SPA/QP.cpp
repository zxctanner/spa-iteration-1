#pragma once

#include "QP.h"
#include "QE.h"
#include <iostream>
#include <fstream>
#include <sstream>
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


QP::QP(string fileName, PKB* pkb) {
	inputFileName = fileName;
	includePKBInstance = pkb;
};

void QP::startQP() {
	ifstream qfile;
	qfile.open(this->inputFileName);
	string currentLine, declarations, queryString;
	vector<string> separatedLine;
	while (getline(qfile, currentLine)) {
		this->ansF = "none";
		if (isQueryLegit(currentLine)) {
			separatedLine = separateDQ(trim(currentLine, " "));
			declarations = trim(separatedLine.at(0), " ");
			queryString = trim(separatedLine.at(1), " ");
			if (validNoSTPattern(queryString)) { //checks if number of ST and Pattern -> 1 each at most
				if (regex_match(declarations, declaration_rgx) && regex_match(queryString, query_rgx)) {
					//HANDLING DECLARATIONS
					valid = processingDeclarations(declarations);
					if (!valid) {
						this->ansF = "none";
						break;
					}
					//HANDLING QUERY STRING
					valid = queryStringHandler(queryString);
					if (valid) {
						this->ansF = "none";
						break;
					}
					//GOING INTO QE
					passIntoQE();
					//CLEAR EVERYTHING
					clearMemory();
				}
				else {
					this->ansF = "none";
				}
			}
			else {
				this->ansF = "none";
			}
		}
		else {
			this->ansF = "none";
		}
		cout << this->ansF << endl;
	}
}

bool QP::isQueryLegit(string rawQueryString) { //checks if the query string follows format: <declarationsstring><SINGLE SPACE><querystring>
	if (rawQueryString.find("Select") == string::npos || rawQueryString.find(";") == string::npos) {
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
		cout << "Invalid query!" << endl;
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
	this->assignD.clear();
	this->whileD.clear();
	this->stmtD.clear();
	this->variableD.clear();
	this->constantD.clear();
	this->prog_lineD.clear();
	this->queryUsedSyns.clear();
	this->queriesForQE.clear();
	this->ansST.clear();
	this->ansP.clear();
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
			queryUsedSyns.push_back(second);
			return true;
		}
		else {
			cout << "One or more of the synonyms used in query was not declared!" << endl;
			return false;
		}
	}
	else if (regex_match(first, synonym_rgx)) {
		if (!checkIfSynDontExist(first)) {
			queryUsedSyns.push_back(first);
			return true;
		}
		else {
			cout << "One or more of the synonyms used in query was not declared!" << endl;
			return false;
		}
	}
	else if (regex_match(second, synonym_rgx)) {
		if (!checkIfSynDontExist(second)) {
			queryUsedSyns.push_back(second);
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
		if (!checkIfSynDontExist(first) && !checkIfSynDontExist(second)) {
			if ((std::find(queryUsedSyns.begin(), queryUsedSyns.end(), first) != queryUsedSyns.end())) {
				count += 1;
			}
			queryUsedSyns.push_back(first);
			if ((std::find(queryUsedSyns.begin(), queryUsedSyns.end(), second) != queryUsedSyns.end()) && count == 1) {
				cout << "Repeated Synonym detected in query!" << endl;
				return false;
			}
			else {
				queryUsedSyns.push_back(second);
				return true;
			}
		}
		else {
			cout << "One or more of the synonyms used in query was not declared!" << endl;
			return false;
		}
	}
	else if (regex_match(first, synonym_rgx)) {
		if (!checkIfSynDontExist(first)) {
			queryUsedSyns.push_back(first);
			return true;
		}
		else {
			cout << "One or more of the synonyms used in query was not declared!" << endl;
			return false;
		}
	}
	else if (regex_match(second, synonym_rgx)) {
		if (!checkIfSynDontExist(second)) {
			queryUsedSyns.push_back(second);
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

bool QP::queryStringHandler(string queryString) {
	queryString.erase(remove_if(queryString.begin(), queryString.end(), isspace), queryString.end());
	int indexOfST = queryString.find("such");
	int indexOfPattern = queryString.find("pattern");
	vector<string> query;
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
				this->queriesForQE.push_back(query);
				return true;
			}
		}
		else {
			return false;
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
				this->queriesForQE.push_back(query);
				return true;
			}
		}
		else {
			return false;
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
					this->queriesForQE.push_back(formattedSTQE(suchthat, querySyn, "suchthat"));
					this->queriesForQE.push_back(formattedSTQE(pattern, querySyn, "pattern"));
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
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
					this->queriesForQE.push_back(formattedSTQE(suchthat, querySyn, "suchthat"));
					this->queriesForQE.push_back(formattedSTQE(pattern, querySyn, "pattern"));
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
}

string QP::checkSynType(string syn) { //syn is assumed to be valid (i.e. already declared)
	string type;
	if (std::find(this->stmtD.begin(), this->stmtD.end(), syn) != this->stmtD.end()) {
		type = "stmt";
	}
	if (std::find(this->assignD.begin(), this->assignD.end(), syn) != this->assignD.end()) {
		type = "assign";
	}
	if (std::find(this->whileD.begin(), this->whileD.end(), syn) != this->whileD.end()) {
		type = "while";
	}
	if (std::find(this->variableD.begin(), this->variableD.end(), syn) != this->variableD.end()) {
		type = "variable";
	}
	if (std::find(this->prog_lineD.begin(), this->prog_lineD.end(), syn) != this->prog_lineD.end()) {
		type = "prog_line";
	}
	if (std::find(this->constantD.begin(), this->constantD.end(), syn) != this->constantD.end()) {
		type = "constant";
	}
	return type;
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
		rel = st.substr(0, 7);
	}
	first = st.substr(indexOfFirstB + 1, indexOfSep - indexOfFirstB - 1);
	second = st.substr(indexOfSep + 1, indexOfSecondB - indexOfSep - 1);
	query.push_back(qSyn);
	query.push_back(rel);
	query.push_back(first);
	query.push_back(second);
	return query;
}

void QP::queryPrinter() {
	cout << "Queries to be passed to QE: " << endl;
	for (auto i : queriesForQE) {
		copy(i.begin(), i.end(), ostream_iterator<string>(cout, " "));
		cout << endl;
	}
	cout << endl;
	cout << "Queried synonym: " << endl;
	cout << querySyn << endl;
	cout << "Synonyms used in query: " << endl;
	copy(queryUsedSyns.begin(), queryUsedSyns.end(), ostream_iterator<string>(cout, " "));
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

void QP::passIntoQE() {
	if ((this->queriesForQE.size() < 1) || (this->queriesForQE.size() > 2)) {
		this->ansF = "none";
	}
	else {
		if (this->queriesForQE.size() == 1) {
			selectField(this->queriesForQE.at(0));
			if (this->queriesForQE.at(0).at(1) == "pattern") {
				ansF = vectorSToString(this->ansP);
			}
			else {
				ansF = vectorSToString(this->ansST);
			}
		}
		else {
			selectField(this->queriesForQE.at(0));
			selectField(this->queriesForQE.at(1));
			ansF = vectorSToString(findCommonAnswer());
		}
	}
}

vector<string> QP::findCommonAnswer() {
	vector<string> answer;
	for (int a = 0; a < this->ansST.size(); a++) {
		if (find(this->ansP.begin(), this->ansP.end(), this->ansST.at(a)) != this->ansP.end()) {
			answer.push_back(this->ansST.at(a));
		}
	}
	return answer;
}

string QP::vectorSToString(vector<string> vecString) {
	string ans;
	stringstream ss;
	for (int i = 0; i < vecString.size(); ++i) {
		if (i != 0) {
			ss << ", ";
		}
		ss << vecString.at(i);
	}
	ans = ss.str();
}


//JEREMY'S FUNCTIONS:
void QP::selectField(vector<string> fields) {
	string select = fields[0];
	string command = fields[1];
	string one = fields[2];
	string two = fields[3];
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
			this->ansST = QE::ModifiesS(select, one, two);
		}
		else {
			this->ansST = QE::ModifiesS(select, one, b);
		}
	}
	else if (command.compare("UsesS") == 0) {
		if (isNum1 && !isNum2) {
			this->ansST = QE::UsesS(select, a, two);
		}
		else {
			this->ansST = QE::UsesS(select, one, b);
		}
	}
	else if (command.compare("Parent") == 0) {
		if (isNum1 && !isNum2) {
			this->ansST = QE::Parent(select, a, two);
		}
		else if (!isNum1 && isNum2) {
			this->ansST = QE::Parent(select, one, b);
		}
		else {
			this->ansST = QE::Parent(select, one, two);
		}
	}
	else if (command.compare("ParentT") == 0) {
		if (isNum1 && !isNum2) {
			this->ansST = QE::ParentT(select, a, two);
		}
		else if (!isNum1 && isNum2) {
			this->ansST = QE::ParentT(select, one, b);
		}
		else {
			this->ansST = QE::ParentT(select, one, two);
		}
	}
	else if (command.compare("Follows") == 0) {
		if (isNum1 && !isNum2) {
			this->ansST = QE::Follows(select, a, two);
		}
		else if (!isNum1 && isNum2) {
			this->ansST = QE::Follows(select, one, b);
		}
		else {
			this->ansST = QE::Follows(select, one, two);
		}
	}
	else if (command.compare("FollowsT") == 0) {
		if (isNum1 && !isNum2) {
			this->ansST = QE::FollowsT(select, a, two);
		}
		else if (!isNum1 && isNum2) {
			this->ansST = QE::FollowsT(select, one, b);
		}
		else {
			this->ansST = QE::FollowsT(select, one, two);
		}
	}
	else {
		this->ansP = QE::pattern(select, one, two);
	}
}

bool QP::isInt(string input) {
	bool isNumber = true;
	for (string::const_iterator k = input.begin(); k != input.end(); ++k) {
		isNumber && = isdigit(*k);
	}
	return isNumber;
}