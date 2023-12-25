#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
ssize_t getline(char **lineptr, size_t *n, FILE *stream);

#include "dict.h"

static int process_file(FILE *);

int main(int argc, const char *argv[])
{
    const char *program = argv[0];
    printf("%s -- counts words in a FILE", program);
    if (argc < 2)
    {
        printf("\nUsage: %s FILE\n", program);
        printf("FILE -- input text file\n");
        printf("All other args are ignored\n");
        return 1;
    }
    const char *file_path = argv[1];
    printf(" %s.", file_path);

    FILE *input = fopen(file_path, "r");
    if (input == NULL)
    {
        printf("ERROR: could not open your file %s.\n", file_path);
        return 2;
    }
    if (process_file(input) < 0)
        printf("FAIL.\n");
    else
        printf("OK.\n");
    fclose(input);

    return 0;
}

static struct Dict word_count;

void process_word(const char *word)
{
    int *count = dict_get(&word_count, word);
    if (count)
        ++(*count);
    else
        dict_insert(&word_count, word);
}

static const char *separators = "\n \t,.:;-*&?!\'\"()[]{}<>=\\/";
#define WORD_BUF_SIZE 1024

int process_line(char *line)
{
    char word_buffer[WORD_BUF_SIZE];
    int len = strlen(line);
    int current = 0;
    while (current < len)
    {
        /* skip separators */
        while (current < len)
        {
            if (strchr(separators, line[current]) == NULL)
                break;
            ++current;
        }
        int start = current;
        /* take word */
        while (current < len)
        {
            if (strchr(separators, line[current]) != NULL)
                break;
            ++current;
        }
        int size = current - start;
        if (size >= WORD_BUF_SIZE)
            size = WORD_BUF_SIZE - 1;
        if (size > 0)
        {
            for (int i = 0; i < size; ++i)
            {
                word_buffer[i] = line[start + i];
            }
            word_buffer[size] = '\0';
            const char *word = word_buffer;
            process_word(word);
        }
    }
    return 0;
}

int process_file(FILE *input)
{
    char *line = NULL;
    int result = 0;
    word_count = dict_with_capacity(10);
    while (!feof(input) && !ferror(input))
    {
        line = NULL;
        size_t size = 0;
        ssize_t len = getline(&line, &size, input);
        if (len < 0)
        {
            printf("ERROR: could not read line from file.\n");
            result = -1;
        }
        else
            result = process_line(line);
        if (line)
            free(line);
        if (result < 0)
            break;
    }
    printf("\n");

    dict_print(&word_count);
    dict_delete(&word_count);
    return result;
}