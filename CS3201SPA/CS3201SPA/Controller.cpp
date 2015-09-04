#include "LineTokenizer.h"
#include "PKB.h"
#include "QP.h"

using namespace std;


void main() {
	PKB pkb;
	QP qp;

	//Controller calls tokenizer to token up SIMPLE.txt
	LineTokenizer tokenizer = LineTokenizer("SIMPLE.txt");

	//After tokenizing, the tokens are passed to PKB to consturct tables
	//After constructed table will be found in PKB
	pkb.constructAllTables(tokenizer.getTokenVector());
	
	//Controller then calls QueryParser to analyse query and produce answers from the PKB tables
	//Hence pkb is passed into QueryParse so that table can be accessed by them
	/*
	
	qp.startQP("QUERY.txt", pkb);

	*/
}