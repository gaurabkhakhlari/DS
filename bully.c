#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PROCESSES 3

int logicalClocks[NUM_PROCESSES];
pthread_mutex_t clockMutex = PTHREAD_MUTEX_INITIALIZER;

void updateLogicalClock(int processId, int eventTime) {
    pthread_mutex_lock(&clockMutex);
    if (eventTime > logicalClocks[processId]) {
        logicalClocks[processId] = eventTime;
    }
    pthread_mutex_unlock(&clockMutex);
}

void* processFunction(void* arg) {
    int processId = *((int*)arg);

    for (int i = 1; i <= 5; ++i) {
        // Simulate some local processing
        sleep(1);

        // Event occurs, update logical clock
        updateLogicalClock(processId, logicalClocks[processId] + 1);

        // Print event details
        printf("Process %d: Event %d at logical time %d\n", processId + 1, i, logicalClocks[processId]);
        
        // Simulate message exchange between processes
        if (processId == 0) {
            sleep(1);
            updateLogicalClock(1, logicalClocks[0] + 1);
            printf("Process 1: Sends message to Process 2 at logical time %d\n", logicalClocks[0]);
        } else if (processId == 1) {
            sleep(1);
            updateLogicalClock(2, logicalClocks[1] + 1);
            printf("Process 2: Sends message to Process 3 at logical time %d\n", logicalClocks[1]);
        }
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_PROCESSES];
    int processIds[NUM_PROCESSES];

    for (int i = 0; i < NUM_PROCESSES; ++i) {
        logicalClocks[i] = 0;
        processIds[i] = i;
        pthread_create(&threads[i], NULL, processFunction, (void*)&processIds[i]);
    }

    for (int i = 0; i < NUM_PROCESSES; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
