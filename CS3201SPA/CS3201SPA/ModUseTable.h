#pragma once
class ModUseTable
{
public:
	ModUseTable();
	void insert(int stmtNum, string modified, string used);
	string getLHS(int stmtNum);
	string getRHS(int stmtNum);
};

