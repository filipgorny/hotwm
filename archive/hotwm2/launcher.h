#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void launcher_launch() {
    pid_t wm_pid, bar_pid;

    wm_pid = fork();
    exec("./wm");

    bar_pid = fork();
    exec("./bar");
}
