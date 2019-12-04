#include <stdio.h>
#include <stdlib.h>

int check_if_password(int nbr)
{
    int doubled = 0;
    char psswrd[7] = {
        (nbr / 100000) + '0',
        ((nbr % 100000) / 10000) + '0',
        ((nbr % 10000) / 1000) + '0',
        ((nbr % 1000) / 100) + '0',
        ((nbr % 100) / 10) + '0',
        ((nbr % 10) + '0'),
        '\0'
    };

    for (int i = 0; psswrd[i + 1]; i++) {
        if (psswrd[i] > psswrd[i + 1])
            return 0;
        if (psswrd[i] == psswrd[i + 1])
            doubled = 1;
    }
    if (doubled)
        return 1;
    return 0;
}

int main(void)
{
    int count = 0;
    int start = 234208;
    int end = 765869;

    for (int i = start; i < end; i++)
        count += check_if_password(i);
    printf("COUNT=%d\n", count);
    return 0;
}