#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Dict dict_new()
{
    return (struct Dict){.items = NULL, .len = 0, .capacity = 0};
}

struct Dict dict_with_capacity(int capacity)
{
    struct Dict d;
    if (capacity > 0)
    {
        struct Item *container = malloc(sizeof(struct Item) * capacity);
        d = (struct Dict){.items = container, .len = 0, .capacity = capacity};
    }
    else
    {
        d = dict_new();
    }
    return d;
}

void dict_delete(struct Dict d)
{
    for (int i = 0; i < d.len; ++i)
        dict_token_delete(d.items[i].key);
    if (d.items)
        free(d.items);
}

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
