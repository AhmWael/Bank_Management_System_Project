/*
Bank Management System Project
Made by:
Ahmad Zaki Ahmad Zaki Mahrous 8910
Ahmad Wael Mohammed Gaber 8836
Seifeldin Ahmed Maazouz Zeid Ismail 8927
Asser Mohamed Ahmed Hanafy Mahmoud 8833
Abdelrahman Ahmed Mohamed Agha 8918
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

bool logged_in = 0;

typedef struct {
    int month:4, year;
}date;

typedef struct {
    unsigned long long from, to;
    float amount;
}transfer_details;

typedef struct {
    char* username, password;
}employee;

typedef struct {
    unsigned long long account_no;
    char *name, email, phone;
    int balance;
    date date_opened;
    transfer_details recent_transfers[5];
}account;

void menu();
void log_out();
void quit();

int main()
{
    while (1) {
        menu();
    }

    return 0;
}

void menu() {
    printf("Welcome to the main menu.\n");
    if (!logged_in) {
        printf("To login enter 1\n");
        printf("To quit enter 2\n");
        int input;
        scanf("%d", &input);
        while ((input != 1) && (input != 2)) {
            printf("Invalid input! You must enter either 0 or 1.\n");
            printf("To login enter 1\n");
            printf("To quit enter 2\n");
            scanf("%d", &input);
        }
        if (input == 1) {
            printf("logging in\n"); // should be replaced with the function login();
            logged_in = 1; // when login() is created this line should be executed when username and password match
        }
        if (input == 2)
            quit();
        return ;
    }
    printf("To add an account, enter 1\n");
    printf("To delete an account, enter 2\n");
    printf("To modify the data of an account, enter 3\n");
    printf("To search for an account using account number, enter 4\n");
    printf("For advanced searching, enter 5\n");
    printf("To withdraw money from an account, enter 6\n");
    printf("To deposit money into an account, enter 7\n");
    printf("To transfer money from one account to another, enter 8\n");
    printf("To get a report about the transactions of an account, enter 9\n");
    printf("To print data of all accounts, enter 10\n");
    printf("To log out, enter 11\n");
    printf("To quit the program entirely, enter 12\n");
    int input;
    scanf("%d", &input);
    while ((input < 1) || (input > 12)) {
        printf("Invalid input! Input should be between 1 and 12\n");
        printf("To add an account, enter 1\n");
        printf("To delete an account, enter 2\n");
        printf("To modify the data of an account, enter 3\n");
        printf("To search for an account using account number, enter 4\n");
        printf("For advanced searching, enter 5\n");
        printf("To withdraw money from an account, enter 6\n");
        printf("To deposit money into an account, enter 7\n");
        printf("To transfer money from one account to another, enter 8\n");
        printf("To get a report about the transactions of an account, enter 9\n");
        printf("To print data of all accounts, enter 10\n");
        printf("To log out, enter 11\n");
        printf("To quit the program entirely, enter 12\n");
        scanf("%d", &input);
    }
    switch (input) {
        // all the print statements here are just temporary place holders for the actual functions
        // please replace them once the function has been made
        case 1: printf("Adding an account\n"); break;
        case 2: printf("Deleting an account\n"); break;
        case 3: printf("Modifying account\n"); break;
        case 4: printf("Searching for an account\n"); break;
        case 5: printf("Advanced searching\n"); break;
        case 6: printf("Withdrawing money\n"); break;
        case 7: printf("Depositing money\n"); break;
        case 8: printf("Transferring money\n"); break;
        case 9: printf("Reporting\n"); break;
        case 10: printf ("Printing data of all users\n"); break;
        case 11: log_out(); break;
        default : quit();
    }
}

void log_out() {
    printf("Are you sure you want to log out?\n");
    printf("Enter 1 to confirm logging out or 0 if you want to return.\n");
    int conf;
    scanf("%d", &conf);
    while ((conf != 1) && (conf != 0)) {
        printf("Invalid input! You must enter either 0 or 1.\n");
        printf("Enter 1 to confirm logging out or 0 if you want to return.\n");
        scanf("%d", &conf);
    }
    if (conf) {
        logged_in = 0;
        printf("Logging out\nGoodbye + employee name\n"); // should print the username of employee
    }
    printf("Logging out has been cancelled\n");
}

void quit() {
    printf("Are you sure you want to quit?\n");
    printf("Enter 1 to confirm quitting or 0 if you want to return.\n");
    int conf;
    scanf("%d", &conf);
    while ((conf != 1) && (conf != 0)) {
        printf("Invalid input! You must enter either 0 or 1.\n");
        printf("Enter 1 to confirm quitting or 0 if you want to return.\n");
        scanf("%d", &conf);
    }
    if (conf) {
        printf("Quitting the program.\nGoodbye");
        exit(0);
    }
    printf("Quitting the program has been cancelled\n");
}
