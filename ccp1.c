#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "text.txt"  // File name

// Function to load account data from file
// PIN parameter is now a pointer to an integer (int *)
int load_account_data(int *pin, char *name, float *balance) {
    // Open the file for reading
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("Account file not found (%s). Please create a 'text.txt' file with initial data (e.g., 1234,John Doe,500.00).\n", FILE_NAME);
        return 0;
    }

    // Read the account data (PIN, name, balance)
    // Using %d for the integer PIN, and %49[^\n] for the name
    int result = fscanf(file, "%d,%49[^\n],%f", pin, name, balance);
    
    if (result != 3) {
        printf("Error reading account data from file. Please check the format.\n");
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

// Function to save account data to file
// PIN parameter is now a plain integer (int)
void save_account_data(int pin, const char *name, float balance) {
    // Open the file for writing ("w" overwrites existing content)
    FILE *file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        printf("Critical Error: Cannot save account data.\n");
        return;
    }

    // Save the data in the required format: PIN,Name,Balance
    // Using %d for the integer PIN
    fprintf(file, "%d,%s,%.2f\n", pin, name, balance);

    fclose(file);
}

// Main ATM function
void atm_machine() {
    // PIN variables are now integers
    int pin, entered_pin;
    char name[50];
    float balance;

    // Load account data from the file
    if (!load_account_data(&pin, name, &balance)) {
        // If file load fails, terminate the program
        return;
    }

    // Prompt user for PIN
    printf("\n--- Simple ATM Login ---\n");
    printf("Enter your PIN (e.g., %d): ", pin);
    
    // Clear the input buffer before reading the PIN
    while (getchar() != '\n'); 

    // Read PIN as an integer
    if (scanf("%d", &entered_pin) != 1) {
        printf("Invalid PIN format. Exiting...\n");
        // Clear buffer just in case
        while (getchar() != '\n'); 
        return;
    }
    
    // Check if the entered PIN is correct using direct integer comparison
    if (entered_pin != pin) {
        printf("Incorrect PIN. Exiting...\n");
        return;
    }

    printf("\nAccess Granted. Welcome %s!\n", name);

    // ATM menu loop
    int choice;
    float amount;
    
    while (1) {
        printf("\n--- ATM Menu ---\n");
        printf("1. Withdraw\n");
        printf("2. Deposit\n");
        printf("3. Check Balance\n");
        printf("4. Exit\n");
        printf("Please choose an option (1-4): ");
        
        // Use an integer to capture scanf's return value for robustness
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer to prevent infinite loop
            while (getchar() != '\n'); 
            continue;
        }

        if (choice == 1) {  // Withdraw
            printf("Enter amount to withdraw: ");
            if (scanf("%f", &amount) != 1 || amount <= 0) {
                printf("Invalid amount entered.\n");
                while (getchar() != '\n'); 
                continue;
            }

            if (amount > balance) {
                printf("Transaction Failed: Insufficient balance!\n");
            } else {
                balance -= amount;
                printf("Success! Withdrawn $%.2f. New balance: $%.2f\n", amount, balance);
                save_account_data(pin, name, balance);  // Save updated data
            }
        } else if (choice == 2) {  // Deposit
            printf("Enter amount to deposit: ");
            if (scanf("%f", &amount) != 1 || amount <= 0) {
                printf("Invalid amount entered. Must be greater than zero.\n");
                while (getchar() != '\n'); 
                continue;
            }

            balance += amount;
            printf("Success! Deposited $%.2f. New balance: $%.2f\n", amount, balance);
            save_account_data(pin, name, balance);  // Save updated data
            
        } else if (choice == 3) {  // Check Balance
            printf("Your current balance is: $%.2f\n", balance);
            
        } else if (choice == 4) {  // Exit
            printf("Thank you for using the ATM. Goodbye!\n");
            break;
            
        } else {
            printf("Invalid option. Please try again.\n");
        }
    }
}

int main() {
    atm_machine();
    return 0;
}