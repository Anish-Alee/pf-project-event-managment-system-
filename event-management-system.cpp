// START COMMIT 1: Arrays and Stuff
#include <stdio.h>
#include <string.h>

// My event system for PF class. I’m new at this, so it’s super basic.
// Hope I got this right! 

#define MAX_EVENTS 10 // Max events
#define MAX_PEOPLE 200 // Max people
#define MAX_NAME 20 // Names
#define MAX_PEOPLE_PER_EVENT 20 // People per event

// Arrays to store event info
int event_ids[MAX_EVENTS]; // Keeps event IDs
char event_names[MAX_EVENTS][MAX_NAME]; // Stores event names
int event_day[MAX_EVENTS]; // Day of event
int event_month[MAX_EVENTS]; // Month of event
int event_year[MAX_EVENTS]; // Year of event
int event_capacity[MAX_EVENTS]; // Max people allowed in event
int event_people_count[MAX_EVENTS]; // How many joined event
int event_people[MAX_EVENTS][MAX_PEOPLE_PER_EVENT]; // IDs of people in event
int event_count = 0; // Total events

// Arrays to store people info
int people_ids[MAX_PEOPLE]; // Keeps people IDs
char people_names[MAX_PEOPLE][MAX_NAME]; // Stores people names
int people_events[MAX_PEOPLE][MAX_EVENTS]; // Events they joined
int people_event_count[MAX_PEOPLE]; // How many events they joined
int people_count = 0; // Total people
// END COMMIT 1

// Clears extra input from keyboard
void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Checks if date is valid
int is_valid_date(int d, int m, int y) {
    // Year should be between 1900 and 2100
    if (y < 1900 || y > 2100) return 0;
    // Month should be between 1 and 12
    if (m < 1 || m > 12) return 0;
    // Day should not be less than 1
    if (d < 1) return 0;
    int max_day = 31; // Default max days
    // April, June, Sep, Nov have 30 days
    if (m == 4 || m == 6 || m == 9 || m == 11) max_day = 30;
    // February logic
    else if (m == 2) {
        // Check for leap year
        if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) max_day = 29;
        else max_day = 28;
    }
    // Check if day is valid
    return d <= max_day;
} 

// START COMMIT 2: Adding Events and Persons
// Adds a new event
void add_event() {
    // Check if max events reached
    if (event_count >= MAX_EVENTS) {
        printf("Reached max events!\n");
        return;
    }
    
    int idx = event_count; // New event index
    event_ids[idx] = idx + 1; // Set event ID
    
    // Get event name
    printf("Enter event name (max %d chars): ", MAX_NAME - 1);
    if (fgets(event_names[idx], MAX_NAME, stdin) == NULL) {
        printf("Error reading event name.\n");
        clear_input_buffer();
        return;
    }
    event_names[idx][strcspn(event_names[idx], "\n")] = 0; // Remove newline
    
    int d, m, y; // Day, month, year
    while(1) {
        // Get event date
        printf("Enter date (day month year): ");
        if (scanf("%d %d %d", &d, &m, &y) != 3) {
            printf("Invalid input!\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        // Check if date is valid
        if (!is_valid_date(d, m, y)) {
            printf("Invalid date! Please enter a valid date.\n");
            continue;
        }
        break;
    }
    // Save date
    event_day[idx] = d; event_month[idx] = m; event_year[idx] = y;
    
    int cap; // Event capacity
    while(1) {
        // Get max capacity
        printf("Enter max capacity (max %d): ", MAX_PEOPLE_PER_EVENT);
        if (scanf("%d", &cap) != 1) {
            printf("Invalid capacity! Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        // Check if capacity is valid
        if (cap < 1 || cap > MAX_PEOPLE_PER_EVENT) {
            printf("Capacity must be between 1 and %d.\n", MAX_PEOPLE_PER_EVENT);
            continue;
        }
        break;
    }
    event_capacity[idx] = cap; // Save capacity
    
    event_people_count[idx] = 0; // No people joined yet
    
    printf("Event added with ID %d.\n", event_ids[idx]);
    event_count++; // Increase event count
}

// Adds a new person
void add_person() {
    // Check if max people reached
    if (people_count >= MAX_PEOPLE) {
        printf("Max people reached!\n");
        return;
    }
    int idx = people_count; // New person index
    people_ids[idx] = idx + 1; // Set person ID
    
    // Get person name
    printf("Enter person name (max %d chars): ", MAX_NAME - 1);
    if (fgets(people_names[idx], MAX_NAME, stdin) == NULL) {
        printf("Error reading person name.\n");
        clear_input_buffer();
        return;
    }
    people_names[idx][strcspn(people_names[idx], "\n")] = 0; // Remove newline
    
    int event_id; // Event to join
    while (1) {
        // Get event ID (0 to skip)
        printf("Enter event ID to join (0 to skip): ");
        if (scanf("%d", &event_id) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        // Check if event ID is valid
        if (event_id < 0 || event_id > event_count) {
            printf("Invalid event ID! Valid IDs are 0 to %d.\n", event_count);
            continue;
        }
        break;
    }
    
    if (event_id == 0) {
        // No event joined
        people_event_count[idx] = 0;
        printf("Person added without joining an event.\n");
    } else {
        int e_idx = event_id - 1; // Event index
        // Check if event is full
        if (event_people_count[e_idx] >= event_capacity[e_idx]) {
            printf("Event full, cannot join.\n");
            return;
        }
        // Add person to event
        event_people[e_idx][event_people_count[e_idx]] = people_ids[idx];
        event_people_count[e_idx]++;
        people_event_count[idx] = 1; // Person joined one event
        people_events[idx][0] = event_id; // Save event ID
        printf("Person added and joined event %d.\n", event_id);
    }
    people_count++; // Increase people count
}
// END COMMIT 2

// START COMMIT 3: show events and people
// Shows all events
void show_events() {
    // Check if no events
    if (event_count == 0) {
        printf("No events available.\n");
        return;
    }
    printf("\nEvents:\n");
    int i;
    // Print each event details
    for (i = 0; i < event_count; i++) {
        printf("ID: %d, Name: %s, Date: %02d/%02d/%d, Capacity: %d, Joined: %d\n", 
            event_ids[i], event_names[i], event_day[i], event_month[i], event_year[i], 
            event_capacity[i], event_people_count[i]);
    }
}

// Shows people in an event
void show_people_in_event() {
    // Check if no events
    if (event_count == 0) {
        printf("No events available.\n");
        return;
    }
    int event_id;
    // Get event ID
    printf("Enter event ID to see attendees: ");
    if (scanf("%d", &event_id) != 1) {
        printf("Invalid input!\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();
    // Check if event ID is valid
    if (event_id < 1 || event_id > event_count) {
        printf("Invalid event ID.\n");
        return;
    }
    int idx = event_id - 1; // Event index
    // Check if no people in event
    if (event_people_count[idx] == 0) {
        printf("No attendees for this event yet.\n");
        return;
    }
    // Print event name and people
    printf("People in event \"%s\":\n", event_names[idx]);
    int j, k;
    for (j = 0; j < event_people_count[idx]; j++) {
        int pid = event_people[idx][j];
        for (k = 0; k < people_count; k++) {
            if (people_ids[k] == pid) {
                printf("ID: %d, Name: %s\n", pid, people_names[k]);
                break;
            }
        }
    }
}
// END COMMIT 3

// START COMMIT 4: Deleting
// Deletes an event
void delete_event() {
    // Get event ID to delete
    printf("Event ID to delete: ");
    int event_id;
    if (scanf("%d", &event_id) != 1) {
        printf("Invalid input!\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();
    // Check if event ID is valid
    if (event_id < 1 || event_id > event_count) {
        printf("No such event!\n");
        return;
    }
    int j = event_id - 1; // Event index
    // Shift events to remove the deleted one
    for (int i = j; i < event_count - 1; i++) {
        event_ids[i] = event_ids[i + 1];
        strcpy(event_names[i], event_names[i + 1]);
        event_day[i] = event_day[i + 1];
        event_month[i] = event_month[i + 1];
        event_year[i] = event_year[i + 1];
        event_capacity[i] = event_capacity[i + 1];
        event_people_count[i] = event_people_count[i + 1];
        for (int k = 0; k < MAX_PEOPLE_PER_EVENT; k++) {
            event_people[i][k] = event_people[i + 1][k];
        }
    }
    event_count--; // Decrease event count
    printf("Event deleted.\n");
}
// END COMMIT 4

// START COMMIT 5: FINDING EVENTS
// Finds events by name
void find_event() {
    // Check if no events
    if (event_count == 0) {
        printf("No events to search.\n");
        return;
    }
    char query[MAX_NAME];
    // Get search query
    printf("Enter part of the event name to search: ");
    if (fgets(query, MAX_NAME, stdin) == NULL) {
        printf("Error reading input.\n");
        clear_input_buffer();
        return;
    }
    query[strcspn(query, "\n")] = 0; // Remove newline
    // Check if query is empty
    if (strlen(query) == 0) {
        printf("Search query cannot be empty.\n");
        return;
    }
    int found = 0;
    int i;
    printf("Events matching '%s':\n", query);
    // Search for events with matching name
    for (i = 0; i < event_count; i++) {
        if (strstr(event_names[i], query) != NULL) {
            printf("ID: %d, Name: %s, Date: %02d/%02d/%d\n", 
                    event_ids[i], event_names[i], event_day[i], event_month[i], event_year[i]);
            found = 1;
        }
    }
    // If no matches found
    if (!found) {
        printf("No events found matching '%s'.\n", query);
    }
}
// END COMMIT 5

// START COMMIT 6: SAVING DATA
// Saves data to files
void save_data() {
    // Save events to file
    FILE *f = fopen("events.txt", "w");
    if (!f) {
        printf("Error opening events.txt for saving.\n");
        return;
    }
    fprintf(f, "%d\n", event_count);
    for (int i=0; i<event_count; i++) {
        fprintf(f, "%d %s %d %d %d %d %d ", event_ids[i], event_names[i], event_day[i], event_month[i], event_year[i], event_capacity[i], event_people_count[i]);
        for (int j=0; j<event_people_count[i]; j++) {
            fprintf(f, "%d ", event_people[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);

    // Save people to file
    f = fopen("people.txt", "w");
    if (!f) {
        printf("Error opening people.txt for saving.\n");
        return;
    }
    fprintf(f, "%d\n", people_count);
    for(int i=0; i<people_count; i++) {
        fprintf(f, "%d %s %d ", people_ids[i], people_names[i], people_event_count[i]);
        for(int j = 0; j < people_event_count[i]; j++) {
            fprintf(f, "%d ", people_events[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    printf("Data saved successfully!\n");
}
// END COMMIT 6

// START COMMIT 7: LOADING DATA
// Loads data from files
void load_stuff() {
    // Load events
    FILE* file = fopen("events.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &event_count);
        for (int i = 0; i < event_count; i++) {
            fscanf(file, "%d %s %d %d %d %d %d",
                   &event_ids[i], event_names[i], &event_day[i], &event_month[i], &event_year[i],
                   &event_capacity[i], &event_people_count[i]);
            for (int j = 0; j < event_people_count[i]; j++) {
                fscanf(file, "%d", &event_people[i][j]);
            }
        }
        fclose(file);
        printf("Got events!\n");
    } else {
        printf("No event file.\n");
    }

    // Load people
    file = fopen("people.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &people_count);
        for (int i = 0; i < people_count; i++) {
            fscanf(file, "%d %s %d",
                   &people_ids[i], people_names[i], &people_event_count[i]);
            for (int j = 0; j < people_event_count[i]; j++) {
                fscanf(file, "%d", &people_events[i][j]);
            }
        }
        fclose(file);
        printf("Get Registered now!\n");
    } else {
        printf("No people file.\n");
    }
}
//END COMMIT 7 

//START COMMIT 8: SHOWING MENU 
// Main program
int main() {
    printf("Welcome To Event Management System!\n");
    load_stuff(); // Load saved data
    printf("\n--- Event Management System ---\n");
    while(1) {
        // Show menu
        printf("1. Add Event\n");
        printf("2. Add Person\n");
        printf("3. Show Events\n");
        printf("4. Show People in Event\n");
        printf("5. Delete Event by ID\n");
        printf("6. Find Event by Name\n");
        printf("7. Save Data\n");
        printf("8. Exit\n");
        printf("Choose option: ");
        
        int choice;
        // Get user choice
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        // Run selected option
        switch(choice) {
            case 1: add_event(); break;
            case 2: add_person(); break;
            case 3: show_events(); break;
            case 4: show_people_in_event(); break;
            case 5: delete_event(); break;
            case 6: find_event(); break;
            case 7: save_data(); break;
            case 8:
                save_data(); // Save before exit
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid option! Please try again.\n");
        }
    }
    return 0;
}
// END COMMIT 8
