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

		void populateFT(vector<pair<int, int>> *ft) {
			ft->push_back({ 1,2 });
			ft->push_back({ 2,3 });
			ft->push_back({ 3,8 });
			ft->push_back({ 4,5 });
			ft->push_back({ 5,6 });
		}
		void populatePT(vector<pair<int, int>> *pt) {
			pt->push_back({ 3,4 });
			pt->push_back({ 3,5 });
			pt->push_back({ 3,6 });
			pt->push_back({ 6,7 });
		}
		void populateMUT(unordered_map<int, pair<vector<string>, vector<string>>> *mut) {
			mut->insert({ 1,{ vector<string>{"x"}, vector<string>{"0"} } });
			mut->insert({ 2,{ vector<string>{"i"}, vector<string>{"5"} } });
			mut->insert({ 3,{ vector<string>{"x", "i"}, vector<string>{"i", "u", "2", "b", "1", "p", "x"} } });
			mut->insert({ 4,{ vector<string>{"x"}, vector<string>{"u", "2"} } });
			mut->insert({ 5,{ vector<string>{"i"}, vector<string>{"b", "1"} } });
			mut->insert({ 6,{ vector<string>{"x"}, vector<string>{"p", "x", "1"} } });
			mut->insert({ 7,{ vector<string>{"x"}, vector<string>{"x", "1"} } });
			mut->insert({ 8,{ vector<string>{"z"}, vector<string>{"z", "1"} } });
		}
		void populateST(unordered_map<int, LineToken> *st) {
			st->insert({ 0, LineToken("PROCEDURE", "Second", 0, 0) });
			st->insert({ 1, LineToken("ASSIGN", "x", 1, 1, vector<string>{"0"}) });
			st->insert({ 2, LineToken("ASSIGN", "x", 1, 7, vector<string>{"5"}) });
			st->insert({ 3, LineToken("WHILE", "i", 1, 2) });
			st->insert({ 4, LineToken("ASSIGN", "x", 2, 3, vector<string>{ "u", "2" }) });
			st->insert({ 5, LineToken("ASSIGN", "i", 2, 4, vector<string>{ "b", "1" }) });
			st->insert({ 6, LineToken("WHILE", "p", 2, 5) });
			st->insert({ 7, LineToken("ASSIGN", "x", 3, 6, vector<string>{ "x", "1" }) });
			st->insert({ 8, LineToken("ASSIGN", "z", 1, 7, vector<string>{ "z", "1" }) });
		}
		void populateVL(unordered_map<string, int> *vl) {
			vl->insert({ "x", 0 });
			vl->insert({ "i", 1 });
			vl->insert({ "u", 2 });
			vl->insert({ "b", 3 });
			vl->insert({ "p", 4 });
			vl->insert({ "z", 5 });
		}
		void populateAllTables(PKB* pkb) {
			vector<pair<int, int>> ft;
			vector<pair<int, int>> pt;
			unordered_map<int, pair<vector<string>, vector<string>>> mut;
			unordered_map<int, LineToken> st;
			unordered_map<string, int> vl;
			populateFT(&ft);
			populatePT(&pt);
			populateST(&st);
			populateMUT(&mut);
			populateVL(&vl);
			pkb->setFollowTable(new FollowTable(ft));
			pkb->setParentTable(new ParentTable(pt));
			pkb->setStatementTable(new StatementTable(st));
			pkb->setModUseTable(new ModUseTable(mut));
			pkb->setVarList(new VarList(vl));
		}

		TEST_METHOD(TestFollows)
		{
			PKB* pkb = new PKB();
			populateAllTables(pkb);

			//int size = pkb->getFollowTable()->getTable().size();
			//Assert::AreEqual(4, size);

			Query q1 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"a", "Follows", "1", "a"});
			Query q2 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{"w"}, vector<string>{},
				vector<string>{"w", "Follows", "w", "a"});
			Query q3 = Query(vector<string>{"s1", "s2"}, vector<string>{}, vector<string>{},
				vector<string>{"w"}, vector<string>{}, vector<string>{},
				vector<string>{"w", "Follows", "s1", "s2"});
			vector<Query> vQ{ q1, q2, q3 };

			string expected1 = "2";
			string expected2 = "3";
			string expected3 = "3, 6";
			vector<string> expected{ expected1, expected2, expected3 };
			
			QE* qe;
			qe = new QE(vQ, pkb); // error occurs here
			
			vector<string> Ans = qe->getAnswers();
			for (int i = 0; i < vQ.size(); ++i) {
				Assert::AreEqual(expected[i], Ans[i]);
			}
		}

		TEST_METHOD(TestFollowsT)
		{
			PKB* pkb = new PKB();
			populateAllTables(pkb);
			Query q1 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"a", "Follows*", "a", "1"});
			Query q2 = Query(vector<string>{}, vector<string>{"a1","a2"}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"a1", "Follows*", "a1", "a2"});
			Query q3 = Query(vector<string>{"s1", "s2"}, vector<string>{}, vector<string>{},
				vector<string>{"w"}, vector<string>{}, vector<string>{},
				vector<string>{"w", "Follows*", "1", "8"});
			vector<Query> vQ{ q1, q2, q3 };

			//int size = pkb->getFollowTable()->getTable().size();
			//Assert::AreEqual(4, size);

			string expected1 = "none";
			string expected2 = "1, 2, 4";
			string expected3 = "3, 6";
			vector<string> expected{ expected1, expected2, expected3 };

			QE* qe;
			qe = new QE(vQ, pkb); //error occurs here
			vector<string> Ans = qe->getAnswers();
			for (int i = 0; i < vQ.size(); ++i) {
				Assert::AreEqual(expected[i], Ans[i]);
			}
		}

		TEST_METHOD(TestParent)
		{
			PKB* pkb = new PKB();
			populateAllTables(pkb);

			Query q1 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"a", "Parent", "3", "a"});
			Query q2 = Query(vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{}, vector<string>{"w1", "w2"}, vector<string>{},
				vector<string>{"w1", "Parent", "w1", "w2"});
			Query q3 = Query(vector<string>{"s"}, vector<string>{}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"s", "Parent", "_", "s"});
			vector<Query> vQ{ q1, q2, q3 };

			string expected1 = "4, 5";
			string expected2 = "3";
			string expected3 = "4, 5, 6, 7";
			vector<string> expected{ expected1, expected2, expected3 };

			QE* qe;
			qe = new QE(vQ, pkb);
			vector<string> Ans = qe->getAnswers();
			for (int i = 0; i < vQ.size(); ++i) {
				Assert::AreEqual(expected[i], Ans[i]);
			}
		}

		TEST_METHOD(TestParentT)
		{
			PKB* pkb = new PKB();
			populateAllTables(pkb);
			Query q1 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"a", "Parent*", "1", "a"});
			Query q2 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{"w"}, vector<string>{},
				vector<string>{"a", "Parent*", "w", "a"});
			Query q3 = Query(vector<string>{"s1"}, vector<string>{}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"s1", "Parent*", "s1", "7"});
			vector<Query> vQ{ q1, q2, q3 };

			string expected1 = "none";
			string expected2 = "4, 5, 7";
			string expected3 = "6, 3";
			vector<string> expected{ expected1, expected2, expected3 };

			QE* qe;
			qe = new QE(vQ, pkb);
			vector<string> Ans = qe->getAnswers();
			for (int i = 0; i < vQ.size(); ++i) {
				Assert::AreEqual(expected[i], Ans[i]);
			}
		}

		TEST_METHOD(TestPattern)
		{
			PKB* pkb = new PKB();
			populateAllTables(pkb);
			Query q1 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{"v"},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"a", "patterna", "v", "_\"1\"_"});
			Query q2 = Query(vector<string>{}, vector<string>{"b"}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"b", "patternb", "\"x\"", "_"});
			Query q3 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{"w"}, vector<string>{},
				vector<string>{"w", "patterna", "_", "_"});
			vector<Query> vQ{ q1, q2, q3 };

			string expected1 = "5, 7, 8";
			string expected2 = "1, 4, 7";
			string expected3 = "3, 6";
			vector<string> expected{ expected1, expected2, expected3 };

			QE* qe;
			qe = new QE(vQ, pkb);
			vector<string> Ans = qe->getAnswers();
			for (int i = 0; i < vQ.size(); ++i) {
				Assert::AreEqual(expected[i], Ans[i]);
			}
		}

		TEST_METHOD(TestModifies)
		{
			PKB* pkb = new PKB();
			populateAllTables(pkb);
			Query q1 = Query(vector<string>{"s"}, vector<string>{}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"s", "Modifies", "s", "\"x\""});
			Query q2 = Query(vector<string>{}, vector<string>{}, vector<string>{"v"},
				vector<string>{}, vector<string>{"w"}, vector<string>{},
				vector<string>{"v", "Modifies", "w", "v"});
			Query q3 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{"w"}, vector<string>{},
				vector<string>{"a", "Modifies", "a", "\"i\""});
			vector<Query> vQ{ q1, q2, q3 };

			string expected1 = "1, 3, 4, 6, 7";
			string expected2 = "x, i";
			string expected3 = "2, 5";
			vector<string> expected{ expected1, expected2, expected3 };

			QE* qe;
			qe = new QE(vQ, pkb);
			vector<string> Ans = qe->getAnswers();
			for (int i = 0; i < vQ.size(); ++i) {
				Assert::AreEqual(expected[i], Ans[i]);
			}
		}

		TEST_METHOD(TestUses)
		{
			PKB* pkb = new PKB();
			populateAllTables(pkb);
			Query q1 = Query(vector<string>{"s"}, vector<string>{}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"s", "Uses", "s", "\"b\""});
			Query q2 = Query(vector<string>{}, vector<string>{}, vector<string>{"v"},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"v", "Uses", "3", "v"});
			Query q3 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{"v"},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"v", "Uses", "a", "v"});
			vector<Query> vQ{ q1, q2, q3 };

			string expected1 = "3, 5";
			string expected2 = "i, u, b, p, x";
			string expected3 = "u, b, x, z";
			vector<string> expected{ expected1, expected2, expected3 };

			//int size = pkb->getFollowTable()->getTable().size();
			//Assert::AreEqual(4, size);

			QE* qe;

			qe = new QE(vQ, pkb); //error occurs here
			vector<string> Ans = qe->getAnswers();
			for (int i = 0; i < vQ.size(); ++i) {
				Assert::AreEqual(expected[i], Ans[i]);
			}
		}
	};
}