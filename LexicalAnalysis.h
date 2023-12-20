#ifndef LEXICAL_ANALYSIS_HPP
#define LEXICAL_ANALYSIS_HPP

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "RuzalLib/include/Tree.h"
#include <wchar.h>

const int MAX_OPERATOR_SIZE   = 100;
const int MAX_VAR_SIZE        = 100;
const int INIT_TABLE_CAPACITY = 100;
const int CHAR_OPERATION_SIZE = 1;

const char const TATAR_ALPHABET[33] = {'һ', 'й', 'ө', 'у', 'к', 'е', 'н', 'г', 'ш', 'ә',
									   'з', 'х', 'ү', 'ф', 'ы', 'в', 'а', 'п', 'р', 'о',
									   'л', 'д', 'ң', 'э', 'я', 'ч', 'с', 'м', 'и', 'т',
									   'җ', 'б', 'ю'};
const int TATAR_ALPHABET_SIZE		= 33;

enum Operators {
	#define DEF_OPERATOR(name, oper, code, ...) OPERATOR_ ## name = code,

	#include "def_operator.h"

	#undef DEF_OPERATOR
};

Node** ReadTokenArray(const char* str);

#endif