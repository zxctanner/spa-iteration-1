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

		vector<pair<int, int>> populateFT(vector<pair<int, int>> ft) {
			ft.push_back({ 1,2 });
			ft.push_back({ 2,3 });
			ft.push_back({ 3,8 });
			ft.push_back({ 4,5 });
			ft.push_back({ 5,6 });
			return ft;
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
			mut->insert({ 3,{ vector<string>{}, vector<string>{"i"} } });
			mut->insert({ 4,{ vector<string>{"x"}, vector<string>{"u", "2"} } });
			mut->insert({ 5,{ vector<string>{"i"}, vector<string>{"b", "1"} } });
			mut->insert({ 6,{ vector<string>{}, vector<string>{"p"} } });
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
			ft = populateFT(ft);
			//populatePT(&pt);
			//populateST(&st);
			//populateMUT(&mut);
			//populateVL(&vl);
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

			Query q1 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"a", "Follows", "1", "a"});
			string q1e = "assign:a . Query fields:a Follows 1 a ";
			Assert::AreEqual(q1.toString(), q1e);
			Query q2 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{"w"}, vector<string>{},
				vector<string>{"w", "Follows", "w", "a"});
			cout << q1.toString() << endl;
			Query q3 = Query(vector<string>{"s1", "s2"}, vector<string>{}, vector<string>{},
				vector<string>{"w"}, vector<string>{}, vector<string>{},
				vector<string>{"w", "Follows", "s1", "s2"});
			cout << q1.toString() << endl;
			vector<Query> vQ{ q1, q2, q3 };

			string expected1 = "2";
			string expected2 = "3";
			string expected3 = "3 6";
			vector<string> expected{ expected1, expected2, expected3 };
			QE* qe;
			qe = new QE(vQ, pkb);
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
				vector<string>{"a", "Follows", "a", "1"});
			Query q2 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{"w"}, vector<string>{},
				vector<string>{"w", "Follows", "w", "a"});
			Query q3 = Query(vector<string>{"s1", "s2"}, vector<string>{}, vector<string>{},
				vector<string>{"w"}, vector<string>{}, vector<string>{},
				vector<string>{"w", "Follows", "s1", "s2"});
			vector<Query> vQ{ q1, q2, q3 };
			QE* qe;
			qe = new QE(vQ, pkb);
		}

		TEST_METHOD(TestParent)
		{
			PKB* pkb = new PKB();
			populateAllTables(pkb);
			Query q1 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"a", "Follows", "a", "1"});
			Query q2 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{"w"}, vector<string>{},
				vector<string>{"w", "Follows", "w", "a"});
			Query q3 = Query(vector<string>{"s1", "s2"}, vector<string>{}, vector<string>{},
				vector<string>{"w"}, vector<string>{}, vector<string>{},
				vector<string>{"w", "Follows", "s1", "s2"});
			vector<Query> vQ{ q1, q2, q3 };
			QE* qe;
			qe = new QE(vQ, pkb);
		}

		TEST_METHOD(TestParentT)
		{
			PKB* pkb = new PKB();
			populateAllTables(pkb);
			Query q1 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"a", "Follows", "a", "1"});
			Query q2 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{"w"}, vector<string>{},
				vector<string>{"w", "Follows", "w", "a"});
			Query q3 = Query(vector<string>{"s1", "s2"}, vector<string>{}, vector<string>{},
				vector<string>{"w"}, vector<string>{}, vector<string>{},
				vector<string>{"w", "Follows", "s1", "s2"});
			vector<Query> vQ{ q1, q2, q3 };
			QE* qe = new QE(vQ, pkb);
		}

		TEST_METHOD(TestPattern)
		{
			PKB* pkb = new PKB();
			populateAllTables(pkb);
			Query q1 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"a", "Follows", "a", "1"});
			Query q2 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{"w"}, vector<string>{},
				vector<string>{"w", "Follows", "w", "a"});
			Query q3 = Query(vector<string>{"s1", "s2"}, vector<string>{}, vector<string>{},
				vector<string>{"w"}, vector<string>{}, vector<string>{},
				vector<string>{"w", "Follows", "s1", "s2"});
			vector<Query> vQ{ q1, q2, q3 };
			QE* qe;
			qe = new QE(vQ, pkb);
		}

		TEST_METHOD(TestModifies)
		{
			PKB* pkb = new PKB();
			populateAllTables(pkb);
			Query q1 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"a", "Follows", "a", "1"});
			Query q2 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{"w"}, vector<string>{},
				vector<string>{"w", "Follows", "w", "a"});
			Query q3 = Query(vector<string>{"s1", "s2"}, vector<string>{}, vector<string>{},
				vector<string>{"w"}, vector<string>{}, vector<string>{},
				vector<string>{"w", "Follows", "s1", "s2"});
			vector<Query> vQ{ q1, q2, q3 };
			QE* qe;
			qe = new QE(vQ, pkb);
		}

		TEST_METHOD(TestUses)
		{
			PKB* pkb = new PKB();
			populateAllTables(pkb);
			Query q1 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{}, vector<string>{},
				vector<string>{"a", "Follows", "a", "1"});
			Query q2 = Query(vector<string>{}, vector<string>{"a"}, vector<string>{},
				vector<string>{}, vector<string>{"w"}, vector<string>{},
				vector<string>{"w", "Follows", "w", "a"});
			Query q3 = Query(vector<string>{"s1", "s2"}, vector<string>{}, vector<string>{},
				vector<string>{"w"}, vector<string>{}, vector<string>{},
				vector<string>{"w", "Follows", "s1", "s2"});
			vector<Query> vQ{ q1, q2, q3 };
			QE* qe;
			qe = new QE(vQ, pkb);
		}

	};

}