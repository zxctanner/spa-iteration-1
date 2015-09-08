#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestLineTokenizer
{
	TEST_CLASS(TestParser)
	{
	public:

		TEST_METHOD(TestingParser)
		{
			PKB* pkb = new PKB();
			Parser parser("..\\UnitTesting\\unitTestingCode.txt", pkb);

			vector<pair<int, int>> results = pkb->getParentTable()->getTable();
			vector<pair<int, int>> expected;
			pair<int, int> buffer;
			expected.push_back(pair<int, int>(3, 4));
			expected.push_back(pair<int, int>(3, 5));
			expected.push_back(pair<int, int>(3, 6));
			expected.push_back(pair<int, int>(6, 7));

			int i;
			for (i = 0; i < results.size(); i++) {
				Assert::AreEqual(expected.at(i).first, results.at(i).first);
				Assert::AreEqual(expected.at(i).second, results.at(i).second);
			}

			return;
			// TODO: Your test code here
		}

	};
}