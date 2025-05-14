// START COMMIT 1: Arrays and Stuff
#include <stdio.h>
#include <string.h>

// Yo! My event system for PF class. I’m new at this, so it’s super basic.
// Just arrays, no fancy structs or pointers. Using scanf and text files.
// Gotta set up the arrays first, hope I got this right!

#define MAX_EVENTS 50 // Max events
#define MAX_PEOPLE 200 // Max people
#define MAX_NAME 50 // Names
#define MAX_EVENTS_PER_PERSON 5 // Events per person
#define MAX_PEOPLE_PER_EVENT 20 // People per event

int event_nums[MAX_EVENTS]; // Event IDs
char event_names[MAX_EVENTS][MAX_NAME]; // Event names
int event_day[MAX_EVENTS]; // Date
int event_month[MAX_EVENTS];
int event_year[MAX_EVENTS];
int event_max_people[MAX_EVENTS]; // How many can join
int event_people_count[MAX_EVENTS]; // Who’s in
int event_people[MAX_EVENTS][MAX_PEOPLE_PER_EVENT]; // Attendee list
char organizer[MAX_EVENTS][MAX_NAME]; // Organizer name
int event_count = 0; // Total events

int people_ids[MAX_PEOPLE]; // People IDs
char people_names[MAX_PEOPLE][MAX_NAME]; // Names
int people_events[MAX_PEOPLE][MAX_EVENTS_PER_PERSON]; // Their events
int people_event_count[MAX_PEOPLE]; // Event count
int people_count = 0; // Total people
// END COMMIT 1

// START COMMIT 2: Adding Events and People
// Okay, let’s add events and people. This took forever!

void add_event() {
    if (event_count == MAX_EVENTS) {
        printf("No more room!\n");
        return;
    }

    int i = event_count;
    event_nums[i] = i + 1;

    printf("Event name (one word): ");
    scanf(" %s", event_names[i]);

    printf("Date (day month year): ");
    scanf("%d %d %d", &event_day[i], &event_month[i], &event_year[i]);

    printf("How many people? ");
    scanf("%d", &event_max_people[i]);

    printf("Organizer (one word): ");
    scanf(" %s", organizer[i]);

    event_people_count[i] = 0;
    event_count++;
    printf("Event %d added!\n", event_nums[i]);
}

// Okay, let’s add people. This took forever! Added stuff to handle wrong input.
void add_people() {
    if (people_count == MAX_PEOPLE) {
        printf("Too many people!\n");
        return;
    }

    int i = people_count;
    people_ids[i] = i + 1;

    printf("Person name (one word): ");
    scanf(" %s", people_names[i]);

    people_event_count[i] = 0;

    printf("Join event ID (0 to skip): ");
    int event_id;
    // Check if scanf got a number
    if (scanf("%d", &event_id) != 1) {
        printf("Hey, enter a number, not letters!\n");
        // Clear the junk from input so it doesn’t mess up later
        while (getchar() != '\n'); // I added this to stop it crashing!
        return; // Skip the rest, they messed up
    }

    if (event_id > 0 && event_id <= event_count) {
        int j = event_id - 1;
        if (event_people_count[j] < MAX_PEOPLE_PER_EVENT) {
            people_events[i][people_event_count[i]] = event_id;
            people_event_count[i]++;
            event_people[j][event_people_count[j]] = people_ids[i];
            event_people_count[j]++;
            printf("Joined event %d!\n", event_id);
        } else {
            printf("Event’s full!\n");
        }
    }

    people_count++;
    printf("Person %d added!\n", people_ids[i]);

}
// END COMMIT 2



void menu() {
    printf("\n--- Event Management System ---\n");
    printf("1. Add event\n2. Add person\n3. Show events\n4. Show people\n5. Delete event\n6. Find event\n7. Save & quit\n");
    printf("Pick: ");
}

void save_stuff() {
    FILE* file = fopen("events.txt", "w");
    fprintf(file, "%d\n", event_count);
    for (int i = 0; i < event_count; i++) {
        fprintf(file, "%d %s %d %d %d %d %d ",
                event_nums[i], event_names[i], event_day[i], event_month[i], event_year[i],
                event_max_people[i], event_people_count[i]);
        for (int j = 0; j < MAX_PEOPLE_PER_EVENT; j++) {
            fprintf(file, "%d ", event_people[i][j]);
        }
        fprintf(file, "%s\n", organizer[i]);
    }
    fclose(file);

    file = fopen("people.txt", "w");
    fprintf(file, "%d\n", people_count);
    for (int i = 0; i < people_count; i++) {
        fprintf(file, "%d %s %d ",
                people_ids[i], people_names[i], people_event_count[i]);
        for (int j = 0; j < MAX_EVENTS_PER_PERSON; j++) {
            fprintf(file, "%d ", people_events[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);

    printf("Yay, saved!\n");
}

void load_stuff() {
    FILE* file = fopen("events.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &event_count);
        for (int i = 0; i < event_count; i++) {
            fscanf(file, "%d %s %d %d %d %d %d",
                   &event_nums[i], event_names[i], &event_day[i], &event_month[i], &event_year[i],
                   &event_max_people[i], &event_people_count[i]);
            for (int j = 0; j < MAX_PEOPLE_PER_EVENT; j++) {
                fscanf(file, "%d", &event_people[i][j]);
            }
            fscanf(file, "%s", organizer[i]);
        }
        fclose(file);
        printf("Got events!\n");
    } else {
        printf("No event file.\n");
    }

    file = fopen("people.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &people_count);
        for (int i = 0; i < people_count; i++) {
            fscanf(file, "%d %s %d",
                   &people_ids[i], people_names[i], &people_event_count[i]);
            for (int j = 0; j < MAX_EVENTS_PER_PERSON; j++) {
                fscanf(file, "%d", &people_events[i][j]);
            }
        }
        fclose(file);
        printf("Get Registered now!\n");
    } else {
        printf("No people file.\n");
    }
}
// END COMMIT 3
// START COMMIT 4: Showing and Deleting
// Showing events/people and deleting. Deleting was so hard!

void show_stuff(int what) {
    if (what == 0) { // Show events
        if (event_count == 0) {
            printf("No events yet!\n");
            return;
        }
        printf("\n--- Events ---\n");
        for (int i = 0; i < event_count; i++) {
            printf("ID: %d, Name: %s, Date: %d/%d/%d\n",
                   event_nums[i], event_names[i], event_day[i], event_month[i], event_year[i]);
        }
    } else { // Show people
        printf("Event ID: ");
        int event_id;
        scanf("%d", &event_id);
        if (event_id < 1 || event_id > event_count) {
            printf("Wrong ID!\n");
            return;
        }
        int j = event_id - 1;
        printf("\n--- People in Event %d ---\n", event_id);
        if (event_people_count[j] == 0) {
            printf("Nobody’s here.\n");
        } else {
            for (int i = 0; i < event_people_count[j]; i++) {
                int pid = event_people[j][i];
                for (int k = 0; k < people_count; k++) {
                    if (people_ids[k] == pid) {
                        printf("ID: %d, Name: %s\n", people_ids[k], people_names[k]);
                    }
                }
            }
        }
    }
}

int main() {
    printf("Welcome To Event Management System!\n");
    load_stuff();
    int choice;
    while (1) {
        menu();
        scanf("%d", &choice);
        if (choice == 1) add_event();
        else if (choice == 2) add_people();
        else if (choice == 3) show_stuff(0);
        else if (choice == 4) show_stuff(1);
        else if (choice == 7) {
            save_stuff();
            printf("See ya!\n");
            return 0;
        } else printf("1 to 7 only!\n");
    }
    return 0;
}
// END COMMIT 5
