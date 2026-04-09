#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100

typedef struct {
    int pid, arrival, burst, remaining;
    int completion, waiting, turnaround, response;
    int started;
} Process;

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
        p[i].started = 0;
    }
}

/* ---------------- FCFS ---------------- */

void fcfs(Process p[], int n) {
    int time = 0;

    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival)
            time = p[i].arrival;

        p[i].response = time - p[i].arrival;

        time += p[i].burst;

        p[i].completion = time;
        p[i].turnaround = p[i].completion - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;
    }
}

/* ---------------- SJF (Non-preemptive) ---------------- */

void sjf(Process p[], int n) {
    int completed = 0, time = 0;

    while (completed < n) {
        int idx = -1, minBurst = 1e9;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && p[i].remaining > 0 && p[i].burst < minBurst) {
                minBurst = p[i].burst;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        p[idx].response = time - p[idx].arrival;

        time += p[idx].burst;
        p[idx].remaining = 0;

        p[idx].completion = time;
        p[idx].turnaround = p[idx].completion - p[idx].arrival;
        p[idx].waiting = p[idx].turnaround - p[idx].burst;

        completed++;
    }
}

/* ---------------- SRTN (Preemptive SJF) ---------------- */

void srtn(Process p[], int n) {
    int completed = 0, time = 0;

    while (completed < n) {
        int idx = -1, minRem = 1e9;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && p[i].remaining > 0 && p[i].remaining < minRem) {
                minRem = p[i].remaining;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        if (!p[idx].started) {
            p[idx].response = time - p[idx].arrival;
            p[idx].started = 1;
        }

        p[idx].remaining--;
        time++;

        if (p[idx].remaining == 0) {
            completed++;
            p[idx].completion = time;
            p[idx].turnaround = p[idx].completion - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
        }
    }
}

/* ---------------- Round Robin ---------------- */

void rr(Process p[], int n, int quantum) {
    int time = 0, completed = 0;
    int queue[MAX], front = 0, rear = 0;
    int visited[MAX] = {0};

    queue[rear++] = 0;
    visited[0] = 1;

    while (completed < n) {
        int idx = queue[front++];

        if (!p[idx].started) {
            p[idx].response = time - p[idx].arrival;
            p[idx].started = 1;
        }

        int exec = (p[idx].remaining < quantum) ? p[idx].remaining : quantum;

        p[idx].remaining -= exec;
        time += exec;

        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].arrival <= time) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }

        if (p[idx].remaining > 0) {
            queue[rear++] = idx;
        } else {
            completed++;
            p[idx].completion = time;
            p[idx].turnaround = p[idx].completion - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
        }
    }
}

/* ---------------- Output ---------------- */

void printResults(Process p[], int n, const char *name, FILE *fp) {
    float avgWT = 0, avgTAT = 0, avgRT = 0;
    int totalBurst = 0, lastCompletion = 0;

    printf("\n--- %s ---\n", name);
    printf("PID\tAT\tBT\tWT\tTAT\tRT\n");

    fprintf(fp, "\\section*{%s}\n", name);
    fprintf(fp, "PID & AT & BT & WT & TAT & RT \\\\\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst,
               p[i].waiting, p[i].turnaround, p[i].response);

        fprintf(fp, "%d & %d & %d & %d & %d & %d \\\\\n",
                p[i].pid, p[i].arrival, p[i].burst,
                p[i].waiting, p[i].turnaround, p[i].response);

        avgWT += p[i].waiting;
        avgTAT += p[i].turnaround;
        avgRT += p[i].response;
        totalBurst += p[i].burst;

        if (p[i].completion > lastCompletion)
            lastCompletion = p[i].completion;
    }

    printf("Avg WT=%.2f | Avg TAT=%.2f | Avg RT=%.2f\n",
           avgWT/n, avgTAT/n, avgRT/n);

    float cpuUtil = (float)totalBurst / lastCompletion * 100;
    printf("CPU Utilization: %.2f%%\n", cpuUtil);

    fprintf(fp, "\\\\ Avg WT=%.2f, Avg TAT=%.2f, Avg RT=%.2f \\\\\n",
            avgWT/n, avgTAT/n, avgRT/n);
    fprintf(fp, "CPU Utilization=%.2f\\%%\n\n", cpuUtil);
}

/* ---------------- MAIN ---------------- */

int main() {
    int n, choice, quantum = 2;

    Process original[MAX], temp[MAX];

    printf("CPU Scheduling Simulator\n");
    printf("1.FCFS 2.SJF 3.SRTN 4.RR 5.ALL\n");
    scanf("%d", &choice);

    printf("Enter number of processes: ");
    scanf("%d", &n);

    generateProcesses(original, n);

    FILE *fp = fopen("output.tex", "w");

    if (choice == 1 || choice == 5) {
        copyProcesses(original, temp, n);
        fcfs(temp, n);
        printResults(temp, n, "FCFS", fp);
    }

    if (choice == 2 || choice == 5) {
        copyProcesses(original, temp, n);
        sjf(temp, n);
        printResults(temp, n, "SJF", fp);
    }

    if (choice == 3 || choice == 5) {
        copyProcesses(original, temp, n);
        srtn(temp, n);
        printResults(temp, n, "SRTN", fp);
    }

    if (choice == 4 || choice == 5) {
        copyProcesses(original, temp, n);
        rr(temp, n, quantum);
        printResults(temp, n, "Round Robin", fp);
    }

    fclose(fp);

    printf("\nResults also saved in output.tex\n");

    return 0;
}
