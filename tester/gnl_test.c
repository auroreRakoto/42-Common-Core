// gnl_test.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// Adjust include path if your header is elsewhere:
#include "../gnl/get_next_line.h"

static void out(const char *tag, const char *s){
    fputs(tag, stdout); fputc(':', stdout);
    if (s) fputs(s, stdout);
    fputc('\n', stdout);
}

static int run_file(const char *path){
    int fd = open(path, O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }
    char *line = NULL;
    int r;
    while ((r = get_next_line(fd, &line)) == 1){
        out("L", line);
        free(line); line = NULL;
    }
    if (r < 0) out("ERR", "read");
    if (line) free(line);
    close(fd);
    return (r < 0);
}

static int run_multifile(const char *p1, const char *p2){
    int f1 = open(p1, O_RDONLY), f2 = open(p2, O_RDONLY);
    if (f1 < 0 || f2 < 0){ perror("open"); if (f1>=0) close(f1); if (f2>=0) close(f2); return 1; }
    char *l1=NULL,*l2=NULL; int r1=1,r2=1;
    while (r1>0 || r2>0){
        if (r1>0){ r1=get_next_line(f1,&l1); if (r1==1){ out("1",l1); free(l1); l1=NULL; } else if (r1<0){ out("ERR","f1"); break; } }
        if (r2>0){ r2=get_next_line(f2,&l2); if (r2==1){ out("2",l2); free(l2); l2=NULL; } else if (r2<0){ out("ERR","f2"); break; } }
    }
    if (l1) free(l1); if (l2) free(l2); close(f1); close(f2); return 0;
}

int main(int ac, char **av){
    if (ac==3 && strcmp(av[1],"file")==0)      return run_file(av[2]);
    if (ac==4 && strcmp(av[1],"multifile")==0) return run_multifile(av[2], av[3]);
    fprintf(stderr,"Usage:\n  %s file <path>\n  %s multifile <p1> <p2>\n", av[0], av[0]);
    return 1;
}
