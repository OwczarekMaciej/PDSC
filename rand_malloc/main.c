#include "rand_malloc.h"
#include <stdio.h>
#include <stdlib.h>

int error = 0;
size_t nolines = 0;

char *readline()
{
    char *line = NULL;
    int c;
    size_t bufsize = 0;
    size_t size = 0;

    while ((c = getchar()) != EOF)
    {
        if (size >= bufsize)
        {
            char *newbuf;
            if (bufsize == 0)
                bufsize = 2;
            else if (bufsize <= ((size_t)-1) / 2)
                bufsize = 2 * size;
            else
            {
                error = 1;
                free(line);
                return NULL;
            }
            newbuf = realloc(line, bufsize);
            if (!newbuf)
            {
                error = 1;
                free(line);
                return NULL;
            }
            line = newbuf;
        }
        line[size++] = c;
        if (c == '\n')
        {
            break;
        }
    }

    if ((c == EOF) && (size == 0))
        return NULL;

    if (size >= bufsize)
    {
        char *newbuf;
        if (size < (size_t)-1)
            bufsize = size + 1;
        else
        {
            error = 1;
            free(line);
            return NULL;
        }
        newbuf = realloc(line, bufsize);
        if (!newbuf)
        {
            error = 1;
            free(line);
            return NULL;
        }
        line = newbuf;
    }
    line[size++] = '\0';

    return line;
}

char **getlines()
{
    char **lines = NULL;

    size_t nolinesmax = 0;
    char *line;

    while ((line = readline()))
    {
        if (nolines >= nolinesmax)
        {
            char **newlines;
            if (nolines == 0)
                nolinesmax = 1;
            else if (nolines <= ((size_t)-1) / 2 / sizeof(char *))
            {
                nolinesmax = 2 * nolines;
            }
            else
            {
                error = 1;
                
                free(line);
                return NULL;
            }

            newlines = realloc(lines, nolinesmax * sizeof(char *));

            if (newlines == NULL)
            {
                error = 1;
                free(line);
                return lines;
            }

            lines = newlines;
        }
        lines[nolines++] = line;
    }

    return lines;
}

long int strlen(char *line)
{
    int len = 0;
    while (line[len] != '\0')
    {
        len++;
    }
    return len;
}

void reverseWords(char *line)
{
    long int end = strlen(line) - 1;

    end = strlen(line) - 1;
    while (end >= 0)
    {
        while (end >= 0 && line[end] != ' ')
        {
            end--;
        }
        long int start = end + 1;
        end--;

        while (line[start] != ' ' && line[start] != '\0' && line[start] != '\n')
        {
            printf("%c", line[start]);
            start++;
        }
        printf(" ");
    }
    printf("\n");
}

void reverse_and_free_lines(char **lines)
{
    for (size_t l = nolines; l > 0; l--)
    {
        reverseWords(lines[l - 1]);
        free(lines[l - 1]);
    }
    printf("\n");
    free(lines);
}

int main()
{
    char **lines = getlines();

    if (error)
    {
        printf("Allocation memory failed\n");
        for (size_t l = nolines; l > 0; l--)
        {
            printf("%d %d\n", nolines, l);
            printf("%s ",lines[l-1]);
            free(lines[l - 1]);
        }
        free(lines);
        exit(1);
    }
    else
    {
        reverse_and_free_lines(lines);
    }

    return 0;
}
