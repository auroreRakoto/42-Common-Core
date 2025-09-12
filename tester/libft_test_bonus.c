#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helpers déjà proposés
static t_list *list_from_csv(const char *csv) {
    if (!csv || !*csv) return NULL;
    t_list *head = NULL, *tail = NULL;
    const char *p = csv, *q;
    while (1) {
        q = strchr(p, ',');
        size_t len = q ? (size_t)(q - p) : strlen(p);
        char *dup = (char *)malloc(len + 1);
        memcpy(dup, p, len);
        dup[len] = '\0';
        t_list *node = ft_lstnew(dup);
        if (!node) { free(dup); ft_lstclear(&head, free); return NULL; }
        if (!head) head = tail = node;
        else { tail->next = node; tail = node; }
        if (!q) break;
        p = q + 1;
    }
    return head;
}

static void list_print(const char *prefix, t_list *lst) {
    fputs(prefix, stdout);
    for (size_t i = 0; lst; lst = lst->next, ++i) {
        if (i) putchar('|');
        fputs((char*)lst->content, stdout);
    }
    putchar('\n');
}

static void *dup_upper(void *p) {
    const char *s = p;
    size_t n = strlen(s);
    char *r = malloc(n + 1);
    for (size_t i = 0; i < n; i++) r[i] = toupper((unsigned char)s[i]);
    r[n] = '\0';
    return r;
}
static void iter_upper(void *p) {
    char *s = p;
    for (; *s; s++) *s = toupper((unsigned char)*s);
}
static void del_str(void *p) { free(p); }

// portable strndup replacement (works on all platforms)
static char *my_strndup(const char *s, size_t n) {
    char *p = (char *)malloc(n + 1);
    if (!p) return NULL;
    memcpy(p, s, n);
    p[n] = '\0';
    return p;
}

// Main dispatcher
int main(int argc, char **argv) {
    if (argc < 2) { puts("usage: libft_test_bonus FUNCTION [ARGS]"); return 0; }
    const char *fn = argv[1];
    const char *arg = (argc > 2 ? argv[2] : "");

    if (strcmp(fn, "ft_lstnew_bonus") == 0) {
        t_list *n = ft_lstnew(strdup(arg));
        list_print("list:", n);
        free(n->content); free(n);
        return 0;
    }
    if (strcmp(fn, "ft_lstsize_bonus") == 0) {
        t_list *lst = list_from_csv(arg);
        printf("size:%d\n", ft_lstsize(lst));
        ft_lstclear(&lst, del_str);
        return 0;
    }
    if (strcmp(fn, "ft_lstlast_bonus") == 0) {
        t_list *lst = list_from_csv(arg);
        t_list *last = ft_lstlast(lst);
        printf("last:%s\n", last ? (char*)last->content : "");
        ft_lstclear(&lst, del_str);
        return 0;
    }
    if (strcmp(fn, "ft_lstadd_front_bonus") == 0) {
        char *sep = strrchr(arg, '|');
        if (!sep) { puts("list:"); return 0; }
        char *left = my_strndup(arg, (size_t)(sep - arg));
        char *front = strdup(sep + 1);
        t_list *lst = list_from_csv(left);
        t_list *node = ft_lstnew(front);
        ft_lstadd_front(&lst, node);
        list_print("list:", lst);
        ft_lstclear(&lst, del_str);
        free(left);
        return 0;
    }
    if (strcmp(fn, "ft_lstadd_back_bonus") == 0) {
        char *sep = strrchr(arg, '|');
        if (!sep) { puts("list:"); return 0; }
        char *left = my_strndup(arg, (size_t)(sep - arg));
        char *back = strdup(sep + 1);
        t_list *lst = list_from_csv(left);
        t_list *node = ft_lstnew(back);
        ft_lstadd_back(&lst, node);
        list_print("list:", lst);
        ft_lstclear(&lst, del_str);
        free(left);
        return 0;
    }
    if (strcmp(fn, "ft_lstdelone_bonus") == 0)
    {
        t_list *n = ft_lstnew(strdup(arg));
        ft_lstdelone(n, free);
        puts("freed:1");
        return 0;
    }
    if (strcmp(fn, "ft_lstclear_bonus") == 0) {
        t_list *lst = list_from_csv(arg);
        int before = ft_lstsize(lst);
        ft_lstclear(&lst, del_str);
        printf("freed:%d\n", before);
        return 0;
    }
    if (strcmp(fn, "ft_lstiter_bonus") == 0) {
        t_list *lst = list_from_csv(arg);
        ft_lstiter(lst, iter_upper);
        list_print("list:", lst);
        ft_lstclear(&lst, del_str);
        return 0;
    }
    if (strcmp(fn, "ft_lstmap_bonus") == 0) {
        t_list *lst = list_from_csv(arg);
        t_list *newlst = ft_lstmap(lst, dup_upper, del_str);
        list_print("list:", newlst);
        ft_lstclear(&lst, del_str);
        ft_lstclear(&newlst, del_str);
        return 0;
    }

    puts("unknown fn");
    return 0;
}
