#include "../include/TreeRead.h"

int ExprElemCtor(ExprElem* expr_elem, ExprElemType type, Value_t data) {
	assert(expr_elem);

	expr_elem->value = data;
	expr_elem->type = type;

	return 0;
}

ExprElem CreateExprCommand(int cmd_code) {
	ExprElem expr_elem = {};

	Value_t value = {};
	value.operator_type = cmd_code;

	ExprElemCtor(&expr_elem, COMMAND, value);

	return expr_elem;
}

ExprElem CreateExprImm(double double_elem) {
	ExprElem data = {};

	Value_t value = {};
	value.imm_value = double_elem;

	ExprElemCtor(&data, NUM, value);

	return data;
}

ExprElem CreateExprVar(char* name) {

	assert(name != nullptr);

	ExprElem data = {};

	Value_t value = {};

	value.var.name = _strdup(name);
	value.var.imm_value = 0;

	data.value = value;
	ExprElemCtor(&data, VAR, value);

	return data;
}

int ReadExprElem(char* str, Node** node) {

	assert(str != nullptr);

	double double_elem = 0;
	int    elem_read_size = 0;
	int    read_arg_count = sscanf(str, " %lf %n", &double_elem, &elem_read_size);

	if (read_arg_count == 0) {

		elem_read_size = 0;
		char str_elem[MAX_NODEINFO_SIZE] = {};
		read_arg_count = sscanf(str, " %s %n", &str_elem, &elem_read_size);

		if (strcmp(str_elem, DEFAULT_NIL) == 0) {
			*node = nullptr;
		}

	#define DEF_EXPR_CMD(cmd_name, str_command, cmd_code, ...) \
															   \
        else if (strcmp(str_elem, #str_command) == 0){         \
			ExprElem data = CreateExprCommand(cmd_code);       \
			*node = OpNew(data);                               \
		}

	#include "../include/def_expr_cmd.h"
	#undef DEF_EXPR_CMD
		else {
			ExprElem data = CreateExprVar(str_elem);
			*node = OpNew(data);
		}
	}
	else {
		ExprElem data = CreateExprImm(double_elem);
		*node = OpNew(data);
	}

	return elem_read_size;
}

static int DeleteCloseBracket(char* str) {

	assert(str != nullptr);

	char symbol = {};
	int end_read_size = 0;

	sscanf(str, " %c %n", &symbol, &end_read_size);

	if (symbol == ')') {
		return end_read_size;
	}

	return 0;
}

#define MoveStr(num) \
	res_size += num; \
	str      += num;   

#define ReadChild(child_name)                                                \
	symbol = {};                                                             \
	open_bracket_size = 0;                                                   \
	sscanf(str, " %c %n", &symbol, &open_bracket_size);                      \
																			 \
	Node* child_name = {};                                                   \
	int child_name ## _size = 0;                                             \
																		     \
	if (symbol == '(') child_name ## _size = ReadNodeIN(str, &child_name);   \
	else               child_name ## _size = ReadExprElem(str, &child_name); \
		                                                                     \
	MoveStr(child_name ## _size);                                            \
		                                                                     \
	close_bracket_size = DeleteCloseBracket(str);                            \
	MoveStr(close_bracket_size);                 

int ReadNodeIN(char* str, Node** res) {

	assert(str != nullptr);

	int res_size = 0;

	char symbol = {};
	int open_bracket_size = 0;
	sscanf(str, " %c %n", &symbol, &open_bracket_size);

	if (symbol == '(') {
		MoveStr(open_bracket_size);
	}

	int close_bracket_size = 0;
	ReadChild(left);

	int root_size = ReadExprElem(str, res);
	MoveStr(root_size);

	ReadChild(right);

	(*res)->left = left;
	(*res)->right = right;

	return res_size;
}

Node* CreateCommandNode(int command_code, Node* left, Node* right) {
	Node* res = OpNew(CreateExprCommand(command_code));

	res->left = left;
	res->right = right;

	return res;
}

Node* CreateImmNode(double imm_value, Node* left, Node* right) {
	Node* res = OpNew(CreateExprImm(imm_value));

	res->left = left;
	res->right = right;

	return res;
}

Node* CreateVarNode(ExprVar var, Node* left, Node* right) {
	ExprElem data = {};

	Value_t value = {};
	value.var = var;

	data.value = value;
	ExprElemCtor(&data, VAR, value);

	Node* res = OpNew(data);

	res->left = left;
	res->right = right;

	return res;
}


static int CommandPriority(int cmd_code) {
	switch (cmd_code) {
		#define DEF_EXPR_CMD(cmd_name, command, cmd_code, priority, ...) \
			case cmd_code:                                               \
				return priority;        
		#include "../include/def_expr_cmd.h"
		#undef DEF_EXPR_CMD
	}
}

void PrintCommand(int cmd_code) {
	switch (cmd_code) {
		#define DEF_EXPR_CMD(cmd_name, command, cmd_code, priority, ...)  \
			case cmd_code: {                                              \
				printf(" " #command " ");                                 \
				break;                                                    \
			}
		#include "../include/def_expr_cmd.h"
		#undef DEF_EXPR_CMD
	}
}

static bool isNeedBrackets(Node* child, Node* parent) {

	assert(child != nullptr);

	if (parent == nullptr ||
		CommandPriority(NODE_CMD_CODE(child)) < CommandPriority(NODE_CMD_CODE(parent)))
		return false;

	return true;
}

static int GetCommandArgsNum(Node* node) {

	assert(node != nullptr);

	#define DEF_EXPR_CMD(cmd_name, command, cmd_code, priority, args_num, ...)  \
				if (node->data.value.operator_type == cmd_code) {                \
					return args_num;                                            \
				}
	#include "../include/def_expr_cmd.h"
	#undef DEF_EXPR_CMD
}

void PrintNodeExpr(Node* child, Node* parent) {

	assert(child != nullptr);

	if (child->data.type == VAR) {
		printf("%s", NODE_VAR_NAME(child));
		return;
	}
	if (child->data.type == NUM) {
		printf("%.2lf", NODE_IMM_VALUE(child));
		return;
	}
	if (child->data.type == COMMAND) {
		int args_num = GetCommandArgsNum(child);
		if (args_num == 1) {
			PrintCommand(NODE_CMD_CODE(child));

			printf(" ( ");
			PrintNodeExpr(child->left, child);
			printf(" ) ");

			return;
		}

		bool is_need_brackets = isNeedBrackets(child, parent);

		if (is_need_brackets)
			printf(" ( ");

		PrintNodeExpr(child->left, child);
		PrintCommand(NODE_CMD_CODE(child));
		PrintNodeExpr(child->right, child);

		if (is_need_brackets)
			printf(" ) ");
	}
}

void PrintTreeExpr(Tree* tree) {
	PrintNodeExpr(tree->root, nullptr);
}

bool isEqualVar(ExprVar var1, ExprVar var2) {
	return strcmp(var1.name, var2.name) == 0;
}

bool ÑheckVarInNode(Node* node, ExprVar var) {
	if (node == nullptr) return false;
	if (node->data.type == VAR) return isEqualVar(node->data.value.var, var);

	if (ÑheckVarInNode(node->left, var)) return true;
	if (ÑheckVarInNode(node->right, var)) return true;

	return false;
}