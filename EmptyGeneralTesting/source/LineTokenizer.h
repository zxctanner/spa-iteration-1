#pragma once
#include "LineToken.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
class LineTokenizer
{
private:
	string filePath;
	vector<LineToken> tokVec;

public:
	LineTokenizer();
	LineTokenizer(string fileName);
	~LineTokenizer();
	vector<LineToken> getTokenVector();
	void tokenize();
	void printTokenVector();
};


