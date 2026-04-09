#include <stdio.h>

#define MAX 50

/* ---------------- BEST FIT ---------------- */

void bestFit(int blocks[], int m, int processes[], int n) {
    int allocation[MAX];
    for (int i = 0; i < n; i++) allocation[i] = -1;

    for (int i = 0; i < n; i++) {
        int bestIdx = -1;

        for (int j = 0; j < m; j++) {
            if (blocks[j] >= processes[i]) {
                if (bestIdx == -1 || blocks[j] < blocks[bestIdx])
                    bestIdx = j;
            }
        }

        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blocks[bestIdx] -= processes[i];
        }
    }

    printf("\n--- Best Fit ---\n");
    for (int i = 0; i < n; i++)
        printf("Process %d -> Block %d\n", i+1, allocation[i]+1);
}

/* ---------------- WORST FIT ---------------- */

void worstFit(int blocks[], int m, int processes[], int n) {
    int allocation[MAX];
    for (int i = 0; i < n; i++) allocation[i] = -1;

    for (int i = 0; i < n; i++) {
        int worstIdx = -1;

        for (int j = 0; j < m; j++) {
            if (blocks[j] >= processes[i]) {
                if (worstIdx == -1 || blocks[j] > blocks[worstIdx])
                    worstIdx = j;
            }
        }

        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blocks[worstIdx] -= processes[i];
        }
    }

    printf("\n--- Worst Fit ---\n");
    for (int i = 0; i < n; i++)
        printf("Process %d -> Block %d\n", i+1, allocation[i]+1);
}

/* ---------------- PAGING ---------------- */

void paging(int pages[], int n, int frames[], int f) {
    printf("\n--- Paging (Page → Frame Mapping) ---\n");

    for (int i = 0; i < n; i++) {
        printf("Page %d -> Frame %d\n", pages[i], frames[i % f]);
    }
}

/* ---------------- FIFO ---------------- */

void fifo(int pages[], int n, int f) {
    int frames[MAX], front = 0, faults = 0;

    for (int i = 0; i < f; i++) frames[i] = -1;

    printf("\n--- FIFO Page Replacement ---\n");

    for (int i = 0; i < n; i++) {
        int found = 0;

        for (int j = 0; j < f; j++) {
            if (frames[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        if (!found) {
            frames[front] = pages[i];
            front = (front + 1) % f;
            faults++;
        }

        printf("Frames: ");
        for (int j = 0; j < f; j++)
            printf("%d ", frames[j]);
        printf("\n");
    }

    printf("Total Page Faults = %d\n", faults);
}

/* ---------------- LRU ---------------- */

void lru(int pages[], int n, int f) {
    int frames[MAX], time[MAX];
    int faults = 0, counter = 0;

    for (int i = 0; i < f; i++) {
        frames[i] = -1;
        time[i] = 0;
    }

    printf("\n--- LRU Page Replacement ---\n");

    for (int i = 0; i < n; i++) {
        int found = 0;

        for (int j = 0; j < f; j++) {
            if (frames[j] == pages[i]) {
                counter++;
                time[j] = counter;
                found = 1;
                break;
            }
        }

        if (!found) {
            int minTime = 1e9, pos = 0;

            for (int j = 0; j < f; j++) {
                if (time[j] < minTime) {
                    minTime = time[j];
                    pos = j;
                }
            }

            frames[pos] = pages[i];
            counter++;
            time[pos] = counter;
            faults++;
        }

        printf("Frames: ");
        for (int j = 0; j < f; j++)
            printf("%d ", frames[j]);
        printf("\n");
    }

    printf("Total Page Faults = %d\n", faults);
}

/* ---------------- MAIN ---------------- */

int main() {

    int blocks[] = {100, 500, 200, 300, 600};
    int processes[] = {212, 417, 112, 426};

    int pages[] = {1,2,3,4,1,2,5,1,2,3,4,5};

    int frames[] = {10, 11, 12};

    int m = 5, n = 4, f = 3;

    int b1[MAX], b2[MAX];

    for (int i = 0; i < m; i++) {
        b1[i] = blocks[i];
        b2[i] = blocks[i];
    }

    bestFit(b1, m, processes, n);
    worstFit(b2, m, processes, n);

    paging(pages, 12, frames, f);

    fifo(pages, 12, f);
    lru(pages, 12, f);

    return 0;
}
