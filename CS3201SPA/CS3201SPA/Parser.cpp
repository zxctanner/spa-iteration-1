#include "Parser.h"


#include <iostream>
#include <regex>
#include <fstream>

using namespace std;

Parser::Parser()
{
}

int main() {


	std::ifstream file("code.txt");
	std::string file_line;

	std::regex open_bracket_rgx("(.*)\\{");

	std::regex close_bracket_rgx("(.*)\\}");

	std::regex procedure_rgx("Procedure ([a-zA-Z0-9]*)");

	std::regex assignment_rgx("([a-zA-Z0-9]*) = (.*);");

	std::regex while_rgx("while ([a-zA-Z0-9]*)");

	std::regex if_rgx("if ([a-zA-Z0-9]*) then");

	std::regex else_rgx("else");

	std::smatch match;

	int nesting = 0;

	while (std::getline(file, file_line)) {

		const std::string s = file_line.c_str();

		std::cout << "[Nesting]: " << nesting << ' ';

		if (std::regex_search(s.begin(), s.end(), match, procedure_rgx)) {
			std::cout << "[Procedure Name]: " << match[1] << '\n';
		}

		if (std::regex_search(s.begin(), s.end(), match, assignment_rgx)) {
			std::cout << "[LHS : RHS]: " << match[1] << " | " << match[2] << '\n';
		}

		if (std::regex_search(s.begin(), s.end(), match, while_rgx)) {
			std::cout << "[While Name]: " << match[1] << '\n';
		}

		if (std::regex_search(s.begin(), s.end(), match, if_rgx)) {
			std::cout << "[If Name]: " << match[1] << '\n';
		}

		if (std::regex_search(s.begin(), s.end(), match, else_rgx)) {

			std::cout << "[Else]: " << '\n';
		}

		// add the nesting changes at the back as changes to nesting happens after brackets

		if (std::regex_search(s.begin(), s.end(), match, open_bracket_rgx)) {
			nesting = nesting + 1;
		}

		if (std::regex_search(s.begin(), s.end(), match, close_bracket_rgx)) {
			nesting = nesting - 1;
		}

	}

	cin.get();

	return 0;
}

