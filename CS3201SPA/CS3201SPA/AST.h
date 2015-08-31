#include "TNode.h"

class AST {
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