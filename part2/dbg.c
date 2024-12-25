#include "sys/user.h"
#include "time.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/ptrace.h"
#include "sys/wait.h"

void run_target(const char* programname)
{
    /* Allow tracing of this process */
    if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
        perror("ptrace");
        return;
    }

    /* Replace this process's image with the given program */
    execl(programname, programname, 0);
}

void run_debugger(pid_t child_pid)
{
    int wait_status;
    wait(&wait_status);

    unsigned long long target_addr = 0x40101b;
    unsigned long long data_og = ptrace(PTRACE_PEEKTEXT, child_pid, (void*)target_addr, 0);
    printf("\033[32mdata at 0x%llx: 0x%llx\033[0m\n", target_addr, data_og);

    unsigned long long int3 = 0x03CD; // LE
    unsigned long long data_trap = (data_og & 0xFFFFFFFFFFFF0000) | int3;
    ptrace(PTRACE_POKETEXT, child_pid, (void*)target_addr, (void*)data_trap);
    unsigned long long data_after = ptrace(PTRACE_PEEKTEXT, child_pid, (void*)target_addr, 0);
    printf("\033[32mdata at 0x%llx: 0x%llx\033[0m\n", target_addr, data_after);

    ptrace(PTRACE_CONT, child_pid, 0, 0);
    wait(&wait_status);

    if (WIFSTOPPED(wait_status)) {
        printf("\n\033[31;1mbreakpoint hit\033[0m\n");
        struct user_regs_struct regs;
        ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
        printf("\033[31;1mwe're at RIP = 0x%llx\033[0m\n", regs.rip);

        ptrace(PTRACE_POKETEXT, child_pid, (void*)target_addr, (void*)data_og);
        regs.rip -= 2;
        ptrace(PTRACE_SETREGS, child_pid, 0, &regs);
        ptrace(PTRACE_CONT, child_pid, 0, 0);

    } else {
        perror("wait");
        return;
    }
}

int main(int argc, char** argv)
{
    pid_t child_pid;

    if (argc < 2) {
        fprintf(stderr, "Expected a program name as argument\n");
        return -1;
    }
    child_pid = fork();
    if (child_pid == 0)
        run_target(argv[1]);
    else if (child_pid > 0)
        run_debugger(child_pid);
    else {
        perror("fork");
        return -1;
    }

    return 0;
}
