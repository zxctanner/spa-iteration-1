#include "QE.h"
#include "PKB.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <stack>
#include <iostream>
using namespace std;

QE QE::QueryEvaluator(){
}

vector<string> QE::ModifiesS(string one, string two); { //returns statement number that modifies variable two
	unordered_map<int, pair<string, string>> modTable = PKB::getmodUseTable();
	vector<string> ans;
	ostringstream convert;
	if (two.compares("_") != 0) {
		for (auto i = modTable.begin(); i != modTable.end(); ++i) {
			if (modTable.second.first.compares(two) == 0) {           
				convert << modTable.first;      
				ans.push_back(convert.str());
			}
		}
	}
	else {
		for (auto i = modTable.begin(); i != modTable.end(); ++i) {
			convert << modTable.first;
			ans.push_back(convert.str());
	}
	return ans;
}
vector<string> QE::ModifiesS(int one, string two); { //returns variable that is modifies in statement line one
	unordered_map<int, pair<string, string>> modTable = PKB::getmodUseTable();
	vector<string> ans;
	ostringstream convert;
	convert << modTable[one].first;
	ans.push_back(convert.str());
	return ans;
}

vector<string> QE::UsesS(string one, string two); { //returns statment numbers that uses variable two
	unordered_map<int, pair<string, string>> useTable = PKB::getmodUseTable();
	vector<string> ans;
	ostringstream convert;
	if (two.compares("_") != 0) {
		for (auto i = useTable.begin(); i != useTable.end(); ++i) {
			string RHS = useTable.second.second;
			if (RHS.find(two) != string::npos) {
				convert << useTable.first;
				ans.push_back(convert.str());
			}
		}
	}
	else {
		for (auto i = modTable.begin(); i != modTable.end(); ++i) {
			convert << useTable.first;
			ans.push_back(convert.str());
		}
	}
	return ans;
}
vector<string> QE::UsesS(int one, string two); { //returns variables that are used in statement line one
	unordered_map<int, pair<string, string>> modTable = PKB::getmodUseTable();
	vector<string> ans;
	
	string RHS = modTable[one].second;
	string split[] = RHS.split("+");
	for (int i = 0; i < sizeof(RHS); ++i) {
		ans.push_back(split[i]);
	}
	return ans;
}

vector<string> QE::Parent(String one, string two); { //return all the while statement base on condition
	vector<pair<int, int>> parTable = PKB::getParentTable();
	vector<string> ans;
	return ans;
}
vector<string> QE::Parent(int one, string two); { //return all the child of statement line one
	vector<pair<int, int>> parTable = PKB::getParentTable();
	vector<string> ans;
	ostringstream convert;
	for (int i = 0; i < parTable.size(); ++i) {
		if (parTable[i].first == one) {
			convert << parTable[i].second;
			ans.push_back(convert.str());
		}
	}
	return ans;
}
vector<string> QE::Parent(string one, int two); { //return the parent statement line of the statement line two
	vector<pair<int, int>> parTable = PKB::getParentTable();
	vector<string> ans;
	ostringstream convert;
	for (int i = 0; i < parTable.size(); i++) {
		if (parTable[i].second == two) {
			convert << parTable[i].first;
			ans.push_back(convert.str());
			break;
		}
	}
	return ans;
}

vector<string> QE::ParentT(String one, string two); { //return all the while statment base on condition
	vector<pair<int, int>> parTable = PKB::getParentTable();
	vector<string> ans;
	return ans;
}
vector<string> QE::ParentT(int one, string two); { //return all the child* of statement line one
	vector<pair<int, int>> parTable = PKB::getParentTable();
	vector<string> ans;
	stack<int> par;
	ostringstream convert;
	int next = one;
	par.push(one);
	while (!par.empty()) {
		for (int i = 0; i < parTable.size(); ++i) {
			if (parTable[i].first == next) {
				par.push(parTable[i].second);
				convert << parTable[i].second;
				ans.push_back(convert.str());
			}
		}
		par.pop();
		if (par.empty()) {
			break;
		}
		else {
			next = par.top();
		}
	}
	return ans;
}
vector<string> QE::ParentT(String one, int two); { //return all the parent* of statement line two
	vector<pair<int, int>> parTable = PKB::getParentTable();
	vector<string> ans;
	stack<int> par;
	ostringstream convert;
	int previous = two;
	par.push(two);
	while (!par.empty()) {
		for (int i = 0; i < parTable.size(); ++i) {
			if (parTable[i].second == previous) {
				par.push(parTable[i].first);
				convert << parTable[i].first;
				ans.push_back(convert.str());
			}
		}
		par.pop();
		if (par.empty()) {
			break;
		}
		else {
			previous = par.top();
		}
	}
	return ans;
}

vector<string> QE::Follows(string one, string two); { //return statement line of follow base on condition
	vector<pair<int, int>> folTable = PKB::getFollowTable();
	vector<string> ans;
	return ans;
}
vector<string> QE::Follows(int one, string two); { //return the statement line that follows statement line one
	vector<pair<int, int>> folTable = PKB::getFollowTable();
	vector<string> ans;
	ostringstream convert;
	for (int i = 0; i < folTable.size(); ++i) {
		if (folTable[i].first == one) {
			convert << folTable[i].second;
			ans.push_back(convert.str());
			break;
		}
	}
	return ans;
}
vector<string> QE::Follows(string one, int two); { //return the statement line that is before statement line two
	vector<pair<int, int>> folTable = PKB::getFollowTable();
	ventor<string> ans;
	ostringstream convert;
	for (int i = 0; i < folTable.size(); i++) {
		if (folTable[i].second == two) {
			convert << folTable[i].first;
			ans.push_back(convert.str());
			break;
		}
	}
	return ans;
}

vector<string> QE::FollowsT(string one, string two); { //return all the statement lines base on condition
	vector<pair<int, int>> folTable = PKB::getFollowTable();
	vector<string> ans;
	return ans;
}
vector<string> QE::FollowsT(int one, string two); { //return all the statement lines that follows* statement line one
	vector<pair<int, int>> folTable = PKB::getFollowTable();
	vector<string> ans;
	ostringstream convert;
	int line = one;
	for (int i = 0; i < folTable.size(); ++i) {
		if (folTable[i].first == line) {
			convert << folTable[i].second;
			ans.push_back(convert.str());
			line = folTable[i].second;
		}
	}
	return ans;
}
vector<string> QE::FollowsT(string one, int two); { //return all the statements lines that come before statement line two and in the sames nesting level
	vector<pair<int, int>> folTable = PKB::getFollowTable();
	vector<string> ans;
	ostringstream convert;
	int line = two;
	for (int i = folTable.size() - 1; i >= 0; ++i) {
		if (folTable[i].second == line) {
			convert << folTable[i].first;
			ans.push_back(convert.str());
			line = folTable[i].first;
		}
	}
	return ans;
}

vector<string> QE::pattern(string one, string two); { //return the statement lines that has this pattern
	vector<string> ans;
	return ans;
}