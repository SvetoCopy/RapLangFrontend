#include "../include/GraphicDump.h"

int HashStr(NodeInfo_t info) {
	int res = 0;
	while (*info != '\0') {
		res += *info;
		info++;
	}
	return res;
}

int DefineNode(Tree* tree, Node* node) {
	if (node == nullptr) return 0;

	if (node->left != nullptr || node->right != nullptr) {
		fprintf(tree->graph_logfile, 
			   "\"node%d\" [shape=Mrecord, style=filled, fillcolor=" QUESTNODE_COLOR ", "
			   "label = \"%s\"];\n", 
			   HashStr(node->data), 
			   node->data);
	}

	DefineNode(tree, node->left);
	DefineNode(tree, node->right);

	return 0;
}

bool isLeaf(Node* node) {
	if (node->left == nullptr && node->right == nullptr) return true;
	return false;
}

int DefineLeafs(Tree* tree, Node* node) {
	if (node == nullptr) return 0;
	if (isLeaf(node)) {
		fprintf(tree->graph_logfile, 
			    "\"leaf%d\" [shape=Mrecord, style=filled, fillcolor=" LEAFNODE_COLOR ", "
			    "label = \"%s\"];\n", 
				HashStr(node->data), 
			    node->data);
	}
	DefineLeafs(tree, node->left);
	DefineLeafs(tree, node->right);
	return 0;
}

int ConnectNodes(Tree* tree, Node* node) {
	if (node == nullptr) return 0;
	if (node->left != nullptr) {
		const char* node_name = isLeaf(node->left) ? "leaf" : "node";
		fprintf(tree->graph_logfile, 
				"node%d->%s%d\n", 
				HashStr(node->data), node_name, HashStr(node->left->data));
	}

	if (node->right != nullptr) {
		if (isLeaf(node->right))
			fprintf(tree->graph_logfile, "node%d->leaf%d\n",
				HashStr(node->data), HashStr(node->right->data));
		else
			fprintf(tree->graph_logfile, "node%d->node%d\n",
				HashStr(node->data), HashStr(node->right->data));
	}

	ConnectNodes(tree, node->left);
	ConnectNodes(tree, node->right);
	return 0;
}

int  TreeGraphicDump(Tree* tree) {
	fprintf(tree->graph_logfile, 
		    "digraph Tree \n{ bgcolor=\"" BG_COLOR "\";" 
		    "node[margin = \"0.01\"];\n");
	DefineNode(tree, tree->root);
	DefineLeafs(tree, tree->root);
	ConnectNodes(tree, tree->root);
	fprintf(tree->graph_logfile, "}");
	return 0;
}