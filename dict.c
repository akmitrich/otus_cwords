#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int hash(const struct Token *token);
static void rehash(struct Dict *d);
static int find_item_index(const struct Dict *d, const struct Token *key);
static int *get_item_by_key(struct Dict *d, const struct Token *key);
struct Token dict_token_empty(void);

static struct Token dict_token_new(const char *word);
static int dict_token_is_empty(const struct Token *t);
static int dict_token_is_equal(const struct Token *lhs, const struct Token *rhs);
static void dict_token_delete(struct Token *t);

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
        {
            d.items[i].key = dict_token_empty();
            d.items[i].value = 0;
        }
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
    if (!dict_token_is_empty(&t))
    {
        int pos = find_item_index(d, &t);
        if (dict_token_is_empty(&d->items[pos].key))
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
    struct Token key = dict_token_new(word);
    int *result = get_item_by_key(d, &key);
    dict_token_delete(&key);
    return result;
}

void dict_delete(struct Dict *d)
{
    for (int i = 0; i < d->capacity; ++i)
        dict_token_delete(&d->items[i].key);
    if (d->items)
        free(d->items);
}

void dict_print(struct Dict *d)
{
    int count = 1;
    for (int i = 0; i < d->capacity; ++i)
        if (!dict_token_is_empty(&d->items[i].key))
        {
            printf("%d. ", count);
            dict_token_print(&d->items[i].key);
            printf(" => %d\n", d->items[i].value);
            ++count;
        }
}

struct Token dict_token_empty(void)
{
    return (struct Token){.token = NULL, .len = 0};
}

struct Token dict_token_new(const char *word)
{
    int len = strlen(word);
    if (len == 0)
        return dict_token_empty();
    char *token = malloc(len);
    for (int i = 0; i < len; ++i)
        token[i] = word[i];
    return (struct Token){.token = token, .len = len};
}

int dict_token_is_empty(const struct Token *t)
{
    return t->token == NULL;
}

int dict_token_is_equal(const struct Token *lhs, const struct Token *rhs)
{
    if (rhs && lhs)
    {
        int len = lhs->len;
        if (len == rhs->len && len > 0)
            return strncmp(lhs->token, rhs->token, len) == 0;
    }
    return 0;
}

void dict_token_delete(struct Token *t)
{
    if (t->token)
        free(t->token);
}

void dict_token_print(const struct Token *t)
{
    if (dict_token_is_empty(t))
        printf("(empty)");
    else
        for (int i = 0; i < t->len; ++i)
            putchar(t->token[i]);
}

int hash(const struct Token *token)
{
    int sum = 0;
    for (int i = 0; i < token->len; ++i)
        sum += (int)token->token[i];
    return sum;
}

int find_item_index(const struct Dict *d, const struct Token *key)
{
    int h = hash(key) % d->capacity;
    while (!dict_token_is_empty(&d->items[h].key) && !dict_token_is_equal(&d->items[h].key, key))
        h = (h + 1) % d->capacity;
    return h;
}

static int *get_item_by_key(struct Dict *d, const struct Token *key)
{
    if (d->items == NULL)
        return NULL;
    int *result = NULL;
    int pos = find_item_index(d, key);
    if (!dict_token_is_empty(&d->items[pos].key))
        result = &(d->items[pos].value);
    return result;
}

#define WORD_BUF_SIZE 1024
#define MIN_DICT_SIZE 32

void rehash(struct Dict *d)
{
    char word_buffer[WORD_BUF_SIZE];
    int new_capacity = d->capacity * 2;
    if (new_capacity < MIN_DICT_SIZE)
        new_capacity = MIN_DICT_SIZE;
    struct Dict new_d = dict_with_capacity(new_capacity);
    for (int i = 0; i < d->capacity; ++i)
    {
        if (!dict_token_is_empty(&d->items[i].key))
        {
            int size = d->items[i].key.len;
            if (size >= WORD_BUF_SIZE)
                size = WORD_BUF_SIZE - 1;
            for (int j = 0; j < size; ++j)
                word_buffer[j] = d->items[i].key.token[j];
            word_buffer[size] = '\0';
            const char *word = word_buffer;
            dict_insert(&new_d, word);
            int *count = dict_get(&new_d, word);
            *count = d->items[i].value;
        }
    }
    dict_delete(d);
    *d = new_d;
}
