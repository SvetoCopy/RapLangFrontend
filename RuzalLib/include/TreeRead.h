#ifndef TREE_READ_HPP
#define TREE_READ_HPP

#include "Tree.h"

const int   MAX_BD_SIZE = 1000;
const int   DEFAULT_TRACK_SIZE = 10;
const int   MAX_NODEINFO_SIZE = 100;

constexpr const char* DEFAULT_NIL = ".";

enum ParseStatus {
	FOUND = 1,
	UNFOUND = 2
};

enum Order {
	POST = 1,
	IN = 2,
	PRE = 3
};

enum NodeElem {
	LEFT_ELEM = 0,
	ROOT_ELEM = 1,
	RIGHT_ELEM = 2
};


int  ReadNodeIN(char* str, Node** res);
void PrintTreeExpr(Tree* tree);

bool isEqualVar(ExprVar var1, ExprVar var2);
Node* CopyNode(const Node* node);
bool ÑheckVarInNode(Node* node, ExprVar var);

Node* CreateOperatorNode(int command_code, Node* left, Node* right);
Node* CreateImmNode(double imm_value, Node* left, Node* right);
Node* CreateVarNode(ExprVar var, Node* left, Node* right);

#endif // !TREE_READ_HPP
