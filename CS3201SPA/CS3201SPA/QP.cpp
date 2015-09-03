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

bool QP::isInt(String input) {
	bool isNumber = true;
	for (string::const_iterator k = input.begin(); k != input.end(); ++k) {
		isNumber && = isdigit(*k);
	}
	return isNumber;
}