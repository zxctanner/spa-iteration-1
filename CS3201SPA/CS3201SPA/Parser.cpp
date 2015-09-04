#include "LineToken.h"
#include "LineTokenizer.h"
#include "ParentTable.h"
#include "ModUseTable.h"
#include "StatementTable.h"
#include "FollowTable.h"
#include "Parser.h"
#include <unordered_map>
#include <stack>
#include <vector>


using namespace std;

/*
void Parser::runParser(vector<LineToken> tokens, vector<pair<int, int>>* parentTable, unordered_map<int, pair<string, vector<string>>>* modUseTable, unordered_map<int, LineToken*>* statementTable, vector<pair<int, int>>* followTable)
{
	vector<LineToken>::iterator it;
	bool isWhile = false;
	stack<LineToken> nestedStack;

	int *nesting = new int[tokens.size()];

	int counter = 0;

	for (it = tokens.begin(); it != tokens.end(); ++it) {
		nesting[counter] = it->getLevel();
		counter++;
	}

	//create followtable array here

	// pass in a nesting table
	// int nesting[] = { 0,1,2,3,3,2,0 };

	// count the size of the nesting array;
	int countSize = sizeof(nesting) / sizeof(*nesting);

	// n for outer loop, n for inner loop
	int n;

	int m;

	for (n = 0; n < countSize - 1; n++) {

		// get the current nesting level
		int currentNesting = nesting[n];

		for (m = n + 1; m < countSize; m++) {

			int nextNesting = nesting[m];

			// if next nesting level drops below current level, no need to proceed
			if (nextNesting < currentNesting) {
				break;
			}

			// if same level means follow
			if (nextNesting == currentNesting) {
				//std::cout << (n + 1) << (m + 1) << "\n";

				ft.addEntry(n + 1, m + 1);
				break;
			}

		}
	}


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
		*modUseTable = mut.getModUseTable();                
		*statementTable = st.getStmtTable();
		*followTable = ft.getFollowTable();

		
	}
}
*/
Parser::Parser()
{
}

Parser::Parser(string fileName, PKB * p)
{
	pkb = p;
	LineTokenizer tokenizer = LineTokenizer(fileName);
	tokens = tokenizer.getTokenVector();
}

Parser::~Parser()
{
}

void Parser::populateFollowTable()
{
	vector<pair<int,int>>* ft = new vector<pair<int, int>>();
	
	bool isWhile = false;
	stack<LineToken> nestedStack;

	int *nesting = new int[tokens.size()];

	int counter = 0;

	for (auto it = tokens.begin(); it != tokens.end(); ++it) {
		nesting[counter] = it->getLevel();
		counter++;
	}

	//create followtable array here

	// pass in a nesting table
	// int nesting[] = { 0,1,2,3,3,2,0 };

	// count the size of the nesting array;
	int countSize = sizeof(nesting) / sizeof(*nesting);

	// n for outer loop, n for inner loop
	int n, m;
	for (n = 0; n < countSize - 1; n++) {
		// get the current nesting level
		int currentNesting = nesting[n];
		for (m = n + 1; m < countSize; m++) {
			int nextNesting = nesting[m];

			// if next nesting level drops below current level, no need to proceed
			if (nextNesting < currentNesting) {
				break;
			}
			// if same level means follow
			if (nextNesting == currentNesting) {
				//std::cout << (n + 1) << (m + 1) << "\n";
				pair<int, int> entry(n+1, m+1);
				ft->push_back(entry);
				break;
			}
		}
	}
	pkb->setFollowTable(new FollowTable(ft));
}

void Parser::populateParentTable()
{
}

void Parser::populateStatamentTable()
{
}

void Parser::populateModUseTable()
{
}
