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

int add(int **tab, int pos)
{
    int instr = (*tab)[pos];
    int first = ((instr / 100) % 10) ? (*tab)[pos + 1] : (*tab)[(*tab)[pos + 1]];
    int second = ((instr / 1000) % 10) ? (*tab)[pos + 2] : (*tab)[(*tab)[pos + 2]];

    (*tab)[(*tab)[pos + 3]] = first + second;
    return 4;
}

int mul(int **tab, int pos)
{
    int instr = (*tab)[pos];
    int first = ((instr / 100) % 10) ? (*tab)[pos + 1] : (*tab)[(*tab)[pos + 1]];
    int second = ((instr / 1000) % 10) ? (*tab)[pos + 2] : (*tab)[(*tab)[pos + 2]];

    (*tab)[(*tab)[pos + 3]] = first * second;
    return 4;
}

int in(int **tab, int pos)
{
    size_t len = 0;
    char *buf = NULL;

    printf("INPUT=");
    getline(&buf, &len, stdin);
    (*tab)[(*tab)[pos + 1]] = atoi(buf);
    return 2;
}

int out(int **tab, int pos)
{
    int opcode = (*tab)[pos];
    int first = ((opcode / 100) % 10) ? (*tab)[pos + 1] : (*tab)[(*tab)[pos + 1]];

    printf("OUTPUT=%d\n", first);
    return 2;
}

int *compute(int *tab, int len)
{
    int act = 0;
    int opcode = 0;
    int select[] = {1, 2, 3, 4};
    int (*ptr[])(int **, int) = {add, mul, in, out};

    for (int i = 0; i < len;) {
        act = 0;
        opcode = tab[i] % 100;
        for (int j = 0; j < 4; j++) {
            if (select[j] == opcode) {
                i += ptr[j](&tab, i);
                act = 1;
                break;
            }
        }
        if (!act)
            return tab;
    }
    return tab;
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
    itab = compute(itab, ilen);
    // printf("RESULT=%d\n", itab[0]);
    free(itab);
    for (int i = 0; instructions[i]; i++)
        free(instructions[i]);
    free(instructions);
    free(input);
    return 0;
}