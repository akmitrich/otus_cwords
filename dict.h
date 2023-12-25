#ifndef H_DICT
#define H_DICT

struct Token
{
    char *token;
    int len;
};

struct Item
{
    struct Token key;
    int value;
};

struct Dict
{
    struct Item *items;
    int len;
    int capacity;
};

struct Dict dict_new();

struct Dict dict_with_capacity(int capacity);

void dict_insert(struct Dict *d, const char *word);

int *dict_get(struct Dict *d, const char *word);

void dict_delete(struct Dict *d);

void dict_print(struct Dict *d);

void dict_token_print(const struct Token *t);

#endif