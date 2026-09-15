#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"

void find(char* path, char* name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    fd = open(path, 0);
    if (fd < 0)
    {
        fprintf(2, "find: cannot find %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot find %s\n", path);
        close(fd);
        return;
    }
    if (st.type != T_DIR) {
        close(fd);
        return; 
    }

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
        printf("find: path too long\n");
        }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (!strcmp(de.name, ".") || !strcmp(de.name, "..") || de.inum == 0) continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (!strcmp(de.name, name))
        {
            printf("%s\n", buf);
        }
        if (stat(buf, &st) < 0)
        {
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        if (st.type == T_DIR)
        {
            find(buf, name);
        }
    }
    close(fd);
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("find needs two arguments!\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}