#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../libft/libft.h"

// ./libft_test <function> <input>
int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <function> <input>\n", argv[0]);
        return 1;
    }

    char *fn  = argv[1];
    char *arg = argv[2];

    if (strcmp(fn, "ft_atoi") == 0) {
        printf("%d\n", ft_atoi(arg));
        return 0;
    }

    if (strcmp(fn, "ft_strlen") == 0) {
        /* portable on Windows: avoid %zu */
        printf("%lu\n", (unsigned long)ft_strlen(arg));
        return 0;
    }

    fprintf(stderr, "Unknown function: %s\n", fn);
    return 1;
}
