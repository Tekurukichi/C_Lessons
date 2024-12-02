#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSES 10
#define MAX_TIME_QUANTUM 4
#define MAX_BURST_TIME 10
#define MAX_PRIORITY 5
#define AGING_FACTOR 1

// Структура для процесу
typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
} Process;

// Функція для генерації випадкових процесів
void generate_processes(Process processes[], int num_processes) {
    srand(time(NULL));
    for (int i = 0; i < num_processes; i++) {
        processes[i].id = i + 1;
        processes[i].arrival_time = rand() % 10;
        processes[i].burst_time = rand() % MAX_BURST_TIME + 1;
        processes[i].priority = rand() % MAX_PRIORITY + 1;
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }
}

// Функція для сортування за часом прибуття (FCFS)
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

// Функція для сортування за часом виконання (SJF)
void sort_by_burst_time(Process processes[], int num_processes) {
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = i + 1; j < num_processes; j++) {
            if (processes[i].burst_time > processes[j].burst_time) {
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
    Process rr_processes[MAX_PROCESSES];
     for (int i = 0; i < num_processes; i++) {
        rr_processes[i] = processes[i];
     }

    while (completed < num_processes) {
        for (int i = 0; i < num_processes; i++) {
            if (rr_processes[i].remaining_time > 0) {
                int execution_time = (rr_processes[i].remaining_time < time_quantum) ? rr_processes[i].remaining_time : time_quantum;
                rr_processes[i].remaining_time -= execution_time;
                time += execution_time;
                printf("Process %d (Remaining time: %d)\n", rr_processes[i].id, rr_processes[i].remaining_time);

                if (rr_processes[i].remaining_time == 0) {
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
    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;


    for (int i = 0; i < num_processes; i++) {
        int start_time = (processes[i].arrival_time > current_time) ? processes[i].arrival_time : current_time;
        int end_time = start_time + processes[i].burst_time;
        int waiting_time = start_time - processes[i].arrival_time;
        int turnaround_time = end_time - processes[i].arrival_time;

        processes[i].waiting_time = waiting_time;
        processes[i].turnaround_time = turnaround_time;

        printf("Process %d: Start Time = %d, End Time = %d, Waiting Time = %d, Turnaround Time = %d\n",
               processes[i].id, start_time, end_time, waiting_time, turnaround_time);

        current_time = end_time;

         avg_waiting_time += waiting_time;
         avg_turnaround_time += turnaround_time;
    }

     avg_waiting_time /= num_processes;
     avg_turnaround_time /= num_processes;

     printf("Average waiting time: %.2f\n", avg_waiting_time);
     printf("Average turnaround time: %.2f\n", avg_turnaround_time);
}


// Функція для симуляції SJF
void simulate_sjf(Process processes[], int num_processes) {
    printf("\nSJF Scheduling:\n");

    Process sjf_processes[MAX_PROCESSES];
    for(int i=0; i< num_processes; i++){
        sjf_processes[i] = processes[i];
    }
    sort_by_burst_time(sjf_processes, num_processes);

    int current_time = 0;
    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;

    for (int i = 0; i < num_processes; i++) {
        int start_time = (sjf_processes[i].arrival_time > current_time) ? sjf_processes[i].arrival_time : current_time;
        int end_time = start_time + sjf_processes[i].burst_time;
        int waiting_time = start_time - sjf_processes[i].arrival_time;
        int turnaround_time = end_time - sjf_processes[i].arrival_time;


        sjf_processes[i].waiting_time = waiting_time;
        sjf_processes[i].turnaround_time = turnaround_time;


        printf("Process %d: Start Time = %d, End Time = %d, Waiting Time = %d, Turnaround Time = %d\n",
               sjf_processes[i].id, start_time, end_time, waiting_time, turnaround_time);

        current_time = end_time;

        avg_waiting_time += waiting_time;
        avg_turnaround_time += turnaround_time;
    }



    avg_waiting_time /= num_processes;
    avg_turnaround_time /= num_processes;

     printf("Average waiting time: %.2f\n", avg_waiting_time);
     printf("Average turnaround time: %.2f\n", avg_turnaround_time);

}


// Функція для симуляції пріоритетного планування зі старінням
void simulate_priority_aging(Process processes[], int num_processes) {
    printf("\nPriority Scheduling with Aging:\n");

    Process priority_processes[MAX_PROCESSES];
    for(int i = 0; i < num_processes; i++){
        priority_processes[i] = processes[i];
    }


    int completed = 0;
    int current_time = 0;
    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;


    while (completed < num_processes) {
        int highest_priority = -1;
        int highest_priority_index = -1;

        for (int i = 0; i < num_processes; i++) {
            if (priority_processes[i].remaining_time > 0 && (highest_priority_index == -1 || priority_processes[i].priority > highest_priority)) {

                 if (priority_processes[i].arrival_time <= current_time){
                    highest_priority = priority_processes[i].priority;
                    highest_priority_index = i;
                 }

            }
        }

        if (highest_priority_index != -1) {

            int start_time = (priority_processes[highest_priority_index].arrival_time > current_time) ? priority_processes[highest_priority_index].arrival_time : current_time;

            priority_processes[highest_priority_index].remaining_time = 0;
            current_time += priority_processes[highest_priority_index].burst_time;

            int end_time = current_time;
            int waiting_time = start_time - priority_processes[highest_priority_index].arrival_time;
            int turnaround_time = end_time - priority_processes[highest_priority_index].arrival_time;

            priority_processes[highest_priority_index].waiting_time = waiting_time;
            priority_processes[highest_priority_index].turnaround_time = turnaround_time;

            avg_waiting_time += waiting_time;
            avg_turnaround_time += turnaround_time;


            printf("Process %d: Burst Time = %d, Priority = %d, Start Time = %d, End Time = %d, Waiting Time = %d, Turnaround Time = %d\n", priority_processes[highest_priority_index].id, priority_processes[highest_priority_index].burst_time, priority_processes[highest_priority_index].priority, start_time, end_time, waiting_time, turnaround_time);
            completed++;
        } else {
             current_time++;
             for (int i = 0; i < num_processes; i++) {
                if (priority_processes[i].remaining_time > 0 && priority_processes[i].arrival_time < current_time) {
                   priority_processes[i].priority += AGING_FACTOR;
                }
             }

        }


    }


    avg_waiting_time /= num_processes;
    avg_turnaround_time /= num_processes;

     printf("Average waiting time: %.2f\n", avg_waiting_time);
     printf("Average turnaround time: %.2f\n", avg_turnaround_time);
}





int main() {
    Process processes[MAX_PROCESSES];
    int num_processes = 5;

    generate_processes(processes, num_processes);

    simulate_round_robin(processes, num_processes, MAX_TIME_QUANTUM);

    for(int i = 0; i < num_processes; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }
    sort_by_arrival_time(processes, num_processes);
    simulate_fcfs(processes, num_processes);



    for(int i = 0; i < num_processes; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }
    simulate_sjf(processes, num_processes);


    for(int i = 0; i < num_processes; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }
    simulate_priority_aging(processes, num_processes);



    return 0;
}