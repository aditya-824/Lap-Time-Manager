/* INCLUDES */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* MACROS */
#define MAX_NAME_LEN 50

/* DATATYPES */
typedef struct
{
    float *sector_times;
    int sector_count;
} Lap;

typedef struct
{
    char name[MAX_NAME_LEN];
    Lap lap;
} Racer;

typedef struct node
{
    Racer *racer;
    struct node *next;
} node;

/* GLOBAL VARIABLES */
node *front = NULL;
node *rear = NULL;

/* FUNCTION PROTOTYPES */
node *Create(char *name, float *sector_times, int sector_count);
void Enqueue(char *name, float *sector_times, int sector_count);
int Dequeue();
void Display();
void Destroy();
float AvgLapTime();
void FastestLap();
void CompareLapTimes(char *r1, char *r2);

/* MAIN */
int main()
{
    while (1)
    {
        int choice;
        char name[MAX_NAME_LEN];

        printf("\n------- Lap Time Manager -------\n");
        printf("1. Add Lap Time\n");
        printf("2. Display All Lap Times\n");
        printf("3. Show Average Lap Time\n");
        printf("4. Show Fastest Lap\n");
        printf("5. Compare Lap Times\n");
        printf("6. Remove Oldest Lap Time\n");
        printf("0. Exit\n");
        printf("Enter choice (0-6): ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        while (getchar() != '\n')
            ;

        switch (choice)
        {
        case 1:
            printf("Enter racer name: ");
            fgets(name, MAX_NAME_LEN, stdin);
            name[strcspn(name, "\n")] = '\0';

            int sector_count;
            printf("Enter the number of sectors for this lap: ");
            if (scanf("%d", &sector_count) != 1 || sector_count <= 0)
            {
                printf("Invalid number of sectors.\n");
                while (getchar() != '\n')
                    ;
                break;
            }
            while (getchar() != '\n')
                ;

            float *sector_times = malloc(sector_count * sizeof(float));
            if (!sector_times)
            {
                printf("Memory allocation failed!\n");
                break;
            }

            for (int i = 0; i < sector_count; i++)
            {
                printf("Enter time for sector %d: ", i + 1);
                if (scanf("%f", &sector_times[i]) != 1)
                {
                    printf("Invalid input for sector time.\n");
                    sector_times[i] = 0.0f;
                    while (getchar() != '\n')
                        ;
                }
                while (getchar() != '\n')
                    ;
            }

            Enqueue(name, sector_times, sector_count);
            free(sector_times);
            break;

        case 2:
            Display();
            break;

        case 3:
            printf("Average Track Lap Time: %.2f seconds\n", AvgLapTime());
            break;

        case 4:
            FastestLap();
            break;
        case 5:
        {
            char racer1[MAX_NAME_LEN];
            char racer2[MAX_NAME_LEN];
            printf("Enter the name of the first racer: ");
            fgets(racer1, MAX_NAME_LEN, stdin);
            racer1[strcspn(racer1, "\n")] = '\0';
            printf("Enter the name of the second racer: ");
            fgets(racer2, MAX_NAME_LEN, stdin);
            racer2[strcspn(racer2, "\n")] = '\0';
            CompareLapTimes(racer1, racer2);
        }
        break;

        case 6:
        {
            int result = Dequeue();
            if (result == -1)
                printf("Queue is empty!\n");
            else
                printf("Removed successfully.\n");
        }
        break;

        case 0:
            printf("Exiting...\n");
            Destroy();
            return 0;

        default:
            printf("Invalid option entered, please try again");
        }
    }
}

node *Create(char *name, float *sector_times, int sector_count)
{
    node *new_node = (node *)malloc(sizeof(node));
    new_node->racer = (Racer *)malloc(sizeof(Racer));
    strncpy(new_node->racer->name, name, MAX_NAME_LEN - 1);
    new_node->racer->name[MAX_NAME_LEN - 1] = '\0';
    Lap *lap = (Lap *)malloc(sizeof(Lap));
    lap->sector_times = malloc(sector_count * sizeof(float));
    for (int i = 0; i < sector_count; i++)
    {
        lap->sector_times[i] = sector_times[i];
    }
    lap->sector_count = sector_count;
    new_node->racer->lap = *lap;
    free(lap);
    new_node->next = NULL;
    return new_node;
}

void Enqueue(char *name, float *sector_times, int sector_count)
{
    node *new_node = Create(name, sector_times, sector_count);

    if (front == NULL && rear == NULL)
    {
        front = rear = new_node;
    }
    else
    {
        rear->next = new_node;
        rear = new_node;
    }
}

int Dequeue()
{
    if (front == NULL)
    {
        return -1;
    }
    struct node *ptr = front;
    // Free sector_times array
    if (ptr->racer && ptr->racer->lap.sector_times)
        free(ptr->racer->lap.sector_times);
    // Free Racer struct
    if (ptr->racer)
        free(ptr->racer);
    if (front == rear)
    {
        front = rear = NULL;
        free(ptr);
        return 0;
    }
    else
    {
        front = front->next;
        free(ptr);
        return 0;
    }
}

void Display()
{
    if (front == NULL)
    {
        printf("Queue is empty!\n");
        return;
    }
    printf("\n========================== RACETRACK SCOREBOARD ==========================\n");
    printf("| %-20s | %-12s | %-10s | %-20s |\n", "Racer Name", "Total Time", "Sectors", "Sector Times");
    printf("--------------------------------------------------------------------------\n");
    struct node *ptr = front;
    while (ptr != NULL)
    {
        float lap_time = 0.0f;
        for (int i = 0; i < ptr->racer->lap.sector_count; i++)
        {
            lap_time += ptr->racer->lap.sector_times[i];
        }
        printf("| %-20s | %10.2f s | %-10d | ", ptr->racer->name, lap_time, ptr->racer->lap.sector_count);
        for (int i = 0; i < ptr->racer->lap.sector_count; i++)
        {
            printf("%6.2f ", ptr->racer->lap.sector_times[i]);
        }
        printf("|");
        printf("\n--------------------------------------------------------------------------\n");
        ptr = ptr->next;
    }
}

void Destroy()
{
    if (front == NULL)
    {
        printf("Queue is empty!\n");
        return;
    }
    while (front != NULL)
        Dequeue();
    printf("Queue destroyed.\n");
}

float AvgLapTime()
{
    if (front == NULL)
    {
        printf("Queue is empty!\n");
        return 0.0f;
    }

    float total_time = 0.0f;
    int lap_count = 0;
    struct node *ptr = front;

    while (ptr != NULL)
    {
        float lap_time = 0.0f;
        for (int i = 0; i < ptr->racer->lap.sector_count; i++)
        {
            lap_time += ptr->racer->lap.sector_times[i];
        }
        total_time += lap_time;
        lap_count++;
        ptr = ptr->next;
    }

    return lap_count ? total_time / lap_count : 0.0f;
}

void FastestLap()
{
    if (front == NULL)
    {
        printf("Queue is empty!\n");
        return;
    }

    float min_time = -1.0f;
    char best_name[MAX_NAME_LEN] = "";
    struct node *ptr = front;

    while (ptr != NULL)
    {
        float lap_time = 0.0f;
        for (int i = 0; i < ptr->racer->lap.sector_count; i++)
        {
            lap_time += ptr->racer->lap.sector_times[i];
        }

        if (min_time == -1.0f || lap_time < min_time)
        {
            min_time = lap_time;
            strncpy(best_name, ptr->racer->name, MAX_NAME_LEN - 1);
            best_name[MAX_NAME_LEN - 1] = '\0';
        }

        ptr = ptr->next;
    }

    printf("Fastest Lap: %s with %.2f seconds\n", best_name, min_time);
}

void CompareLapTimes(char *r1, char *r2)
{
    node *temp = front;
    float time1 = -1.0f;
    float time2 = -1.0f;
    float avg_time1 = -1.0f;
    float avg_time2 = -1.0f;
    int count1 = 1;
    int count2 = 1;

    while (temp != NULL)
    {
        if (strcmp(temp->racer->name, r1) == 0)
        {
            time1 = 0.0f;
            for (int i = 0; i < temp->racer->lap.sector_count; i++)
            {
                time1 += temp->racer->lap.sector_times[i];
            }
            count1++;
        }
        if (strcmp(temp->racer->name, r2) == 0)
        {
            time2 = 0.0f;
            for (int i = 0; i < temp->racer->lap.sector_count; i++)
            {
                time2 += temp->racer->lap.sector_times[i];
            }
            count2++;
        }
        temp = temp->next;
    }

    if (time1 == -1.0f)
    {
        printf("Racer %s not found.\n", r1);
        return;
    }
    if (time2 == -1.0f)
    {
        printf("Racer %s not found.\n", r2);
        return;
    }

    avg_time1 = time1 / (count1 - 1);
    avg_time2 = time2 / (count2 - 1);

    printf("Comparison of average times between %s and %s:\n", r1, r2);
    if (avg_time1 < avg_time2)
    {
        printf("%s is faster by %.2f seconds\n", r1, avg_time2 - avg_time1);
    }
    else if (avg_time1 > avg_time2)
    {
        printf("%s is faster by %.2f seconds\n", r2, avg_time1 - avg_time2);
    }
    else
    {
        printf("Both racers have the same time of %.2f seconds\n", avg_time1);
    }
}
