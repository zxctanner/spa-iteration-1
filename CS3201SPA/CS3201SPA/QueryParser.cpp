#include "QueryParser.h"
#include <string>
using namespace std;



QueryParser::QueryParser() {
}

QueryParser::ParseInput(String input){
	String[] parts = input.split(" ");
	int num = parts.length;
}

QueryParser::ParseField(String field) {
	String[] split1 = field.split("(");
	String command = split1[0];
	String[] split2 = split1[1].split(",");
	String first = split2[0].erase(remove_if(split2[0].begin(), split2[0].end(), isspace), split2[0].end());
	String second = split2[1].erase(second.end());
	second = second.erase(remove_if(second.begin(), second.end(), isspace), second.end());

	if (command.compare("Modifies") == 0) {
		call QueryEvaluator::Modifies(first, second);
	}
	else if (command.compare("Uses") == 0) {
		call QueryEvaluator::Uses(first, second);
	}
	else if (command.compare("Parent") == 0) {
		call QueryEvaluator::Parent(first, second);
	}
	else if (command.compare("Parent*") == 0) {
		call QueryEvaluator::ParentS(first, second);
	}
	else if (command.compare("Follows") == 0) {
		call QueryEvaluator::Follows(first, second);
	}
	else if (command.compare("FollowsS") == 0) {
		call QueryEvaluator::FollowsS(first, second);
	}
	else if (command.compare("a") == 0) {
		call QueryEvaluator::Pattern(first, second);
	}
	else {
		cout << ("Invalid input");
	}