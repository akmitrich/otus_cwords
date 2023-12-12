#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Token
dict_token_new(const char *word)
{
    int len = strlen(word);
    char *token = malloc(len);
    for (int i = 0; i < len; ++i)
        token[i] = word[i];
    return (struct Token){.token = token, .len = len};
}

void dict_token_delete(struct Token t)
{
    if (t.token)
        free(t.token);
}

void dict_token_print(struct Token t)
{
    for (int i = 0; i < t.len; ++i)
        putchar(t.token[i]);
}
