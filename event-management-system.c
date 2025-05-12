#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_EVENTS 100
#define FILE_NAME "events.txt"

struct Event {
    int id;
    char name[100];
    char type[50];
    char date[20]; // Format: YYYY-MM-DD
    char location[100];
    float budget;
};

// Function declarations
void addEvent(struct Event events[], int *count);
void deleteEvent(struct Event events[], int *count);
void editEvent(struct Event events[], int count);
void displaySpecificEvent(struct Event events[], int count);
void displayAllEvents(struct Event events[], int count);
void deleteAllEvents(int *count);
void filterEventsByType(struct Event events[], int count);
void saveEventsToFile(struct Event events[], int count);
void loadEventsFromFile(struct Event events[], int *count);
int isDuplicateId(int id, struct Event events[], int count);

int main() {
    struct Event events[MAX_EVENTS];
    int count = 0;
    int choice;

    loadEventsFromFile(events, &count);

    printf("\n\t\t\tEVENT MANAGEMENT SYSTEM\n");
    printf("\t\t\t   By: MOSOODA\n");
    printf("\t\t\t   PF SEMESTER PROJECT\n");

    while (1) {
        printf("\n--- Menu ---\n");
        printf("1. Add Event\n");
        printf("2. Delete Event\n");
        printf("3. Edit Event\n");
        printf("4. Display Specific Event\n");
        printf("5. Display All Events\n");
        printf("6. Delete All Events\n");
        printf("7. Filter Events by Type\n");
        printf("8. Save Events to File\n");
        printf("9. Load Events from File\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // clear buffer
            continue;
        }

        switch (choice) {
            case 1: addEvent(events, &count); break;
            case 2: deleteEvent(events, &count); break;
            case 3: editEvent(events, count); break;
            case 4: displaySpecificEvent(events, count); break;
            case 5: displayAllEvents(events, count); break;
            case 6: deleteAllEvents(&count); break;
            case 7: filterEventsByType(events, count); break;
            case 8: saveEventsToFile(events, count); break;
            case 9: loadEventsFromFile(events, &count); break;
            case 10:
                printf("Exiting program...\n");
                saveEventsToFile(events, count);
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

int isDuplicateId(int id, struct Event events[], int count) {
    for (int i = 0; i < count; i++) {
        if (events[i].id == id)
            return 1;
    }
    return 0;
}

void addEvent(struct Event events[], int *count) {
    if (*count >= MAX_EVENTS) {
        printf("Cannot add more events.\n");
        return;
    }

    struct Event e;
    int valid = 0;

    while (!valid) {
        printf("Enter event ID: ");
        if (scanf("%d", &e.id) != 1) {
            printf("Invalid input. Enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        if (isDuplicateId(e.id, events, *count)) {
            printf("This ID already exists. Try a different one.\n");
            continue;
        }

        valid = 1;
    }

    getchar();
    printf("Enter event name: ");
    fgets(e.name, sizeof(e.name), stdin);
    e.name[strcspn(e.name, "\n")] = 0;

    printf("Enter event type (e.g., Wedding, Seminar): ");
    fgets(e.type, sizeof(e.type), stdin);
    e.type[strcspn(e.type, "\n")] = 0;

    printf("Enter event date (YYYY-MM-DD): ");
    fgets(e.date, sizeof(e.date), stdin);
    e.date[strcspn(e.date, "\n")] = 0;

    printf("Enter event location: ");
    fgets(e.location, sizeof(e.location), stdin);
    e.location[strcspn(e.location, "\n")] = 0;

    printf("Enter budget: ");
    scanf("%f", &e.budget);

    events[*count] = e;
    (*count)++;
    printf("Event added successfully.\n");
}

void deleteEvent(struct Event events[], int *count) {
    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < *count; i++) {
        if (events[i].id == id) {
            for (int j = i; j < *count - 1; j++) {
                events[j] = events[j + 1];
            }
            (*count)--;
            printf("Event deleted.\n");
            return;
        }
    }

    printf("Event with ID %d not found.\n", id);
}

void editEvent(struct Event events[], int count) {
    int id;
    printf("Enter ID to edit: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (events[i].id == id) {
            getchar();
            printf("Enter new name: ");
            fgets(events[i].name, sizeof(events[i].name), stdin);
            events[i].name[strcspn(events[i].name, "\n")] = 0;

            printf("Enter new type: ");
            fgets(events[i].type, sizeof(events[i].type), stdin);
            events[i].type[strcspn(events[i].type, "\n")] = 0;

            printf("Enter new date: ");
            fgets(events[i].date, sizeof(events[i].date), stdin);
            events[i].date[strcspn(events[i].date, "\n")] = 0;

            printf("Enter new location: ");
            fgets(events[i].location, sizeof(events[i].location), stdin);
            events[i].location[strcspn(events[i].location, "\n")] = 0;

            printf("Enter new budget: ");
            scanf("%f", &events[i].budget);

            printf("Event updated.\n");
            return;
        }
    }

    printf("Event with ID %d not found.\n", id);
}

void displaySpecificEvent(struct Event events[], int count) {
    int id;
    printf("Enter ID to display: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (events[i].id == id) {
            printf("\nID: %d\nName: %s\nType: %s\nDate: %s\nLocation: %s\nBudget: %.2f\n",
                   events[i].id, events[i].name, events[i].type,
                   events[i].date, events[i].location, events[i].budget);
            return;
        }
    }

    printf("Event not found.\n");
}

void displayAllEvents(struct Event events[], int count) {
    printf("\n%-5s %-20s %-15s %-12s %-20s %-10s\n", "ID", "Name", "Type", "Date", "Location", "Budget");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %-15s %-12s %-20s %.2f\n",
               events[i].id, events[i].name, events[i].type,
               events[i].date, events[i].location, events[i].budget);
    }
}

void deleteAllEvents(int *count) {
    *count = 0;
    printf("All events deleted.\n");
}

void filterEventsByType(struct Event events[], int count) {
    char type[50];
    getchar();
    printf("Enter type to filter (e.g., Wedding): ");
    fgets(type, sizeof(type), stdin);
    type[strcspn(type, "\n")] = 0;

    printf("\n--- Events of type: %s ---\n", type);
    for (int i = 0; i < count; i++) {
        if (strcasecmp(events[i].type, type) == 0) {
            printf("%d: %s on %s at %s (Budget: %.2f)\n",
                   events[i].id, events[i].name, events[i].date,
                   events[i].location, events[i].budget);
        }
    }
}

void saveEventsToFile(struct Event events[], int count) {
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        printf("Error saving file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%s,%s,%s,%.2f\n",
                events[i].id, events[i].name, events[i].type,
                events[i].date, events[i].location, events[i].budget);
    }

    fclose(file);
    printf("Data saved successfully.\n");
}

void loadEventsFromFile(struct Event events[], int *count) {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("No existing file found.\n");
        return;
    }

    *count = 0;
    while (*count < MAX_EVENTS &&
           fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,],%f\n",
                  &events[*count].id, events[*count].name, events[*count].type,
                  events[*count].date, events[*count].location, &events[*count].budget) == 6) {
        (*count)++;
    }

    fclose(file);
    printf("Events loaded successfully.\n");
}
