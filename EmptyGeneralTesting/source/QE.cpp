#include "QE.h"
#include "LineToken.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <stack>
#include <iostream>
#include <algorithm>
#include <regex>
#include <sstream>

using namespace std;

/* Query Evaluator
1. Each method is called by the Query Parser depending on the command
2. The field obtained is the select string, field 1 and field 2
3. According to the command, the respective method is used
4. The method will access the PKB for the information
5. There will be a filter to access the type of the string to further evaluate the answers
6. The query answers will be placed in vector<string> ans1 for all commands and ans2 for pattern command
7. If there are only 1 command, the respective vector will be printed else AND the two vectors before printing*/

QE::QE()
{
}

QE::QE(string fileName, PKB * p)
{
	filePath = fileName;
	QP qp(fileName);
	qp.process();
	queryVector = qp.getVectorQuery();
	pkb = p;
	vector<Query> query = qp.getVectorQuery();
	vector<string> ansST;
	vector<string> ansP;
	for (int i = 0; i < query.size(); i++) {
		Query q = query[i];
		vector<string> fields = q.getQueryFields();
		int size = fields.size();
		if (size == 0) {
			answers.push_back("none");
		}
		else if (size == 4) {
			//cout << "Currently in size = 4" << endl;
			string select = fields[0];
			string command = fields[1];
			string one = fields[2];
			string two = fields[3];
			//cout << select << endl;
			//cout << command << endl;
			//cout << one << endl;
			//cout << two << endl;
			if (checkAnswerSize(selectField(select, command, one, two, q))) {
				answers.push_back(vectorSToString(selectField(select, command, one, two, q)));
			}
			else {
				answers.push_back("none");
			}
		}
		else if (size == 8) {
			string select = fields[0];
			string command = fields[1];
			string one = fields[2];
			string two = fields[3];
			string select2 = fields[4];
			string command2 = fields[5];
			string one2 = fields[6];
			string two2 = fields[7];
			if (checkAnswerSize(findCommonAnswer(selectField(select, command, one, two, q), selectField(select2, command2, one2, two2, q)))) {
				answers.push_back(vectorSToString(findCommonAnswer(selectField(select, command, one, two, q), selectField(select2, command2, one2, two2, q))));
			}
			else {
				answers.push_back("none");
			}
		}
	}
}

vector<string> QE::selectField(string select, string command, string one, string two, Query q) {
	bool isNum1 = isInt(one);
	bool isNum2 = isInt(two);
	vector<string> ansP;
	vector<string> ansST;
	int a, b;
	if (isNum1) {
		a = atoi(one.c_str());
	}
	if (isNum2) {
		b = atoi(two.c_str());
	}
	if (command.substr(0, 7).compare("pattern") == 0) {
		ansP = pattern(select, command, one, two, q);
		return ansP;
	}
	else {
		//cout << "Such that detected!" << endl;
		if (command.compare("Modifies") == 0) {
			if (!isNum1 && !isNum2) {
				ansST = ModifiesS(select, one, two, q);
			}
			else {
				ansST = ModifiesS(select, a, two, q);
			}
		}
		else if (command.compare("Uses") == 0) {
			if (isNum1 && !isNum2) {
				ansST = UsesS(select, a, two, q);
			}
			else {
				ansST = UsesS(select, one, two, q);
			}
		}
		else if (command.compare("Parent") == 0) {
			if (isNum1 && !isNum2) {
				ansST = Parent(select, a, two, q);
			}
			else if (!isNum1 && isNum2) {
				ansST = Parent(select, one, b, q);
			}
			else if (isNum1 && isNum2) {
				ansST = Parent(select, a, b, q);
			}
			else {
				ansST = Parent(select, one, two, q);
			}
		}
		else if (command.compare("Parent*") == 0) {
			if (isNum1 && !isNum2) {
				ansST = ParentT(select, a, two, q);
			}
			else if (!isNum1 && isNum2) {
				ansST = ParentT(select, one, b, q);
			}
			else {
				ansST = ParentT(select, one, two, q);
			}
		}
		else if (command.compare("Follows") == 0) {
			//cout << "Follows detected!" << endl;
			if (isNum1 && !isNum2) {
				//cout << "Hello" << endl;
				ansST = Follows(select, a, two, q);
			}
			else if (!isNum1 && isNum2) {
				ansST = Follows(select, one, b, q);
			}
			else {
				ansST = Follows(select, one, two, q);
			}
		}
		else if (command.compare("Follows*") == 0) {
			if (isNum1 && !isNum2) {
				ansST = FollowsT(select, a, two, q);
			}
			else if (!isNum1 && isNum2) {
				ansST = FollowsT(select, one, b, q);
			}
			else {
				ansST = FollowsT(select, one, two, q);
			}
		}
		return ansST;
	}
}

bool QE::isInt(string input) {
	regex integer_rgx("^[0-9]+$");
	return regex_match(input, integer_rgx);
}

vector<string> QE::ModifiesS(string select, string one, string two, Query q) { //returns statement number that modifies variable two
	unordered_map<int, pair<vector<string>, vector<string>>> modUseTable = pkb->getmodUseTable()->getTable();
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	vector<string> ans;
	vector<string> check;
	int relate = relation(select, one, two);
	if (relate == 1) {
		if (two == "_") {
			for (auto i = modUseTable.begin(); i != modUseTable.end(); ++i) {
				string str = to_string(i->first);
				ans.push_back(str);
			}
			ans = filter(ans, one, q);
			return ans;
		}
		else {
			size_t found = two.find("\"");
			if (found != std::string::npos) {
				two.erase(remove(two.begin(), two.end(), '\"'), two.end());
				for (auto i = modUseTable.begin(); i != modUseTable.end(); ++i) {
					check = i->second.first;
					for (int j = 0; j < check.size(); ++j) {
						if (check[j].compare(two) == 0) {
							string str = to_string(i->first);
							ans.push_back(str);
						}
					}
				}
			}
			ans = filter(ans, one, q);
			return ans;
		}
	}
	else if (relate == 2) {
		vector<string> modify;
		vector<string> variables;
		for (auto i = modUseTable.begin(); i != modUseTable.end(); ++i) {
			string str = to_string(i->first);
			ans.push_back(str);
		}
		ans = filter(ans, one, q);
		for (int j = 0; j < ans.size(); ++j) {
			int value = atoi(ans[j].c_str());
			modify = modUseTable[value].first;
			for (int k = 0; k < modify.size(); ++k) {
				if (find(variables.begin(), variables.end(), modify[k]) != variables.end())
					continue;
				else {
					variables.push_back(modify[k]);
				}
			}
		}
		return variables;
	}
	else {
		bool status;
		if (two == "_" || q.checkSynType(two) == "VARIABLE") {
			for (auto i = modUseTable.begin(); i != modUseTable.end(); ++i) {
				string str = to_string(i->first);
				ans.push_back(str);
			}
			ans = filter(ans, one, q);
			if (ans.size() > 0) {
				status = true;
			}
			else {
				status = false;
			}
		}
		else {
			size_t found = two.find("\"");
			if (found != std::string::npos) {
				two.erase(remove(two.begin(), two.end(), '\"'), two.end());
				for (auto i = modUseTable.begin(); i != modUseTable.end(); ++i) {
					check = i->second.first;
					for (int j = 0; j < check.size(); ++j) {
						if (check[j].compare(two) == 0) {
							string str = to_string(i->first);
							ans.push_back(str);
						}
					}
				}
				ans = filter(ans, one, q);
			}
			if (ans.size() > 0) {
				status = true;
			}
			else {
				status = false;
			}
		}
		string choice = q.checkSynType(select);
		ans = Choices(choice, status);
		return ans;
	}
}
vector<string> QE::ModifiesS(string select, int one, string two, Query q) { //returns variable that is modifies in statement line one
	unordered_map<int, pair<vector<string>, vector<string>>> modUseTable = pkb->getmodUseTable()->getTable();
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	vector<string> ans;
	vector<string> modify = modUseTable[one].first;
	int relate = relation(select, to_string(one), two);
	if (relate == 2) {
		for (int i = 0; i < modify.size(); ++i) {
			ans.push_back(modify[i]);
		}
		return ans;
	}
	else {
		bool status = false;
		if (two == "_" || q.checkSynType(two) == "VARIABLE") {
			status = true;
		}
		else {
			size_t found = two.find("\"");
			if (found != std::string::npos) {
				two.erase(remove(two.begin(), two.end(), '\"'), two.end());
			}
			for (int i = 0; i < modify.size(); ++i) {
				if (modify[i] == two) {
					status = true;
					break;
				}
			}
		}
		string choice = q.checkSynType(select);
		ans = Choices(choice, status);
		return ans;
	}
}

vector<string> QE::UsesS(string select, string one, string two, Query q) { //returns statment numbers that uses variable two
	unordered_map<int, pair<vector<string>, vector<string>>> modUseTable = pkb->getmodUseTable()->getTable();
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	vector<string> ans;
	vector<string> use;
	vector<string> check;
	int relate = relation(select, one, two);
	if (relate == 1) {
		if (two == "_") {
			for (auto i = modUseTable.begin(); i != modUseTable.end(); ++i) {
				string str = to_string(i->first);
				ans.push_back(str);
			}
			ans = filter(ans, one, q);
			return ans;
		}
		else {
			size_t found = two.find("\"");
			if (found != std::string::npos) {
				two.erase(remove(two.begin(), two.end(), '\"'), two.end());
				for (auto i = modUseTable.begin(); i != modUseTable.end(); ++i) {
					check = i->second.second;
					for (int j = 0; j < check.size(); ++j) {
						if (check[j].compare(two) == 0) {
							string str = to_string(i->first);
							ans.push_back(str);
						}
					}
				}
			}
			ans = filter(ans, one, q);
			return ans;
		}
	}
	else if (relate == 2) {
		vector<string> variables;
		for (auto i = modUseTable.begin(); i != modUseTable.end(); ++i) {
			string str = to_string(i->first);
			ans.push_back(str);
		}
		ans = filter(ans, one, q);
		for (int j = 0; j < ans.size(); ++j) {
			int value = atoi(ans[j].c_str());
			use = modUseTable[value].second;
			for (int k = 0; k < use.size(); ++k) {
				if (find(variables.begin(), variables.end(), use[k]) != variables.end())
					continue;
				else {
					variables.push_back(use[k]);
				}
			}
		}
		return variables;
	}
	else {
		bool status;
		if (two == "_" || q.checkSynType(two) == "VARIABLE") {
			for (auto i = modUseTable.begin(); i != modUseTable.end(); ++i) {
				string str = to_string(i->first);
				ans.push_back(str);
			}
			ans = filter(ans, one, q);
			if (ans.size() > 0) {
				status = true;
			}
			else {
				status = false;
			}
		}
		else {
			size_t found = two.find("\"");
			if (found != std::string::npos) {
				two.erase(remove(two.begin(), two.end(), '\"'), two.end());
				for (auto i = modUseTable.begin(); i != modUseTable.end(); ++i) {
					check = i->second.second;
					for (int j = 0; j < check.size(); ++j) {
						if (check[j].compare(two) == 0) {
							string str = to_string(i->first);
							ans.push_back(str);
						}
					}
				}
				ans = filter(ans, one, q);
			}
			if (ans.size() > 0) {
				status = true;
			}
			else {
				status = false;
			}
		}
		string choice = q.checkSynType(select);
		ans = Choices(choice, status);
		return ans;
	}
}
vector<string> QE::UsesS(string select, int one, string two, Query q) { //returns variables that are used in statement line one
	unordered_map<int, pair<vector<string>, vector<string>>> modUseTable = pkb->getmodUseTable()->getTable();
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	vector<string> ans;
	vector<string> use = modUseTable[one].second;
	int relate = relation(select, to_string(one), two);
	if (relate == 2) {
		for (int i = 0; i < use.size(); ++i) {
			ans.push_back(use[i]);
		}
		return ans;
	}
	else {
		bool status = false;
		if (two == "_" || q.checkSynType(two) == "VARIABLE") {
			status = true;
		}
		else {
			size_t found = two.find("\"");
			if (found != std::string::npos) {
				two.erase(remove(two.begin(), two.end(), '\"'), two.end());
			}
			for (int i = 0; i < use.size(); ++i) {
				if (use[i] == two) {
					status = true;
					break;
				}
			}
		}
		string choice = q.checkSynType(select);
		ans = Choices(choice, status);
		return ans;
	}
}

vector<string> QE::Parent(string select, string one, string two, Query q) { //return all the while statement base on condition
	vector<pair<int, int>> parTable = pkb->getParentTable()->getTable();
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	vector<string> sub;
	vector<string> ans;
	bool status = false;
	int relate = relation(select, one, two);
	if (relate == 1) {
		if (q.checkSynType(one) == "WHILE" || one == "_" || q.checkSynType(one) == "STATEMENT") { //if one is not while, return none
			for (int i = 0; i < parTable.size(); ++i) {	//select all the child statements of one	
				string str = to_string(parTable[i].second);
				sub.push_back(str);
			}
			sub = filter(sub, two, q); // sub contains only the correct declaration of two
			for (int j = 0; j < parTable.size(); ++j) { //comparing sub and child of parTable to obtain parent statement line
				for (int k = 0; k < sub.size(); ++k) {
					int value = atoi(sub[k].c_str()); //int of sub
					if (parTable[j].second == value) {
						string str = to_string(parTable[j].first);
						if (find(ans.begin(), ans.end(), str) != ans.end()) {
							continue; //parent statement line already exist in ans
						}
						else {
							ans.push_back(str); //adding parent statement line that does not exist in ans
						}
					}
				}
			}
			return ans;
		}
		else { //if one is not while or _, return none
			return ans;
		}
	}
	else {
		if (q.checkSynType(one) == "WHILE" || one == "_" || q.checkSynType(one) == "STATEMENT") {
			for (int i = 0; i < parTable.size(); ++i) {
				string str = to_string(parTable[i].second);
				ans.push_back(str);
			}
			ans = filter(ans, two, q);
		}
		else {
			return ans;
		}
	}
	if (relate == 2) {
		return ans;
	}
	else {
		if (ans.size() > 0) {
			status = true;
		}
		else {
			status = false;
		}
		string choice = q.checkSynType(select);
		ans = Choices(choice, status);
		return ans;
	}
}

vector<string> QE::Parent(string select, int one, string two, Query q) { //return all the child of statement line one
	vector<pair<int, int>> parTable = pkb->getParentTable()->getTable();
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	vector<string> ans;
	int relate = relation(select, to_string(one), two);
	if (relate == 2) {
		for (int i = 0; i < parTable.size(); ++i) {
			if (parTable[i].first == one) {
				string str = to_string(parTable[i].second);
				ans.push_back(str);
			}
		}
		ans = filter(ans, two, q);
		return ans;
	}
	else {
		bool status = false;
		if (two == "_") {
			status = true;
		}
		else {
			for (int i = 0; i < parTable.size(); ++i) {
				if (parTable[i].first == one) {
					string str = to_string(parTable[i].second);
					ans.push_back(str);
				}
			}
			ans = filter(ans, two, q);
			if (ans.size() > 0) {
				status = true;
			}
			else {
				status = false;
			}
		}
		string choice = q.checkSynType(select);
		ans = Choices(choice, status);
		return ans;
	}
}
vector<string> QE::Parent(string select, string one, int two, Query q) { //return the parent statement line of the statement line two
	vector<pair<int, int>> parTable = pkb->getParentTable()->getTable();
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	vector<string> ans;
	bool status = false;
	int relate = relation(select, one, to_string(two));
	for (int i = 0; i < parTable.size(); i++) {
		if (parTable[i].second == two) {
			string str = to_string(parTable[i].first);
			ans.push_back(str);
			break;
		}
	}
	ans = filter(ans, one, q);
	if (relate == 1) {
		return ans;
	}
	else {
		if (ans.size() > 0) {
			status = true;
		}
		else {
			status = false;
		}
		string choice = q.checkSynType(select);
		ans = Choices(choice, status);
		return ans;
	}
}

vector<string> QE::Parent(string select, int one, int two, Query q) {
	vector<pair<int, int>> parTable = pkb->getParentTable()->getTable();
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	vector<string> ans;
	bool status = false;
	for (int i = 0; i < parTable.size(); i++) {
		if (parTable[i].first == one && parTable[i].second == two) {
			status = true;
			break;
		}
	}
	string choice = q.checkSynType(select);
	ans = Choices(choice, status);
	return ans;
}

vector<string> QE::ParentT(string select, string one, string two, Query q) { //return all the while statment base on condition
	vector<pair<int, int>> parTable = pkb->getParentTable()->getTable();
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	vector<string> sub;
	vector<string> ans;
	bool status = false;
	int relate = relation(select, one, two);
	if (relate == 1) {
		if (q.checkSynType(one) == "WHILE" || one == "_" || q.checkSynType(one) == "STATEMENT") {
			for (int i = 0; i < parTable.size(); ++i) {	//select all the child statements of one	
				string str = to_string(parTable[i].second);
				sub.push_back(str);
			}
			sub = filter(sub, two, q); // sub contains only the correct declaration of two
			for (int j = 0; j < parTable.size(); ++j) { //comparing sub and child of parTable to obtain parent statement line
				for (int k = 0; k < sub.size(); ++k) {
					int value = atoi(sub[k].c_str()); //int of sub
					if (parTable[j].second == value) {
						string str = to_string(parTable[j].first);
						if (find(ans.begin(), ans.end(), str) != ans.end()) {
							continue; //parent statement line already exist in ans
						}
						else {
							ans.push_back(str); //adding parent statement line that does not exist in ans
						}
					}
				}
			}
			return ans;
		}
		else {
			return ans;
		}
	}
	else {
		if (q.checkSynType(one) == "WHILE" || one == "_" || q.checkSynType(one) == "STATEMENT") {
			for (int i = 0; i < parTable.size(); ++i) {
				string str = to_string(parTable[i].second);
				ans.push_back(str);
			}
			ans = filter(ans, two, q);
		}
		else {
			return ans;
		}
		if (relate == 2) {
			return ans;
		}
		else {
			if (ans.size() > 0) {
				status = true;
			}
			else {
				status = false;
			}
			string choice = q.checkSynType(select);
			ans = Choices(choice, status);
			return ans;
		}
	}
}
vector<string> QE::ParentT(string select, int one, string two, Query q) { //return all the child* of statement line one
	vector<pair<int, int>> parTable = pkb->getParentTable()->getTable();
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	bool status = false;
	vector<string> ans;
	stack<int> par;
	int relate = relation(select, to_string(one), two);
	int next = one;
	while (true) {
		for (int i = 0; i < parTable.size(); ++i) {
			if (parTable[i].first == next) {
				par.push(parTable[i].second);
				string str = to_string(parTable[i].second);
				ans.push_back(str);
			}
		}
		if (par.empty()) {
			break;
		}
		else {
			next = par.top();
			par.pop();
		}
	}
	ans = filter(ans, two, q);
	if (relate == 2) {
		return ans;
	}
	else {
		if (ans.size() > 0) {
			status = true;
		}
		else {
			status = false;
		}
		string choice = q.checkSynType(select);
		ans = Choices(choice, status);
		return ans;
	}
}
vector<string> QE::ParentT(string select, string one, int two, Query q) { //return all the parent* of statement line two
	vector<pair<int, int>> parTable = pkb->getParentTable()->getTable();
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	vector<string> ans;
	stack<int> par;
	bool status = false;
	int relate = relation(select, one, to_string(two));
	int previous = two;
	while (true) {
		for (int i = 0; i < parTable.size(); ++i) {
			if (parTable[i].second == previous) {
				par.push(parTable[i].first);
				string str = to_string(parTable[i].first);
				ans.push_back(str);
			}
		}
		if (par.empty()) {
			break;
		}
		else {
			previous = par.top();
			par.pop();
		}
	}
	ans = filter(ans, one, q);
	if (relate == 1) {
		return ans;
	}
	else {
		if (ans.size() > 0) {
			status = true;
		}
		else {
			status = false;
		}
		string choice = q.checkSynType(select);
		ans = Choices(choice, status);
		return ans;
	}
}
vector<string> QE::ParentT(string select, int one, int two, Query q) {
	vector<pair<int, int>> parTable = pkb->getParentTable()->getTable();
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	vector<string> ans;
	stack<int> par; 
	bool status = false;
	int next = one;
	while (true) {
		for (int i = 0; i < parTable.size(); ++i) {
			if (parTable[i].first == next) {
				par.push(parTable[i].second);
				string str = to_string(parTable[i].second);
				ans.push_back(str);
			}
		}
		if (par.empty()) {
			break;
		}
		else {
			next = par.top();
			par.pop();
		}
	}
	for (int j = 0; j < ans.size(); ++j) {
		int value = atoi(ans[j].c_str());
		if (value == two) {
			status = true;
			break;
		}
	}
	string choice = q.checkSynType(select);
	ans = Choices(choice, status);
	return ans;	
}

vector<string> QE::Follows(string select, string one, string two, Query q) { //return statement line of follow base on condition
	vector<pair<int, int>> folTable = pkb->getFollowTable()->getTable();
	vector<string> sub;
	vector<string> sub2;
	vector<string> ans;
	if (select.compare(one) == 0) {
		for (int i = 0; i < folTable.size(); ++i) {
			string str = to_string(folTable[i].first);
			sub.push_back(str);
		}
		sub = filter(sub, one, q); // sub contains only the correct declaration of one
		for (int j = 0; j < folTable.size(); ++j) { //comparing sub and second of parTable to obtain follow statement line
			for (int k = 0; k < sub.size(); ++k) {
				int value = atoi(sub[k].c_str()); //int of sub
				if (folTable[j].first == value) {
					string str = to_string(folTable[j].second);
					sub2.push_back(str);
				}
			}
		}
		sub2 = filter(sub2, two, q);
		for (int a = 0; a < folTable.size(); ++a) { //comparing sub2 and first of parTable to obtain follow statement line
			for (int b = 0; b < sub2.size(); ++b) {
				int value = atoi(sub2[b].c_str()); //int of sub2
				if (folTable[a].second == value) {
					string str = to_string(folTable[a].first);
					ans.push_back(str);
				}
			}
		}
	}
	else {
		for (int i = 0; i < folTable.size(); ++i) {
			string str = to_string(folTable[i].second);
			sub.push_back(str);
		}
		sub = filter(sub, two, q); // sub contains only the correct declaration of second
		for (int j = 0; j < folTable.size(); ++j) { //comparing sub and first of parTable to obtain follow statement line
			for (int k = 0; k < sub.size(); ++k) {
				int value = atoi(sub[k].c_str()); //int of sub
				if (folTable[j].second == value) {
					string str = to_string(folTable[j].first);
					sub2.push_back(str);
				}
			}
		}
		sub2 = filter(sub2, one, q);
		for (int a = 0; a < folTable.size(); ++a) { //comparing sub2 and second of parTable to obtain follow statement line
			for (int b = 0; b < sub2.size(); ++b) {
				int value = atoi(sub2[b].c_str()); //int of sub
				if (folTable[a].first == value) {
					string str = to_string(folTable[a].second);
					ans.push_back(str);
				}
			}
		}
	}
	return ans;
}
vector<string> QE::Follows(string select, int one, string two, Query q) { //return the statement line that follows statement line one
	vector<pair<int, int>> folTable = pkb->getFollowTable()->getTable();
	vector<string> ans;
	cout << folTable.size() << endl;
	for (int i = 0; i < folTable.size(); ++i) {
		cout << folTable[i].first << one << endl;
		if (folTable[i].first == one) {
			string str = to_string(folTable[i].second);
			ans.push_back(str);
			cout << ans.size() << endl;
			break;
		}
	}
	ans = filter(ans, two, q);
	return ans;
}
vector<string> QE::Follows(string select, string one, int two, Query q) { //return the statement line that is before statement line two
	vector<pair<int, int>> folTable = pkb->getFollowTable()->getTable();
	vector<string> ans;
	for (int i = 0; i < folTable.size(); i++) {
		if (folTable[i].second == two) {
			string str = to_string(folTable[i].first);
			ans.push_back(str);
			break;
		}
	}
	ans = filter(ans, one, q);
	return ans;
}

vector<string> QE::FollowsT(string select, string one, string two, Query q) { //return all the statement lines base on condition

																			  // e.g Follows* _,_ a,w and 
	vector<pair<int, int>> folTable = pkb->getFollowTable()->getTable();
	vector<string> ans;

	string type1 = q.checkSynType(one);

	string type2 = q.checkSynType(two);

	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();


	for (int i = 0; i < folTable.size(); ++i) {

		int current = folTable[i].first;

		if (one.compare("_") == 0) {

			if (type1.compare(stmtTable[current].getType()) != 0) {
				continue;
			}
		}

		for (int j = i + 1; j < folTable.size(); ++j) {

			int next = folTable[j].second;

			// check type of current and skip ahead if fail

			if (folTable[i].first == current) {

				// check for a and w on the right
				// ignore for _

				int next = folTable[i].second;
				// make sure not _
				if (two.compare("_") == 0) {

					// we get a match so we move on instead of adding to our answer
					if (type2.compare(stmtTable[next].getType()) != 0) {
						continue;
					}
				}

				string str = to_string(folTable[i].second);
				ans.push_back(str);
				current = folTable[i].second;
			}
		}
	}

	/*
	vector<pair<int, int>> folTable = pkb->getFollowTable()->getTable();
	vector<string> sub;
	vector<string> sub2;
	vector<string> ans;
	if (select.compare(one) == 0) {
	for (int i = 0; i < folTable.size(); ++i) {
	string str = to_string(folTable[i].first);
	sub.push_back(str);
	}
	sub = filter(sub, one, q); // sub contains only the correct declaration of one
	for (int j = 0; j < folTable.size(); ++j) { //comparing sub and second of parTable to obtain follow statement line
	for (int k = 0; k < sub.size(); ++k) {
	int value = atoi(sub[k].c_str()); //int of sub
	if (folTable[j].first == value) {
	string str = to_string(folTable[j].second);
	sub2.push_back(str);
	}
	}
	}
	sub2 = filter(sub2, two, q);
	for (int a = 0; a < folTable.size(); ++a) { //comparing sub2 and first of parTable to obtain follow statement line
	for (int b = 0; b < sub2.size(); ++b) {
	int value = atoi(sub2[b].c_str()); //int of sub2
	if (folTable[a].second == value) {
	string str = to_string(folTable[a].first);
	ans.push_back(str);
	}
	}
	}
	}
	else {
	for (int i = 0; i < folTable.size(); ++i) {
	string str = to_string(folTable[i].second);
	sub.push_back(str);
	}
	sub = filter(sub, two, q); // sub contains only the correct declaration of second
	for (int j = 0; j < folTable.size(); ++j) { //comparing sub and first of parTable to obtain follow statement line
	for (int k = 0; k < sub.size(); ++k) {
	int value = atoi(sub[k].c_str()); //int of sub
	if (folTable[j].second == value) {
	string str = to_string(folTable[j].first);
	sub2.push_back(str);
	}
	}
	}
	sub2 = filter(sub2, one, q);
	for (int a = 0; a < folTable.size(); ++a) { //comparing sub2 and second of parTable to obtain follow statement line
	for (int b = 0; b < sub2.size(); ++b) {
	int value = atoi(sub2[b].c_str()); //int of sub
	if (folTable[a].first == value) {
	string str = to_string(folTable[a].second);
	ans.push_back(str);
	}
	}
	}
	}

	*/

	return ans;
}
vector<string> QE::FollowsT(string select, int one, string two, Query q) { //return all the statement lines that follows* statement line one

																		   // e.g Follows* 2,_ and 3,a and 4,w 
	vector<pair<int, int>> folTable = pkb->getFollowTable()->getTable();
	vector<string> ans;

	int current = one;

	string type = q.checkSynType(two);

	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();

	for (int i = 0; i < folTable.size(); ++i) {

		if (folTable[i].first == current) {

			// check for a and w on the right
			// ignore for _

			int next = folTable[i].second;
			// make sure not _
			if (two.compare("_") == 0) {

				// we didn't get a match so we move on instead of adding to our answer
				if (type.compare(stmtTable[next].getType()) != 0) {
					
					string str = to_string(folTable[i].second);
					ans.push_back(str);
				}

			}

			current = folTable[i].second;

		}
	}

	/*
	vector<pair<int, int>> folTable = pkb->getFollowTable()->getTable();
	vector<string> ans;
	int line = one;
	for (int i = 0; i < folTable.size(); ++i) {
	if (folTable[i].first == line) {
	string str = to_string(folTable[i].second);
	ans.push_back(str);
	line = folTable[i].second;
	}
	}
	ans = filter(ans, two, q);
	return ans;

	*/

	return ans;
}
vector<string> QE::FollowsT(string select, string one, int two, Query q) { //return all the statements lines that come before statement line two and in the sames nesting level

																		   // e.g Follows* 2,_ and 3,a and 4,w 
	vector<pair<int, int>> folTable = pkb->getFollowTable()->getTable();
	vector<string> ans;

	int current = two;

	string type = q.checkSynType(one);

	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();

	// a,2 _,3 w,4 so we do from the back instead

	for (int i = folTable.size() - 1; i >= 0; --i) {

		if (folTable[i].second == current) {

			// check for a and w on the right
			// ignore for _

			int next = folTable[i].first;
			// make sure not _
			if (one.compare("_") == 0) {

				// we get a match so we move on instead of adding to our answer
				if (type.compare(stmtTable[next].getType()) != 0) {
					
					string str = to_string(folTable[i].first);
					ans.push_back(str);
				}

			}


			current = folTable[i].first;

		}
	}

	/*
	vector<pair<int, int>> folTable = pkb->getFollowTable()->getTable();
	vector<string> ans;
	int line = two;
	for (int i = folTable.size() - 1; i >= 0; ++i) {
	if (folTable[i].second == line) {
	string str = to_string(folTable[i].first);
	ans.push_back(str);
	line = folTable[i].first;
	}
	}
	ans = filter(ans, one, q);
	return ans;
	}

	vector<string> QE::filter(vector<string> vec, string one, string two, Query q) {
	vector<string> filAns;
	string type1 = q.checkSynType(one);
	string type2 = q.checkSynType(two);
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();

	if (one.compare("_") == 0 && two.compare("_") == 0) {
	return vec;
	} else if (one.compare("_") == 0 || two.compare("_") == 0) {
	// would be e.g _,a or w,_
	if (one.compare("_") == 0) {
	//make sure two matches the type required e.g _,a or _,w
	for (int i = 0; i < vec.size(); ++i) {
	//obtain token of the statement number and compare with the type in two
	int value = atoi(vec[i].c_str());
	if (type2 == (stmtTable[value].getType())){
	// add it to final answer
	filAns.push_back(vec[i]);
	}
	}
	}
	if (two.compare("_") == 0 ) {
	//make sure one matches the type required e.g a,_ or w,_
	for (int i = 0; i < vec.size(); ++i) {
	//obtain token of the statement number and compare with the type in two
	int value = atoi(vec[i].c_str());
	if (type1 == (stmtTable[value].getType())) {
	// add it to final answer
	filAns.push_back(vec[i]);
	}
	}
	}
	}

	*/
	return ans;
}

vector<string> QE::pattern(string select, string command, string one, string two, Query q) { //return the statement lines that has this pattern
	unordered_map<int, pair<vector<string>, vector<string>>> modUseTable = pkb->getmodUseTable()->getTable();
	bool isUnderscore = false;
	bool returnAllFlag = false;
	// check if the synonym to return, matches the conditions

	vector<string> ans;
	//check if LHS is '_' or variable synonym v, which have the same meaning
	string type = q.checkSynType(one);
	if (one == "_" || type == "VARIABLE") {
		isUnderscore = true;
	}
	// "one" is a variable
	else {
		for (auto it = modUseTable.begin(); it != modUseTable.end(); ++it) {
			// strip the apostophes from one, assuming var is surrounded by " and "
			string var = one.substr(1, one.size() - 2);
			vector<string> modEntry = it->second.first;
			if (find(modEntry.begin(), modEntry.end(), var) != modEntry.end()) {
				ans.push_back(to_string(it->first));
			}
		}
	}
	//check if both sides are "_"
	if (two == "_" && isUnderscore) {
		returnAllFlag = true;
	}
	// if LHS was "_", and RHS is a sub expression
	else if (isUnderscore) {
		//take out the var in the middle, assuming it is surrounded by _" and "_
		string var = two.substr(2, two.size() - 4);
		for (auto it = modUseTable.begin(); it != modUseTable.end(); ++it) {
			vector<string> useEntry = it->second.second;
			if (find(useEntry.begin(), useEntry.end(), var) != useEntry.end()) {
				ans.push_back(to_string(it->first));
			}
		}
	}
	//if RHS is a subexpression and LHS was not "_"
	else if ((two.find("_", 0) != string::npos) && (two.find("_", 1) != string::npos) && (!isUnderscore)) {
		//take out the var in the middle, assuming it is surrounded by _" and "_
		cout << "check" << endl;
		string var = two.substr(2, two.size() - 4);
		for (int i = 0; i < ans.size(); ++i) {
			char* num;
			int stmtNum = strtol(ans[i].c_str(), &num, 10);
			vector<string> useEntry = modUseTable[stmtNum].second;
			//if the current stmts in ans do not satisfy RHS pattern, erase them
			if (find(useEntry.begin(), useEntry.end(), var) == useEntry.end()) {
				ans.erase(ans.begin() + i);
				--i;
			}
		}
	}
	//if RHS is an expression (1st iteration doesn't have this)
	else {

	}
	//keep only stmts of the assignment type
	ans = filter(ans, "a", q);

	//check if var to select matches variables in condition
	int relate = relation(select, command.substr(7, command.size() - 7), "");
	//cout << "Relate = " << relate << ", select = " << select << ", command = " << endl;
	// if not related and conditionals are satisfied, or pattern a(_,_), return all of type 'select'
	if (relate == 0 && ans.size() != 0 || returnAllFlag) {
		string type = q.checkSynType(select);
		ans = getAllType(type);
	}
	return ans;
}

vector<string> QE::getAllType(string type) {
	vector<string> all;
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	if (type == "STATEMENT" || type == "PROG_LINE") {
		for (auto it = stmtTable.begin(); it != stmtTable.end(); ++it) {
			if (it->first != 0) {
				all.push_back(to_string(it->first));
			}
		}
	}
	else if (type == "ASSIGN" || type == "WHILE") {
		for (auto it = stmtTable.begin(); it != stmtTable.end(); ++it) {
			if (it->second.getType() == type) {
				all.push_back(to_string(it->first));
			}
		}
	}
	else if (type == "VARIABLE") {
		all = pkb->getVarList()->getAllVar();
	}
	else if (type == "CONSTANT") {
		unordered_map<int, pair<vector<string>, vector<string>>> modUseTable = pkb->getmodUseTable()->getTable();
		set<string> constantSet;
		for (auto it = modUseTable.begin(); it != modUseTable.end(); ++it) {
			vector<string> used = it->second.second;
			for (int i = 0; i < used.size(); ++i) {
				char* isInt;
				int converted = strtol(used[i].c_str(), &isInt, 10);
				if (isInt) {
					constantSet.insert(used[i]);
				}
			}
		}
		all = vector<string>(constantSet.begin(), constantSet.end());
	}
	return all;
}

int QE::relation(string select, string one, string two) {
	if (select == one) {
		return 1;
	}
	else if (select == two) {
		return 2;
	}
	else {
		return 0;
	}
}

vector<string> QE::filter(vector<string> vec, string field, Query q) {
	vector<string> filAns;
	string type = q.checkSynType(field);
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	if (type == "WHILE") {
		for (int i = 0; i < vec.size(); ++i) {
			int value = atoi(vec[i].c_str());
			if (type == stmtTable[value].getType()) {
				filAns.push_back(vec[i]);
			}
		}
	}
	else  if (type == "ASSIGN") {
		for (int i = 0; i < vec.size(); ++i) {
			//obtain token of the statement number and compare with the type in two
			int value = atoi(vec[i].c_str());
			if (type == stmtTable[value].getType()) {
				// add it to final answer
				filAns.push_back(vec[i]);
			}
		}
	}
	else {
		return vec;
	}
	return filAns;
}

vector<string> QE::findCommonAnswer(vector<string> ansST, vector<string> ansP) {
	vector<string> answer;
	for (int a = 0; a < ansST.size(); a++) {
		if (find(ansP.begin(), ansP.end(), ansST.at(a)) != ansP.end()) {
			answer.push_back(ansST.at(a));
		}
	}
	return answer;
}

string QE::vectorSToString(vector<string> vecString) {
	string ans;
	stringstream ss;
	for (int i = 0; i < vecString.size(); ++i) {
		if (i != 0) {
			ss << ", ";
		}
		ss << vecString.at(i);
	}
	ans = ss.str();
	return ans;
}

void QE::displayAllAnswers() {
	cout << answers.size() << endl;
	for (int i = 0; i < answers.size(); ++i) {
		cout << "Statement number is: " << answers.at(i) << endl;
	}
}

vector<string> QE::Choices(string choice, bool status) {
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	vector<string> finAns;
	if (status == true) {
		if (choice == "ASSIGN" || choice == "WHILE") {
			for (auto i = 0; i < stmtTable.size(); ++i)
				if (choice == stmtTable[i].getType()) {
					// add it to final answer
					finAns.push_back(to_string(i));
				}
			return finAns;
		}

		else if (choice == "STATEMENT") {
			for (auto i = 1; i < stmtTable.size(); ++i) {
				finAns.push_back(to_string(i));
				return finAns;
			}
		}
		else if (choice == "VARIABLE") {
			finAns = pkb->getVarList()->getAllVar();
			return finAns;
		}
	}
	else {
		return finAns;
	}
}

bool QE::checkAnswerSize(vector<string> answerVector) {
	if (answerVector.size() == 0) {
		return false;
	}
	else {
		return true;
	}
}