#include "TNode.h"
#include <string>
#include <vector>

using namespace std;

class TNode {
	string name;
	string nodeType;
	int stmtNum;
	TNode* parent;
	vector<TNode*> childrenNodes;
	TNode* leftChild;
	TNode* rightChild;
	TNode* leftSib;
	TNode* rightSib;

public:
	TNode(string name, string nodeType, int stmtNum);
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
	int numChildren();
	TNode(string name, string nodeType, int stmtNum);
};

TNode::TNode(string name, string nodeType, int stmtNum) {
	TNode::name = name;
	TNode::nodeType = nodeType;
	TNode::stmtNum = stmtNum;
	rightSib = NULL;
}

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

void TNode::addChild(TNode node) {
	TNode::childrenNodes.push_back(&node);
}

void TNode::insertChild(TNode node, int index) {
	vector<TNode*>::iterator iter;
	iter = childrenNodes.begin();
	TNode::childrenNodes.insert(iter+index, &node);
}

TNode TNode::getChild(int index)
{
	return *TNode::childrenNodes.at(index);
}

int TNode::numChildren() {
	return TNode::childrenNodes.size();
}


