#include <stdio.h>
#include <stdlib.h>

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
    if (process_file(input) != 0)
        printf("FAIL.\n");
    else
        printf("OK.\n");
    fclose(input);

    return 0;
}

int process_file(FILE *input)
{
    while (!feof(input) && !ferror(input))
    {
        char *line = NULL;
        size_t len = 0;
        getline(&line, &len, input);
        printf("%s", line);
        if (line)
            free(line);
    }

    return 0;
}