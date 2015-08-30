#include "SimpleParser.h"


#include <iostream>
#include <regex>
#include <fstream>

using namespace std;

SimpleParser::SimpleParser()
{
}

class SimpleStructure {
public:
	string type = "";
	string procedureName = "";
	string ifVariable = "";
	int nestingLevel = 0;
	string assignmentLeft = "";
	string assignmentRight = "";
	string whileVariable = "";
};

int main() {

	std::ifstream file("code.txt");
	std::string file_line;

	std::regex openBracketRgx("(.*)\\{");

	std::regex closeBacketRgx("(.*)\\}");

	std::regex procedureRgx("Procedure ([a-zA-Z0-9]*)");

	std::regex assignmentRgx("([a-zA-Z0-9]*) = (.*);");

	std::regex whileRgx("while ([a-zA-Z0-9]*)");

	std::regex ifRgx("if ([a-zA-Z0-9]*) then");

	std::regex elseRgx("else");

	std::smatch match;

	int nesting = 0;

	SimpleStructure mySimpleStructure;

	while (std::getline(file, file_line)) {

		const std::string s = file_line.c_str();

		std::cout << "[Nesting]: " << nesting << ' ';
		mySimpleStructure.nestingLevel = nesting;

		if (std::regex_search(s.begin(), s.end(), match, procedureRgx)) {
			// type = procecure
			std::cout << "[Procedure Name]: " << match[1] << '\n';
			mySimpleStructure.procedureName = match[1];
			mySimpleStructure.type = "procedure";
		}

		if (std::regex_search(s.begin(), s.end(), match, assignmentRgx)) {
			std::cout << "[LHS : RHS]: " << match[1] << " | " << match[2] << '\n';
			mySimpleStructure.type = "assigment";
			mySimpleStructure.assignmentLeft = match[1];
			mySimpleStructure.assignmentRight = match[2];
		}

		if (std::regex_search(s.begin(), s.end(), match, whileRgx)) {
			std::cout << "[While Name]: " << match[1] << '\n';
			mySimpleStructure.type = "while";
			mySimpleStructure.whileVariable = match[1];
		}

		if (std::regex_search(s.begin(), s.end(), match, ifRgx)) {
			std::cout << "[If Name]: " << match[1] << '\n';
			mySimpleStructure.type = "if";
			mySimpleStructure.ifVariable = match[1];
		}

		if (std::regex_search(s.begin(), s.end(), match, elseRgx)) {

			std::cout << "[Else]: " << '\n';
			mySimpleStructure.type = "else";
		}

		// add the nesting changes at the back as changes to nesting happens after brackets

		if (std::regex_search(s.begin(), s.end(), match, openBracketRgx)) {
			nesting = nesting + 1;
		}

		if (std::regex_search(s.begin(), s.end(), match, closeBacketRgx)) {
			nesting = nesting - 1;
		}

	}

	cin.get();

	return 0;
}

