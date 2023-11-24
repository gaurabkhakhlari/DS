#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct
{
    int id;
    int is_leader;
    pthread_t thread_id;
} Process;

Process *processes;
int num_processes;

void *process_function(void *arg)
{
    Process *process = (Process *)arg;

    while (1)
    {
        // Simulate some work in the process
        sleep(2);

        // Check if the process is the current leader
        if (process->is_leader)
        {
            printf("Process %d is the leader.\n", process->id);
        }
    }

    return NULL;
}

void initiate_election(int failed_process_id)
{
    printf("Process %d initiates the election.\n", failed_process_id);

    int highest_id = -1;
    int new_leader_id = -1;

    for (int i = 0; i < num_processes; ++i)
    {
        if (processes[i].id > highest_id && processes[i].id > failed_process_id)
        {
            highest_id = processes[i].id;
            new_leader_id = i;
        }
    }

    if (new_leader_id != -1)
    {
        processes[new_leader_id].is_leader = 1;
        printf("Process %d is the new leader.\n", processes[new_leader_id].id);
    }
}

int main()
{
    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);

    processes = (Process *)malloc(num_processes * sizeof(Process));

    // Initialize processes
    for (int i = 0; i < num_processes; ++i)
    {
        processes[i].id = i;
        processes[i].is_leader = 0;
        pthread_create(&processes[i].thread_id, NULL, process_function, (void *)&processes[i]);
    }

    // Simulate a process failure (e.g., process 3 fails)
    int failed_process_id;
    printf("Enter the ID of the failed process: ");
    scanf("%d", &failed_process_id);

    if (failed_process_id >= 0 && failed_process_id < num_processes)
    {
        // Initiate election when a process fails
        initiate_election(failed_process_id);

        // Let the remaining processes continue their work
        sleep(10);
    }
    else
    {
        printf("Invalid process ID.\n");
    }

    free(processes);

    return 0;
}
