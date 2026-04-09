#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    // Step 1: Check argument
    if (argc != 2) {
        printf("Usage: %s <positive integer>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    if (n <= 0) {
        printf("Please enter a positive integer\n");
        return 1;
    }

    // Step 2: Create child
    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
        return 1;
    }

    // Step 3: Child process
    else if (pid == 0) {
        printf("Child Process (PID: %d): ", getpid());

        while (n > 0) {
            printf("%d ", n);
            n = n / 2;
        }
        printf("\n");
    }

    // Step 4: Parent process
    else {
        wait(NULL);
        printf("Parent Process (PID: %d): Child completed\n", getpid());
    }

    return 0;
}
