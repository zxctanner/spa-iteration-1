#include "LineTokenizer.h"
#include "LineToken.h"
#include <string>
#include <vector>
#include <regex>

using namespace std;


LineTokenizer::LineTokenizer()
{

}

LineTokenizer::LineTokenizer(string fileName)
{
	filePath = fileName;
}


LineTokenizer::~LineTokenizer()
{
}

vector<LineToken> LineTokenizer::getTokenVector() {
	return tokVec;
}

void LineTokenizer::tokenize()
{
	ifstream inFile(filePath);
	if (!inFile) {
		cerr << "File does not exist!" << endl;
		return;
	}

	regex procedureRgx("Procedure\\s+([a-zA-Z][a-zA-Z0-9]*)");
	regex assignmentRgx("([a-zA-Z][a-zA-Z0-9]*)\\s+=\\s+(.*);");
	regex whileRgx("while\\s+([a-zA-Z][a-zA-Z0-9]*)");
	regex openBracketRgx("\\{");
	regex closeBracketRgx("\\}");

	smatch match;
	string line;
	int nestingLevel = 0;
	int lineNumber = 1;
	while (getline(inFile, line)) {
		if (regex_search(line, match, procedureRgx)) {
			string procName = match[1];
			cout << "[Procedure Name]: " << procName << endl;
			tokVec.push_back(LineToken(PROCEDURE, procName, nestingLevel, lineNumber));
		}

		if (regex_search(line, match, assignmentRgx)) {
			string varName = match[1];
			string rhsexpr = match[2];
			cout << "[Variable Name]: " << varName << endl;
			cout << "[RHS Exprssion]: " << rhsexpr << endl;
			tokVec.push_back(LineToken(ASSIGN, varName, nestingLevel, lineNumber, rhsexpr));
		}

		if (regex_search(line, match, whileRgx)) {
			string var = match[1];
			cout << "[While Variable Name]: " << var << endl;
			tokVec.push_back(LineToken(WHILE, var, nestingLevel, lineNumber));
		}

		if (regex_search(line, openBracketRgx)) {
			//To deal with the case of multiple "{" at the same line, so we need to count them
			int match_count = distance(sregex_iterator(line.begin(), line.end(), openBracketRgx), sregex_iterator());
			nestingLevel += match_count;
		}
		
		if (regex_search(line, closeBracketRgx)) {
			int match_count = distance(sregex_iterator(line.begin(), line.end(), openBracketRgx), sregex_iterator());
			nestingLevel -= match_count;
			if (nestingLevel < 0) {
				cerr << "Nesting Level Decreased to below zero." << endl;
			}
		}
		lineNumber++;
	}

}



