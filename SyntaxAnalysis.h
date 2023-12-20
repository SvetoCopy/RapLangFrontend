#include "RuzalLib/include/Tree.h"
#include "RuzalLib/include/TreeRead.h"
#include "assert.h"
#include "LexicalAnalysis.h"

#define GET_OPERATOR_TYPE(node)  node->data.value.operator_type
#define GET_NODE_TYPE(node)      node->data.type
#define GET_NODE_IMM_VALUE(node) node->data.value.imm_value

enum NameTableElemType {
	KEYWORD = 0,
	VARIABLE = 1
};

struct NameTableElem {
	const char* name;
	int				  code;
	NameTableElemType type;
};

struct NameTable {
	NameTableElem* table;
	size_t		   size;
};

void NameTableCtor(NameTable* name_table);
void NameTableDtor(NameTable* name_table);

bool CheckTree(Tree* tree, NameTable* name_table);

Node* GetOperator(Node** token_array, int* counter, NameTable* name_table);

Node* GetWhile(Node** token_array, int* counter, NameTable* name_table);
Node* GetIf(Node** token_array, int* counter, NameTable* name_table);

Node* GetAssign(Node** token_array, int* counter, NameTable* name_table);
Node* GetIdentifier(Node** token_array, int* counter, NameTable* name_table);

Node* GetPrimary(Node** token_array, int* counter, NameTable* name_table);
Node* GetExpression(Node** token_array, int* counter, NameTable* name_table);
Node* GetTerm(Node** token_array, int* counter, NameTable* name_table);
Node* GetNum(Node** token_array, int* counter, NameTable* name_table);
Node* GetGeneral(Node** token_array, NameTable* name_table);
