/*
 * Project : KALNIRNAY – Gregorian to Hindi (Saka) Calendar Converter
 * Purpose : Convert a Gregorian calendar date (DD/MM/YYYY) to the
 *           corresponding date in the Hindi/Saka calendar.
 *
 * Team Members:
 *  - Aastha  : Overall problem definition, user interaction in main()
 *  - Aditya  : Linked list design for Hindi months (Node, createNode, append, freeList)
 *  - Archit  : Calendar data initialisation & day-of-year calculation
 *              (createCalendarList, isLeapYear, getDayOfYear)
 *  - Adarsh  : Core conversion logic and final output formatting (convertToHindi)
 *
 * Note: The code is intentionally documented in detail so that the complete
 *       conversion pipeline is easy to understand for academic evaluation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------- DATA STRUCTURE DEFINITIONS ---------------------- */
/* Implemented by Aditya */

typedef struct HindiMonth {
    char name[20];
    int startDayOfYear;
    struct HindiMonth* next;
} Node;

/* Implemented by Archit */
int daysInMonths[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

Node* createNode(char* name, int startDay);
void append(Node** head, char* name, int startDay);
void createCalendarList(Node** head);
int isLeapYear(int year);
int getDayOfYear(int d, int m, int y);
void convertToHindi(Node* head, int d, int m, int y);
void freeList(Node* head);

/* --------------------------- MAIN APPLICATION --------------------------- */
/* User interaction and program flow – Implemented by Aastha */

int main() {
    Node* head = NULL;      // Head pointer for the linked list of Hindi months
    int d, m, y;            // Day, month, year in Gregorian calendar
    char choice;            // To repeat conversions

    // Build the linked list representing the Hindi/Saka calendar months
    createCalendarList(&head);

    printf("=========================================\n");
    printf("   KALNIRNAY: Gregorian to Hindi Converter\n");
    printf("=========================================\n");
    printf(" Team: Aastha | Aditya | Archit | Adarsh\n");
    printf("=========================================\n");

    // Repeatedly take input from user until they choose to exit
    do {
        printf("\nEnter Gregorian Date (DD MM YYYY): ");
        if (scanf("%d %d %d", &d, &m, &y) != 3) {
            printf("Invalid input format.\n");
            break;
        }

        // Basic validation of date range. Detailed validation (like Feb 30) is not included.
        if (m < 1 || m > 12 || d < 1 || d > 31) {
            printf("Invalid Date Entered!\n");
        } else {
            convertToHindi(head, d, m, y);
        }

        printf("\nDo you want to convert another date? (y/n): ");
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    // Free dynamically allocated memory before exiting
    freeList(head);
    return 0;
}

/* --------------------- LINKED LIST UTILITY FUNCTIONS -------------------- */
/* Implemented by Aditya */

Node* createNode(char* name, int startDay) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    strcpy(newNode->name, name);
    newNode->startDayOfYear = startDay;
    newNode->next = NULL;
    return newNode;
}

// Append a new Hindi month node at the end of the linked list
void append(Node** head, char* name, int startDay) {
    Node* newNode = createNode(name, startDay);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

/* ------------------- HINDI CALENDAR LIST INITIALISATION ----------------- */
/* Implemented by Archit */

// Initialise the Hindi/Saka calendar as a linked list with
// each month mapped to its starting day number in the Gregorian year.
void createCalendarList(Node** head) {
    append(head, "Chaitra", 81);
    append(head, "Vaisakha", 112);
    append(head, "Jyaistha", 143);
    append(head, "Asadha", 174);
    append(head, "Shravana", 205);
    append(head, "Bhadra", 236);
    append(head, "Ashvina", 267);
    append(head, "Kartika", 298);
    append(head, "Agrahayana", 328);
    append(head, "Pausha", 358);
    append(head, "Magha", 21);
    append(head, "Phalguna", 51);
}

/* Free all nodes in the Hindi month linked list – Implemented by Aditya */
void freeList(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/* ------------------------ DATE HELPER CALCULATIONS ---------------------- */
/* Implemented by Archit */

// Check if a given Gregorian year is a leap year
int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Compute "day of year" (1–365 or 366) for a given Gregorian date.
int getDayOfYear(int d, int m, int y) {
    int dayCount = 0;
    int i;
    
    // Adjust February for leap years
    if (isLeapYear(y)) daysInMonths[2] = 29;
    else daysInMonths[2] = 28;

    // Sum days of all previous months
    for (i = 1; i < m; i++) {
        dayCount += daysInMonths[i];
    }
    // Add current day
    dayCount += d;
    return dayCount;
}

/* -------------------- GREGORIAN → HINDI DATE CONVERSION ----------------- */
/* Core conversion logic – Implemented by Adarsh */

void convertToHindi(Node* head, int d, int m, int y) {
    int dayOfYear = getDayOfYear(d, m, y);  // Position of the date within the Gregorian year
    int sakaYear = y - 78;                 // Approximate relation: Saka = Gregorian - 78
    
    // If date is before start of Chaitra (81st day), it belongs to the previous Saka year
    if (dayOfYear < 81) {
        sakaYear--;
    }

    Node* current = head;
    Node* resultMonth = NULL;
    
    // For dates before the 81st day, search only among months starting before 81
    if (dayOfYear < 81) {
         while (current != NULL) {
             if (current->startDayOfYear <= dayOfYear && current->startDayOfYear < 81) {
                 resultMonth = current;
             }
             current = current->next;
         }
         // Edge case: if no month is found, assume Pausha (late winter month)
         if (resultMonth == NULL) resultMonth = createNode("Pausha", 0); 
    } 
    else {
        // For normal case (after Chaitra begins), take the last month whose
        // starting day is less than or equal to the current dayOfYear.
        while (current != NULL) {
            if (current->startDayOfYear <= dayOfYear) {
                 resultMonth = current;
            }
            current = current->next;
        }
    }

    int hindiDay;
    // If resultMonth start is greater (wrapped around end of year), adjust by adding 365
    if (resultMonth->startDayOfYear > dayOfYear) { 
        hindiDay = (dayOfYear + 365) - resultMonth->startDayOfYear + 1;
    } else {
        hindiDay = dayOfYear - resultMonth->startDayOfYear + 1;
    }

    // Pretty output of final result
    printf("-----------------------------------------\n");
    printf("Gregorian: %02d/%02d/%d\n", d, m, y);
    printf("Hindi Date: %d %s, Saka %d\n", hindiDay, resultMonth->name, sakaYear);
    printf("-----------------------------------------\n");
    
    // If we allocated a temporary node for Pausha, free it here.
    if (strcmp(resultMonth->name, "Pausha") == 0 && resultMonth->next == NULL) free(resultMonth);
}