#include <stdio.h>
#include <unistd.h>

int main()
{
    int pid;
    
    // Printing details about the original process (parent)
    printf("I'm the original process with PID %d and PPID %d.\n", getpid(), getppid());

    // Creating a new process using fork
    pid = fork();

    /* Duplicate. Child and parent continue from here. */
    if (pid != 0)  // Parent process: pid is non-zero
    {
        printf("I'm the parent process with PID %d and PPID %d.\n", getpid(), getppid());
        printf("My child's PID is %d.\n", pid);
    }
    else  // Child process: pid is zero
    {
        printf("I'm the child process with PID %d and PPID %d.\n", getpid(), getppid());
    }

    // Both processes execute this
    printf("PID %d terminates.\n", getpid());

    return 0;
}
