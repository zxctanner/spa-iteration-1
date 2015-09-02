#include "LineToken.h"
#include "ParentTable.h"
#include "ModUseTable.h"
#include "StatementTable.h"
#include <unordered_map>
#include <stack>

using namespace std;

class Parser {
public:

	StatementTable st;
	ModUseTable mut;
	ParentTable pt;

	void runParser(vector<LineToken> tokens,vector<pair<int,int>>* parentTable, unordered_map<int,pair<string, string>>* modUseTable,
		unordered_map<int, LineToken*>* statementTable, vector<pair<int,int>>* followTable);
};

void Parser::runParser(vector<LineToken> tokens, vector<pair<int, int>>* parentTable, unordered_map<int, pair<string, string>>* modUseTable, unordered_map<int, LineToken*>* statementTable, vector<pair<int, int>>* followTable)
{
	vector<LineToken>::iterator it;
	bool isWhile = false;
	stack<LineToken> nestedStack;

	for (it = tokens.begin(); it != tokens.end(); ++it) {
		
		//populating stmtNumber Table
		st.insert(it->getStmtNumber(), *it);

		//populating ModifyUsedTable when assign statements is dedected
		if(it->getType()==ASSIGN) {
			mut.insert(it->getStmtNumber(), it->getName(), it->getExpr());
		}

		//populates ParentTable
		if (isWhile) {
			//current statement is still within the current nesting level
			if (nestedStack.top().getLevel() == 1 + it->getLevel()) {
				pt.addEntry(nestedStack.top().getStmtNumber(), it->getStmtNumber());
			}
			else //current statement goes back to same nesting level as "while" or even lower
			{
				//pop out all while loops that has ended
				while (nestedStack.top().getLevel() >= it->getLevel() || nestedStack.empty()) {
					nestedStack.pop();
				}
				//if stack is still not empty, current statement is a child of the while loop in the stack
				if (!nestedStack.empty()) {
					pt.addEntry(nestedStack.top().getStmtNumber(), it->getStmtNumber());
				}
				else //nestedStack is empty
					isWhile = false;
			}
		}

		if (it->getType() == WHILE) {
			isWhile = true;
			nestedStack.push(*it);
		}

		//before ending parser, return all tables
		*parentTable = pt.getParentTable();
		// *modUseTable = mut.getTable();                
		// *statementTable = st.getTable();
		// *followTable = ft.getTable();

		/*
		//THIS SWITCH CASE IS USE FOR MAKING AST
		switch (it->getType()) {
			case PROCEDURE:
				//do prodcedure stuff
				//create procedure node followed by stmtList node below
				break;
			case ASSIGN:
				//do assign stuff
				//create assign node followed by left variable node and right expression node
				break;
			case WHILE:
				//do while stuff
				//create while node followed by left vraiable and right stmtList
				break;
		} */
	}
}
