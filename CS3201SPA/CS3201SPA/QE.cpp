#include "QE.h"
#include "PKB.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <stack>
#include <iostream>
#include <algorithm>

using namespace std;

/* Query Evaluator
1. Each method is called by the Query Parser depending on the command
2. The field obtained is the select string, field 1 and field 2
3. According to the command, the respective method is used
4. The method will access the PKB for the information
5. There will be a filter to access the type of the string to further evaluate the answers
6. The query answers will be placed in vector<string> ans1 for all commands and ans2 for pattern command
7. If there are only 1 command, the respective vector will be printed else AND the two vectors before printing*/

QE QE::QueryEvaluator(){
}

vector<string> QE::ModifiesS(string select, string one, string two); { //returns statement number that modifies variable two
	unordered_map<int, pair<vector<string>, vector<string>>> modTable = PKB::getmodUseTable();
	vector<string> ans;
	vector<string> check;
	ostringstream convert;
	if (two.compares("_") != 0) {
		for (auto i = modTable.begin(); i != modTable.end(); ++i) {
			check = modTable.second.first;
			for (int j = 0; j < check.size(); ++j) {
				if (check[j].compares(two) == 0) {
					convert << modTable.first;
					ans.push_back(convert.str());
				}
			}
		}
		ans = filter(ans, one, two);
	}
	else {
		for (auto i = modTable.begin(); i != modTable.end(); ++i) {
			convert << modTable.first;
			ans.push_back(convert.str());
		}
		ans = filter(ans, one, two);
	}

	return ans;
}
vector<string> QE::ModifiesS(string select, int one, string two); { //returns variable that is modifies in statement line one
	unordered_map<int, pair<vector<string>, vector<string>>> modTable = PKB::getmodUseTable();
	vector<string> ans;
	ostringstream convert;
	convert << modTable[one].first;
	ans.push_back(convert.str());
	return ans;
}

vector<string> QE::UsesS(string select, string one, string two); { //returns statment numbers that uses variable two
	unordered_map<int, pair<vector<string>, vector<string>>> useTable = PKB::getmodUseTable();
	vector<string> ans;
	vector<string> use;
	ostringstream convert;
	if (two.compares("_") != 0) {
		for (auto i = useTable.begin(); i != useTable.end(); ++i) {
			use = useTable.second.second;
			for (int j = 0; i < use.size(); ++j) {
				if (use[j].compares(two) == 0) {
					convert << useTable.first;
					ans.push_back(convert.str());
				}
			}
		}
		ans = filter(ans, one, two);
	}
	else {
		for (auto i = modTable.begin(); i != modTable.end(); ++i) {
			convert << useTable.first;
			ans.push_back(convert.str());
		}
		ans = filter(ans, one, two);
	}
	return ans;
}
vector<string> QE::UsesS(string select, int one, string two); { //returns variables that are used in statement line one
	unordered_map<int, pair<vector<string>, vector<string>>> useTable = PKB::getmodUseTable();
	vector<string> ans;
	
	vector<string> use = useTable[one].second;
	for (int i = 0; i < use.size(); ++i) {
		ans.push_back(use[i]);
	}
	return ans;
}

vector<string> QE::Parent(string select, string one, string two); { //return all the while statement base on condition
	vector<pair<int, int>> parTable = PKB::getParentTable();
	vector<string> sub;
	vector<string> ans;
	ostringstream convert;
	if (select.compares(one) == 0) {
		if (QP::checkSynType(one).compares("while") != 0) { //if one is not while, return none
			return ans;
		}
		else {
			for (int i = 0; i < parTable.size(); ++i) {	//select all the child statements of one	
				convert << parTable[i].second;
				sub.push_back(convert.str());
			}
			sub = filter(sub, two); // sub contains only the correct declaration of two

		}
		for (int j = 0; j < parTable.size(); ++j) { //comparing sub and child of parTable to obtain parent statement line
			for (int k = 0; j < sub.size(); ++k) {
				int value = atoi(sub[k].c_str()); //int of sub
				if (parTable[j].second == value) {
					convert << parTable[i].first;
					if (find(ans.begin(), ans.end(), convert.str()) != ans.end()) {
						continue; //parent statement line already exist in ans
					}
					else {
						ans.push_back(convert.str()); //adding parent statement line that does not exist in ans
					}
				}
			}
		}
	}
	else {
		if (QP::checkSynType(one).compares("while") != 0 || one.compares("_") != 0) { //if one is not while, return none
			return ans;
		}
		else {
			for (int i = 0; i < parTable.size(); ++i) {
				convert << parTable[i].second;
				ans.push_back(convert.str());
			}
		}
		ans = filter(ans, two);
	}
	return ans;
}
vector<string> QE::Parent(string select, int one, string two); { //return all the child of statement line one
	vector<pair<int, int>> parTable = PKB::getParentTable();
	vector<string> ans;
	ostringstream convert;
	for (int i = 0; i < parTable.size(); ++i) {
		if (parTable[i].first == one) {
			convert << parTable[i].second;
			ans.push_back(convert.str());
		}
	}
	ans = filter(ans, two);
	return ans;
}
vector<string> QE::Parent(string select, string one, int two); { //return the parent statement line of the statement line two
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
	ans = filter(ans, one);
	return ans;
}

vector<string> QE::ParentT(string select, string one, string two); { //return all the while statment base on condition
	vector<pair<int, int>> parTable = PKB::getParentTable();
	vector<string> ans;
	return ans;
}
vector<string> QE::ParentT(string select, int one, string two); { //return all the child* of statement line one
	vector<pair<int, int>> parTable = PKB::getParentTable();
	vector<string> ans;
	stack<int> par;
	ostringstream convert;
	int next = one;
	while (true) {
		for (int i = 0; i < parTable.size(); ++i) {
			if (parTable[i].first == next) {
				par.push(parTable[i].second);
				convert << parTable[i].second;
				ans.push_back(convert.str());
			}
		}
		if (par.empty()) {
			break;
		}
		else {
			next = par.pop();
		}
	}
	ans = filter(ans, two);
	return ans;
}
vector<string> QE::ParentT(string select, string one, int two); { //return all the parent* of statement line two
	vector<pair<int, int>> parTable = PKB::getParentTable();
	vector<string> ans;
	stack<int> par;
	ostringstream convert;
	int previous = two;
	while (true) {
		for (int i = 0; i < parTable.size(); ++i) {
			if (parTable[i].second == previous) {
				par.push(parTable[i].first);
				convert << parTable[i].first;
				ans.push_back(convert.str());
			}
		}
		if (par.empty()) {
			break;
		}
		else {
			previous = par.pop();
		}
	}
	ans = filter(ans, one);
	return ans;
}

vector<string> QE::Follows(string select, string one, string two); { //return statement line of follow base on condition
	vector<pair<int, int>> folTable = PKB::getFollowTable();
	vector<string> ans;
	return ans;
}
vector<string> QE::Follows(string select, int one, string two); { //return the statement line that follows statement line one
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
	ans = filter(ans, two);
	return ans;
}
vector<string> QE::Follows(string select, string one, int two); { //return the statement line that is before statement line two
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
	ans = filter(ans, one);
	return ans;
}

vector<string> QE::FollowsT(string select, string one, string two); { //return all the statement lines base on condition
	vector<pair<int, int>> folTable = PKB::getFollowTable();
	vector<string> ans;
	ostringstream convert;

	for (int i = 0; i < folTable.size(); ++i) {
		convert << folTable[i].second;
		ans.push_back(convert.str());
		convert << folTable[i].first;
		ans.push_back(convert.str());
	}

	return ans;
}
vector<string> QE::FollowsT(string select, int one, string two); { //return all the statement lines that follows* statement line one
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
	ans = filter(ans, two);
	return ans;
}
vector<string> QE::FollowsT(string select, string one, int two); { //return all the statements lines that come before statement line two and in the sames nesting level
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
	ans = filter(ans, one);
	return ans;
}

vector<string> QE::pattern(string select, string one, string two); { //return the statement lines that has this pattern
	vector<string> ans;
	return ans;
}

vector<string> QE::filter(vector<string> vec, string one, string two) {
	vector<string> filAns;
	string type1 = QP::checkSynType(one);
	string type2 = QP::checkSynType(two);
	unordered_map<int, LineToken*>* stmtTable = PKB::getStatementTable.getTable;
	
	if (one.compare("_") == 0 && two.compare("_") == 0) {
		return vec;
	} else if (one.compare("_") == 0 || two.compare("_") == 0) {
		// would be e.g _,a or w,_		
		if (one.compare("_") == 0) {
			//make sure two matches the type required e.g _,a or _,w
			for (int i = 0; i < vec.size(); ++i) {
				//obtain token of the statement number and compare with the type in two
				int value = atoi(vec[i].c_str());
				int same = type2.compare(stmtTable[value].second.getType);
				if (same == 0){
					// add it to final answer
					filAns.push_back(vec[i]));
				}
			}
		}
		if (two.compare("_") == 0 ) {
			//make sure one matches the type required e.g a,_ or w,_
			for (int i = 0; i < vec.size(); ++i) {
				//obtain token of the statement number and compare with the type in two
				int value = atoi(vec[i].c_str());
				int same = type1.compare(stmtTable[value].second.getType);
				if (same == 0) {
					// add it to final answer
					filAns.push_back(vec[i]));
				}
			}
		}
	} else {
		// must check both type e.g a,w or w,w
			//make sure one matches the type required e.g a,_ or w,_
			for (int i = 0; i < ans.size(); ++i) {
				if (vector.get(i).one.type == typeOne && vector.get(i).two.type == typeTwo) {
					// add it to final answer
					filAns.add(vector.get(i));
				}
			}
	}
	return filAns;
}

vector<string> QE::filter(vector<string> vec, string field) {
	vector<string> filAns;
	string type = QP::checkSynType(field);

	if (field.compare("_") == 0) {
		return vec;
	}
	else {
		for (int i = 0; i < vec.size(); ++i) {
			//obtain token of the statement number and compare with the type in two
			int value = atoi(vec[i].c_str());
			int same = type.compare(stmtTable[value].second.getType);
			if (same == 0) {
				// add it to final answer
				filAns.push_back(vec[i]));
			}
		}
	}
	return filAns;
}