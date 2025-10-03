#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_RACERS 100

typedef struct {
    char name[MAX_NAME_LEN];
    float lap_time;
} RacerLap;

typedef struct {
    RacerLap queue[MAX_RACERS];
    int front;
    int rear;
    int size;
} LapQueue;

// Initialize the queue
void initQueue(LapQueue *q) {
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}

// Check if the queue is empty
int isEmpty(LapQueue *q) {
    return q->size == 0;
}

// Check if the queue is full
int isFull(LapQueue *q) {
    return q->size == MAX_RACERS;
}

// Add a lap time to the queue
int enqueue(LapQueue *q, const char *name, float lap_time) {
    if (isFull(q)) {
        printf("Queue is full. Cannot add more lap times.\n");
        return 0;
    }
    q->rear = (q->rear + 1) % MAX_RACERS;
    strncpy(q->queue[q->rear].name, name, MAX_NAME_LEN - 1);
    q->queue[q->rear].name[MAX_NAME_LEN - 1] = '\0';
    q->queue[q->rear].lap_time = lap_time;
    q->size++;
    return 1;
}

// Remove and return the first lap time from the queue
int dequeue(LapQueue *q, RacerLap *lap) {
    if (isEmpty(q)) {
        printf("Queue is empty.\n");
        return 0;
    }
    *lap = q->queue[q->front];
    q->front = (q->front + 1) % MAX_RACERS;
    q->size--;
    return 1;
}

// Display all lap times
void displayQueue(LapQueue *q) {
    if (isEmpty(q)) {
        printf("No lap times recorded.\n");
        return;
    }
    printf("Lap Times:\n");
    int count = q->size;
    int idx = q->front;
    while (count--) {
        printf("Racer: %s, Lap Time: %.2f\n", q->queue[idx].name, q->queue[idx].lap_time);
        idx = (idx + 1) % MAX_RACERS;
    }
}

// Calculate average lap time
float averageLapTime(LapQueue *q) {
    if (isEmpty(q)) return 0.0f;
    float sum = 0.0f;
    int count = q->size;
    int idx = q->front;
    while (count--) {
        sum += q->queue[idx].lap_time;
        idx = (idx + 1) % MAX_RACERS;
    }
    return sum / q->size;
}

// Find the fastest lap
void fastestLap(LapQueue *q) {
    if (isEmpty(q)) {
        printf("No lap times recorded.\n");
        return;
    }
    float min_time = q->queue[q->front].lap_time;
    char fastest_racer[MAX_NAME_LEN];
    strncpy(fastest_racer, q->queue[q->front].name, MAX_NAME_LEN);
    int count = q->size;
    int idx = q->front;
    while (count--) {
        if (q->queue[idx].lap_time < min_time) {
            min_time = q->queue[idx].lap_time;
            strncpy(fastest_racer, q->queue[idx].name, MAX_NAME_LEN);
        }
        idx = (idx + 1) % MAX_RACERS;
    }
    printf("Fastest Lap: %s with %.2f seconds\n", fastest_racer, min_time);
}

int main() {
    LapQueue lapQueue;
    initQueue(&lapQueue);

    int choice;
    char name[MAX_NAME_LEN];
    float lap_time;
    RacerLap lap;

    do {
        printf("\nLap Time Manager\n");
        printf("1. Add Lap Time\n");
        printf("2. Process Next Lap Time\n");
        printf("3. Display All Lap Times\n");
        printf("4. Show Average Lap Time\n");
        printf("5. Show Fastest Lap\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                printf("Enter racer name: ");
                fgets(name, MAX_NAME_LEN, stdin);
                name[strcspn(name, "\n")] = '\0'; // remove newline
                printf("Enter lap time (seconds): ");
                scanf("%f", &lap_time);
                getchar();
                enqueue(&lapQueue, name, lap_time);
                break;
            case 2:
                if (dequeue(&lapQueue, &lap)) {
                    printf("Processed Lap - Racer: %s, Time: %.2f\n", lap.name, lap.lap_time);
                }
                break;
            case 3:
                displayQueue(&lapQueue);
                break;
            case 4:
                printf("Average Lap Time: %.2f seconds\n", averageLapTime(&lapQueue));
                break;
            case 5:
                fastestLap(&lapQueue);
                break;
            case 0:
                printf("Exiting Lap Time Manager.\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    return 0;
}