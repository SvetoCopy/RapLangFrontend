#define _CRT_SECURE_NO_WARNINGS

#include "LexicalAnalysis.h"

// return value size
static int ReadIntFromStr(const char* str, int* value) {

	assert(str != nullptr);

	int iter = 0;
	int res = 0;

	while (isdigit(*str)) {
		res *= 10;
		res += *str - '0';
		iter++;
		str++;
	}

	*value = res;

	return iter;
}

bool isOperator(const char* str, int* operator_type) {

	assert(str != nullptr);

	#define DEF_OPERATOR(oper_name, oper_str, oper_code, ...)		  \
		if (strcmp(str, oper_str) == 0 && strlen(oper_str) != 1) {	  \
			*operator_type = oper_code;                               \
			return true;               								  \
		}

	#include "def_operator.h"

	#undef DEF_OPERATOR

	return false;
}

int ReadCharToken(const char* str, Node** node) {
	
	#define DEF_OPERATOR(oper_name, oper_str, oper_code, ...)		  \
		if (*str == *oper_str && strlen(oper_str) == 1) {			  \
			*node = CreateOperatorNode(oper_code, nullptr, nullptr);  \
			return CHAR_OPERATION_SIZE;								  \
		}

	#include "def_operator.h"

	#undef DEF_OPERATOR

	return 0;
}

bool isTatarianLetter(char c) {
	

	return true;
}

int ReadStrToken(const char* str, Node** node) {

	assert(str != nullptr);
	assert(node != nullptr);

	int   iter = 0;
	char* ident = (char*)calloc(MAX_VAR_SIZE, sizeof(char));

	while (
		('A' <= str[iter] && str[iter] <= 'Z') ||
		('a' <= str[iter] && str[iter] <= 'z') ||
		('à' <= str[iter] && str[iter] <= 'ÿ') ||
		('A' <= str[iter] && str[iter] <= 'ß') ||
		(str[iter] == '_') ||
		(isdigit(str[iter]))
		)
	{
		ident[iter] = str[iter];
		iter++;
	}

	int oper_code = 0;
	if (isOperator(ident, &oper_code)) {
		*node = CreateOperatorNode(oper_code, nullptr, nullptr);
	}

	else {
		ExprVar var = {};
		var.name = ident;

		*node = CreateVarNode(var, nullptr, nullptr);
	}

	return iter;
}

Node** ReadTokenArray(const char* str) {

	Node** node_array = (Node**)calloc(strlen(str), sizeof(Node*));
	int iter = 0;

	while (*str != '\0') {

		if (isspace(*str) || *str == '\n') {
			str++;
		}

		else if (ReadCharToken(str, &(node_array[iter])) != 0) {
			iter++;
			str++;
		}

		else if (isdigit(*str)) {
			int number = 0;
			str += ReadIntFromStr(str, &number);

			node_array[iter] = _IMM(number);

			iter++;
		}

		else {
			Node* node = {};
			int str_token_size = ReadStrToken(str, &node);

			node_array[iter] = node;

			iter++;
			str += str_token_size;
		}
	}

	node_array[iter] = CreateOperatorNode(OPERATOR_END1, nullptr, nullptr);

	return node_array;
}