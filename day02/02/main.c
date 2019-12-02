#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int how_many(char *str, char sign)
{
    int count = 0;

    for (int i = 0; str[i]; i++)
        if (str[i] == sign)
            count++;
    return count;
}

char *my_cut_str(char *str, int start, int end)
{
    char *result = malloc(sizeof(char) * (end - start + 1));

    if (!result)
        return NULL;
    for (int j = start; j < end; j++)
        result[j - start] = str[j];
    result[end - start] = '\0';
    return result;
}

char **my_str_to_tab(char *str, char splitter)
{
    int i = 0;
    int start = 0;
    int index = 0;
    int count = how_many(str, splitter);
    char **tab = malloc(sizeof(char *) * (count + 2));

    if (!tab)
        return NULL;
    tab[count + 1] = NULL;
    for (; str[i]; i++)
        if (str[i] == splitter) {
            if (i > start) {
                tab[index] = my_cut_str(str, start, i);
                index++;
            }
            start = i + 1;
        }
    tab[index] = my_cut_str(str, start, i);
    return tab;
}

char *read_input(char *input)
{
    int fd = open(input, O_RDONLY);
    char *buf = malloc(sizeof(char) * 1024);

    if (fd == -1 || buf == NULL)
        return NULL;
    buf[read(fd, buf, 1023)] = '\0';
    close(fd);
    return buf;
}

int *convert_to_int_tab(char **tab)
{
    int len = 0;
    int *itab = NULL;

    for (; tab[len]; len++);
    itab = malloc(sizeof(int) * len);
    for (int i = 0; tab[i]; i++)
        itab[i] = atoi(tab[i]);
    return itab;
}

int *compute(int *tab, int len)
{
    for (int i = 0; i < len; i += 4) {
        if (tab[i] == 1) {
            tab[tab[i + 3]] = tab[tab[i + 1]] + tab[tab[i + 2]];
        } else if (tab[i] == 2) {
            tab[tab[i + 3]] = tab[tab[i + 1]] * tab[tab[i + 2]];
        } else {
            return tab;
        }
    }
    return tab;
}

int *copy(int *tab, int len)
{
    int *cp = malloc(sizeof(int) * len);

    for (int i = 0; i < len; i++)
        cp[i] = tab[i];
    return cp;
}

int prog(int *itab, int ilen)
{
    int *old = NULL;

    for (int noun = 0; noun < 100; noun++) {
        for (int verb = 0; verb < 100; verb++) {
            old = copy(itab, ilen);
            itab[1] = noun;
            itab[2] = verb;
            itab = compute(itab, ilen);
            if (itab[0] == 19690720) {
                printf("noun=%d and verb=%d\n", noun, verb);
                return 0;
            }
            itab = old;
        }
    }
    return 0;
}

int main(int ac, char **av)
{
    char *input = read_input(av[1]);
    char **instructions = NULL;
    int *itab = NULL;
    int ilen = 0;

    if (!input)
        return 84;
    instructions = my_str_to_tab(input, ',');
    for (; instructions[ilen]; ilen++);
    itab = convert_to_int_tab(instructions);
    prog(itab, ilen);
    free(itab);
    for (int i = 0; instructions[i]; i++)
        free(instructions[i]);
    free(instructions);
    free(input);
    return 0;
}