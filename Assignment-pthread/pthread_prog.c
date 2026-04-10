#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 3

FILE *fp;
pthread_mutex_t lock;

/* ----------- Function: Find Max ----------- */
int findMax(int a, int b) {
    return (a > b) ? a : b;
}

/* ----------- Function: Swap using pointers ----------- */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* ----------- Thread Function ----------- */
void* threadFunc(void* arg) {
    int id = *(int*)arg;

    int x = id * 10;
    int y = id * 20;

    int maxVal = findMax(x, y);

    pthread_mutex_lock(&lock);

    fprintf(fp, "\n[Thread %d]\n", id);
    fprintf(fp, "Initial x=%d, y=%d\n", x, y);

    fprintf(fp, "Address of x: %p\n", &x);
    fprintf(fp, "Address of y: %p\n", &y);

    fprintf(fp, "Max value: %d\n", maxVal);

    /* For loop */
    for (int i = 0; i < 3; i++) {
        fprintf(fp, "Thread %d - For Loop i=%d\n", id, i);
    }

    /* While loop */
    int j = 0;
    while (j < 2) {
        fprintf(fp, "Thread %d - While Loop j=%d\n", id, j);
        j++;
    }

    /* Swap using pointers */
    swap(&x, &y);

    fprintf(fp, "After swap x=%d, y=%d\n", x, y);

    pthread_mutex_unlock(&lock);

    return NULL;
}

/* ----------- MAIN ----------- */
int main() {

    pthread_t threads[THREADS];
    int ids[THREADS];

    /* Open file with buffering */
    fp = fopen("output.txt", "w");

    if (fp == NULL) {
        printf("File open error\n");
        return 1;
    }

    /* Initialize mutex */
    pthread_mutex_init(&lock, NULL);

    /* Create threads */
    for (int i = 0; i < THREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, threadFunc, &ids[i]);
    }

    /* Join threads */
    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Cleanup */
    pthread_mutex_destroy(&lock);
    fclose(fp);

    printf("Execution completed. Check output.txt\n");

    return 0;
}
