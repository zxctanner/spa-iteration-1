#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "QE.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQE)
	{
	public:

		PKB* pkb = new PKB();

		void populateFT(vector<pair<int, int>> ft) {
			ft.push_back({ 1,2 });
			ft.push_back({ 2,3 });
			ft.push_back({ 3,8 });
			ft.push_back({ 4,5 });
			ft.push_back({ 5,6 });
		}
		void populatePT(vector<pair<int, int>> pt) {
			pt.push_back({ 3,4 });
			pt.push_back({ 3,5 });
			pt.push_back({ 3,6 });
			pt.push_back({ 6,7 });
		}
		void populateMUT(unordered_map<int, pair<vector<string>, vector<string>>> mut) {
			mut[1] = { vector<string>{"x"}, vector<string>{"0"} };
			mut[2] = { vector<string>{"i"}, vector<string>{"5"} };
			mut[3] = { vector<string>{}, vector<string>{"i"} };
			mut[4] = { vector<string>{"x"}, vector<string>{"u", "2"} };
			mut[5] = { vector<string>{"i"}, vector<string>{"b", "1"} };
			mut[6] = { vector<string>{}, vector<string>{"p"} };
			mut[7] = { vector<string>{"x"}, vector<string>{"x", "1"} };
			mut[8] = { vector<string>{"z"}, vector<string>{"z", "1"} };
		}
		void populateST(unordered_map<int, LineToken> st) {
			st[0] = LineToken("PROCEDURE", "Second", 0, 0);
			st[1] = LineToken("ASSIGN", "x", 1, 1, vector<string>{"0"});
			st[2] = LineToken("ASSIGN", "x", 1, 7, vector<string>{"5"});
			st[3] = LineToken("WHILE", "i", 1, 2);
			st[4] = LineToken("ASSIGN", "x", 2, 3, vector<string>{ "u", "2" });
			st[5] = LineToken("ASSIGN", "i", 2, 4, vector<string>{ "b", "1" });
			st[6] = LineToken("WHILE", "p", 2, 5);
			st[7] = LineToken("ASSIGN", "x", 3, 6, vector<string>{ "x", "1" });
			st[7] = LineToken("ASSIGN", "z", 1, 7, vector<string>{ "z", "1" });
		}
		void populateVL(unordered_map<string, int> vl) {
			vl["x"] = 0;
			vl["i"] = 1;
			vl["u"] = 2;
			vl["b"] = 3;
			vl["p"] = 3;
			vl["z"] = 3;
		}
		void populateAllTables(PKB* pkb) {
			vector<pair<int, int>> ft;
			vector<pair<int, int>> pt;
			unordered_map<int, pair<vector<string>, vector<string>>> mut;
			unordered_map<int, LineToken> st;
			unordered_map<string, int> vl;
			populateFT(ft);
			populatePT(pt);
			populateST(st);
			populateMUT(mut);
			populateVL(vl);
			pkb->setParentTable(new ParentTable(pt));
			pkb->setStatementTable(new StatementTable(st));
			pkb->setModUseTable(new ModUseTable(mut));
			pkb->setVarList(new VarList(vl));
		}

		TEST_METHOD(TestFollows)
		{
			populateAllTables(pkb);
		}

		TEST_METHOD(TestFollowsT)
		{
			populateAllTables(pkb);
		}

		TEST_METHOD(TestParent)
		{
			populateAllTables(pkb);
		}

		TEST_METHOD(TestParentT)
		{
			populateAllTables(pkb);
		}

		TEST_METHOD(TestPattern)
		{
			populateAllTables(pkb);
		}

		TEST_METHOD(TestModifies)
		{
			populateAllTables(pkb);
		}

		TEST_METHOD(TestUses)
		{
			populateAllTables(pkb);
		}

	};

}