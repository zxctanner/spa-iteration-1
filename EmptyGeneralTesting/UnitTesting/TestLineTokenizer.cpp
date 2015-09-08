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
			string expected = "LineToken TYPE PROCEDURE NAME Second LEVEL0 LineNumber 0 RHS ,LineToken TYPE ASSIGN NAME x LEVEL1 LineNumber 1 RHS 0 ,LineToken TYPE ASSIGN NAME i LEVEL1 LineNumber 2 RHS 5 ,LineToken TYPE WHILE NAME i LEVEL1 LineNumber 3 RHS ,LineToken TYPE ASSIGN NAME x LEVEL2 LineNumber 4 RHS u 2 ,LineToken TYPE ASSIGN NAME i LEVEL2 LineNumber 5 RHS b 1 ,LineToken TYPE WHILE NAME p LEVEL2 LineNumber 6 RHS ,LineToken TYPE ASSIGN NAME x LEVEL3 LineNumber 7 RHS x 1 ,LineToken TYPE ASSIGN NAME z LEVEL1 LineNumber 8 RHS z 1 ,";
			//Sample tokens to be parsed in
			LineTokenizer tokenizer = LineTokenizer("..\\UnitTesting\\unitTestingCode.txt");
			tokenizer.tokenize();
			vector<LineToken> tokVect = tokenizer.getTokenVector();
			Assert::AreEqual(expected, tokenizer.toString());

			return;
		}

	};
}