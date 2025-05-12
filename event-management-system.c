#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global Constants
#define MAX_EVENTS 100
#define MAX_ATTENDEES 1000
#define MAX_NAME 100
#define MAX_CONTACT 15
#define MAX_VENUE 100
#define MAX_EVENTS_PER_ATTENDEE 10

// Global Arrays for Events
int event_ids[MAX_EVENTS];
char event_names[MAX_EVENTS][MAX_NAME];
int event_days[MAX_EVENTS];
int event_months[MAX_EVENTS];
int event_years[MAX_EVENTS];
int event_hours[MAX_EVENTS];
int event_minutes[MAX_EVENTS];
char event_venues[MAX_EVENTS][MAX_VENUE];
int event_capacities[MAX_EVENTS];
int event_num_attendees[MAX_EVENTS];
int* event_attendees[MAX_EVENTS]; // Array of pointers to dynamic attendee ID arrays
int organizer_ids[MAX_EVENTS];
char organizer_names[MAX_EVENTS][MAX_NAME];
char organizer_contacts[MAX_EVENTS][MAX_CONTACT];
int event_count = 0;

// Global Arrays for Attendees
int attendee_ids[MAX_ATTENDEES];
char attendee_names[MAX_ATTENDEES][MAX_NAME];
char attendee_contacts[MAX_ATTENDEES][MAX_CONTACT];
int attendee_registered_events[MAX_ATTENDEES][MAX_EVENTS_PER_ATTENDEE];
int attendee_num_events[MAX_ATTENDEES];
int attendee_count = 0;

// Function to Validate Contact (Basic Phone Number Check)
int is_valid_contact(const char* contact) {
    int len = strlen(contact);
    if (len < 10 || len > 14) return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(contact[i]) && contact[i] != '-' && contact[i] != '+') return 0;
    }
    return 1;
}

// Function to Add an Event
void add_event() {
    if (event_count >= MAX_EVENTS) {
        printf("Error: Event limit reached!\n");
        return;
    }

    int index = event_count;
    event_ids[index] = index + 1;

    printf("Enter event name: ");
    char temp_name[MAX_NAME];
    fgets(temp_name, MAX_NAME, stdin);
    temp_name[strcspn(temp_name, "\n")] = 0;
    if (strlen(temp_name) == 0) {
        printf("Error: Event name cannot be empty!\n");
        return;
    }
    strcpy(event_names[index], temp_name);

    printf("Enter date (DD MM YYYY): ");
    int day, month, year;
    if (scanf("%d %d %d", &day, &month, &year) != 3) {
        printf("Error: Invalid date format!\n");
        getchar();
        return;
    }
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 2020) {
        printf("Error: Invalid date (day: 1-31, month: 1-12, year: >=2020)!\n");
        getchar();
        return;
    }
    event_days[index] = day;
    event_months[index] = month;
    event_years[index] = year;

    printf("Enter time (HH MM): ");
    int hour, minute;
    if (scanf("%d %d", &hour, &minute) != 2) {
        printf("Error: Invalid time format!\n");
        getchar();
        return;
    }
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        printf("Error: Invalid time (hour: 0-23, minute: 0-59)!\n");
        getchar();
        return;
    }
    event_hours[index] = hour;
    event_minutes[index] = minute;

    printf("Enter venue: ");
    getchar();
    char temp_venue[MAX_VENUE];
    fgets(temp_venue, MAX_VENUE, stdin);
    temp_venue[strcspn(temp_venue, "\n")] = 0;
    if (strlen(temp_venue) == 0) {
        printf("Error: Venue cannot be empty!\n");
        return;
    }
    strcpy(event_venues[index], temp_venue);

    printf("Enter capacity: ");
    int capacity;
    if (scanf("%d", &capacity) != 1 || capacity < 1) {
        printf("Error: Capacity must be a positive number!\n");
        getchar();
        return;
    }
    event_capacities[index] = capacity;

    printf("Enter organizer name: ");
    getchar();
    char temp_org_name[MAX_NAME];
    fgets(temp_org_name, MAX_NAME, stdin);
    temp_org_name[strcspn(temp_org_name, "\n")] = 0;
    if (strlen(temp_org_name) == 0) {
        printf("Error: Organizer name cannot be empty!\n");
        return;
    }
    strcpy(organizer_names[index], temp_org_name);

    printf("Enter organizer contact: ");
    char temp_contact[MAX_CONTACT];
    fgets(temp_contact, MAX_CONTACT, stdin);
    temp_contact[strcspn(temp_contact, "\n")] = 0;
    if (!is_valid_contact(temp_contact)) {
        printf("Error: Invalid contact (10-14 digits, may include - or +)!\n");
        return;
    }
    strcpy(organizer_contacts[index], temp_contact);
    organizer_ids[index] = index + 1;

    event_num_attendees[index] = 0;
    event_attendees[index] = NULL;

    event_count++;
    printf("Event added successfully! ID: %d\n", event_ids[index]);
}

// Function to Add an Attendee
void add_attendee() {
    if (attendee_count >= MAX_ATTENDEES) {
        printf("Error: Attendee limit reached!\n");
        return;
    }

    int index = attendee_count;
    attendee_ids[index] = index + 1;

    printf("Enter attendee name: ");
    getchar();
    char temp_name[MAX_NAME];
    fgets(temp_name, MAX_NAME, stdin);
    temp_name[strcspn(temp_name, "\n")] = 0;
    if (strlen(temp_name) == 0) {
        printf("Error: Attendee name cannot be empty!\n");
        return;
    }
    strcpy(attendee_names[index], temp_name);

    printf("Enter attendee contact: ");
    char temp_contact[MAX_CONTACT];
    fgets(temp_contact, MAX_CONTACT, stdin);
    temp_contact[strcspn(temp_contact, "\n")] = 0;
    if (!is_valid_contact(temp_contact)) {
        printf("Error: Invalid contact (10-14 digits, may include - or +)!\n");
        return;
    }
    strcpy(attendee_contacts[index], temp_contact);

    attendee_num_events[index] = 0;

    printf("Enter event ID to register (0 to skip): ");
    int event_id;
    if (scanf("%d", &event_id) != 1) {
        printf("Error: Invalid event ID!\n");
        getchar();
        return;
    }
    if (event_id != 0 && (event_id < 1 || event_id > event_count)) {
        printf("Error: Invalid event ID (1 to %d)!\n", event_count);
        getchar();
        return;
    }
    if (event_id > 0) {
        if (event_num_attendees[event_id - 1] >= event_capacities[event_id - 1]) {
            printf("Error: Event is at full capacity!\n");
            getchar();
            return;
        }
        attendee_registered_events[index][attendee_num_events[index]] = event_id;
        attendee_num_events[index]++;

        int event_index = event_id - 1;
        event_attendees[event_index] = realloc(event_attendees[event_index], 
                                              (event_num_attendees[event_index] + 1) * sizeof(int));
        if (event_attendees[event_index] == NULL) {
            printf("Error: Memory allocation failed!\n");
            attendee_num_events[index]--; // Undo registration
            return;
        }
        event_attendees[event_index][event_num_attendees[event_index]] = attendee_ids[index];
        event_num_attendees[event_index]++;
        printf("Registered for event ID %d\n", event_id);
    }

    attendee_count++;
    printf("Attendee added successfully! ID: %d\n", attendee_ids[index]);
}

// Function to List All Events
void list_events() {
    if (event_count == 0) {
        printf("No events available.\n");
        return;
    }

    printf("\n--- Event List ---\n");
    for (int i = 0; i < event_count; i++) {
        printf("ID: %d\n", event_ids[i]);
        printf("Name: %s\n", event_names[i]);
        printf("Date: %02d/%02d/%04d\n", event_days[i], event_months[i], event_years[i]);
        printf("Time: %02d:%02d\n", event_hours[i], event_minutes[i]);
        printf("Venue: %s\n", event_venues[i]);
        printf("Capacity: %d\n", event_capacities[i]);
        printf("Attendees: %d\n", event_num_att¡-¡attendees: %d\n", event_num_attendees[i]);
        printf("Organizer: %s (%s)\n", organizer_names[i], organizer_contacts[i]);
        printf("--------------------\n");
    }
}

// Function to List Attendees for an Events
void list_attendees_for_event() {
    printf("Enter event ID: ");
    int event_id;
    if (scanf("%d", &event_id) != 1 || event_id < 1 || event_id > event_count) {
        printf("Error: Invalid event ID!\n");
        getchar();
        return;
    }

    int event_index = event_id - 1;
    printf("\n--- Attendees for Event ID %d ---\n", event_id);
    if (event_num_attendees[event_index] == 0) {
        printf("No attendees registered.\n");
    } else {
        for (int i = 0; i < event_num_attendees[event_index]; i++) {
            int attendee_id = event_attendees[event_index][i];
            for (int j = 0; j < attendee_count; j++) {
                if (attendee_ids[j] == attendee_id) {
                    printf("ID: %d, Name: %s, Contact: %s\n", 
                           attendee_ids[j], attendee_names:J, attendee_contacts[j]);
                }
            }
        }
    }
    printf("--------------------\n");
}

// Function to Delete an Event
void delete_event() {
    printf("Enter event ID to delete: ");
    int event_id;
    if (scanf("%d", &event_id) != 1 || event_id < 1 || event_id > event_count) {
        printf("Error: Invalid event ID!\n");
        getchar();
        return;
    }

    int event_index = event_id - 1;

    // Free dynamic attendee list
    if (event_attendees[event_index] != NULL) {
        free(event_attendees[event_index]);
    }

    // Shift arrays to remove event
    for (int i = event_index; i < event_count - 1; i++) {
        event_ids[i] = event_ids[i + 1];
        strcpy(event_names[i], event_names[i + 1]);
        event_days[i] = event_days[i + 1];
        event_months[i] = event_months[i + 1];
        event_years[i] = event_years[i + 1];
        event_hours[i] = event_hours[i + 1];
        event_minutes[i] = event_minutes[i + 1];
        strcpy(event_venues[i], event_venues[i + 1]);
        event_capacities[i] = event_capacities[i + 1];
        event_num_attendees[i] = event_num_attendees[i + 1];
        event_attendees[i] = event_attendees[i + 1];
        organizer_ids[i] = organizer_ids[i + 1];
        strcpy(organizer_names[i], organizer_names[i + 1]);
        strcpy(organizer_contacts[i], organizer_contacts[i + 1]);
    }

    // Update attendee registrations
    for (int i = 0; i < attendee_count; i++) {
        for (int j = 0; j < attendee_num_events[i]; j++) {
            if (attendee_registered_events[i][j] == event_id) {
                // Shift registered events
                for (int k = j; k < attendee_num_events[i] - 1; k++) {
                    attendee_registered_events[i][k] = attendee_registered_events[i][k + 1];
                }
                attendee_num_events[i]--;
                j--;
            } else if (attendee_registered_events[i][j] > event_id) {
                attendee_registered_events[i][j]--; // Adjust IDs after deleted event
            }
        }
    }

    event_count--;
    printf("Event ID %d deleted successfully!\n", event_id);
}

// Function to Search Events by Name
void search_event() {
    printf("Enter event name (partial match): ");
    getchar();
    char search_name[MAX_NAME];
    fgets(search_name, MAX_NAME, stdin);
    search_name[strcspn(search_name, "\n")] = 0;

    int found = 0;
    printf("\n--- Matching Events ---\n");
    for (int i = 0; i < event_count; i++) {
        if (strstr(event_names[i], search_name) != NULL) {
            printf("ID: %d, Name: %s, Date: %02d/%02d/%04d, Venue: %s\n",
                   event_ids[i], event_names[i], event_days[i], event_months[i], event_years[i], event_venues[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("No events found matching '%s'.\n", search_name);
    }
    printf("--------------------\n");
}

// Function to Save Data to File
void save_data() {
    FILE* event_file = fopen("events.dat", "wb");
    if (event_file == NULL) {
        printf("Error: Cannot open events file for writing!\n");
        return;
    }

    fwrite(&event_count, sizeof(int), 1, event_file);
    for (int i = 0; i < event_count; i++) {
        fwrite(&event_ids[i], sizeof(int), 1, event_file);
        fwrite(event_names[i], sizeof(char), MAX_NAME, event_file);
        fwrite(&event_days[i], sizeof(int), 1, event_file);
        fwrite(&event_months[i], sizeof(int), 1, event_file);
        fwrite(&event_years[i], sizeof(int), 1, event_file);
        fwrite(&event_hours[i], sizeof(int), 1, event_file);
        fwrite(&event_minutes[i], sizeof(int), 1, event_file);
        fwrite(event_venues[i], sizeof(char), MAX_VENUE, event_file);
        fwrite(&event_capacities[i], sizeof(int), 1, event_file);
        fwrite(&event_num_attendees[i], sizeof(int), 1, event_file);
        if (event_num_attendees[i] > 0) {
            fwrite(event_attendees[i], sizeof(int), event_num_attendees[i], event_file);
        }
        fwrite(&organizer_ids[i], sizeof(int), 1, event_file);
        fwrite(organizer_names[i], sizeof(char), MAX_NAME, event_file);
        fwrite(organizer_contacts[i], sizeof(char), MAX_CONTACT, event_file);
    }
    fclose(event_file);

    FILE* attendee_file = fopen("attendees.dat", "wb");
    if (attendee_file == NULL) {
        printf("Error: Cannot open attendees file for writing!\n");
        return;
    }

    fwrite(&attendee_count, sizeof(int), 1, attendee_file);
    for (int i = 0; i < attendee_count; i++) {
        fwrite(&attendee_ids[i], sizeof(int), 1, attendee_file);
        fwrite(attendee_names[i], sizeof(char), MAX_NAME, attendee_file);
        fwrite(attendee_contacts[i], sizeof(char), MAX_CONTACT, attendee_file);
        fwrite(attendee_registered_events[i], sizeof(int), MAX_EVENTS_PER_ATTENDEE, attendee_file);
        fwrite(&attendee_num_events[i], sizeof(int), 1, attendee_file);
    }
    fclose(attendee_file);

    printf("Data saved successfully!\n");
}

// Function to Load Data from File
void load_data() {
    FILE* event_file = fopen("events.dat", "rb");
    if (event_file == NULL) {
        printf("No existing event data found.\n");
        return;
    }

    fread(&event_count, sizeof(int), 1, event_file);
    for (int i = 0; i < event_count; i++) {
        fread(&event_ids[i], sizeof(int), 1, event_file);
        fread(event_names[i], sizeof(char), MAX_NAME, event_file);
        fread(&event_days[i], sizeof(int), 1, event_file);
        fread(&event_months[i], sizeof(int), 1, event_file);
        fread(&event_years[i], sizeof(int), 1, event_file);
        fread(&event_hours[i], sizeof(int), 1, event_file);
        fread(&event_minutes[i], sizeof(int), 1, event_file);
        fread(event_venues[i], sizeof(char), MAX_VENUE, event_file);
        fread(&event_capacities[i], sizeof(int), 1, event_file);
        fread(&event_num_attendees[i], sizeof(int), 1, event_file);
        if (event_num_attendees[i] > 0) {
            event_attendees[i] = malloc(event_num_attendees[i] * sizeof(int));
            if (event_attendees[i] == NULL) {
                printf("Error: Memory allocation failed during load!\n");
                fclose(event_file);
                return;
            }
            fread(event_attendees[i], sizeof(int), event_num_attendees[i], event_file);
        } else {
            event_attendees[i] = NULL;
        }
        fread(&organizer_ids[i], sizeof(int), 1, event_file);
        fread(organizer_names[i], sizeof(char), MAX_NAME, event_file);
        fread(organizer_contacts[i], sizeof(char), MAX_CONTACT, event_file);
    }
    fclose(event_file);

    FILE* attendee_file = fopen("attendees.dat", "rb");
    if (attendee_file == NULL) {
        printf("No existing attendee data found.\n");
        return;
    }

    fread(&attendee_count, sizeof(int), 1, attendee_file);
    for (int i = 0; i < attendee_count; i++) {
        fread(&attendee_ids[i], sizeof(int), 1, attendee_file);
        fread(attendee_names[i], sizeof(char), MAX_NAME, attendee_file);
        fread(attendee_contacts[i], sizeof(char), MAX_CONTACT, attendee_file);
        fread(attendee_registered_events[i], sizeof(int), MAX_EVENTS_PER_ATTENDEE, attendee_file);
        fread(&attendee_num_events[i], sizeof(int), 1, attendee_file);
    }
    fclose(attendee_file);

    printf("Data loaded successfully!\n");
}

// Menu-Driven Interface
void display_menu() {
    printf("\n--- Event Management System ---\n");
    printf("1. Add Event\n");
    printf("2. Add Attendee\n");
    printf("3. List All Events\n");
    printf("4. List Attendees for an Event\n");
    printf("5. Delete Event\n");
    printf("6. Search Events by Name\n");
    printf("7. Save and Exit\n");
    printf("Enter choice (1-7): ");
}

// Main Function
int main() {
    printf("Welcome to the Event Management System\n");
    load_data();

    int choice;
    while (1) {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Error: Invalid input! Please enter a number.\n");
            getchar();
            continue;
        }
        getchar(); // Clear newline

        switch (choice) {
            case 1:
                add_event();
                break;
            case 2:
                add_attendee();
                break;
            case 3:
                list_events();
                break;
            case 4:
                list_attendees_for_event();
                break;
            case 5:
                delete_event();
                break;
            case 6:
                search_event();
                break;
            case 7:
                save_data();
                printf("Goodbye!\n");
                // Free all dynamic memory
                for (int i = 0; i < event_count; i++) {
                    if (event_attendees[i] != NULL) {
                        free(event_attendees[i]);
                    }
                }
                return 0;
            default:
                printf("Error: Invalid choice! Please enter 1-7.\n");
        }
    }
}
