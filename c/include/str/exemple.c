#include <stdio.h>
#include "str.h"


int main(void)
{
    // criar a estrutura e organizar ela
    string_t usuario = NEW_STRING;

    // pegar texto
     usuario.input(&usuario);
     // imprimir
     usuario.printl(&usuario);

     return 0;
}
