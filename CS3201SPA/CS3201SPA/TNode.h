#pragma once

class TNode {
public:
	void setName(string name);
	string getName();
	void setNodeType(string nodeType);
	string getNodeType();
	void setStmtNum(int stmtNum);
	int getStmtNum();
	void setParent(TNode node);
	TNode getParent();
	void setLeftSibling(TNode node);
	TNode getLeftSibling();
	void setRightSibling(TNode node);
	TNode getRightSibling();
	void addChild(TNode node);
	void insertChild(TNode node, int index);
	TNode getChild(int index);
};

