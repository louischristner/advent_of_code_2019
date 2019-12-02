#include <stdio.h>
#include <stdlib.h>

int calcul_fuel(char *str)
{
    int weight = atoi(str);

    return (weight / 3) - 2;
}

int main(int ac, char **av)
{
    int fuel = 0;
    size_t len = 0;
    char *line = NULL;
    FILE *fd = fopen(av[1], "r");

    if (fd == NULL)
        return 84;
    while (getline(&line, &len, fd) != -1) {
        fuel += calcul_fuel(line);
        free(line);
        line = NULL;
        len = 0;
    }
    free(line);
    fclose(fd);
    printf("%d\n", fuel);
    return 0;
}