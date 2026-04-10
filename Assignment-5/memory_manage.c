#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

/* Data structure to pass to threads */
typedef struct {
    int *blocks;
    int *processes;
    int *pages;
    int m, n, f, total_pages;
    char name[20];
} AlgoData;

pthread_mutex_t print_lock;

/* ---------------- BEST FIT THREAD ---------------- */
void* bestFitThread(void* arg) {
    AlgoData *d = (AlgoData*)arg;
    int *allocation = malloc(d->n * sizeof(int));
    for (int i = 0; i < d->n; i++) allocation[i] = -1;

    for (int i = 0; i < d->n; i++) {
        int bestIdx = -1;
        for (int j = 0; j < d->m; j++) {
            if (d->blocks[j] >= d->processes[i]) {
                if (bestIdx == -1 || d->blocks[j] < d->blocks[bestIdx])
                    bestIdx = j;
            }
        }
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            d->blocks[bestIdx] -= d->processes[i];
        }
    }

    pthread_mutex_lock(&print_lock);
    printf("\n--- %s ---\n", d->name);
    for (int i = 0; i < d->n; i++)
        printf("Process %d (%d) -> Block %d\n", i+1, d->processes[i], allocation[i]+1);
    pthread_mutex_unlock(&print_lock);

    free(allocation);
    return NULL;
}

/* ---------------- FIFO THREAD ---------------- */
void* fifoThread(void* arg) {
    AlgoData *d = (AlgoData*)arg;
    int *frames = malloc(d->f * sizeof(int));
    int front = 0, faults = 0;

    for (int i = 0; i < d->f; i++) frames[i] = -1;

    pthread_mutex_lock(&print_lock);
    printf("\n--- %s ---\n", d->name);
    for (int i = 0; i < d->total_pages; i++) {
        int found = 0;
        for (int j = 0; j < d->f; j++) {
            if (frames[j] == d->pages[i]) { found = 1; break; }
        }
        if (!found) {
            frames[front] = d->pages[i];
            front = (front + 1) % d->f;
            faults++;
        }
    }
    printf("Total Page Faults (FIFO) = %d\n", faults);
    pthread_mutex_unlock(&print_lock);

    free(frames);
    return NULL;
}

/* ---------------- MAIN ---------------- */
int main() {
    int m = 5, n = 4, f = 3, total_pages = 12;

    // KEYWORD: malloc - Allocating memory on the HEAP dynamically
    int *orig_blocks = (int*)malloc(m * sizeof(int));
    int *processes = (int*)malloc(n * sizeof(int));
    int *pages = (int*)malloc(total_pages * sizeof(int));

    int b_init[] = {100, 500, 200, 300, 600};
    int p_init[] = {212, 417, 112, 426};
    int pg_init[] = {1,2,3,4,1,2,5,1,2,3,4,5};

    memcpy(orig_blocks, b_init, m * sizeof(int));
    memcpy(processes, p_init, n * sizeof(int));
    memcpy(pages, pg_init, total_pages * sizeof(int));

    pthread_t threads[2];
    AlgoData d1, d2;
    pthread_mutex_init(&print_lock, NULL);

    // Setup for Best Fit Thread
    d1.m = m; d1.n = n;
    d1.blocks = (int*)malloc(m * sizeof(int)); // Give thread its own copy
    memcpy(d1.blocks, orig_blocks, m * sizeof(int));
    d1.processes = processes;
    strcpy(d1.name, "Best Fit");

    // Setup for FIFO Thread
    d2.f = f; d2.total_pages = total_pages;
    d2.pages = pages;
    strcpy(d2.name, "FIFO");

    pthread_create(&threads[0], NULL, bestFitThread, &d1);
    pthread_create(&threads[1], NULL, fifoThread, &d2);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    // KEYWORD: free - Releasing the heap memory to prevent memory leaks
    free(orig_blocks); free(processes); free(pages);
    free(d1.blocks);
    pthread_mutex_destroy(&print_lock);

    return 0;
}
