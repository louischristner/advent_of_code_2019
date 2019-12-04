#include <stdio.h>
#include <stdlib.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))
#define man_d(a, b) (abs(a) + abs(b))

typedef struct s_point point_t;
struct s_point {
    int x;
    int y;
    point_t *next;
    point_t *prev;
};

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

char *replace(char *str, char replaced, char replacor)
{
    for (int i = 0; str[i]; i++)
        if (str[i] == replaced)
            str[i] = replacor;
    return str;
}

int my_strlen(char *str)
{
    int len = 0;

    if (!str)
        return 0;
    for (; str[len]; len++);
    return len;
}

char **get_lines(FILE *stream)
{
    char **tab = NULL;
    char *buf = NULL;
    size_t len = 0;

    getline(&buf, &len, stream);
    replace(buf, '\n', '\0');
    tab = my_str_to_tab(buf, ',');
    return tab;
}

int get_nbr(char *instr)
{
    char *cpy = malloc(sizeof(char) * my_strlen(instr));

    for (int i = 1; instr[i]; i++)
        cpy[i - 1] = instr[i];
    cpy[my_strlen(instr) - 1] = '\0';
    return atoi(cpy);
}

point_t *new_point(point_t *point, int x, int y)
{
    point_t *new = malloc(sizeof(point_t) * 1);

    if (!new)
        return NULL;
    new->x = x;
    new->y = y;
    new->next = point;
    new->prev = NULL;
    if (point)
        point->prev = new;
    return new;
}

point_t *compute(point_t *point, FILE *stream)
{
    int x = 0;
    int y = 0;
    char **instr = get_lines(stream);

    for (int i = 0; instr[i]; i++) {
        switch (instr[i][0]) {
            case 'R' : x = get_nbr(instr[i]); y = 0; break;
            case 'L' : x = -1 * get_nbr(instr[i]); y = 0; break;
            case 'U' : y = get_nbr(instr[i]); x = 0; break;
            case 'D' : y = -1 * get_nbr(instr[i]); x = 0; break;
            default : break;
        }
        point = new_point(point, point->x + x, point->y + y);
    }
    return point;
}

int tt(point_t *first, point_t *second, point_t *result)
{
    if (first->x == first->next->x) {
        if (second->x == second->next->x) {
            return 0;
        } else {
            if (first->x > min(second->x, second->next->x)
                && first->x < max(second->x, second->next->x)
                && second->y > min(first->y, first->next->y)
                && second->y < max(first->y, first->next->y)) {
                (*result).x = abs(first->x);
                (*result).y = abs(second->y);
            } else {
                return 0;
            }
        }
    } else if (first->y == first->next->y) {
        if (second->y == second->next->y) {
            return 0;
        } else {
            if (second->x > min(first->x, first->next->x)
                && second->x < max(first->x, first->next->x)
                && first->y > min(second->y, second->next->y)
                && first->y < max(second->y, second->next->y)) {
                (*result).x = abs(second->x);
                (*result).y = abs(first->y);
            } else {
                return 0;
            }
        }
    }
    return 1;
}

void calcul(point_t *first, point_t *second)
{
    point_t *inter = NULL;
    point_t *tmp_f = first;
    point_t *tmp_s = second;
    point_t result = {__INT32_MAX__, __INT32_MAX__, NULL, NULL};

    for (; first->next; first = first->next) {
        for (; second->next; second = second->next) {
            if (tt(first, second, &result))
                inter = new_point(inter, result.x, result.y);
        }
        second = tmp_s;
    }
    first = tmp_f;
    for (; inter; inter = inter->next) {
        if (man_d(inter->x, inter->y) < man_d(result.x, result.y)) {
            result.x = inter->x;
            result.y = inter->y;
            printf("x=%d y=%d\n", result.x, result.y);
        }
    }
    printf("RESULT: %d\n", man_d(result.x, result.y));
}

int main(int ac, char **av)
{
    point_t *first = new_point(NULL, 0, 0);
    point_t *second = new_point(NULL, 0, 0);
    FILE *stream = fopen(av[1], "r");

    if (!stream)
        return 84;
    first = compute(first, stream);
    second = compute(second, stream);
    calcul(first, second);
    return 0;
}