#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Contact {
    char name[50];
    char phone[15];
};

void addContact();
void viewContacts();
void searchContact();
void deleteContact();
void sortContacts(int byName);
int compareByName(const void *a, const void *b);
int compareByPhone(const void *a, const void *b);

int main() {
    int choice;

    while (1) {
        printf("\n----- Contact Book -----\n");
        printf("1. Add Contact\n");
        printf("2. View Contacts\n");
        printf("3. Search Contact\n");
        printf("4. Delete Contact\n");
        printf("5. Sort by Name\n");
        printf("6. Sort by Phone\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1: addContact(); break;
            case 2: viewContacts(); break;
            case 3: searchContact(); break;
            case 4: deleteContact(); break;
            case 5: sortContacts(1); break; // Sort by name
            case 6: sortContacts(0); break; // Sort by phone
            case 7: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

// Add contact
void addContact() {
    struct Contact c;
    FILE *fp = fopen("contacts.txt", "a");

    if (fp == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    printf("Enter name: ");
    fgets(c.name, sizeof(c.name), stdin);
    c.name[strcspn(c.name, "\n")] = 0; // remove newline

    printf("Enter phone number: ");
    fgets(c.phone, sizeof(c.phone), stdin);
    c.phone[strcspn(c.phone, "\n")] = 0;

    fwrite(&c, sizeof(c), 1, fp);
    fclose(fp);
    printf("Contact added successfully.\n");
}

// View contacts
void viewContacts() {
    struct Contact c;
    FILE *fp = fopen("contacts.txt", "r");

    if (fp == NULL) {
        printf("No contacts found.\n");
        return;
    }

    printf("\n---- Contact List ----\n");
    while (fread(&c, sizeof(c), 1, fp)) {
        printf("Name : %s\n", c.name);
        printf("Phone: %s\n", c.phone);
        printf("----------------------\n");
    }
    fclose(fp);
}

// Search contact by name
void searchContact() {
    struct Contact c;
    char searchName[50];
    int found = 0;

    FILE *fp = fopen("contacts.txt", "r");
    if (fp == NULL) {
        printf("No contacts found.\n");
        return;
    }

    printf("Enter name to search: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = 0;

    while (fread(&c, sizeof(c), 1, fp)) {
        if (strcasecmp(c.name, searchName) == 0) {
            printf("Contact found:\n");
            printf("Name : %s\n", c.name);
            printf("Phone: %s\n", c.phone);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Contact not found.\n");
    }

    fclose(fp);
}

// Delete contact by name
void deleteContact() {
    struct Contact c;
    char deleteName[50];
    int found = 0;

    FILE *fp = fopen("contacts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    printf("Enter name to delete: ");
    fgets(deleteName, sizeof(deleteName), stdin);
    deleteName[strcspn(deleteName, "\n")] = 0;

    while (fread(&c, sizeof(c), 1, fp)) {
        if (strcasecmp(c.name, deleteName) != 0) {
            fwrite(&c, sizeof(c), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("contacts.txt");
    rename("temp.txt", "contacts.txt");

    if (found)
        printf("Contact deleted successfully.\n");
    else
        printf("Contact not found.\n");
}

// Comparison function for sorting by name
int compareByName(const void *a, const void *b) {
    struct Contact *c1 = (struct Contact *)a;
    struct Contact *c2 = (struct Contact *)b;
    return strcasecmp(c1->name, c2->name);
}

// Comparison function for sorting by phone
int compareByPhone(const void *a, const void *b) {
    struct Contact *c1 = (struct Contact *)a;
    struct Contact *c2 = (struct Contact *)b;
    return strcmp(c1->phone, c2->phone);
}

// Sort contacts by name or phone
void sortContacts(int byName) {
    struct Contact c[100]; // adjust max as needed
    int count = 0;

    FILE *fp = fopen("contacts.txt", "r");
    if (!fp) {
        printf("No contacts found.\n");
        return;
    }

    // Read all contacts into array
    while (fread(&c[count], sizeof(struct Contact), 1, fp) && count < 100) {
        count++;
    }
    fclose(fp);

    if (count == 0) {
        printf("No contacts to sort.\n");
        return;
    }

    // Sort the array
    if (byName)
        qsort(c, count, sizeof(struct Contact), compareByName);
    else
        qsort(c, count, sizeof(struct Contact), compareByPhone);

    // Display sorted contacts
    printf("\n--- Sorted Contacts (%s) ---\n", byName ? "By Name" : "By Phone");
    for (int i = 0; i < count; i++) {
        printf("Name : %s\n", c[i].name);
        printf("Phone: %s\n", c[i].phone);
        printf("--------------------------\n");
    }

    // Optional: Save sorted contacts back to file
    FILE *fout = fopen("contacts.txt", "w");
    if (fout) {
        for (int i = 0; i < count; i++) {
            fwrite(&c[i], sizeof(struct Contact), 1, fout);
        }
        fclose(fout);
        printf("Contacts have been sorted and saved.\n");
    }
}