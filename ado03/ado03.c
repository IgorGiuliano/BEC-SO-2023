#include <stdio.h>
#include <sys/utsname.h>

int main(void) {
    struct utsname buf;

    if (uname(&buf) == -1) {
        perror("uname");
        return 1;
    }

    printf("System name: %s\n", buf.sysname);
    printf("Node name: %s\n", buf.nodename);
    printf("Release: %s\n", buf.release);
    printf("Version: %s\n", buf.version);
    printf("Machine: %s\n", buf.machine);

    return 0;
}