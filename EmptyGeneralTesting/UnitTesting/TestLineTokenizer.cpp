#include "stdafx.h"
#include "CppUnitTest.h"
#include "LineTokenizer.h"
#include "LineToken.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestLineTokenizer
{
	TEST_CLASS(TestLineTokenizer)
	{
	public:

		TEST_METHOD(TestingTonkenize)
		{
			stringstream buffer;
			int counter = 0;
			string expectedTokens[] = { "LineToken TYPE ASSIGN NAME x LEVEL0 LineNumber 0 RHS 1 y 2 " };
			//Sample tokens to be parsed in
			LineTokenizer tokenizer = LineTokenizer("..\\UnitTesting\\unitTestingCode.txt");
			tokenizer.tokenize();
			vector<LineToken> tokVect = tokenizer.getTokenVector();
			//Assert::AreEqual(1, (int)tokVect.size());

			for (auto iter = tokVect.begin(); iter != tokVect.end(); ++iter) {
				buffer << *iter;
				Assert::AreEqual(expectedTokens[counter], buffer.str());
				counter++;
			}

			return;
			// TODO: Your test code here
		}

	};
}