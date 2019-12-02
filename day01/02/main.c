#include <stdio.h>
#include <stdlib.h>

int calcul_fuel(int weight)
{
    int result = (weight / 3) - 2;

    if (result < 0)
        result = 0;
    return ((result / 3) - 2 <= 0) ? (result) : (result += calcul_fuel(result));
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
        fuel += calcul_fuel(atoi(line));
        free(line);
        line = NULL;
        len = 0;
    }
    free(line);
    fclose(fd);
    printf("%d\n", fuel);
    return 0;
}