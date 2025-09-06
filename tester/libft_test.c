// tester/libft_test.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../libft/libft.h"

/* ---------- helpers ---------- */

static void hex_dump(const unsigned char *buf, size_t len) {
    static const char *hex = "0123456789abcdef";
    fputs("hex:", stdout);
    for (size_t i = 0; i < len; ++i) {
        unsigned char b = buf[i];
        putchar(hex[b >> 4]);
        putchar(hex[b & 0xF]);
    }
    putchar('\n');
}

static int all_zero(const unsigned char *p, size_t n) {
    for (size_t i = 0; i < n; ++i) if (p[i] != 0) return 0;
    return 1;
}

/* Parse "STRING,N" (no extra commas). Returns malloc'd bytes + out_len/out_n. */
static unsigned char* parse_str_and_n(const char *arg, size_t *out_len, size_t *out_n) {
    const char *comma = strrchr(arg, ',');
    if (!comma) return NULL;
    size_t s_len = (size_t)(comma - arg);
    char *s = (char*)malloc(s_len + 1);
    if (!s) return NULL;
    if (s_len) memcpy(s, arg, s_len);
    s[s_len] = '\0';

    long n = strtol(comma + 1, NULL, 10);
    if (n < 0) n = 0;

    unsigned char *buf = (unsigned char*)malloc(s_len);
    if (!buf) { free(s); return NULL; }
    if (s_len) memcpy(buf, s, s_len);

    *out_len = s_len;
    *out_n = (size_t)n;
    free(s);
    return buf;
}

/* split "A|B|C" into malloc'd array; returns count via *outc */
static char **split_fields(const char *s, size_t *outc) {
    size_t cap = 8, n = 0;
    char **arr = (char**)malloc(cap * sizeof(char*));
    if (!arr) return NULL;
    const char *p = s, *q;
    while (1) {
        q = strchr(p, '|');
        size_t len = q ? (size_t)(q - p) : strlen(p);
        char *field = (char*)malloc(len + 1);
        if (!field) { for (size_t i=0;i<n;i++) free(arr[i]); free(arr); return NULL; }
        if (len) memcpy(field, p, len);
        field[len] = '\0';
        if (n == cap) {
            cap *= 2;
            char **tmp = (char**)realloc(arr, cap * sizeof(char*));
            if (!tmp) { for (size_t i=0;i<n;i++) free(arr[i]); free(arr); free(field); return NULL; }
            arr = tmp;
        }
        arr[n++] = field;
        if (!q) break;
        p = q + 1;
    }
    *outc = n;
    return arr;
}
static void free_fields(char **a, size_t n){ for(size_t i=0;i<n;i++) free(a[i]); free(a); }

/* tolerant unsigned long parser */
static unsigned long uln(const char *s){ char *e; unsigned long v=strtoul(s,&e,10); return v; }

/* callbacks used by strmapi / striteri */
static char map_plus1(unsigned int i, char c){ (void)i; return (char)(c + 1); }
static void iter_plus1(unsigned int i, char *c){ (void)i; if (c) *c = (char)(*c + 1); }

/* ---------- dispatcher ---------- */
/*
 * Conventions (match your libft_data.txt lines):
 *
 * memchr:   "STRING|CHAR|N"      -> "idx:<i>" or "null"
 * memcmp:   "S1|S2|N"            -> "<int>"
 * memcpy:   "SRC|N"              -> dst = zeros(len=|SRC|), copy N from SRC, print hex of dst
 * memmove:  "STRING|FROM|TO|N"   -> in-place move, print hex of final buffer
 * memset:   "STRING|CH|N"        -> set first N bytes to CH, print hex
 *
 * put*fd:   print exactly what the function writes to fd=1 (stdout)
 *
 * split:    "STRING|SEP"         -> "split:a|b|c"
 * strchr:   "STRING|CHAR"        -> "idx:<i>" or "null"
 * strdup:   "STRING"             -> duplicated string
 * striteri: "STRING"             -> +1 on each char (in-place), print result
 * strjoin:  "S1|S2"              -> joined string
 * strlcat:  "DST|SRC|SIZE"       -> "ret:<n>;dst:<string>"
 * strlcpy:  "SRC|SIZE"           -> "ret:<n>;dst:<string>"
 * strlen:   "STRING"             -> number (with %lu cast)
 * strmapi:  "STRING"             -> +1 on each char (new string), print result
 * strncmp:  "S1|S2|N"            -> "<int>"
 * strnstr:  "HAY|NEEDLE|LEN"     -> "idx:<i>" or "null"
 * strrchr:  "STRING|CHAR"        -> "idx:<i>" or "null"
 * strtrim:  "STRING|SET"         -> trimmed string
 * substr:   "STRING|START|LEN"   -> substring result
 * tolower:  "C"                  -> single lowered char
 * toupper:  "C"                  -> single uppered char
 */

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <function> <input>\n", argv[0]);
        return 1;
    }
    const char *fn  = argv[1];
    const char *arg = argv[2];

    /* ----- simple returners ----- */
    if (strcmp(fn, "ft_atoi") == 0) { printf("%d\n", ft_atoi(arg)); return 0; }
    if (strcmp(fn, "ft_itoa") == 0) {
        int n = (int)strtol(arg, NULL, 10);
        char *s = ft_itoa(n);
        if (!s) { puts("(null)"); return 0; }
        puts(s); free(s); return 0;
    }
    if (strcmp(fn, "ft_strlen") == 0) { printf("%lu\n", (unsigned long)ft_strlen(arg)); return 0; }
    if (strcmp(fn, "ft_isalnum") == 0){ int c=(unsigned char)arg[0]; printf("%d\n", ft_isalnum(c)?1:0); return 0; }
    if (strcmp(fn, "ft_isalpha") == 0){ int c=(unsigned char)arg[0]; printf("%d\n", ft_isalpha(c)?1:0); return 0; }
    if (strcmp(fn, "ft_isascii") == 0){ int c=(unsigned char)arg[0]; printf("%d\n", ft_isascii(c)?1:0); return 0; }
    if (strcmp(fn, "ft_isdigit") == 0){ int c=(unsigned char)arg[0]; printf("%d\n", ft_isdigit(c)?1:0); return 0; }
    if (strcmp(fn, "ft_isprint") == 0){ int c=(unsigned char)arg[0]; printf("%d\n", ft_isprint(c)?1:0); return 0; }

    /* ----- memory ops ----- */
    if (strcmp(fn, "ft_bzero") == 0) {
        size_t L=0,N=0; unsigned char *buf = parse_str_and_n(arg,&L,&N);
        if (!buf) { puts("hex:"); return 0; }
        if (N > L) N = L;
        ft_bzero(buf, N); hex_dump(buf, L); free(buf); return 0;
    }
    if (strcmp(fn, "ft_calloc") == 0) {
        const char *comma = strrchr(arg, ',');
        if (!comma) { puts("zero"); return 0; }
        /* ft_calloc branch */
        long count = strtol(arg, NULL, 10);
        long size  = strtol(comma + 1, NULL, 10);
        /* before: if (count < 0) count = 0; if (size < 0) size = 0; */
        if (count < 0) count = 0;
        if (size  < 0) size  = 0;
        size_t total = (size_t)count * (size_t)size;
        void *p = ft_calloc((size_t)count, (size_t)size);
        if (!p) { puts("null"); return 0; }
        printf("%s\n", all_zero((const unsigned char*)p, total) ? "zero" : "nonzero");
        free(p); return 0;
    }
    if (strcmp(fn, "ft_memchr") == 0) {
        size_t nfields=0; char **f = split_fields(arg,&nfields);
        if (!f || nfields != 3) { puts("null"); if(f)free_fields(f,nfields); return 0; }
        size_t len = strlen(f[0]); unsigned char *buf = (unsigned char*)malloc(len?len:1);
        if (!buf) { free_fields(f,nfields); puts("null"); return 0; }
        if (len) memcpy(buf,f[0],len);
        int c = (unsigned char)f[1][0]; unsigned long n = uln(f[2]); if (n > len) n = len;
        void *p = ft_memchr(buf, c, (size_t)n);
        if (p) printf("idx:%lu\n", (unsigned long)((unsigned char*)p - buf)); else puts("null");
        free(buf); free_fields(f,nfields); return 0;
    }
    if (strcmp(fn, "ft_memcmp") == 0) {
        size_t nfields=0; char **f = split_fields(arg,&nfields);
        if (!f || nfields != 3) { puts("0"); if(f)free_fields(f,nfields); return 0; }
        unsigned long n = uln(f[2]); int r = ft_memcmp(f[0], f[1], (size_t)n);
        printf("%d\n", r); free_fields(f,nfields); return 0;
    }
    if (strcmp(fn, "ft_memcpy") == 0) {
        size_t nfields=0; char **f = split_fields(arg,&nfields);
        if (!f || nfields != 2) { puts("hex:"); if(f)free_fields(f,nfields); return 0; }
        size_t len = strlen(f[0]); unsigned long n = uln(f[1]); if (n > len) n = len;
        unsigned char *dst = (unsigned char*)calloc(len?len:1,1);
        if (!dst) { free_fields(f,nfields); puts("hex:"); return 0; }
        if (n) ft_memcpy(dst, f[0], (size_t)n);
        hex_dump(dst, len); free(dst); free_fields(f,nfields); return 0;
    }
    if (strcmp(fn, "ft_memmove") == 0) {
        size_t nfields=0; char **f = split_fields(arg,&nfields); /* "STRING|FROM|TO|N" */
        if (!f || nfields != 4) { puts("hex:"); if(f)free_fields(f,nfields); return 0; }
        size_t len = strlen(f[0]); unsigned char *buf = (unsigned char*)malloc(len?len:1);
        if (!buf) { free_fields(f,nfields); puts("hex:"); return 0; }
        if (len) memcpy(buf, f[0], len);
        unsigned long from = uln(f[1]), to = uln(f[2]), n = uln(f[3]);
        /* ft_memmove branch */
        /* before: if (from > len) from = len; if (to > len) to = len; */
        if (from > len) from = len;
        if (to   > len) to   = len;

        if (n > len) n = len;
        if (from + n > len) n = (unsigned long)(len - from);
        if (to   + n > len) n = (unsigned long)(len - to);
        if (n) ft_memmove(buf + to, buf + from, (size_t)n);
        hex_dump(buf, len); free(buf); free_fields(f,nfields); return 0;
    }
    if (strcmp(fn, "ft_memset") == 0) {
        size_t nfields=0; char **f = split_fields(arg,&nfields); /* "STRING|CH|N" */
        if (!f || nfields != 3) { puts("hex:"); if(f)free_fields(f,nfields); return 0; }
        size_t len = strlen(f[0]); unsigned char *buf = (unsigned char*)malloc(len?len:1);
        if (!buf) { free_fields(f,nfields); puts("hex:"); return 0; }
        if (len) memcpy(buf, f[0], len);
        int ch = (unsigned char)f[1][0]; unsigned long n = uln(f[2]); if (n > len) n = len;
        if (n) ft_memset(buf, ch, (size_t)n);
        hex_dump(buf, len); free(buf); free_fields(f,nfields); return 0;
    }

    /* ----- fd output ----- */
    if (strcmp(fn, "ft_putchar_fd") == 0) { ft_putchar_fd((unsigned char)arg[0], 1); return 0; }
    if (strcmp(fn, "ft_putendl_fd") == 0) { ft_putendl_fd((char*)arg, 1); return 0; }
    if (strcmp(fn, "ft_putnbr_fd")  == 0) { int n=(int)strtol(arg,NULL,10); ft_putnbr_fd(n,1); return 0; }
    if (strcmp(fn, "ft_putstr_fd")  == 0) { ft_putstr_fd((char*)arg, 1); return 0; }

    /* ----- strings ----- */
    if (strcmp(fn, "ft_split") == 0) {
        size_t nfields=0; char **f = split_fields(arg,&nfields); /* "STRING|SEP" */
        if (!f || nfields != 2) { puts("split:"); if(f)free_fields(f,nfields); return 0; }
        char **arr = ft_split(f[0], (unsigned char)f[1][0]);
        fputs("split:", stdout);
        if (arr) {
            for (size_t i=0; arr[i]; ++i) {
                if (i) putchar('|');
                fputs(arr[i], stdout);
                free(arr[i]);
            }
            free(arr);
        }
        putchar('\n'); free_fields(f,nfields); return 0;
    }
    if (strcmp(fn, "ft_strchr") == 0 || strcmp(fn, "ft_strrchr") == 0) {
        size_t nfields=0; char **f = split_fields(arg,&nfields); /* "STRING|CHAR" */
        if (!f || nfields != 2) { puts("null"); if(f)free_fields(f,nfields); return 0; }
        const char *s = f[0]; int c = (unsigned char)f[1][0];
        char *p = (strcmp(fn,"ft_strchr")==0) ? ft_strchr(s,c) : ft_strrchr(s,c);
        if (p) printf("idx:%lu\n", (unsigned long)(p - s)); else puts("null");
        free_fields(f,nfields); return 0;
    }
    if (strcmp(fn, "ft_strdup") == 0) {
        char *dup = ft_strdup(arg); if (!dup) { puts("(null)"); return 0; }
        puts(dup); free(dup); return 0;
    }
    if (strcmp(fn, "ft_striteri") == 0) {
        char *buf = strdup(arg ? arg : ""); if (!buf) { puts("(null)"); return 0; }
        ft_striteri(buf, iter_plus1); puts(buf); free(buf); return 0;
    }
    if (strcmp(fn, "ft_strjoin") == 0) {
        size_t nfields=0; char **f = split_fields(arg,&nfields); /* "S1|S2" */
        if (!f || nfields != 2) { puts("(null)"); if(f)free_fields(f,nfields); return 0; }
        char *res = ft_strjoin(f[0], f[1]); if (!res) puts("(null)"); else { puts(res); free(res); }
        free_fields(f,nfields); return 0;
    }
    if (strcmp(fn, "ft_strlcat") == 0) {
        size_t nfields=0; char **f = split_fields(arg,&nfields); /* "DST|SRC|SIZE" */
        if (!f || nfields != 3) { puts("ret:0;dst:"); if(f)free_fields(f,nfields); return 0; }
        const char *dst_in = f[0], *src = f[1]; unsigned long size = uln(f[2]);
        char *buf = (char*)malloc(size ? size : 1); if (!buf) { free_fields(f,nfields); puts("ret:0;dst:"); return 0; }
        if (size) {
            size_t in_len = strlen(dst_in);
            size_t copy = (size > 0) ? ((in_len < (size - 1)) ? in_len : (size - 1)) : 0;
            if (copy) memcpy(buf, dst_in, copy);
            buf[copy] = '\0';
        } else {
            buf[0] = '\0';
        }
        unsigned long ret = (unsigned long)ft_strlcat(buf, src, (size_t)size);
        printf("ret:%lu;dst:%s\n", ret, buf);
        free(buf); free_fields(f,nfields); return 0;
    }
    if (strcmp(fn, "ft_strlcpy") == 0) {
        size_t nfields=0; char **f = split_fields(arg,&nfields); /* "SRC|SIZE" */
        if (!f || nfields != 2) { puts("ret:0;dst:"); if(f)free_fields(f,nfields); return 0; }
        const char *src = f[0]; unsigned long size = uln(f[1]);
        char *buf = (char*)malloc(size ? size : 1); if (!buf) { free_fields(f,nfields); puts("ret:0;dst:"); return 0; }
        if (size) buf[0] = '\0';
        unsigned long ret = (unsigned long)ft_strlcpy(buf, src, (size_t)size);
        printf("ret:%lu;dst:%s\n", ret, buf);
        free(buf); free_fields(f,nfields); return 0;
    }
    if (strcmp(fn, "ft_strmapi") == 0) {
        char *res = ft_strmapi(arg ? arg : "", map_plus1);
        if (!res) { puts("(null)"); return 0; }
        puts(res); free(res); return 0;
    }
    if (strcmp(fn, "ft_strncmp") == 0) {
        size_t nfields=0; char **f = split_fields(arg,&nfields); /* "S1|S2|N" */
        if (!f || nfields != 3) { puts("0"); if(f)free_fields(f,nfields); return 0; }
        unsigned long n = uln(f[2]); int r = ft_strncmp(f[0], f[1], (size_t)n);
        printf("%d\n", r); free_fields(f,nfields); return 0;
    }
    if (strcmp(fn, "ft_strnstr") == 0) {
        size_t nfields=0; char **f = split_fields(arg,&nfields); /* "HAY|NEEDLE|LEN" */
        if (!f || nfields != 3) { puts("null"); if(f)free_fields(f,nfields); return 0; }
        const char *h=f[0], *n=f[1]; unsigned long L=uln(f[2]);
        char *p = ft_strnstr(h, n, (size_t)L);
        if (p) printf("idx:%lu\n", (unsigned long)(p - h)); else puts("null");
        free_fields(f,nfields); return 0;
    }
    if (strcmp(fn, "ft_strrchr") == 0) {
        size_t nfields=0; char **f = split_fields(arg,&nfields); /* "STRING|CHAR" */
        if (!f || nfields != 2) { puts("null"); if(f)free_fields(f,nfields); return 0; }
        const char *s=f[0]; int c=(unsigned char)f[1][0];
        char *p=ft_strrchr(s,c);
        if (p) printf("idx:%lu\n", (unsigned long)(p - s)); else puts("null");
        free_fields(f,nfields); return 0;
    }
    if (strcmp(fn, "ft_strtrim") == 0) {
        size_t nfields=0; char **f = split_fields(arg,&nfields); /* "STRING|SET" */
        if (!f || nfields != 2) { puts("(null)"); if(f)free_fields(f,nfields); return 0; }
        char *res = ft_strtrim(f[0], f[1]); if (!res) puts("(null)"); else { puts(res); free(res); }
        free_fields(f,nfields); return 0;
    }
    if (strcmp(fn, "ft_substr") == 0) {
        size_t nfields=0; char **f = split_fields(arg,&nfields); /* "STRING|START|LEN" */
        if (!f || nfields != 3) { puts("(null)"); if(f)free_fields(f,nfields); return 0; }
        unsigned long start = uln(f[1]), len = uln(f[2]);
        char *res = ft_substr(f[0], (unsigned int)start, (size_t)len);
        if (!res) puts("(null)"); else { puts(res); free(res); }
        free_fields(f,nfields); return 0;
    }
    if (strcmp(fn, "ft_tolower") == 0) { int c=(unsigned char)arg[0]; putchar(ft_tolower(c)); putchar('\n'); return 0; }
    if (strcmp(fn, "ft_toupper") == 0) { int c=(unsigned char)arg[0]; putchar(ft_toupper(c)); putchar('\n'); return 0; }

    fprintf(stderr, "Unknown function: %s\n", fn);
    return 1;
}
