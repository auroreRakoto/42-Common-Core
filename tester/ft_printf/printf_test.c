#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../ft_printf/ft_printf.h"   // <-- adapte le chemin selon ton repo

/*
 * This harness captures ft_printf output into a buffer and prints it to stdout,
 * so the Python tester can compare it easily.
 */

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <format> [args...]\n", argv[0]);
        return 1;
    }

    // We build a command string: the first arg is the format, next args are inputs
    const char *format = argv[1];

    // For simplicity, we limit tests to cases where args are just strings or integers.
    // You can extend this parser later if you want more specifiers.
    if (strstr(format, "%s")) {
        if (argc < 3) { fprintf(stderr, "Missing string arg\n"); return 1; }
        ft_printf(format, argv[2]);
    }
    else if (strstr(format, "%d") || strstr(format, "%i")) {
        if (argc < 3) { fprintf(stderr, "Missing int arg\n"); return 1; }
        int n = atoi(argv[2]);
        ft_printf(format, n);
    }
    else if (strstr(format, "%c")) {
        if (argc < 3) { fprintf(stderr, "Missing char arg\n"); return 1; }
        char c = argv[2][0];
        ft_printf(format, c);
    }
    else if (strstr(format, "%p")) {
        // For pointer tests, we just pass a fake pointer (the string arg as void*)
        if (argc < 3) { fprintf(stderr, "Missing ptr arg\n"); return 1; }
        void *p = argv[2];
        ft_printf(format, p);
    }
    else {
        // No args
        ft_printf(format);
    }

    return 0;
}
