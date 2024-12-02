#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSES 10
#define MAX_TIME_QUANTUM 4
#define MAX_BURST_TIME 10
#define MAX_PRIORITY 5

// Структура для процесу
typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
} Process;

// Функція для генерації випадкових процесів
void generate_processes(Process processes[], int num_processes) {
    srand(time(NULL));
    for (int i = 0; i < num_processes; i++) {
        processes[i].id = i + 1;
        processes[i].arrival_time = rand() % 10; // Випадковий час прибуття
        processes[i].burst_time = rand() % MAX_BURST_TIME + 1; // Випадковий час виконання (мінімум 1)
        processes[i].priority = rand() % MAX_PRIORITY + 1; // Випадковий пріоритет (мінімум 1)
        processes[i].remaining_time = processes[i].burst_time;
    }
}

// Функція для сортування процесів за часом прибуття (для FCFS)
void sort_by_arrival_time(Process processes[], int num_processes) {
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = i + 1; j < num_processes; j++) {
            if (processes[i].arrival_time > processes[j].arrival_time) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}


// Функція для сортування процесів за пріоритетом
void sort_by_priority(Process processes[], int num_processes) {
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = i + 1; j < num_processes; j++) {
            if (processes[i].priority < processes[j].priority) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

// Функція для симуляції Round Robin
void simulate_round_robin(Process processes[], int num_processes, int time_quantum) {
    printf("Round Robin Scheduling:\n");
    int time = 0;
    int completed = 0;

    while (completed < num_processes) {
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].remaining_time > 0) {
                int execution_time = (processes[i].remaining_time < time_quantum) ? processes[i].remaining_time : time_quantum;
                processes[i].remaining_time -= execution_time;
                time += execution_time;
                printf("Process %d (Remaining time: %d)\n", processes[i].id, processes[i].remaining_time);

                if (processes[i].remaining_time == 0) {
                    completed++;
                }
            }
        }
    }
}



// Функція для симуляції FCFS
void simulate_fcfs(Process processes[], int num_processes) {
    printf("\nFCFS Scheduling:\n");
    int current_time = 0;

    for (int i = 0; i < num_processes; i++) {
        int start_time = (processes[i].arrival_time > current_time) ? processes[i].arrival_time : current_time;
        int end_time = start_time + processes[i].burst_time;
        int waiting_time = start_time - processes[i].arrival_time;

        printf("Process %d: Start Time = %d, End Time = %d, Waiting Time = %d\n",
               processes[i].id, start_time, end_time, waiting_time);

        current_time = end_time;
    }
}




// Функція для симуляції пріоритетного планування
void simulate_priority(Process processes[], int num_processes) {
    printf("\nPriority Scheduling:\n");


    sort_by_priority(processes, num_processes);

    for (int i = 0; i < num_processes; i++) {
         printf("Process %d (Burst Time: %d, Priority: %d)\n", processes[i].id, processes[i].burst_time, processes[i].priority);
    }
}



int main() {
    Process processes[MAX_PROCESSES];
    int num_processes = 5; // Кількість процесів

    generate_processes(processes, num_processes);


    simulate_round_robin(processes, num_processes, MAX_TIME_QUANTUM);

    // Відновлення початкових значень remaining_time для FCFS та Priority
    for(int i = 0; i < num_processes; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }
    sort_by_arrival_time(processes, num_processes);
    simulate_fcfs(processes, num_processes);


    // Відновлення початкових значень remaining_time для Priority
    for(int i = 0; i < num_processes; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }

    simulate_priority(processes, num_processes);

    return 0;
}