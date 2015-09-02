#pragma once

#include "QP.h"
#include "QueryEvaluator.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <map>
#include <regex>
#include <string>

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
QP::ParseInput(string input) {
	string parts[] = input.split(" ");
	int num = parts.length;
}

QP::ParseField(string field) {
	string split1[] = field.split("(");
	string command = split1[0];
	string split2[] = split1[1].split(",");
	string first = split2[0].erase(remove_if(split2[0].begin(), split2[0].end(), isspace), split2[0].end());
	string second = split2[1].erase(second.end());
	second = second.erase(remove_if(second.begin(), second.end(), isspace), second.end());

	if (command.compare("Modifies") == 0) {
		call QueryEvaluator::Modifies(first, second);
	}
	else if (command.compare("Uses") == 0) {
		call QueryEvaluator::Uses(first, second);
	}
	else if (command.compare("Parent") == 0) {
		call QueryEvaluator::Parent(first, second);
	}
	else if (command.compare("Parent*") == 0) {
		call QueryEvaluator::ParentS(first, second);
	}
	else if (command.compare("Follows") == 0) {
		call QueryEvaluator::Follows(first, second);
	}
	else if (command.compare("FollowsS") == 0) {
		call QueryEvaluator::FollowsS(first, second);
	}
	else if (command.compare("a") == 0) {
		call QueryEvaluator::Pattern(first, second);
	}
	else {
		cout << ("Invalid input");
	}
}