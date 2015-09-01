#pragma once
class StatementTable
{
public:
	StatementTable();
	void insert(int stmtNum, LineToken);
	LineToken getStmt(int stmtNum);
};

