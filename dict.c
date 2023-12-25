#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int hash(const char *token);
static void rehash(struct Dict *d);
static int find_item_index(struct Dict *d, const char *key);

#define THRESHOLD 75

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
        for (int i = 0; i < d.capacity; ++i)
            d.items[i].key = dict_token_empty();
    }
    else
    {
        d = dict_new();
    }
    return d;
}

void dict_insert(struct Dict *d, const char *word)
{
    if (d->items == NULL)
        return;
    struct Token t = dict_token_new(word);
    if (!dict_token_is_empty(t))
    {
        int pos = find_item_index(d, word);
        if (dict_token_is_empty(d->items[pos].key))
            d->len += 1;
        d->items[pos].key = t;
        d->items[pos].value = 1;
        if (d->len * 100 / d->capacity > THRESHOLD)
            rehash(d);
    }
}

int *dict_get(struct Dict *d, const char *word)
{
    if (d->items == NULL)
        return NULL;
    int *result = NULL;
    if (word && word[0] != '\0')
    {
        int pos = find_item_index(d, word);
        if (!dict_token_is_empty(d->items[pos].key))
            result = &(d->items[pos].value);
    }
    return result;
}

void dict_delete(struct Dict *d)
{
    for (int i = 0; i < d->capacity; ++i)
        if (!dict_token_is_empty(d->items[i].key))
            dict_token_delete(d->items[i].key);
    if (d->items)
        free(d->items);
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

int find_item_index(struct Dict *d, const char *key)
{
    int h = hash(key) % d->capacity;
    while (!dict_token_is_empty(d->items[h].key) && !dict_token_is_equal(d->items[h].key, key))
        h = (h + 1) % d->capacity;
    return h;
}

void rehash(struct Dict *d)
{
    printf("Dict was: items=%p, len=%d, capacity=%d\n", (void *)d->items, d->len, d->capacity);
    int new_capacity = d->capacity * 2;
    if (new_capacity < 64)
        new_capacity = 64;
    struct Dict new_d = dict_with_capacity(new_capacity);
    for (int i = 0; i < d->capacity; ++i)
    {
        if (!dict_token_is_empty(d->items[i].key))
        {
            const char *word = d->items[i].key.token;
            dict_insert(&new_d, word);
            int *count = dict_get(&new_d, word);
            *count = d->items[i].value;
        }
    }
    dict_delete(d);
    *d = new_d;
    printf("Dict becomes: items=%p, len=%d, capacity=%d\n", (void *)d->items, d->len, d->capacity);
}