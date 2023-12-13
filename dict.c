#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int hash(const char *token);

struct Dict dict_new()
{
    return (struct Dict){.items = NULL, .capacity = 0};
}

struct Dict dict_with_capacity(int capacity)
{
    struct Dict d;
    if (capacity > 0)
    {
        struct Item *container = malloc(sizeof(struct Item) * capacity);
        d = (struct Dict){.items = container, .capacity = capacity};
        for (int i = 0; i < d.capacity; ++i)
            d.items[i].key = dict_token_empty();
    }
    else
    {
        d = dict_new();
    }
    return d;
}

void dict_insert(struct Dict d, const char *word)
{
    if (d.items == NULL)
        return;
    struct Token t = dict_token_new(word);
    if (!dict_token_is_empty(t))
    {
        int pos = hash(word) % d.capacity;
        d.items[pos].key = t;
        d.items[pos].value = 1;
    }
}

int *dict_get(struct Dict d, const char *word)
{
    if (d.items == NULL)
        return NULL;
    int *result = NULL;
    if (word && word[0] != '\0')
    {
        int pos = hash(word) % d.capacity;
        if (dict_token_is_equal(d.items[pos].key, word))
            result = &(d.items[pos].value);
    }
    return result;
}

void dict_delete(struct Dict d)
{
    for (int i = 0; i < d.capacity; ++i)
        if (!dict_token_is_empty(d.items[i].key))
            dict_token_delete(d.items[i].key);
    if (d.items)
        free(d.items);
}

struct Token dict_token_empty(void)
{
    return (struct Token){.token = NULL, .len = 0};
}

struct Token
dict_token_new(const char *word)
{
    int len = strlen(word);
    if (len > 0)
    {
        char *token = malloc(len);
        for (int i = 0; i < len; ++i)
            token[i] = word[i];
        return (struct Token){.token = token, .len = len};
    }
    return dict_token_empty();
}

int dict_token_is_empty(struct Token t)
{
    return t.token == NULL;
}

int dict_token_is_equal(struct Token lhs, const char *rhs)
{
    if (rhs && rhs[0] != '\0' && lhs.len > 0)
        return strncmp(lhs.token, rhs, lhs.len) == 0;
    else
        return 0;
}

void dict_token_delete(struct Token t)
{
    if (t.token)
        free(t.token);
}

void dict_token_print(struct Token t)
{
    if (dict_token_is_empty(t))
        printf("(empty)");
    else
        for (int i = 0; i < t.len; ++i)
            putchar(t.token[i]);
}

int hash(const char *token)
{
    int sum = 0;
    for (int i = 0; token[i] != '\0'; ++i)
        sum += (int)token[i];
    return sum;
}