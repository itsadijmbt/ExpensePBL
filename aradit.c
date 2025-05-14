#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char date[11];    // format: DD/MM/YYYY
    float amt;        // amount spent
    char desc[100];   // description/type of expense
} Exp;

void addExp(void);
void viewExp(void);
void clearExp(void);
void flushInput(void);

int main(void) {
    int choice;

    do {
        printf("\n1. Add Expense\n"
               "2. View Expenses\n"
               "3. Clear All\n"
               "4. Exit\n"
               "Choose: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            flushInput();
            continue;
        }
        flushInput();

        switch (choice) {
            case 1: addExp();      break;
            case 2: viewExp();     break;
            case 3: clearExp();    break;
            case 4: printf("Goodbye!\n"); break;
            default: printf("Invalid choice, try again.\n");
        }
    } while (choice != 4);

    return 0;
}

void flushInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void addExp(void) {
    Exp e;
    char buf[128];
    FILE *f = fopen("exp.dat", "a");
    if (!f) {
        perror("Error opening file for writing");
        return;
    }

    // 1) Read date
    printf("Enter Date (DD/MM/YYYY): ");
    if (!fgets(buf, sizeof buf, stdin)) {
        printf("Input error.\n");
        fclose(f);
        return;
    }
    buf[strcspn(buf, "\r\n")] = '\0';
    strncpy(e.date, buf, sizeof e.date);
    e.date[sizeof(e.date)-1] = '\0';

    // 2) Read amount
    printf("Enter Amount: ");
    if (!fgets(buf, sizeof buf, stdin)) {
        printf("Input error.\n");
        fclose(f);
        return;
    }
    e.amt = strtof(buf, NULL);

    // 3) Read description
    printf("Enter Description: ");
    if (!fgets(buf, sizeof buf, stdin)) {
        printf("Input error.\n");
        fclose(f);
        return;
    }
    buf[strcspn(buf, "\r\n")] = '\0';
    strncpy(e.desc, buf, sizeof e.desc);
    e.desc[sizeof(e.desc)-1] = '\0';

    // Save record
    fprintf(f, "%s|%.2f|%s\n", e.date, e.amt, e.desc);
    fclose(f);

    printf("Record added!\n");
}

void viewExp(void) {
    Exp e;
    FILE *f = fopen("exp.dat", "r");
    if (!f) {
        printf("No records found.\n");
        return;
    }

    printf("\nDate       |  Amount  | Description\n");
    printf("---------------------------------------------\n");
    while (fscanf(f, "%10[^|]|%f|%99[^\n]\n", e.date, &e.amt, e.desc) == 3) {
        printf("%-10s | %8.2f | %s\n", e.date, e.amt, e.desc);
    }
    fclose(f);
}

void clearExp(void) {
    if (remove("exp.dat") == 0)
        printf("All records cleared!\n");
    else
        printf("No records to clear (or error deleting).\n");
}
