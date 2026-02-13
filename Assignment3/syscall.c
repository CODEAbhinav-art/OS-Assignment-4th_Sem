// syscall_demo.c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int fd;
    pid_t pid;
    char buffer[100];

    // 1. File Operation System Calls (open, write)
    // O_CREAT: Create file if not exists
    // O_WRONLY: Write only
    // 0644: Permissions (rw-r--r--)
    fd = open("syscall_output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    
    if (fd < 0) {
        perror("Open failed");
        return 1;
    }

    sprintf(buffer, "Starting System Call Demo...\n");
    // write() is a direct system call, unlike printf()
    write(fd, buffer, strlen(buffer));

    // 2. Process Control System Call (fork)
    printf("Forking process now...\n");
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        // CHILD PROCESS
        sprintf(buffer, "Child Process: PID = %d\n", getpid());
        write(fd, buffer, strlen(buffer));
        
        // Simulate some work for 'top' command
        // This loop just burns CPU cycles for a few seconds
        long i; 
        for(i = 0; i < 500000000; i++); 
        
    } else {
        // PARENT PROCESS
        sprintf(buffer, "Parent Process: PID = %d, Child PID = %d\n", getpid(), pid);
        write(fd, buffer, strlen(buffer));
        
        // 3. Wait System Call
        wait(NULL); // Wait for child to finish
        sprintf(buffer, "Child finished. Parent exiting.\n");
        write(fd, buffer, strlen(buffer));
    }

    // 4. Close System Call
    close(fd);
    return 0;
}
