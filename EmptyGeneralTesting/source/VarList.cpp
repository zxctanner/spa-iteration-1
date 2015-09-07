#include "VarList.h"

using namespace std;

VarList::VarList()
{
	varList = unordered_map<string, int>();
}

VarList::VarList(unordered_map<string, int> vl) {
	varList = vl;
}

VarList::~VarList()
{
}

unordered_map<string, int> VarList::getTable()
{
	return varList;
}


void VarList::setTable(unordered_map<string, int> vList)
{
	varList = vList;
}

bool VarList::isInList(string var) {
	auto search = varList.find(var);
	return search != varList.end();
}

void VarList::insert(string var) {
	int index = varList.size();
	if (!this->isInList(var)) {
		varList[var] = index;
	}
}

vector<string> VarList::getAllVar() {
	vector<string> allVar;
	for (auto it = varList.begin(); it != varList.end(); ++it) {
		allVar.push_back(it->first);
	}
	return allVar;
}