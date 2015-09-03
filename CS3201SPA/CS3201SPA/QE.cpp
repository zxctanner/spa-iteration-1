#include "QE.h"
#include "PKB.h"
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

QE QE::QueryEvaluator(){
}

vector<int> QE::ModifiesS(string one, string two); { //returns statement number that modifies variable two
	unordered_map<int, pair<string, string>> modTable = PKB::getmodUseTable();
	vector<int> ans;
	for (auto i = modTable.begin(); i != modTable.end(); ++i){
		if (modTable.second.first.compares(two) == 0) {
			ans.push_back(modTable.first);
		}
	}
	return ans;
}
vector<string> QE::ModifiesS(int one, string two); { //returns variable that is modifies in statement line one
	unordered_map<int, pair<string, string>> modTable = PKB::getmodUseTable();
	vector<string> ans; 
	ans.push_back(modTable[one].first);
	return ans;
}

vector<int> QE::UsesS(string one, string two); { //returns statment numbers that uses variable two
	unordered_map<int, pair<string, string>> modTable = PKB::getmodUseTable();
	vector<int> ans;
	for (auto i = modTable.begin(); i != modTable.end(); ++i) {
		string RHS = modTable.second.second;
		if (RHS.find(two) != string::npos) {
			ans.push_back(modTable.first);
		}
	}
	return ans;
}
vector<string> QE::UsesS(int one, string two); { //returns variables that are used in statement line one
	unordered_map<int, pair<string, string>> modTable = PKB::getmodUseTable();
	string RHS = modTable[one].second;
	string split[] = RHS.split("+");
	vector<string> ans;
	for (int i = 0; i < sizeof(RHS); ++i) {
		ans.push_back(split[i]);
	}
	return ans;
}

vector<int> QE::Parent(String one, string two); { //return all the while statement
	vector<pair<int, int>> parTable = PKB::getParentTable();
}
vector<int> QE::Parent(int one, string two); { //return all the child of statement line one
	vector<pair<int, int>> parTable = PKB::getParentTable();
	vector<int> ans;
	for (int i = 0; i < parTable.size(); ++i) {
		if (parTable[i].first == one) {
			ans.push_back(parTable[i].second);
		}
	}
	return ans;
}
vector<int> QE::Parent(String one, int two); { //return the parent of the statement line two
	vector<pair<int, int>> parTable = PKB::getParentTable();
	vector<int> ans;
	for (int i = 0; i < parTable.size(); i++) {
		if (parTable[i].second == two) {
			ans.push_back(parTable[i].first);
			break;
		}
	}
	return ans;
}

vector<int> QE::ParentT(String one, string two); {

}
vector<int> QE::ParentT(int one, string two); {

}
vector<int> QE::ParentT(String one, int two); {

}

vector<int> QE::Follows(string one, string two); {
	vector<pair<int, int>> folTable = PKB::getFollowTable();
}
vector<int> QE::Follows(int one, string two); {
	vector<pair<int, int>> folTable = PKB::getFollowTable();
}
vector<int> QE::Follows(string one, int two); {
	vector<pair<int, int>> folTable = PKB::getFollowTable();
}

vector<int> QE::FollowsT(string one, int two); {

}
vector<int> QE::FollowsT(int one, string two); {

}
vector<int> QE::FollowsT(string one, int two); {

}

vector<int> QE::pattern(string one, string two); {

}