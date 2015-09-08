#include "stdafx.h"
#include "CppUnitTest.h"
#include "QP.h"
#include "Query.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQP)
	{

	public:
		
		TEST_METHOD(testProcess)
		{
			string fileName = "..\\UnitTesting\\testquery.txt";
			QP qp(fileName);
			qp.process();
			std::vector<Query> queryVector = qp.getVectorQuery();

			//Expected query strings
			string expected1 = "assign:a . Query fields:a Modifies a \"x\" ";
			string expected2 = "assign:a while:w . Query fields:w Parent w a ";
			string expected3 = "while:w1 while:w2 . Query fields:w1 Follows* w1 w2 "; //doesnt work
			string expected4 = "assign:a while:w . Query fields:a Parent* w a "; //doesnt work
			string expected5 = "assign:a . Query fields:a patterna _ _ ";
			string expected6 = "assign:a . Query fields:a patterna \"x\" _\"x\"_ ";
			string expected7 = "assign:a . Query fields:a patterna _ _\"z\"_ ";
			string expected8 = "assign:a . Query fields:a patterna _ _\"1\"_ ";
			vector<string> expected{expected1, expected2, expected3, expected4, expected5, expected6, expected7, expected8};
			
			//check if the queryVector is produced correctly
			for (int i = 0; i < expected.size(); ++i) {
				Assert::AreEqual(expected[i], queryVector[i].toString());
			}
		}

	};
}