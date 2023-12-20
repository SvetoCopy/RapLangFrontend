#include "SyntaxAnalysis.h"

bool CheckIfWhileOperator(Node* if_oper) {

    if (if_oper->left == nullptr)
        return false;

    if (GET_NODE_TYPE(if_oper->right) != OPERATOR || 
        (GET_OPERATOR_TYPE(if_oper->right) != OPERATOR_END1 && 
         GET_OPERATOR_TYPE(if_oper->right) != OPERATOR_END2)
        )
    {
        return false;
    }

    return true;
}

bool CheckVar(Node* var_node, NameTable* name_table) {

    int assign_count = 0;

    for (int i = 0; i < name_table->size; i++) 
    {
        if (strcmp(var_node->data.value.var.name, name_table->table[i].name) == 0)
            assign_count++;
    }

    if (assign_count == 0)
        return false;

    if (assign_count > 1)
        return false;

    return true;
}

bool CheckAssignOperator(Node* assign_oper) {

    if (assign_oper->left == nullptr)
        return false;

    if (GET_NODE_TYPE(assign_oper->right) != VAR)
    {
        return false;
    }
    
    return true;
}

bool CheckNode(Node* node, NameTable* name_table) {
    
    assert(name_table != nullptr);

    if (node == nullptr)
        return true;

    if (GET_NODE_TYPE(node) == OPERATOR) 
    {
        if ((GET_OPERATOR_TYPE(node) == OPERATOR_IF ||
            GET_OPERATOR_TYPE(node) == OPERATOR_WHILE) &&
            !CheckIfWhileOperator(node)) 
        {
            return false;
        }

        if (GET_OPERATOR_TYPE(node) == OPERATOR_ASSIGN && !CheckAssignOperator(node))
            return false;
    }

    if (GET_NODE_TYPE(node) == VAR) {
        if (!CheckVar(node, name_table))
            return false;
    }
    
    if (!CheckNode(node->left, name_table) ||
        !CheckNode(node->right, name_table))
    {
        return false;
    }
    
    return true;
}

bool CheckTree(Tree* tree, NameTable* name_table) {

    if (!CheckNode(tree->root, name_table))
        return false;
}

void NameTableInsertKeyWord(NameTable* name_table, const char* name, int code) {

	assert(name != nullptr);
	assert(name_table != nullptr);

	NameTableElem elem = {};
	elem.code = code;
	elem.name = name;
	elem.type = KEYWORD; 

	name_table->table[name_table->size] = elem;
	name_table->size++;
}

void NameTableInsertVar(NameTable* name_table, const char* name) {

	assert(name != nullptr);
	assert(name_table != nullptr);

	NameTableElem elem = {};
	elem.code = name_table->size;
	elem.name = name;
	elem.type = VARIABLE;

	name_table->table[name_table->size] = elem;

	name_table->size++;
}

void NameTableCtor(NameTable* name_table) {

	name_table->table = (NameTableElem*)calloc(INIT_TABLE_CAPACITY, sizeof(NameTableElem));

	NameTableElem elem = {};
}

void NameTableDtor(NameTable* name_table) {

	free(name_table->table);
	name_table->size = 0;
}

Node* GetOperator(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    Node* if_node = GetIf(token_array, token_counter, name_table);

    if (if_node != nullptr) {

        if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_END1)
            assert(!"skipped end char");

        return if_node;
    }
        
    Node* assign_node = GetAssign(token_array, token_counter, name_table);

    if (assign_node != nullptr) {

        if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_END1)
            assert(!"skipped end char");

        return assign_node;
    }

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_OPEN_BRACKET_2)
        return nullptr;

    (*token_counter)++;

    Node* res_node = CreateOperatorNode(OPERATOR_END1, nullptr, nullptr);
    Node* var_node = res_node;

    Node* next_oper = GetOperator(token_array, token_counter, name_table);

    while (next_oper != nullptr) 
    {
        if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_END1)
            assert(!"skipped end char");

        (*token_counter)++;

        var_node->left = next_oper;
        next_oper = GetOperator(token_array, token_counter, name_table);

        if (next_oper != nullptr)
            var_node->right = CreateOperatorNode(OPERATOR_END1, nullptr, nullptr);

        var_node = var_node->right;
        
    }

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_CLOSE_BRACKET_2)
        assert(!"skipped close bracket");

    (*token_counter)++;

    return res_node;
}

Node* GetIf(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_IF)
        return nullptr;

    (*token_counter)++;

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_OPEN_BRACKET_1)
        return nullptr;

    (*token_counter)++;

    Node* condition = GetExpression(token_array, token_counter, name_table);

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) != OPERATOR_CLOSE_BRACKET_1)
        assert(!"skipped close bracket");

    (*token_counter)++;

    Node* condition_body = GetOperator(token_array, token_counter, name_table);

    return CreateOperatorNode(OPERATOR_IF, condition, condition_body);
}

Node* GetAssign(Node** token_array, int* token_counter, NameTable* name_table) {
    
    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    Node* equal_node = token_array[*token_counter];

    if (GET_OPERATOR_TYPE(equal_node) != OPERATOR_ASSIGN)
        return nullptr;

    (*token_counter)++;

    Node* l_value = GetIdentifier(token_array, token_counter, name_table);

    if (l_value == nullptr)
        return nullptr;

    Node* to_node = token_array[*token_counter];

    if (GET_OPERATOR_TYPE(to_node) != OPERATOR_ASSIGN_TO)
        return nullptr;

    (*token_counter)++;

    Node* r_value = GetExpression(token_array, token_counter, name_table);
    if (r_value == nullptr)
        return nullptr;

    NameTableInsertVar(name_table, l_value->data.value.var.name);
    equal_node->left  = r_value;
    equal_node->right = l_value;

    return equal_node;
}

Node* GetIdentifier(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    if (GET_NODE_TYPE(token_array[*token_counter]) == VAR) {
        
        Node* res = token_array[*token_counter];
        (*token_counter)++;

        return res;
    }

    return nullptr;
}

Node* GetPrimary(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    Node* val = 0;

    if (GET_OPERATOR_TYPE(token_array[*token_counter]) == OPERATOR_OPEN_BRACKET_1) {
        (*token_counter)++;

        val = GetExpression(token_array, token_counter, name_table);

        (*token_counter)++;
        return val;
    }

    int old_counter = *token_counter;
    val = GetNum(token_array, token_counter, name_table);

    if (old_counter == *token_counter)
        val = GetIdentifier(token_array, token_counter, name_table);
    
    return val;
}

Node* GetTerm(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    Node* first_val = GetPrimary(token_array, token_counter, name_table);

    while (GET_OPERATOR_TYPE(token_array[*token_counter]) == OPERATOR_MUL ||
           GET_OPERATOR_TYPE(token_array[*token_counter]) == OPERATOR_DIV)
    {
        Node* oper_node = token_array[*token_counter];

        (*token_counter)++;
        Node* second_val = GetPrimary(token_array, token_counter, name_table);

        oper_node->left = first_val;
        oper_node->right = second_val;

        first_val = oper_node;
    }

    return first_val;
}

Node* GetExpression(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    Node* first_val = GetTerm(token_array, token_counter, name_table);

    while (GET_OPERATOR_TYPE(token_array[*token_counter]) == OPERATOR_ADD ||
           GET_OPERATOR_TYPE(token_array[*token_counter]) == OPERATOR_SUB)
    {
        int op = GET_OPERATOR_TYPE(token_array[*token_counter]);

        Node* oper_node = token_array[*token_counter];

        (*token_counter)++;
        Node* second_val = GetTerm(token_array, token_counter, name_table);

        oper_node->left = first_val;
        oper_node->right = second_val;

        first_val = oper_node;
    }

    return first_val;
}

Node* GetNum(Node** token_array, int* token_counter, NameTable* name_table) {

    assert(token_array != nullptr);
    assert(token_counter != nullptr);
    assert(name_table != nullptr);

    if (token_array[*token_counter]->data.type == NUM)
    {
        Node* node = token_array[*token_counter];
        (*token_counter)++;

        return node;
    }

    return nullptr;
}

Node* GetGeneral(Node** token_array, NameTable* name_table) {

    assert(token_array != nullptr);

    int token_counter = 0;

    Node* val = GetOperator(token_array, &token_counter, name_table);

    return val;
}
