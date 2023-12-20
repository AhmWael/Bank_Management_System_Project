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
#include <ctype.h>
#include <stdbool.h>

bool logged_in = 0;

typedef struct {
    unsigned month:4, year;
}date;

typedef struct {
    unsigned long long from, to;
    float amount;
}transfer_details;

typedef struct {
    char *username, *password;
}employee;

typedef struct {
    unsigned long long account_no;
    char *name, *email, *phone;
    double balance;
    date date_opened;
    transfer_details recent_transfers[5];
}account;

account* constAcc(unsigned long long account_no, char *name, char *email, double balance, char *phone, date date_opened);
void distAcc(account *p);
account* decodeText(char* line);
void load(account** accounts, int *numRec);
void unload(account** accounts, int numRec);
date *constDate(int month, int year);
void setDate(account *a, int month, int year);
void printAccount(account *a);
void menu();
void log_out();
void quit();

int main()
{
    /*
    //{FOR TESTING LOAD}

    account** accounts;
    int num_acc;
    load(accounts, &num_acc);  //loads accounts
    int i;
    for (i = 0; i < num_acc; i++)
    {
        printAccount(accounts[i]);  //prints all accounts
    }
    unload(accounts, num_acc);  //unloads accounts
    */

    while (1) {
        menu();
    }

    return 0;
}

account* constAcc(unsigned long long account_no, char *name, char *email, double balance, char *phone, date date_opened)
{
    account* p = malloc(sizeof(account));

    p->account_no = account_no;

    p->name = malloc(strlen(name) + 1);
    strcpy(p->name, name);

    p->email = malloc(strlen(email) + 1);
    strcpy(p->email, email);

    p->balance = balance;

    p->phone = malloc(strlen(phone) + 1);
    strcpy(p->phone, phone);

    p->date_opened = date_opened;

    return p;
}

void distAcc(account *p)
{
    free(p->name);
    free(p->email);
    free(p->phone);
    free(p);
}

account* decodeText(char* line)
{
    unsigned long long accNum = atoll(strtok(line, ","));
    char *name = strtok(NULL, ",");
    char *email = strtok(NULL, ",");
    double balance = strtod(strtok(NULL, ","), NULL);
    char *phone = strtok(NULL, ",");
    date date_opened;
    date_opened.month = atoi(strtok(NULL, "-"));
    date_opened.year =  atoi(strtok(NULL, ","));

    return constAcc(accNum, name, email, balance, phone, date_opened);
}

void load(account** accounts, int *numRec)
{
    FILE* fp;
    fp = fopen("accounts.txt", "r");
    if(!fp)
    {
        printf("File not found");
        return;
    }

    char record[100];
    rewind(fp);
    *numRec = 0;
    while(fgets(record, sizeof(record), fp))
    {
        if (record[0] == '\n') continue; //skip empty line
        (*numRec)++;
    }

    //printf("num of acc = %d\n", *numRec);  for testing delete later


    *accounts = malloc((*numRec) * sizeof(account));

    rewind(fp);
    int i;
    for (i = 0; i < *numRec; i++)
    {
        if(!fgets(record, sizeof(record), fp)) break;

        if (record[0] == '\n') //skip empty line
        {
            i--;
            continue;
        }
        accounts[i] = decodeText(record);
    }

    fclose(fp);
}

void unload(account** accounts, int numRec)
{
    int i;
    for (i = 0; i < numRec; i++)
    {
        distAcc(accounts[i]);
    }
    //free(*accounts); //not working returns error(returned -1073740940) FIX LATER
}

date *constDate(int month, int year)
{
    date *d = malloc(sizeof(date));
    d->month = month;
    d->year = year;
    return d;
}

void setDate(account *a, int month, int year)
{
    date *d = constDate(month, year);
    a->date_opened = *d;
    free(d);
}

void printAccount(account *a){
    printf("\nAccount Number : %lld\n", a->account_no);
    printf("Name: %s\n", a->name);
    printf("E-mail : %s\n", a->email);
    printf("Balance: %.2lf\n", a->balance);
    printf("Mobile: %s\n", a->phone);
    printf("Date Opened: %d-%d\n", a->date_opened.month, a->date_opened.year);// change format of printing date
}

void menu() {
    if (!logged_in) {
        printf("To login enter 1\n");
        printf("To quit enter 2\n");
        char input[100] = "";
        //fflush(stdin);
        gets(input);
        while ((strcmp(input, "1") != 0) && (strcmp(input, "2") != 0)) {
            printf("Invalid input! You must enter either 1 or 2.\n");
            printf("To login enter 1\n");
            printf("To quit enter 2\n");
            //fflush(stdin);
            gets(input);
        }
        if (input[0] == '1') {
            printf("logging in\n"); // should be replaced with the function login();
            logged_in = 1; // when login() is created this line should be executed when username and password match
        }
        if (input[0] == '2')
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
    char inputs[100] = "";
    int i;
    bool contain_char = 0, bad_input = 1;
    while (bad_input) {
        contain_char = 0;
        gets(inputs);
        for (i = 0; inputs[i] != '\0'; i ++) {
            if ((inputs[i] == '-') && (i == 0))
                continue ;
            if (!isdigit(inputs[i])) {
                contain_char = 1;
                break ;
            }
        }
        if (contain_char) {
            printf("Invalid input! Input should only consist of digits\n");
        }
        else {
            if ((strcmp(inputs, "1") == 0) || (strcmp(inputs, "2") == 0) || (strcmp(inputs, "3") == 0)
                || (strcmp(inputs, "4") == 0) || (strcmp(inputs, "5") == 0) || (strcmp(inputs, "6") == 0)
                || (strcmp(inputs, "7") == 0) || (strcmp(inputs, "8") == 0) || (strcmp(inputs, "9") == 0)
                || (strcmp(inputs, "10") == 0) || (strcmp(inputs, "11") == 0) || (strcmp(inputs, "12") == 0)) {
                    bad_input = 0;
                    continue ;
                }
            else {
                printf("Invalid input! Input should be between 1 and 12\n");
            }
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
    }
    int input = 0;
    for (i = 0; inputs[i] != '\0'; i ++) {
        input = input * 10 + inputs[i] - '0';
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
    char conf[100] = "";
    //fflush(stdin);
    gets(conf);
    while ((strcmp(conf, "1") != 0) && (strcmp(conf, "0") != 0)) {
        printf("Invalid input! You must enter either 0 or 1.\n");
        printf("Enter 1 to confirm logging out or 0 if you want to return.\n");
        //fflush(stdin);
        gets(conf);
    }
    if (conf[0] == '1') {
        logged_in = 0;
        printf("Logging out\nGoodbye + employee name\n"); // should print the username of employee
    }
    printf("Logging out has been cancelled\n");
}

void quit() {
    printf("Are you sure you want to quit?\n");
    printf("Enter 1 to confirm quitting or 0 if you want to return.\n");
    char conf[100] = "";
    //fflush(stdin);
    gets(conf);
    while ((strcmp(conf, "1") != 0) && (strcmp(conf, "0") != 0)) {
        printf("Invalid input! You must enter either 0 or 1.\n");
        printf("Enter 1 to confirm quitting or 0 if you want to return.\n");
        //fflush(stdin);
        gets(conf);
    }
    if (conf[0] == '1') {
        printf("Quitting the program.\nGoodbye");
        exit(0);
    }
    printf("Quitting the program has been cancelled\n");
}
