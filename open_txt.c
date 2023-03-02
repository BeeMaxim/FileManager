#include <unistd.h>
#include <sys/wait.h>

const char *get_extension() {
    return ".txt";
}

int open_file(const char *name) {
    pid_t pid = fork();

    if (pid == 0) {
        execlp("vim", "vim", "-f", "--nofork", name, NULL);
    }

    int status;
    waitpid(pid, &status, 0);

    return 0;
}