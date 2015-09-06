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
	cout << query.size() << endl;
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
			cout << "Currently in size = 4" << endl;
			string select = fields[0];
			string command = fields[1];	
			string one = fields[2];
			string two = fields[3];
			cout << select << endl;
			cout << command << endl;
			cout << one << endl;
			cout << two << endl;
			answers.push_back(vectorSToString(selectField(select, command, one, two, q)));
		}
		else if(size == 8) {
			string select = fields[0];
			string command = fields[1];
			string one = fields[2];
			string two = fields[3];
			string select2 = fields[4];
			string command2 = fields[5];
			string one2 = fields[6];
			string two2 = fields[7];
			answers.push_back(vectorSToString(findCommonAnswer(selectField(select, command, one, two, q), selectField(select2, command2, one2, two2, q))));
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
	if (command.compare("pattern") == 0) {
		ansP = pattern(select, one, two);
		return ansP;
	}
	else {
		cout << "Such that detected!" << endl;
		if (command.compare("Modifies") == 0) {
			if (!isNum1 && !isNum2) {
				ansST = ModifiesS(select, one, two, q);
			}
			else {
				ansST = ModifiesS(select, a, two);
			}
		}
		else if (command.compare("Uses") == 0) {
			if (isNum1 && !isNum2) {
				ansST = UsesS(select, a, two);
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
			else {
				ansST = Parent(select, one, two, q);
			}
		}
		else if (command.compare("ParentT") == 0) {
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
			cout << "Follows detected!" << endl;
			if (isNum1 && !isNum2) {
				cout << "Hello" << endl;
				ansST = Follows(select, a, two, q);
			}
			else if (!isNum1 && isNum2) {
				ansST = Follows(select, one, b, q);
			}
			else {
				ansST = Follows(select, one, two, q);
			}
		}
		else if (command.compare("FollowsT") == 0) {
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
	vector<string> ans;
	vector<string> check;
	ostringstream convert;
	if (two.compare("_") != 0) {
		for (auto i = modUseTable.begin(); i != modUseTable.end(); ++i) {
			check = i->second.first;
			for (int j = 0; j < check.size(); ++j) {
				if (check[j].compare(two) == 0) {
					convert << i->first;
					ans.push_back(convert.str());
				}
			}
		}
		ans = filter(ans, one, two, q);
	}
	else {
		for (auto i = modUseTable.begin(); i != modUseTable.end(); ++i) {
			convert << i->first;
			ans.push_back(convert.str());
		}
		ans = filter(ans, one, two, q);
	}

	return ans;
}
vector<string> QE::ModifiesS(string select, int one, string two) { //returns variable that is modifies in statement line one
	unordered_map<int, pair<vector<string>, vector<string>>> modUseTable = pkb->getmodUseTable()->getTable();
	vector<string> ans;
	ostringstream convert;
	vector<string> modify = modUseTable[one].first;
	for (int i = 0; i < modify.size(); ++i){
		convert << modify[i];
		ans.push_back(convert.str());
	}
	return ans;
}

vector<string> QE::UsesS(string select, string one, string two, Query q) { //returns statment numbers that uses variable two
	unordered_map<int, pair<vector<string>, vector<string>>> modUseTable = pkb->getmodUseTable()->getTable();
	vector<string> ans;
	vector<string> use;
	ostringstream convert;
	if (two.compare("_") != 0) {
		for (auto i = modUseTable.begin(); i != modUseTable.end(); ++i) {
			use = i->second.second;
			for (int j = 0; j < use.size(); ++j) {
				if (use[j].compare(two) == 0) {
					convert << i->first;
					ans.push_back(convert.str());
				}
			}
		}
		ans = filter(ans, one, two, q);
	}
	else {
		for (auto i = modUseTable.begin(); i != modUseTable.end(); ++i) {
			convert << i->first;
			ans.push_back(convert.str());
		}
		ans = filter(ans, one, two, q);
	}
	return ans;
}
vector<string> QE::UsesS(string select, int one, string two) { //returns variables that are used in statement line one
	unordered_map<int, pair<vector<string>, vector<string>>> modUseTable = pkb->getmodUseTable()->getTable();
	vector<string> ans;
	
	vector<string> use = modUseTable[one].second;
	for (int i = 0; i < use.size(); ++i) {
		ans.push_back(use[i]);
	}
	return ans;
}

vector<string> QE::Parent(string select, string one, string two, Query q) { //return all the while statement base on condition
	vector<pair<int, int>> parTable = pkb->getParentTable()->getTable();
	vector<string> sub;
	vector<string> ans;
	ostringstream convert;
	if (select.compare(one) == 0) {
		if (q.checkSynType(one) != WHILE) { //if one is not while, return none
			return ans;
		}
		else {
			for (int i = 0; i < parTable.size(); ++i) {	//select all the child statements of one	
				convert << parTable[i].second;
				sub.push_back(convert.str());
			}
			sub = filter(sub, two, q); // sub contains only the correct declaration of two

		}
		for (int j = 0; j < parTable.size(); ++j) { //comparing sub and child of parTable to obtain parent statement line
			for (int k = 0; k < sub.size(); ++k) {
				int value = atoi(sub[k].c_str()); //int of sub
				if (parTable[j].second == value) {
					convert << parTable[j].first;
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
		if (q.checkSynType(one) != WHILE || one.compare("_") != 0) { //if one is not while, return none
			return ans;
		}
		else {
			for (int i = 0; i < parTable.size(); ++i) {
				convert << parTable[i].second;
				ans.push_back(convert.str());
			}
		}
		ans = filter(ans, two, q);
	}
	return ans;
}
vector<string> QE::Parent(string select, int one, string two, Query q) { //return all the child of statement line one
	vector<pair<int, int>> parTable = pkb->getParentTable()->getTable();	
	vector<string> ans;
	ostringstream convert;
	for (int i = 0; i < parTable.size(); ++i) {
		if (parTable[i].first == one) {
			convert << parTable[i].second;
			ans.push_back(convert.str());
		}
	}
	cout << ans.size() << endl;
	ans = filter(ans, two, q);
	cout << ans.size() << endl;
	return ans;
}

vector<string> QE::Parent(string select, string one, int two, Query q) { //return the parent statement line of the statement line two
	vector<pair<int, int>> parTable = pkb->getParentTable()->getTable();	
	vector<string> ans;
	ostringstream convert;
	for (int i = 0; i < parTable.size(); i++) {
		if (parTable[i].second == two) {
			convert << parTable[i].first;
			ans.push_back(convert.str());
			break;
		}
	}
	ans = filter(ans, one, q);
	return ans;
}

vector<string> QE::ParentT(string select, string one, string two, Query q) { //return all the while statment base on condition
	vector<pair<int, int>> parTable = pkb->getParentTable()->getTable();
	vector<string> sub;
	vector<string> ans;
	ostringstream convert;
	if (select.compare(one) == 0) {
		if (q.checkSynType(one) != WHILE) { //if one is not while, return none
			return ans;
		}
		else {
			for (int i = 0; i < parTable.size(); ++i) {	//select all the child statements of one	
				convert << parTable[i].second;
				sub.push_back(convert.str());
			}
			sub = filter(sub, two, q); // sub contains only the correct declaration of two
		}
		for (int j = 0; j < parTable.size(); ++j) { //comparing sub and child of parTable to obtain parent statement line
			for (int k = 0; k < sub.size(); ++k) {
				int value = atoi(sub[k].c_str()); //int of sub
				if (parTable[j].second == value) {
					convert << parTable[j].first;
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
		if (q.checkSynType(one) != WHILE || one.compare("_") != 0) { //if one is not while, return none
			return ans;
		}
		else {
			for (int i = 0; i < parTable.size(); ++i) {
				convert << parTable[i].second;
				ans.push_back(convert.str());
			}
		}
		ans = filter(ans, two, q);
	}
	return ans;
}
vector<string> QE::ParentT(string select, int one, string two, Query q){ //return all the child* of statement line one
	vector<pair<int, int>> parTable = pkb->getParentTable()->getTable();
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
			next = par.top();
			par.pop();
		}
	}
	ans = filter(ans, two, q);
	return ans;
}
vector<string> QE::ParentT(string select, string one, int two, Query q) { //return all the parent* of statement line two
	vector<pair<int, int>> parTable = pkb->getParentTable()->getTable();
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
			previous = par.top();
			par.pop();
		}
	}
	ans = filter(ans, one, q);
	return ans;
}

vector<string> QE::Follows(string select, string one, string two, Query q) { //return statement line of follow base on condition
	vector<pair<int, int>> folTable = pkb->getFollowTable()->getTable();
	vector<string> sub;
	vector<string> sub2;
	vector<string> ans;
	ostringstream convert;
	if (select.compare(one) == 0) {
		for (int i = 0; i < folTable.size(); ++i) {	
			convert << folTable[i].first;
			sub.push_back(convert.str());
		}
		sub = filter(sub, one, q); // sub contains only the correct declaration of one
		for (int j = 0; j < folTable.size(); ++j) { //comparing sub and second of parTable to obtain follow statement line
			for (int k = 0; k < sub.size(); ++k) {
				int value = atoi(sub[k].c_str()); //int of sub
				if (folTable[j].first == value) {
					convert << folTable[j].second;
					sub2.push_back(convert.str());
				}
			}
		}
		sub2 = filter(sub2, two, q);
		for (int a = 0; a < folTable.size(); ++a) { //comparing sub2 and first of parTable to obtain follow statement line
			for (int b = 0; b < sub2.size(); ++b) {
				int value = atoi(sub2[b].c_str()); //int of sub2
				if (folTable[a].second == value) {
					convert << folTable[a].first;
					ans.push_back(convert.str());
				}
			}
		}
	}
	else {
		for (int i = 0; i < folTable.size(); ++i) {
			convert << folTable[i].second;
			sub.push_back(convert.str());
		}
		sub = filter(sub, two, q); // sub contains only the correct declaration of second
		for (int j = 0; j < folTable.size(); ++j) { //comparing sub and first of parTable to obtain follow statement line
			for (int k = 0; k < sub.size(); ++k) {
				int value = atoi(sub[k].c_str()); //int of sub
				if (folTable[j].second == value) {
					convert << folTable[j].first;
					sub2.push_back(convert.str());
				}
			}
		}
		sub2 = filter(sub2, one, q);
		for (int a = 0; a < folTable.size(); ++a) { //comparing sub2 and second of parTable to obtain follow statement line
			for (int b = 0; b < sub2.size(); ++b) {
				int value = atoi(sub2[b].c_str()); //int of sub
				if (folTable[a].first == value) {
					convert << folTable[a].second;
					ans.push_back(convert.str());
				}
			}
		}
	}
	return ans;
}
vector<string> QE::Follows(string select, int one, string two, Query q) { //return the statement line that follows statement line one
	vector<pair<int, int>> folTable = pkb->getFollowTable()->getTable();
	vector<string> ans;
	ostringstream convert;
	for (int i = 0; i < folTable.size(); ++i) {
		if (folTable[i].first == one) {
			convert << folTable[i].second;
			ans.push_back(convert.str());
			break;
		}
	}
	ans = filter(ans, two, q);
	return ans;
}
vector<string> QE::Follows(string select, string one, int two, Query q) { //return the statement line that is before statement line two
	vector<pair<int, int>> folTable = pkb->getFollowTable()->getTable();
	vector<string> ans;
	ostringstream convert;
	for (int i = 0; i < folTable.size(); i++) {
		if (folTable[i].second == two) {
			convert << folTable[i].first;
			ans.push_back(convert.str());
			break;
		}
	}
	ans = filter(ans, one, q);
	return ans;
}

vector<string> QE::FollowsT(string select, string one, string two, Query q) { //return all the statement lines base on condition
	vector<pair<int, int>> folTable = pkb->getFollowTable()->getTable();
	vector<string> sub;
	vector<string> sub2;
	vector<string> ans;
	ostringstream convert;
	if (select.compare(one) == 0) {
		for (int i = 0; i < folTable.size(); ++i) {
			convert << folTable[i].first;
			sub.push_back(convert.str());
		}
		sub = filter(sub, one, q); // sub contains only the correct declaration of one
		for (int j = 0; j < folTable.size(); ++j) { //comparing sub and second of parTable to obtain follow statement line
			for (int k = 0; k < sub.size(); ++k) {
				int value = atoi(sub[k].c_str()); //int of sub
				if (folTable[j].first == value) {
					convert << folTable[j].second;
					sub2.push_back(convert.str());
				}
			}
		}
		sub2 = filter(sub2, two, q);
		for (int a = 0; a < folTable.size(); ++a) { //comparing sub2 and first of parTable to obtain follow statement line
			for (int b = 0; b < sub2.size(); ++b) {
				int value = atoi(sub2[b].c_str()); //int of sub2
				if (folTable[a].second == value) {
					convert << folTable[a].first;
					ans.push_back(convert.str());
				}
			}
		}
	}
	else {
		for (int i = 0; i < folTable.size(); ++i) {
			convert << folTable[i].second;
			sub.push_back(convert.str());
		}
		sub = filter(sub, two, q); // sub contains only the correct declaration of second
		for (int j = 0; j < folTable.size(); ++j) { //comparing sub and first of parTable to obtain follow statement line
			for (int k = 0; k < sub.size(); ++k) {
				int value = atoi(sub[k].c_str()); //int of sub
				if (folTable[j].second == value) {
					convert << folTable[j].first;
					sub2.push_back(convert.str());
				}
			}
		}
		sub2 = filter(sub2, one, q);
		for (int a = 0; a < folTable.size(); ++a) { //comparing sub2 and second of parTable to obtain follow statement line
			for (int b = 0; b < sub2.size(); ++b) {
				int value = atoi(sub2[b].c_str()); //int of sub
				if (folTable[a].first == value) {
					convert << folTable[a].second;
					ans.push_back(convert.str());
				}
			}
		}
	}
	return ans;
}
vector<string> QE::FollowsT(string select, int one, string two, Query q) { //return all the statement lines that follows* statement line one
	vector<pair<int, int>> folTable = pkb->getFollowTable()->getTable();
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
	ans = filter(ans, two, q);
	return ans;
}
vector<string> QE::FollowsT(string select, string one, int two, Query q) { //return all the statements lines that come before statement line two and in the sames nesting level
	vector<pair<int, int>> folTable = pkb->getFollowTable()->getTable();
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
	ans = filter(ans, one, q);
	return ans;
}

vector<string> QE::pattern(string select, string one, string two) { //return the statement lines that has this pattern
	vector<string> ans;
	return ans;
}

vector<string> QE::filter(vector<string> vec, string one, string two, Query q) {
	vector<string> filAns;
	stmtType type1 = q.checkSynType(one);
	stmtType type2 = q.checkSynType(two);
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
	return filAns;
}

vector<string> QE::filter(vector<string> vec, string field, Query q) {
	vector<string> filAns;
	stmtType type = q.checkSynType(field);
	unordered_map<int, LineToken> stmtTable = pkb->getStatementTable()->getTable();
	if (field.compare("_") == 0) {
		return vec;
	}
	else {
		for (int i = 0; i < vec.size(); ++i) {
			//obtain token of the statement number and compare with the type in two
			int value = atoi(vec[i].c_str());
			if (type == stmtTable[value].getType()) {
				// add it to final answer
				filAns.push_back(vec[i]);
			}
		}
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
	for (int i = 0; i < answers.size(); ++i) {
		cout << "Statement number is: " << answers.at(i) << endl;
	}
}