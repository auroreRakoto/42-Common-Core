#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../ft_printf/ft_printf.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <format> [args...]\n", argv[0]);
        return 1;
    }

    const char *format = argv[1];
    int argi = 2; // index in argv

    for (const char *p = format; *p; p++) {
        if (*p == '%' && *(p + 1)) {
            p++;
            switch (*p) {
                case 's':
                    if (argi >= argc) { fprintf(stderr, "Missing string arg\n"); return 1; }
                    ft_printf("%s", argv[argi++]);
                    break;
                case 'd': case 'i':
                    if (argi >= argc) { fprintf(stderr, "Missing int arg\n"); return 1; }
                    ft_printf("%d", atoi(argv[argi++]));
                    break;
                case 'u':
                    if (argi >= argc) { fprintf(stderr, "Missing unsigned arg\n"); return 1; }
                    ft_printf("%u", (unsigned)atoi(argv[argi++]));
                    break;
                case 'x':
                case 'X':
                    if (argi >= argc) { fprintf(stderr, "Missing hex arg\n"); return 1; }
                    ft_printf(*p == 'x' ? "%x" : "%X", atoi(argv[argi++]));
                    break;
                case 'c':
                    if (argi >= argc) { fprintf(stderr, "Missing char arg\n"); return 1; }
                    ft_printf("%c", argv[argi++][0]);
                    break;
                case 'p':
                    if (argi >= argc) { fprintf(stderr, "Missing ptr arg\n"); return 1; }
                    ft_printf("%p", argv[argi++]); // crude, but ok for test
                    break;
                case '%':
                    ft_printf("%%");
                    break;
                default:
                    // Just print as-is if unknown
                    write(1, p - 1, 2);
                    break;
            }
        } else {
            write(1, p, 1);
        }
    }

    return 0;
}
