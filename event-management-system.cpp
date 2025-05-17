// START COMMIT 1: Arrays and Stuff
#include <stdio.h>
#include <string.h>

// My event system for PF class. I’m new at this, so it’s super basic.
// Just arrays, no fancy structs or pointers. Using scanf and text files.
// Gotta set up the arrays first, hope I got this right!

#define MAX_EVENTS 10 // Max events
#define MAX_PEOPLE 100 // Max people
#define MAX_NAME 10 // Names
#define MAX_EVENTS_PER_PERSON 2 // Events per person
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

    printf("Event name : ");
    scanf(" %s", &event_names[i]);

    printf("Date (day month year): ");
    scanf("%d %d %d", &event_day[i], &event_month[i], &event_year[i]);

    printf("How many people? ");
    scanf("%d", &event_max_people[i]);

    printf("Organizer (one word): ");
    scanf(" %s", &organizer[i]);

    event_people_count[i] = 0;
    event_count++;
    printf("Event %d added!\n", event_nums[i]);
}

// Okay, let’s add people. This took forever! Added stuff to handle wrong input.
void add_people() {
    // Check if we've hit the maximum number of people allowed
    if (people_count == MAX_PEOPLE) {
        printf("Too many people!\n");
        return;
    }

    // Use the current count as the index for the new person
    int i = people_count;
    // Assign the next available ID to the new person
    people_ids[i] = i + 1;

    // Prompt for the person's name (one word only)
    printf("Person name (one word): ");
    // Read the name into the people_names array
    scanf(" %s", people_names[i]);

    // Initialize the event count for this person to zero
    people_event_count[i] = 0;

    // Ask for an event ID to join (0 means skip joining an event)
    printf("Join event ID (0 to skip): ");
    int event_id;
    // Check if the input is actually a number
    if (scanf("%d", &event_id) != 1) {
        // If they entered something other than a number, show an error
        printf("Hey, enter a number, not letters!\n");
        // Clear out any bad input to prevent issues later
        while (getchar() != '\n'); // I added this to stop it crashing!
        return; // Exit the function since the input was invalid
    }

    // If they entered a valid event ID (greater than 0 and within the event count)
    if (event_id > 0 && event_id <= event_count) {
        // Get the index for the event (event IDs are 1-based, arrays are 0-based)
        int j = event_id - 1;
        // Check if the event has room for more people
        if (event_people_count[j] < MAX_PEOPLE_PER_EVENT) {
            // Add the event to the person's event list
            people_events[i][people_event_count[i]] = event_id;
            // Increment the person's event count
            people_event_count[i]++;
            // Add the person to the event's people list
            event_people[j][event_people_count[j]] = people_ids[i];
            // Increment the event's people count
            event_people_count[j]++;
            // Confirm the person joined the event
            printf("Joined event %d!\n", event_id);
        } else {
            // If the event is full, let them know
            printf("Event is full!\n");
        }
    }

    // Increment the total people count
    people_count++;
    // Confirm the person was added with their ID
    printf("Person %d added!\n", people_ids[i]);
    }
	// END COMMIT 2

// START COMMIT 3: Saving to Files
// Gotta save stuff so it doesn’t vanish. 

void save_stuff() {
    // Open the events file in write mode to store event data
    FILE* file = fopen("events.txt", "w");
    
    // Write the total number of events to the file
    fprintf(file, "%d\n", event_count);
    
    // Loop through each event to save its details
    for (int i = 0; i < event_count; i++) {
        // Save event number, name, date, max people, and current people count
        fprintf(file, "%d %s %d %d %d %d %d ",
                event_nums[i], event_names[i], event_day[i], event_month[i], event_year[i],
                event_max_people[i], event_people_count[i]);
        
        // Save the list of people attending this event
        for (int j = 0; j < MAX_PEOPLE_PER_EVENT; j++) {
            fprintf(file, "%d ", event_people[i][j]);
        }
        
        // Save the organizer's name for this event
        fprintf(file, "%s\n", organizer[i]);
    }
    
    // Close the events file
    fclose(file);

    // Open the people file in write mode to store people data
    file = fopen("people.txt", "w");
    
    // Write the total number of people to the file
    fprintf(file, "%d\n", people_count);
    
    // Loop through each person to save their details
    for (int i = 0; i < people_count; i++) {
        // Save person ID, name, and number of events they are attending
        fprintf(file, "%d %s %d ",
                people_ids[i], people_names[i], people_event_count[i]);
        
        // Save the list of events this person is attending
        for (int j = 0; j < MAX_EVENTS_PER_PERSON; j++) {
            fprintf(file, "%d ", people_events[i][j]);
        }
        
        // Add a newline to separate each person's data
        fprintf(file, "\n");
    }
    
    // Close the people file
    fclose(file);

    // Print a confirmation message to the console
    printf("Successfully, Saved!\n");
}

void load_stuff() {
    // Open the events file for reading
    FILE* file = fopen("events.txt", "r");
    
    // Check if the events file was opened successfully
    if (file != NULL) {
        // Read the total number of events from the file
        fscanf(file, "%d", &event_count);
        
        // Loop through each event to read its details
        for (int i = 0; i < event_count; i++) {
            // Read event number, name, date (day, month, year), max people, and current people count
            fscanf(file, "%d %s %d %d %d %d %d",
                   &event_nums[i], event_names[i], &event_day[i], &event_month[i], &event_year[i],
                   &event_max_people[i], &event_people_count[i]);
            
            // Read the list of people attending this event
            for (int j = 0; j < MAX_PEOPLE_PER_EVENT; j++) {
                fscanf(file, "%d", &event_people[i][j]);
            }
            
            // Read the organizer's name for this event
            fscanf(file, "%s", organizer[i]);
        }
        
        // Close the events file after reading
        fclose(file);
        printf("Got events!\n");
    } else {
        // Print an error message if the events file couldn't be opened
        printf("No event file.\n");
    }

    // Open the people file for reading
    file = fopen("people.txt", "r");
    
    // Check if the people file was opened successfully
    if (file != NULL) {
        // Read the total number of people from the file
        fscanf(file, "%d", &people_count);
        
        // Loop through each person to read their details
        for (int i = 0; i < people_count; i++) {
            // Read person ID, name, and the number of events they're attending
            fscanf(file, "%d %s %d",
                   &people_ids[i], people_names[i], &people_event_count[i]);
            
            // Read the list of event IDs this person is attending
            for (int j = 0; j < MAX_EVENTS_PER_PERSON; j++) {
                fscanf(file, "%d", &people_events[i][j]);
            }
        }
        
        // Close the people file after reading
        fclose(file);
        printf("Get Registered now!\n");
    } else {
        // Print an error message if the people file couldn't be opened
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
            printf("Nobody is here.\n");
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

void delete_event() {
    // Ask the user which event to delete by its ID
    printf("Event ID to delete: ");
    // Create a variable to store the ID the user types
    int event_id;
    // Read the ID from the user
    scanf("%d", &event_id);
    // Check if the ID is valid (not less than 1 or more than the number of events)
    if (event_id < 1 || event_id > event_count) {
        // Oops, that ID doesn’t exist, so tell the user and exit the function
        printf("No event!\n");
        return;
    }
    // Convert the ID to array index (IDs start at 1, arrays start at 0)
    int j = event_id - 1;
    // Shift all events after the deleted one to fill the gap
    for (int i = j; i < event_count - 1; i++) {
        // Copy the next event’s ID to the current spot
        event_nums[i] = event_nums[i + 1];
        // Copy the next event’s name (using strcpy since it’s a string)
        strcpy(event_names[i], event_names[i + 1]);
        // Copy the next event’s day
        event_day[i] = event_day[i + 1];
        // Copy the next event’s month
        event_month[i] = event_month[i + 1];
        // Copy the next event’s year
        event_year[i] = event_year[i + 1];
        // Copy the next event’s max people limit
        event_max_people[i] = event_max_people[i + 1];
        // Copy the next event’s current people count
        event_people_count[i] = event_people_count[i + 1];
        // Loop through the attendee list and copy each person’s ID
        for (int k = 0; k < MAX_PEOPLE_PER_EVENT; k++) {
            event_people[i][k] = event_people[i + 1][k];
        }
        // Copy the next event’s organizer name
        strcpy(organizer[i], organizer[i + 1]);
    }
    // Decrease the total event count since we removed one
    event_count--;
	}

void find_event() {
    // Ask the user to type a part of the event name to search for
    printf("Part of name: ");
    // Make a small array to store the name the user types (up to MAX_NAME characters)
    char name[MAX_NAME];
    // Get the name from the user (the space before %s skips any leftover whitespace)
    scanf(" %s", name);
    // Print a header to show where the results start
    printf("\n--- Found ---\n");
    // Keep track if we found any matching events (0 means none found yet)
    int any = 0;
    // Loop through all the events we have (up to event_count)
    for (int i = 0; i < event_count; i++) {
        // Check if the user's input is part of the event name using strstr
        if (strstr(event_names[i], name) != NULL) {
            // Found a match! Print the event's ID and full name
            printf("ID: %d, Name: %s\n", event_nums[i], event_names[i]);
            // Set any to 1 so we know we found something
            any = 1;
        }
    }
    // If we didn’t find any matches (any is still 0), let the user know
    if (!any) printf("Nothing!\n");
}

// END COMMIT 4

// START COMMIT 5: Menu Time 
// Menu to run it all. This makes it easy!
void menu() {
    printf("\n--- Event Management System ---\n");
    printf("1. Add event\n2. Add person\n3. Show events\n4. Show people\n5. Delete event\n6. Find event\n7. Save & quit\n");
    printf("Pick: ");
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
        else if (choice == 5) delete_event();
        else if (choice == 6) find_event();
        else if (choice == 7) {
            save_stuff();
            printf("Catch You Later!\n");
            return 0;
        } else printf("1 to 7 only!\n");
    }
    return 0;
}
// END COMMIT 5
