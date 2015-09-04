#include "TNode.h"
#include <string>

using namespace std;

class AST {
private:
	TNode root;
	TNode *currentNode;

public:
	//Constructor with root 
	AST(TNode root);
	TNode getRoot();
	void addParent(TNode parent);
	void addChild(TNode child);
	void addLeftSibling(TNode leftSibling);
	void addRightSibling(TNode rightSibling);
	//Traversing the AST
	void gotoParent();
	void gotoChild();
	void gotoLeftSibling();
	void gotoRightSibling();
	//Reading node
	string getName();
	string getNodeType();
	int getStmtNum();

};

AST::AST(TNode root) {
	AST::root = root;
	currentNode = &root;
}

TNode AST::getRoot()
{
	return AST::root;
}

void AST::addParent(TNode parent) {
	currentNode->setParent(parent);

}

void AST::addChild(TNode child) {
	//currentNode->setChild(child);
}

void AST::addLeftSibling(TNode leftSibling)
{
	currentNode->setLeftSibling(leftSibling);
}

void AST::addRightSibling(TNode rightSibling)
{
	currentNode->setRightSibling(rightSibling);
}

void AST::gotoParent()
{
	currentNode = &currentNode->getParent();
}

void AST::gotoChild()
{
	//currentNode = &currentNode->getChild(1);
}

void AST::gotoLeftSibling()
{
	currentNode = &currentNode->getLeftSibling();
}

void AST::gotoRightSibling()
{
	currentNode = &currentNode->getRightSibling();
}

string AST::getName()
{
	//return currentNode->getName();
	return "";
}

string AST::getNodeType()
{
	//return currentNode->getNodeType();
	return "";
}

int AST::getStmtNum()
{
	return currentNode->getStmtNum();
}


