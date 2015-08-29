#include "QueryParser.h"
#include "QueryEvaluator.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>

using namespace std;



QueryParser::QueryParser() {

}

QueryParser::ParseInput(string input){
	string[] parts = input.split(" ");
	int num = parts.length;
}

QueryParser::ParseField(string field) {
	String[] split1 = field.split("(");
	string command = split1[0];
	String[] split2 = split1[1].split(",");
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

/* Work in progress
multimap<string,string> QueryParser::createQTable(vector<string>& str) {
	string designEntity;
	vector<string> tokens;
	for (auto i = str.begin(); i != str.end(); i++) {
		designEntity = QueryParser::ExtractFirstWord(*i);
		
	}
}*/

string QueryParser::extractPattern(string& str) {
	string pattern = "pattern";
	size_t found = str.find(pattern);
	size_t end = str.find_first_of(")");
	if (found != string::npos && end != string::npos) {
		//cout << str.find_first_of(")");
		return str.substr(found + 8, end - (found + 7));
	}
	else {
		return "";
	}
}

string QueryParser::extractST(string& str) {
	string suchThat = "such that";
	size_t found = str.find(suchThat);
	size_t end = str.find_first_of(")");
	if (found != string::npos && end != string::npos) {
		//cout << str.find_first_of(")");
		return str.substr(found + 10, end - (found + 9));
	}
	else {
		return "";
	}
}

string QueryParser::trim(const string& str, const string& trimmers) {
	const auto strBegin = str.find_first_not_of(trimmers);
	if (strBegin == string::npos) {
		return "";
	}
	const auto strEnd = str.find_last_not_of(trimmers);
	const auto strRange = strEnd - strBegin + 1;
	return str.substr(strBegin, strRange);
}

vector<string> QueryParser::Tokenize(const string& str, const string& delimiters) {
	vector<string> tokens;
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	string::size_type pos = str.find_first_of(delimiters, lastPos);
	while (string::npos != pos || string::npos != lastPos) {
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastPos);
	}
	return tokens;
}

string QueryParser::extractFirstWord(string& str) {
	int firstWhiteSpace = str.find_first_of(" ");
	string word = "";
	if (firstWhiteSpace != string::npos) {
		word = str.substr(0, firstWhiteSpace);
	}
	return word;
}

vector<string> QueryParser::extractSynonyms(string& str) {
	vector<string> synonyms;
	string trimmed = trim(str, " ");
	int findBeginning = trimmed.find_first_of(" ");
	string essentials = removeCharsFromString(trimmed.substr(findBeginning), " ;");
	synonyms = Tokenize(essentials, ",");
	return synonyms;
}

string QueryParser::removeCharsFromString(string &str, char* charsToRemove) {
	for (unsigned int i = 0; i < strlen(charsToRemove); ++i) {
		str.erase(remove(str.begin(), str.end(), charsToRemove[i]), str.end());
	}
	return str;
}