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
    int capacity;
};

struct Dict dict_new();

struct Dict dict_with_capacity(int capacity);

void dict_insert(struct Dict *d, const char *word);

int *dict_get(struct Dict *d, const char *word);

void dict_delete(struct Dict *d);

struct Token dict_token_empty(void);

struct Token
dict_token_new(const char *word);

int dict_token_is_empty(struct Token t);

int dict_token_is_equal(struct Token lhs, const char *rhs);

void dict_token_delete(struct Token t);

void dict_token_print(struct Token t);

#endif