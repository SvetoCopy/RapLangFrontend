#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "RuzalLib/include/Differentiator.h"

#include <locale.h>

int main(int argc, const char* argv[])
{                              
    if (argc < 2) {
        printf("Usage: %s filename.bin output.txt\n", argv[0]);
        //return -1;
    }

    setlocale(LC_CTYPE, "");
    
    FileInfo file = {};

    Node** node_array = ReadTokenArray("LegitCheck pisyat 1 dva Love Bless b To 6 grr Sosa");
    node_array[0];
    node_array[1];
    node_array[2];
    
    NameTable table = {};
    NameTableCtor(&table);

    Node* node = GetGeneral(node_array, &table);

    Tree tree = {};
    tree.root = node;

    printf("%d", CheckTree(&tree, &table));
    //printf("%lf", Eval(node));
}