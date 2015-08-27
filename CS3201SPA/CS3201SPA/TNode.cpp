#include <string>

using namespace std;

class TNode {
	string name;
	string nodeType;
	int stmtNum;
	TNode* parent;
	TNode* leftChild;
	TNode* rightChild;
	TNode* leftSib;
	TNode* rightSib;

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
	void setLeftChild(TNode node);
	TNode getLeftChild();
	void setRightChild(TNode node);
	TNode getRightChild();
};

void TNode::setName(string name) {
	TNode::name = name;
}

string TNode::getName()
{
	return TNode::name;
}

void TNode::setNodeType(string nodeType) {
	TNode::nodeType = nodeType;
}

string TNode::getNodeType()
{
	return TNode::nodeType;
}

void TNode::setStmtNum(int stmtNum) {
	TNode::stmtNum = stmtNum;
}

int TNode::getStmtNum()
{
	return TNode::stmtNum;
}

void TNode::setParent(TNode node) {
	parent = &node;
}

TNode TNode::getParent()
{
	return *TNode::parent;
}

void TNode::setLeftSibling(TNode node) {
	leftSib = &node;
}

TNode TNode::getLeftSibling()
{
	return *TNode::leftSib;
}

void TNode::setRightSibling(TNode node) {
	rightSib = &node;
}

TNode TNode::getRightSibling()
{
	return *TNode::rightSib;
}

void TNode::setLeftChild(TNode node) {
	leftChild = &node;
}

TNode TNode::getLeftChild()
{
	return *TNode::leftChild;
}

void TNode::setRightChild(TNode node) {
	rightChild = &node;
}

TNode TNode::getRightChild()
{
	return *TNode::rightChild;
}


