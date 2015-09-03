#pragma once
class StatementTable
{
public:
	unordered_map<int, LineToken*> getStmtTable();
	void insert(int stmtNum, LineToken);
	LineToken getStmt(int stmtNum);
};

