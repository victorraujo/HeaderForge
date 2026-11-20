#ifndef HEADERFORGE_STR_H
#define HEADERFORGE_STR_H

#include <stdio.h>
#include <stdlib.h> // alocar
#include <string.h> // strlen
#include <stdint.h> // limite de variaveis
#include <stddef.h> // size_t

//------------------STRING FERRAMENTA(64 BYTES)------------------

typedef struct string_t
{
    char *str;
    size_t size;

    void(*input)(struct string_t*);               // entrada
    void(*delect)(struct string_t*);              // deletar string e tamanho
    void(*swap)(struct string_t*, char*);         //trocar a string
    void(*append)(struct string_t*, const char*); // adicionar ao final da string
    void(*print)(struct string_t*);               // exibir string sem quebra de linha
    void(*printl)(struct string_t*);              // exibir string com quebra de linha
}string_t;


void string_input_fun(string_t *string)
{  
    // 1. SEGURANÇA -> STRUCT NULL
    if (string == NULL) { return; }

    if (string->str != NULL)
    {
        free(string->str);
        string->str = NULL;
        string->size = 0;
    }
    
    size_t size_bloco = 20;
    size_t size_atual = 0;
    char *str = malloc(size_bloco * sizeof(char));
    if (str == NULL) { return; }
    
    int caractere = 0;
    const size_t block_add = 1;
    while ((caractere = getchar()) != '\n' && caractere != EOF)
    {
        if (size_atual >= size_bloco - block_add)
        {
            // 2. SEGURANÇA -> EVITA OVERFLOW NO CRESCIMENTO DO BLOCO
            if (size_bloco > SIZE_MAX - 20) { free(str); return; }

            size_bloco += 20;
            char* tmp = realloc(str, size_bloco);
            if (tmp == NULL) { free(str); return; }
            
            str = tmp;
        }
        str[size_atual] = (char)caractere;
        size_atual++;
    }
    str[size_atual] = '\0';

    // 3. ENCOLHE APENAS SE ESTIVER SOBRANDO MEMÓRIA (e não perde o texto se falhar)
    if (size_atual + 1 < size_bloco)
    {
        char* enviar = realloc(str, size_atual + 1);
        if (enviar != NULL) 
        { 
            str = enviar; 
        }
    }
    
    string->str = str;
    string->size = size_atual; // Já salva sem o '\0' (perfeito!)
}

void string_delect_fun(string_t *string)
{
    if (string == NULL)
    {
       return;
    }
    if (string->str == NULL)
    {
      return;
    }
    free(string->str);
    string->str = NULL;
    string->size = 0;
    
    return;
}

void string_swap_fun(string_t* string, char* text)
{
    // 1. SEGURANÇA -> PONTEIROS NULL OU ENDEREÇOS IGUAIS
    if (string == NULL || text == NULL || string->str == text) { return; }
    
    // 2. SEGURANÇA -> STRING MÁXIMA (checa antes de apagar a antiga)
    size_t size = strlen(text);
    // SEGURANÇA -> STRING MAXIMA
    if (size >= SIZE_MAX) { return; }
    
    // 3. LIMPA A ANTIGA
    if (string->str != NULL)
    {
        free(string->str);
        string->str = NULL;
    }
    
     // 4. ALOCA E COPIA
    string->str = malloc(size +1);
    if (string->str == NULL) { return; }
    size_t atual = 0;
    while(atual < size)
    {
        string->str[atual] = text[atual];
        atual++;
    }
    string->str[atual] = '\0';
    string->size = size;
    
    return;
}
void string_append_fun(string_t* string, const char* text)
{
        // 1. SEGURANÇA -> PONTEIROS NULL OU ENDEREÇOS IGUAIS
    if (string == NULL || text == NULL || string->str == text) { return; }
    
    size_t size_str = string->size;
    size_t size_newText = strlen(text);
    
        // 2. SEGURANÇA -> SIZE MAX
    if (size_newText >= SIZE_MAX) { return; }
    
    size_t size_full =  size_str + size_newText;
    char* str_tmp = realloc(string->str, size_full+1);
    if (str_tmp == NULL) { return; }
    
    // REMOÇÃO DA BARREIRA DA STRING E COPIAÇÃO
    size_t current = 0;
    while(current < size_newText)
    {
      str_tmp[size_str + current] = text[current];
      current++;
    }
    str_tmp[size_str + current] = '\0';
    
    string->str  = str_tmp;
    string->size = size_full;
    return;
}

void string_print_fun(string_t* string)
{
    if (string == NULL || string->str == NULL)
    {
        printf("null");
        return;
    }
    printf("%s", string->str);
}

void string_printl_fun(string_t* string)
{
    if (string == NULL || string->str == NULL)
    {
        printf("null\n");
        return;
    }
    printf("%s\n", string->str);
}

//------------------DESIGNATED INITIALIZERS------------------
#define NEW_STRING            \
{                              \
    .input  = string_input_fun, \
    .delect = string_delect_fun, \
    .swap   = string_swap_fun,    \
    .append = string_append_fun,   \
    .print  = string_print_fun,     \
    .printl = string_printl_fun,     \
    .str    = NULL,                   \
    .size   = 0                        \
}

#endif
