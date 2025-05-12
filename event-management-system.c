#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants for maximum limits
#define MAX_EVENTS 100         // Maximum number of events
#define MAX_NAME 100           // Maximum length of names
#define MAX_CONTACT 15         // Maximum length of contact numbers
#define MAX_VENUE 100          // Maximum length of venue name

// Global arrays to store event details
int event_ids[MAX_EVENTS];                                 // Unique event IDs
char event_names[MAX_EVENTS][MAX_NAME];                    // Event names
int event_days[MAX_EVENTS];                                // Event day (DD)
int event_months[MAX_EVENTS];                              // Event month (MM)
int event_years[MAX_EVENTS];                               // Event year (YYYY)
int event_hours[MAX_EVENTS];                               // Event hour (HH)
int event_minutes[MAX_EVENTS];                             // Event minute (MM)
char event_venues[MAX_EVENTS][MAX_VENUE];                  // Event venue names
int event_capacities[MAX_EVENTS];                          // Event capacity
int organizer_ids[MAX_EVENTS];                             // Organizer IDs
char organizer_names[MAX_EVENTS][MAX_NAME];                // Organizer names
char organizer_contacts[MAX_EVENTS][MAX_CONTACT];          // Organizer contact numbers
int event_count = 0;                                       // Count of total events added

// Function to add a new event
void add_event() {
    // Check if event limit is reached
    if (event_count >= MAX_EVENTS) {
        printf("Event limit reached!\n");
        return;
    }

    // Assign a new unique event ID
    event_ids[event_count] = event_count + 1;

    // Input event name
    printf("Enter event name: ");
    fgets(event_names[event_count], MAX_NAME, stdin);
    event_names[event_count][strcspn(event_names[event_count], "\n")] = 0;  // Remove newline

    // Input date
    printf("Enter date (DD MM YYYY): ");
    scanf("%d %d %d", &event_days[event_count], &event_months[event_count], &event_years[event_count]);

    // Input time
    printf("Enter time (HH MM): ");
    scanf("%d %d", &event_hours[event_count], &event_minutes[event_count]);

    // Input venue name
    getchar();  // Clear leftover newline from buffer
    printf("Enter venue: ");
    fgets(event_venues[event_count], MAX_VENUE, stdin);
    event_venues[event_count][strcspn(event_venues[event_count], "\n")] = 0;  // Remove newline

    // Input capacity
    printf("Enter capacity: ");
    scanf("%d", &event_capacities[event_count]);

    // Input organizer name
    getchar();  // Clear buffer
    printf("Enter organizer name: ");
    fgets(organizer_names[event_count], MAX_NAME, stdin);
    organizer_names[event_count][strcspn(organizer_names[event_count], "\n")] = 0;  // Remove newline

    // Input organizer contact
    printf("Enter organizer contact: ");
    fgets(organizer_contacts[event_count], MAX_CONTACT, stdin);
    organizer_contacts[event_count][strcspn(organizer_contacts[event_count], "\n")] = 0;  // Remove newline

    // Assign organizer ID (same as event ID for simplicity)
    organizer_ids[event_count] = event_count + 1;

    // Increment the event counter
    event_count++;

    // Confirmation message
    printf("Event added successfully! ID: %d\n", event_ids[event_count - 1]);
}

// Main function
int main() {
    printf("Event Management System - Add Event\n");
    add_event();  // Call function to add one event
    return 0;
}

