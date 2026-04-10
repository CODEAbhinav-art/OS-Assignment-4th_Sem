#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define MAX 200
#define QUANTUM 2

typedef struct {
    int pid, arrival, burst, remaining, priority;
    int completion, waiting, turnaround, response;
    int started;
} Process;

typedef struct {
    Process p[MAX];
    int n;
    int algo;   // 1 FCFS, 2 SJF, 3 SRTN, 4 RR, 5 PRIORITY
} ThreadData;

pthread_mutex_t lock;
FILE *fp;

/* ---------------- Utility ---------------- */

void copyProcesses(Process src[], Process dest[], int n) {
    for (int i = 0; i < n; i++) dest[i] = src[i];
}

void generateProcesses(Process p[], int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        p[i].arrival = rand() % 5;
        p[i].burst = (rand() % 10) + 1;
        p[i].remaining = p[i].burst;
        p[i].priority = rand() % 10;
        p[i].started = 0;
    }
}

/* ---------------- Algorithms ---------------- */

void fcfs(Process p[], int n) {
    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival) time = p[i].arrival;
        p[i].response = time - p[i].arrival;
        time += p[i].burst;
        p[i].completion = time;
        p[i].turnaround = time - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;
    }
}

void sjf(Process p[], int n) {
    int done = 0, time = 0;
    while (done < n) {
        int idx = -1, min = 1e9;
        for (int i = 0; i < n; i++) {
            if (p[i].remaining > 0 && p[i].arrival <= time && p[i].burst < min) {
                min = p[i].burst;
                idx = i;
            }
        }
        if (idx == -1) { time++; continue; }
        p[idx].response = time - p[idx].arrival;
        time += p[idx].burst;
        p[idx].remaining = 0;
        p[idx].completion = time;
        p[idx].turnaround = time - p[idx].arrival;
        p[idx].waiting = p[idx].turnaround - p[idx].burst;
        done++;
    }
}

void srtn(Process p[], int n) {
    int done = 0, time = 0;
    while (done < n) {
        int idx = -1, min = 1e9;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && p[i].remaining > 0 && p[i].remaining < min) {
                min = p[i].remaining;
                idx = i;
            }
        }
        if (idx == -1) { time++; continue; }
        if (!p[idx].started) {
            p[idx].response = time - p[idx].arrival;
            p[idx].started = 1;
        }
        p[idx].remaining--;
        time++;
        if (p[idx].remaining == 0) {
            done++;
            p[idx].completion = time;
            p[idx].turnaround = time - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
        }
    }
}

void rr(Process p[], int n) {
    int queue[MAX], front = 0, rear = 0;
    int visited[MAX] = {0};
    int time = 0, done = 0;

    queue[rear++] = 0;
    visited[0] = 1;

    while (done < n) {
        int i = queue[front++];

        if (!p[i].started) {
            p[i].response = time - p[i].arrival;
            p[i].started = 1;
        }

        int exec = (p[i].remaining < QUANTUM) ? p[i].remaining : QUANTUM;
        p[i].remaining -= exec;
        time += exec;

        for (int j = 0; j < n; j++) {
            if (!visited[j] && p[j].arrival <= time) {
                queue[rear++] = j;
                visited[j] = 1;
            }
        }

        if (p[i].remaining > 0) {
            queue[rear++] = i;
        } else {
            done++;
            p[i].completion = time;
            p[i].turnaround = time - p[i].arrival;
            p[i].waiting = p[i].turnaround - p[i].burst;
        }
    }
}

void priorityScheduling(Process p[], int n) {
    int done = 0, time = 0;
    while (done < n) {
        int idx = -1, best = 1e9;
        for (int i = 0; i < n; i++) {
            if (p[i].remaining > 0 && p[i].arrival <= time && p[i].priority < best) {
                best = p[i].priority;
                idx = i;
            }
        }
        if (idx == -1) { time++; continue; }
        p[idx].response = time - p[idx].arrival;
        time += p[idx].burst;
        p[idx].remaining = 0;
        p[idx].completion = time;
        p[idx].turnaround = time - p[idx].arrival;
        p[idx].waiting = p[idx].turnaround - p[idx].burst;
        done++;
    }
}

/* ---------------- Output ---------------- */

void printResults(Process p[], int n, char *name) {
    float avgWT=0, avgTAT=0, avgRT=0;
    int totalBurst=0, last=0;

    pthread_mutex_lock(&lock);

    printf("\n--- %s ---\n", name);
    fprintf(fp, "\\section*{%s}\n", name);

    for (int i=0;i<n;i++) {
        avgWT+=p[i].waiting;
        avgTAT+=p[i].turnaround;
        avgRT+=p[i].response;
        totalBurst+=p[i].burst;
        if(p[i].completion>last) last=p[i].completion;
    }

    float cpu = (float)totalBurst/last*100;

    printf("Avg WT=%.2f Avg TAT=%.2f Avg RT=%.2f CPU=%.2f%%\n",
            avgWT/n, avgTAT/n, avgRT/n, cpu);

    fprintf(fp,"AvgWT=%.2f AvgTAT=%.2f AvgRT=%.2f CPU=%.2f\\%%\n\n",
            avgWT/n, avgTAT/n, avgRT/n, cpu);

    pthread_mutex_unlock(&lock);
}

/* ---------------- Thread ---------------- */

void* runAlgo(void* arg) {
    ThreadData *d = (ThreadData*)arg;

    switch(d->algo) {
        case 1: fcfs(d->p,d->n); printResults(d->p,d->n,"FCFS"); break;
        case 2: sjf(d->p,d->n); printResults(d->p,d->n,"SJF"); break;
        case 3: srtn(d->p,d->n); printResults(d->p,d->n,"SRTN"); break;
        case 4: rr(d->p,d->n); printResults(d->p,d->n,"RR"); break;
        case 5: priorityScheduling(d->p,d->n); printResults(d->p,d->n,"PRIORITY"); break;
    }
    pthread_exit(NULL);
}

/* ---------------- MAIN ---------------- */

int main() {
    int choice, n;

    printf("1.FCFS 2.SJF 3.SRTN 4.RR 5.Priority 6.ALL\n");
    scanf("%d",&choice);

    printf("Enter number of processes: ");
    scanf("%d",&n);

    Process original[MAX];
    generateProcesses(original,n);

    fp = fopen("output.tex","w");
    pthread_mutex_init(&lock,NULL);

    pthread_t threads[5];
    ThreadData td[5];

    int total = (choice==6)?5:1;

    for(int i=0;i<total;i++) {
        td[i].n = n;
        td[i].algo = (choice==6)? i+1 : choice;
        copyProcesses(original, td[i].p, n);

        pthread_create(&threads[i],NULL,runAlgo,&td[i]);
    }

    for(int i=0;i<total;i++)
        pthread_join(threads[i],NULL);

    pthread_mutex_destroy(&lock);
    fclose(fp);

    printf("\nResults saved in output.tex\n");
    return 0;
}
