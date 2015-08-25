#pragma once
class TNode
{
public:
	TNode();
	void setParent(TNode parent);
	void setLeftSibling(TNode leftBro);
	void setRightSibling(TNode rightBro);
	void setLeftChild(TNode leftChild);
	void setRightChild(TNode rightChild);
};

