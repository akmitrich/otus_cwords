#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
ssize_t getline(char **lineptr, size_t *n, FILE *stream);

static int process_file(FILE *);

int main(int argc, const char *argv[])
{
    const char *program = argv[0];
    printf("%s -- counts words in a FILE\n", program);
    if (argc < 2)
    {
        printf("Usage: %s FILE\n", program);
        printf("FILE -- input text file\n");
        printf("All other args are ignored\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("ERROR: could not open your file %s.\n", argv[1]);
        return 2;
    }
    if (process_file(input) < 0)
        printf("FAIL.\n");
    else
        printf("OK.\n");
    fclose(input);

    return 0;
}

static const char *separators = "\n \t,.;-*&?!\'\"()[]{}<>=";

int process_line(char *line)
{
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
        if (size > 0)
        {
            char *word = malloc(size + 1);
            for (int i = 0; i < size; ++i)
            {
                word[i] = line[start + i];
            }
            word[size] = '\0';
            printf("|%s", word);
            free(word);
        }
    }
    printf(" <- %s", line);

    return 0;
}

int process_file(FILE *input)
{
    size_t count = 0;
    char *line = NULL;
    int result = 0;
    while (!feof(input) && !ferror(input))
    {
        line = NULL;
        size_t size = 0;
        ssize_t len = getline(&line, &size, input);
        if (len < 0)
        {
            printf("ERROR: could not read line.\n");
            result = -1;
            break;
        }
        else
        {
            printf("%lu. (%ld - %lu)", ++count, len, size);
            result = process_line(line);
        }
        if (line)
            free(line);
        if (result < 0)
            break;
    }
    printf("\n");
    return result;
}