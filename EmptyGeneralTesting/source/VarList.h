#ifndef VarList_H
#define VarList_H

#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

class VarList
{
private:
	unordered_map<string, int> varList;
public:
	VarList();
	VarList(unordered_map<string, int> vl);
	~VarList();
	unordered_map<string, int> getTable();
	void setTable(unordered_map<string, int> vList);
	bool isInList(string var);
	//void insert(string var);
	vector<string> getAllVar();
	vector<pair<string, int>> getAllPair();
};

#endif
