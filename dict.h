#ifndef H_DICT
#define H_DICT

struct Token
{
    char *token;
    int len;
};

struct Token
dict_token_new(const char *word);

void dict_token_delete(struct Token t);

void dict_token_print(struct Token t);

#endif