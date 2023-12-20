//            operation
//  name -----+    |   +--------------------- code
//			  |    |   |    +------ is char operator
//			  |    |   |    |
//            V    v   v    v
DEF_OPERATOR(MUL, "*", 0, true)
DEF_OPERATOR(ADD, "+", 1, true)
DEF_OPERATOR(SUB, "-", 2, true)
DEF_OPERATOR(DIV, "/", 3, true)
DEF_OPERATOR(POW, "^", 4, true)
DEF_OPERATOR(OPEN_BRACKET_1, "pisyat", 5, false)
DEF_OPERATOR(CLOSE_BRACKET_1, "dva", 6, false)
DEF_OPERATOR(END1, "grr", 7, false)
DEF_OPERATOR(END2, "yeei", 8, false)
DEF_OPERATOR(ASSIGN, "Bless", 9, false)
DEF_OPERATOR(ASSIGN_TO, "To", 10, false)
DEF_OPERATOR(IF, "LegitCheck", 11, false)
DEF_OPERATOR(WHILE, "While", 12, false)
DEF_OPERATOR(OPEN_BRACKET_2, "Love", 13, false)
DEF_OPERATOR(CLOSE_BRACKET_2, "Sosa", 14, false)