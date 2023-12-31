/*
Bank Management System Project
Made by:
Ahmad Zaki Ahmad Zaki Mahrous 8910
Ahmed Wael Mohamed Gaber 8836
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
#include <time.h>

bool logged_in = 0;
int num_acc = 0;

typedef struct
{
    unsigned month:4, year;
} date;

typedef struct
{
    unsigned long long from, to;
    float amount;
} transaction_details;

typedef struct
{
    char *username, *password;
} employee;
employee current_employee;

typedef struct
{
    unsigned long long account_no;
    char *name, *email, *phone;
    double balance;
    date date_opened;
} account;

account** accounts = NULL;

void login();
void load();
void query_search();
void advanced_search();
void add ();
void delete_account();
void modify_acc();
void withdraw();
void deposit();
void transfer();
void report();
void print();
void save();
void log_out();
void quit();
void menu();

account *constAcc(unsigned long long account_no, char *name, char *email, double balance, char *phone, date date_opened);
void printAccount(account *a);
void distAcc(account *p);
date *constDate(int month, int year);
void setDate(account *a, int month, int year);
account *decodeText(char *line);
void unload();

void create_transaction_file(unsigned long long account_no);
void saveTransaction(unsigned long long account_no, unsigned long long from, unsigned long long to, double amount);
/****Input Validation Functions***/
int readInteger();
unsigned long long read_account_no();
bool cont_dig(char *x);
bool cont_spec(char *x, int mode);
char *readPhone();
double readAmount();
char *readEmail();
char *readName();
/*********************************/
int binary_search(unsigned long long account_no, int *acc_index);
int SortByNum(const void *a,const void *b);
int SortByName(const void *a,const void *b);
int SortByDate(const void *a,const void *b);
int SortByBalance(const void *a,const void *b);


int main()
{
    //load();  //loads accounts

    while (1)
    {
        menu();
    }

    return 0;
}

void login()
{
    FILE *fptr;
    int i=0, flagu=0,flagp=0,inv_p=0,inv_u=0;
    char *c = malloc(100);
    char *x = malloc(50);
    char *pass = malloc(50);
    char *token;
    current_employee.username = malloc(50);
    current_employee.password = malloc(50);

    fptr = fopen("users.txt", "r");
    if (fptr == NULL)
    {
        printf("Error opening file");
        exit(1);
    }
    do
    {
        rewind(fptr);                //used to reset pointer to beginning of file
        printf("Enter username: ");
        scanf("%s", x);
        i=1;
        while(i&&!feof(fptr))
        {
            fgets(c,99,fptr);
            token=strtok(c," ");           //used to separate password and username
            i=strcmp(token,x);
        }
        if(!i)
        {
            flagu=1;
            strcpy(current_employee.username, x);
            token=strtok(NULL," \n"); //checks second part of string
            do
            {
                printf("Enter password: ");
                scanf("%s",pass);
                if(strcmp(token,pass))
                {
                    printf("invalid password\n");
                    inv_p++;
                    printf("You have %d tries left\n", 5 - inv_p);
                }
                else
                {
                    flagp=1;
                    strcpy(current_employee.password, pass);
                    break;
                }
            }
            while(!flagp&&(inv_p<5));
            if(inv_p==5)
            {
                printf("too many invalid passwords have been entered\nQuitting the program");
                exit(3);
            }
        }
        else
        {
            printf("Invalid username\n");
            inv_u++;
            printf("You have %d tries left\n", 5 - inv_u);
        }
    }
    while (!flagu&&(inv_u<5));
    if(inv_u==5)
    {
        printf("too many invalid usernames have been entered\nQuitting the program");
        exit(3);
    }

    fclose(fptr);
    free(c);
    free(x);
    fflush(stdin);
}

void load()
{
    printf("\nLoading accounts...\n");
    FILE* fp;
    fp = fopen("accounts.txt", "r");
    if(!fp)
    {
        printf("Error: accounts.txt file not found");
        return;
    }

    char record[100];
    rewind(fp);
    num_acc = 0;
    while(fgets(record, sizeof(record), fp))
    {
        if (record[0] == '\n') continue; //skip empty line
        num_acc++;
    }

    //printf("num of acc = %d\n", num_acc);  //for testing delete later

    accounts = (account**)malloc((num_acc) * sizeof(account*));

    rewind(fp);
    int i;
    for (i = 0; i < num_acc; i++)
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

void query_search()
{
    unsigned long long account_no;

    qsort(accounts, num_acc, sizeof(*accounts), SortByNum);
    int found, acc_index;
    do
    {
        do
        {
            printf("Enter account number: ");
            account_no = read_account_no();
            if (account_no == 0)
                printf("Invalid Account Number!\n");
        }
        while (account_no == 0);
        found = binary_search(account_no,&acc_index);
        if (!found)
            printf("The Specified Account is not found!\n\n");
    }
    while (!found);
    printAccount(accounts[acc_index]);
    printf("\n");
}

void advanced_search()
{
    printf("Enter keyword [enter -1 to return to menu]: ");
    char key[50];
    int len;
    do{
        fgets(key, 49, stdin);
        len = strlen(key);
        if (len > 0 && key[len - 1] == '\n')
            key[len - 1] = '\0';
        if(!strcmp(key, "-1")){
            printf("Returning to Main Menu.\n");
            return;
        }
        if(!((key[0] >= 'a' && key[0] <= 'z') || (key[0] >= 'A' && key[0] <= 'Z') || (key[0] >= '0' && key[0] <= '9')))
            printf("Invalid input!\nPlease enter keyword starting with a letter or a number only[enter -1 to return to menu]: ");
    }while(!((key[0] >= 'a' && key[0] <= 'z') || (key[0] >= 'A' && key[0] <= 'Z') || (key[0] >= '0' && key[0] <= '9')));

    printf("\nSearching for \"%s\"\n", key);
    int i, char_count = 0, search_crit = 0;
    if(key[0] >= '0' && key[0] <= '9')
    {
        for(i = 0; i<strlen(key); i++)
            if(!(key[i] >= '0' && key[i] <= '9'))
            {
                char_count = 1;
                break;
            }
        if(!char_count)
            search_crit = 1;
    }
    else
        search_crit = 2;

    if(search_crit == 1)  // Search in account_number, phone
    {
        bool found = 0;
        printf("Search Results: \n\n");

        int i;
        for(i = 0; i<num_acc; i++)
        {
            char account_no_str[11];
            snprintf(account_no_str, sizeof(account_no_str), "%llu", accounts[i]->account_no);

            if(strstr(account_no_str, key) || strstr(accounts[i]->phone, key))
            {
                printAccount(accounts[i]);
                printf("\n");
                found = 1;
            }
        }
        if(!found){
            printf("\nNo match found.\nTry entering different keyword.\n");
            advanced_search();
        }
    }
    else if(search_crit == 2)  // Search in name, email
    {
        bool found = 0;
        printf("Search Results: \n\n");

        int i;
        for(i = 0; i<num_acc; i++)
        {
            char *name_str = malloc(strlen(accounts[i]->name) + 1);
            char *mail_str = malloc(strlen(accounts[i]->email) + 1);
            strcpy(name_str, accounts[i]->name);
            strcpy(mail_str, accounts[i]->email);

            int j;
            for(j = 0; key[j]; j++)
                key[j] = tolower(key[j]);

            for(j = 0; name_str[j]; j++)
                name_str[j] = tolower(name_str[j]);

            for(j = 0; mail_str[j]; j++)
                mail_str[j] = tolower(mail_str[j]);

            if(strstr(name_str, key) || strstr(mail_str, key))
            {
                printAccount(accounts[i]);
                printf("\n");
                found = 1;
            }
            free(name_str);
            free(mail_str);
        }
        if(!found){
            printf("\nNo match found.\nTry entering different keyword.\n");
            advanced_search();
        }
    }
}
void add()
{
    unsigned long long x,acc_n;
    char *name = malloc(50);
    char *phone = malloc(50);
    char *email = malloc(50);
    int i=1,flag=0,month,year,check;
    double bal=0.0;
    date d;

    printf("Enter account number:");
    do
    {
        x=read_account_no();

        if(x==0)
        {
            printf("Invalid Input! Enter digits and make sure to enter 10 digits\nEnter a new one:");
            continue;
        }
        flag=1;
        for(i = 0; i<num_acc; i++)
        {
            if(accounts[i]->account_no == x)
            {
                printf("Account number already exist\nEnter a new one:");
                flag=0;
                break;
            }

        }


    }
    while(!flag);
    accounts=realloc(accounts,(num_acc+1) * sizeof(account*));
    num_acc++;
    do
    {
        flag=0;
        printf("Enter the new name: ");
        strcpy(name, readName());
        if(!strcmp(name, "1"))
        {
            printf("Invalid Input! Can't leave field empty!\n");
            flag=1;
        }
        if(!strcmp(name, "2"))
        {
            printf("Invalid Input! Can't start with a space!\n");
            flag=1;
        }
        if(!strcmp(name, "3"))
        {
            printf("Invalid Input! Only enter characters!\n");
            flag=1;
        }
    }
    while(flag);
    printf("\nEnter email: ");
    do
    {
        flag=0;
        email=readEmail();
        if(!strcmp("NULL",email))
        {
            printf("Incorrect format of email\nEnter again: ");
            flag=1;
        }
    }
    while(flag);

    /* printf("enter balance:");
     do
     { bal=readAmount();
     if(bal==-1)
         printf("please only enter digits\nEnter again:");
     }while(bal==-1);*/

    printf("\nEnter phone number: ");
    do
    {
        flag=0;
        phone=readPhone();
        if(strcmp(phone,"NULL") == 0)
        {
            printf("Only enter digits\nEnter again: ");
            flag=1;
        }

    }
    while(flag);

    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);
    localTime = localtime(&currentTime);
    d.month=localTime->tm_mon + 1 ;
    d.year=localTime->tm_year + 1900 ;

    accounts[num_acc-1]= constAcc(x,name,email,bal,phone,d);
    save();
    printf("\nAccount added successfully!\nExiting to main menu....\n");

    free(email);
    free(phone);
    free(name);

}


void delete_account()
{
    printf("Enter account number [enter -1 to return to menu]: ");

    unsigned long long input;
    input = read_account_no();

    while(input == 0)
    {
        printf("Invalid Input!\nEnter valid account number(10 digits)[enter -1 to return to menu]: ");
        input = read_account_no();
    }

    if(input == 1)
    {
        printf("Returning to Main Menu.\n");
        return;
    }

    bool found = 0;
    int i;
    for(i = 0; i<num_acc; i++)
    {
        if(accounts[i] != NULL)
        {
            if(accounts[i]->account_no == input)
            {
                found = 1;
                if(accounts[i]->balance > 0)
                {
                    printf("\nCan't delete an account with positive available balance.\n");
                    break;
                }
                else
                {
                    printf("\nAre you sure you want to delete this account: ");
                    printAccount(accounts[i]);
                    printf("\n[1] Confirm\n[2] Cancel and return to the Main Menu\n");
                    int confirm;
                    do
                    {
                        confirm = readInteger();
                        if (confirm == 1)
                        {
                            char acc_num[15];
                            snprintf(acc_num, sizeof(acc_num), "%llu.txt", accounts[i]->account_no);
                            if (unlink(acc_num) != 0) {
                                perror("Error removing file");
                            }
                            distAcc(accounts[i]);
                            num_acc--;
                            accounts = realloc(accounts, num_acc * sizeof(account*));
                            int j;
                            for(j = i; j < num_acc; j++)
                            {
                                accounts[j] = accounts[j+1];
                            }
                            printf("\nAccount deleted Successfully.\n");
                            save();
                            break;
                        }
                        else if (confirm == 2)
                        {
                            printf("\nCancelled deletion!\n");
                            return;
                        }
                        else
                            printf("Invalid Input! Enter 1 or 2.\n");
                    }
                    while (confirm != 1 && confirm != 2);
                }
            }
        }
    }
    if(!found)
        printf("\nCouldn't find account.\n");
}

void modify_acc()
{
    qsort(accounts, num_acc, sizeof(*accounts), SortByNum);
    unsigned long long account_no;
    int found, acc_index;
    do
    {
        do
        {
            printf("Enter account number: ");
            account_no = read_account_no();
            if (account_no == 0)
                printf("Invalid Account Number!\n");
        }
        while (account_no == 0);
        found = binary_search(account_no,&acc_index);
        if (!found)
            printf("The Specified Account is not found!\n\n");
    }
    while (!found);
    int done = 1;
    do
    {
        int field;
        do
        {
            printf("[1] Name\n");
            printf("[2] E-mail Address\n");
            printf("[3] Mobile\n");
            field = readInteger();
            char str[50];
            if (field == 1)
            {
                int flag;
                do
                {
                    flag=0;
                    printf("Enter the new name: ");
                    strcpy(str, readName());
                    if(!strcmp(str, "1"))
                    {
                        printf("Invalid Input! Can't leave field empty!\n");
                        flag=1;
                    }
                    if(!strcmp(str, "2"))
                    {
                        printf("Invalid Input! Can't start with a space!\n");
                        flag=1;
                    }
                    if(!strcmp(str, "3"))
                    {
                        printf("Invalid Input! Only enter characters!\n");
                        flag=1;
                    }
                }
                while(flag);
                printf("The new name is: %s\n", str);
                printf("[1] Confirm the modification\n[2] Cancel\n");
                int confirm;
                do
                {
                    confirm = readInteger();
                    if (confirm == 1)
                    {
                        free(accounts[acc_index]->name);
                        accounts[acc_index]->name = malloc(strlen(str) + 1);
                        strcpy(accounts[acc_index]->name, str);
                        save();
                        printf("Name changed Successfully!\n");
                    }
                    else if (confirm == 2)
                        printf("\nName Modification Cancelled!\n");
                    else
                        printf("Invalid Input! Enter 1 or 2.\n");
                }
                while (confirm != 1 && confirm != 2);
            }
            else if (field == 2)
            {
                do
                {
                    printf("Enter the new e-mail address: ");
                    strcpy(str, readEmail());
                    if (!strcmp(str, "NULL"))
                        printf("Invalid E-mail Address!\n");
                }
                while (!strcmp(str, "NULL"));
                printf("The new e-mail address is: %s \n", str);
                printf("[1] Confirm the modification\n[2] Cancel\n");
                int confirm;
                do
                {
                    confirm = readInteger();
                    if (confirm == 1)
                    {
                        free(accounts[acc_index]->email);
                        accounts[acc_index]->email = malloc(strlen(str) + 1);
                        strcpy(accounts[acc_index]->email, str);
                        save();
                        printf("E-mail Address changed Successfully!\n");
                    }
                    else if (confirm == 2)
                        printf("\nE-mail Modification Cancelled!\n");
                    else
                        printf("Invalid Input! Enter 1 or 2.\n");
                }
                while (confirm != 1 && confirm != 2);
            }
            else if (field == 3)
            {
                do
                {
                    printf("Enter the new mobile number: ");
                    strcpy(str, readPhone());
                    if (!strcmp(str, "NULL"))
                        printf("Invalid Mobile Number!\n");
                }
                while (!strcmp(str, "NULL"));
                printf("The new mobile: %s \n", str);
                printf("[1] Confirm the modification\n[2] Cancel\n");
                int confirm;
                do
                {
                    confirm = readInteger();
                    if (confirm == 1)
                    {
                        strcpy(accounts[acc_index]->phone, str);
                        save();
                        printf("Mobile changed Successfully!\n");
                    }
                    else if (confirm == 2)
                        printf("\nMobile Modification Cancelled!\n");
                    else
                        printf("Invalid Input! Enter 1 or 2.\n");
                }
                while (confirm != 1 && confirm != 2);
            }
            else printf("Invalid Input! Enter 1 or 2 or 3.\n\n");
        }
        while (field != 1 && field != 2 && field != 3);
        do
        {
            printf("[1] Modify Another Field\n");
            printf("[2] Return to Main Menu \n");
            done = readInteger();
            if (done == 1)
                continue;
            else if (done == 2)
                menu();
            else
                printf("Invalid Input! Enter 1 or 2.\n");
        }
        while (done != 1 && done != 2);
    }
    while (done == 1);

}
void withdraw()
{
    unsigned long long accNum;
    int flag=0;
    do
    {
        printf("\nEnter account number\nAccount Number: ");
        accNum = read_account_no();

        if (accNum == 0)
            printf("Invalid Account Number!\n");
    }
    while (accNum == 0);

    qsort(accounts, num_acc, sizeof(*accounts), SortByNum);
    int acc_index;
    int found = binary_search(accNum, &acc_index);
    if (!found)
    {
        printf("\nThe Specified Account is not found!\n\n");
        withdraw();
        return;
    }
       double amount,oldBalance = accounts[acc_index]->balance;

      do
    {
        flag = 0;
        printf("\nEnter amount to withdraw from account [limit: 10,000$]\nAmount{$}: ");
        amount = readAmount();
        if (amount == -2){
            printf("\nError: Amount entered cannot be negative\n");
            flag=1;
        }

        else if (amount == -1){
            printf("\nError: Invalid Amount (Do not enter characters)\n");
            flag=1;
        }

        else if (amount == 0){
            printf("\nError: Amount entered cannot be 0 $\n");
            flag=1;
        }

        else if (amount > 10000.00){
            printf("\nError: Amount entered is beyond limit\n");
            flag=1;
        }
        else if(oldBalance<amount){
            printf("\nError: Amount entered is larger than balance\n");
            flag=1;
        }

    } while (flag);
    fflush(stdin);
    printf("\nAccount Number: %llu\nWithdrawn Amount: $%.2f\n\n", accNum, amount);
    printf("[1] Confirm the transaction\n[2] Cancel\n");

    int confirm;
    do
    {
        confirm = readInteger();
        if (confirm == 1)
        {
            printf("\nWithdrawing $%.2f from account...\n", amount);
            accounts[acc_index]->balance -= (double)amount;
            printf("Success!\n\nPrevious balance: $%.2f\nNew balance: $%.2f\n\nReturning to main menu...", oldBalance, accounts[acc_index]->balance);
            saveTransaction(accNum, accNum, 5, amount); //from: accNum,  to: 5 = bank,  amount: $
            save();
        }
        else if (confirm == 2){
            printf("\nTransaction Cancelled!\n");
            return;
        }
        else
            printf("Invalid Input! Enter 1 or 2.\n");
    }
    while (confirm != 1 && confirm != 2);

}

void deposit()
{
    unsigned long long accNum;
    do
    {
        printf("\nEnter account number\nAccount Number: ");
        accNum = read_account_no();

        if (accNum == 0)
            printf("Invalid Account Number!\n");
    }
    while (accNum == 0);

    qsort(accounts, num_acc, sizeof(*accounts), SortByNum);
    int acc_index;
    int found = binary_search(accNum, &acc_index);
    if (!found)
    {
        printf("\nThe Specified Account is not found!\n\n");
        deposit();
        return;
    }

    double amount;
    do
    {
        printf("\nEnter amount to deposit to account [limit: 10,000$]\nAmount{$}: ");
        amount = readAmount();
        if (amount == -2)
            printf("\nError: Amount entered cannot be negative\n");

        else if (amount == -1)
            printf("\nError: Invalid Amount (Do not enter characters)\n");

        else if (amount == 0)
            printf("\nError: Amount entered cannot be 0 $\n");

        else if (amount > 10000.00)
            printf("\nError: Amount entered is beyond limit\n");

    } while (amount <= 0 || amount > 10000.00);

    printf("\nAccount Number: %llu\nDeposit Amount: $%.2lf\n\n", accNum, amount);
    printf("[1] Confirm the transaction\n[2] Cancel\n");
    int confirm;
    do
    {
        confirm = readInteger();
        if (confirm == 1)
        {
            printf("\nDepositing $%.2f to account...\n", amount);

            double oldBalance = accounts[acc_index]->balance;
            accounts[acc_index]->balance += amount;
            printf("Success!\n\nPrevious balance: $%.2lf\nNew balance: $%.2lf\n", oldBalance, accounts[acc_index]->balance);
            saveTransaction(accNum, 5, accNum, amount); //from: 5 = bank,  to: accNum,  amount: $
            save();
        }
        else if (confirm == 2)
        {
            printf("\nTransaction Cancelled!\n");
            return;
        }
        else
            printf("Invalid Input! Enter 1 or 2.\n");
    }
    while (confirm != 1 && confirm != 2);
}

void transfer()
{
    qsort(accounts, num_acc, sizeof(*accounts), SortByNum);
    unsigned long long account_no_sender;
    int found_sender, acc_index_sender;
    do
    {
        do
        {
            printf("Enter account number of the sender: ");
            account_no_sender = read_account_no();
            if (account_no_sender == 0)
                printf("Invalid Account Number!\n");
        }
        while (account_no_sender == 0);
        found_sender = binary_search(account_no_sender,&acc_index_sender);
        if (!found_sender)
            printf("The Specified Account is not found!\n\n");
    }
    while (!found_sender);
    unsigned long long account_no_receiver;
    int found_receiver, acc_index_receiver;
    do
    {
        do
        {
            printf("Enter account number of the receiver: ");
            account_no_receiver = read_account_no();
            if (account_no_receiver == 0)
                printf("Error: Invalid Account Number!\n");
            else if (account_no_receiver == account_no_sender)
                printf("Error: Sender and receiver account numbers cannot be the same!\n");
        }
        while (account_no_receiver == 0 || account_no_receiver == account_no_sender);
        found_receiver = binary_search(account_no_receiver,&acc_index_receiver);
        if (!found_receiver)
            printf("The Specified Account is not found!\n\n");
    }
    while (!found_receiver);
    double amount;
    int valid;
    do
    {
        valid = 1;
        do
        {
            printf("Enter the amount to be transferred\nAmount{$}: ");
            amount = readAmount();
            if (amount == -2)
                printf("Error: Amount entered cannot be negative\n");
            else if (amount == -1)
                printf("Error: Invalid Amount (Do not enter characters)\n");
            else if (amount == 0)
                printf("Error: Amount entered cannot be 0 $\n");
        }
        while (amount <= 0);
        if (amount > accounts[acc_index_sender]->balance)
        {
            valid = 0;
            printf("Error: Insufficient Balance\n");
            printf("[1] Enter another amount\n[2] Cancel\n");
            int confirm;
            do
            {
                confirm = readInteger();
                if (confirm == 1)
                    continue;
                else if (confirm == 2)
                    return;
                else
                    printf("Invalid Input! Enter 1 or 2.\n");
            }
            while (confirm != 1 && confirm != 2);
        }
    }
    while (!valid);
    printf("Sender's Account Number: %llu\n", account_no_sender);
    printf("Receiver's Account Number: %llu\n", account_no_receiver);
    printf("Amount{$}: %.2lf\n", amount);
    printf("[1] Confirm Transaction\n[2] Cancel\n");
    int confirm;
    do
    {
        confirm = readInteger();
        if (confirm == 1)
        {
            double oldBalance_sender = accounts[acc_index_sender]->balance;
            double oldBalance_receiver = accounts[acc_index_receiver]->balance;
            accounts[acc_index_sender]->balance -= amount;
            accounts[acc_index_receiver]->balance += amount;
            saveTransaction(account_no_sender, account_no_sender, account_no_receiver, amount);
            saveTransaction(account_no_receiver, account_no_sender, account_no_receiver, amount);
            save();
            printf("Transaction Completed Successfully!\n\n");
            printf("Sender(%llu)\t\t\tReceiver(%llu)\n", account_no_sender, account_no_receiver);
            printf("Old Balance: %.2lf\t\t\tOld Balance: %.2lf\n", oldBalance_sender, oldBalance_receiver);
            printf("New Balance: %.2lf\t\t\tNew Balance: %.2lf\n", accounts[acc_index_sender]->balance, accounts[acc_index_receiver]->balance);
        }
        else if (confirm == 2)
            printf("\nTransaction Cancelled!\n");
        else
            printf("Invalid Input! Enter 1 or 2.\n");
    }
    while (confirm != 1 && confirm != 2);
}

void report()
{
    unsigned long long acc_no;
    int i, tries_left = 5;
    printf("You must enter a correct account number within a maximum of 5 tries.\n");
    printf("Enter account number: ");
    acc_no = read_account_no();
    while((acc_no == 0) && (tries_left > 0))
    {
        tries_left --;
        printf("Invalid input!\nYou have %d tries left.\n", tries_left);
        printf("Enter valid account number(10 digits): ");
        acc_no = read_account_no();
    }
    if (tries_left == 0)
    {
        printf("Too many invalid inputs. Returning to main menu.\n");
        return ;
    }
    bool found = 0;
    for(i = 0; i < num_acc; i ++)
    {
        if(accounts[i] != NULL)
        {
            if(accounts[i]->account_no == acc_no)
            {
                found = 1;
            }
        }
    }
    if (!found)
    {
        printf("The account you requested can't be found.\n");
        return ;
    }
    char inputs[11] = "";
    for (i = 9; i >= 0; i --)
    {
        inputs[i] = '0' + acc_no % 10;
        acc_no /= 10;
    }
    strcat(inputs, ".txt");
    //snprintf(inputs, sizeof(inputs), "%llu.txt", acc_no);
    FILE *fp;
    fp = fopen(inputs, "r");
    if (fp == NULL)   // this shouldn't happen as we have found the account number in the accounts array
    {
        printf("Error opening file %s\n", inputs);
        printf("Quitting the program.\n");
        fclose(fp);
        exit(2);
    }
    transaction_details transactions[5];
    for (i = 0; i < 5; i ++)
    {
        transactions[i].from = 1;
        transactions[i].to = 1;
        transactions[i].amount = 1;
    }
    while(!feof(fp))
    {
        for (i = 4; i >= 1; i --)
        {
            transactions[i] = transactions[i - 1];
        }
        fscanf(fp, "%llu %llu %f\n", &transactions[0].from, &transactions[0].to, &transactions[0].amount);
    }
    printf("\nTransactions sorted from most recent to oldest:\n");
    for (i = 0; i < 5; i ++)
    {
        if(transactions[i].from != 1)  // checks whether there is a transaction or not
        {
            printf("-->");
            if (transactions[i].to == 5)   // the number 5 symbolises the client, it is as if that the client has received the amount
            {
                printf("$%.2f have been withdrawn from the account no.: %llu\n", transactions[i].amount, transactions[i].from);
            }
            else if (transactions[i].from == 5)
            {
                printf("$%.2f have been deposited into the account no.: %llu\n", transactions[i].amount, transactions[i].to);
            }
            else
            {
                printf("The account no.: %llu has transferred $%.2f to the account no.: %llu\n", transactions[i].from, transactions[i].amount, transactions[i].to);
            }
        }
    }
    fclose(fp);
}

void print()
{
    printf("\nSelect order to print the accounts (Enter 1, 2 or 3 accordingly)\n");
    printf("[1] Sort by name\n[2] Sort by balance\n[3] Sort by date opened\n");
    int sortType = readInteger();

    switch (sortType)
    {
    case 1:
        qsort(accounts, num_acc, sizeof(account*), SortByName);
        break;
    case 2:
        qsort(accounts, num_acc, sizeof(account*), SortByBalance);
        break;
    case 3:
        qsort(accounts, num_acc, sizeof(account*), SortByDate);
        break;
    default:
        printf("\nINVALID INPUT! (Enter 1, 2 or 3 only)\n");
        print();
        return;
    }
    //prints all accounts after sorting
    int i;
    for (i = 0; i < num_acc; i++)
    {
        printAccount(accounts[i]);
    }
    printf("\n");
}

void save()
{
    qsort(accounts, num_acc, sizeof(*accounts), SortByNum);
    FILE *fp = fopen("accounts.txt", "w");
    if(fp == NULL)
        printf("Error: accounts.txt file not found");
    int i;
    for(i = 0; i<num_acc; i++)
    {
        if(accounts[i] != NULL)
        {
            fprintf(fp, "%llu,%s,%s,%.2f,%s,%d-%d\n", accounts[i]->account_no,
                    accounts[i]->name, accounts[i]->email, accounts[i]->balance,
                    accounts[i]->phone, accounts[i]->date_opened.month, accounts[i]->date_opened.year);
        }
    }
    fclose(fp);
}

void log_out()
{
    printf("Are you sure you want to log out?\n");
    printf("[1] Confirm Logout\n[2] Cancel\n");
    int conf = readInteger();
    while(conf != 1 && conf != 2)
    {
        printf("Invalid input! You must enter either 1 or 2.\n");
        printf("[1] Confirm Logout\n[2] Cancel\n");
        conf = readInteger();
    }
    if (conf == 1)
    {
        logged_in = 0;
        printf("Logging out\nGoodbye %s\n",current_employee.username);
        return;
    }
    printf("Logging out has been cancelled\n");
}

void quit()
{
    printf("Are you sure you want to quit?\n");
    printf("[1] Confirm quit\n[2] Cancel\n");
    int conf = readInteger();
    while(conf != 1 && conf != 2)
    {
        printf("Invalid input! You must enter either 1 or 2.\n");
        printf("[1] Confirm quit\n[2] Cancel\n");
        conf = readInteger();
    }
    if (conf == 1)
    {
        unload();  //unloads accounts
        printf("Quitting the program.\nGoodbye");
        exit(0);
    }
    printf("Quitting the program has been cancelled\n");
}

void menu()
{
    if (!logged_in)
    {
        printf("[1] Login\n");
        printf("[2] Quit\n");
        int input = readInteger();
        while(input != 1 && input != 2)
        {
            printf("Invalid input! You must enter either 1 or 2.\n");
            printf("[1] Login\n");
            printf("[2] Quit\n");
            input = readInteger();
        }
        if (input == 1)
        {
            login();
            logged_in = 1;
            load();  //loads accounts
        }
        if (input == 2)
            quit();
        return ;
    }
    printf("\nWelcome to the main menu.\n");
    int input;
    do{
        printf("[1]  Add an account\n");
        printf("[2]  Delete an account\n");
        printf("[3]  Modify the data of an account\n");
        printf("[4]  Search for an account using account number\n");
        printf("[5]  Advanced searching\n");
        printf("[6]  Withdraw money from an account\n");
        printf("[7]  Deposit money into an account\n");
        printf("[8]  Transfer money from one account to another\n");
        printf("[9]  Report about the transactions of an account\n");
        printf("[10] Print data of all accounts\n");
        printf("[11] Log out\n");
        printf("[12] Quit the program\n");
        input = readInteger();
        if(input < 1 || input > 12)
            printf("Invalid input! Input should be between 1 and 12\n\n");
    }while(input < 1 || input > 12);

    switch (input)
    {
    case 1:
        printf("\nAdding an account\n");
        add();
        break;
    case 2:
        printf("\nDeleting an account\n");
        delete_account();
        break;
    case 3:
        printf("\nModifying account\n");
        modify_acc();
        break;
    case 4:
        printf("\nSearching for an account\n");
        query_search();
        break;
    case 5:
        printf("\nAdvanced searching\n");
        advanced_search();
        break;
    case 6:
        printf("\nWithdrawing money\n");
        withdraw();
        break;
    case 7:
        printf("\nDepositing money\n");
        deposit();
        break;
    case 8:
        printf("\nTransferring money\n");
        transfer();
        break;
    case 9:
        printf("\nReporting\n");
        report();
        break;
    case 10:
        print();
        break;
    case 11:
        log_out();
        break;
    case 12:
        quit();
    }
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

    create_transaction_file(account_no);

    return p;
}

void printAccount(account *a)
{
    printf("\nAccount Number : %lld\n", a->account_no);
    printf("Name: %s\n", a->name);
    printf("E-mail : %s\n", a->email);
    printf("Balance: %.2lf $\n", a->balance);
    printf("Mobile: %s\n", a->phone);
    printf("Date Opened: ");
    switch (a->date_opened.month)
    {
    case 1:
        printf("January");
        break;
    case 2:
        printf("February");
        break;
    case 3:
        printf("March");
        break;
    case 4:
        printf("April");
        break;
    case 5:
        printf("May");
        break;
    case 6:
        printf("June");
        break;
    case 7:
        printf("July");
        break;
    case 8:
        printf("August");
        break;
    case 9:
        printf("September");
        break;
    case 10:
        printf("October");
        break;
    case 11:
        printf("November");
        break;
    case 12:
        printf("December");
        break;
    }
    printf(" %d\n", a->date_opened.year);
}

void distAcc(account *p)
{
    free(p->name);
    free(p->email);
    free(p->phone);
    free(p);
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

account* decodeText(char* line)
{
    unsigned long long accNum = strtoull(strtok(line, ","), NULL, 10);
    char *name = strtok(NULL, ",");
    char *email = strtok(NULL, ",");
    double balance = strtod(strtok(NULL, ","), NULL);
    char *phone = strtok(NULL, ",");
    date date_opened;
    date_opened.month = atoi(strtok(NULL, "-"));
    date_opened.year =  atoi(strtok(NULL, ","));

    return constAcc(accNum, name, email, balance, phone, date_opened);
}

void unload()
{
    int i;
    for (i = 0; i < num_acc; i++)
    {
        distAcc((accounts)[i]);
    }
    free(accounts);
}

void create_transaction_file(unsigned long long account_no)
{

    char filename[15];  //10 for account_no 4 for .txt and 1 for \0    10 + 4 + 1 = 15

    snprintf(filename, sizeof(filename), "%010llu.txt", account_no);

    FILE *fp = fopen(filename, "r");
    if (!fp) fp = fopen(filename, "w");
    fclose(fp);
}

void saveTransaction(unsigned long long account_no, unsigned long long from, unsigned long long to, double amount)
{
    char filename[15];  //10 for account_no 4 for .txt and 1 for \0    10 + 4 + 1 = 15
    snprintf(filename, sizeof(filename), "%010llu.txt", account_no);

    FILE* fp;
    fp = fopen(filename, "r");
    if(!fp)
    {
        printf("Error: %s file not found", filename);
        return;
    }
    fp = fopen(filename, "a");
    if(fprintf(fp, "%llu %llu %.2lf\n", from, to, amount) < 0)
        printf("Error: data could not be saved");

    fclose(fp);
}

int readInteger()
{
    char inputs[100];
    fgets(inputs, 99, stdin);
    int len = strlen(inputs);
    if(inputs[0] == '\n')
        return -1;
    if (len > 0 && inputs[len - 1] == '\n')
    {
        inputs[len - 1] = '\0';
    }
    int i;
    for (i = 0; inputs[i] != '\0'; i ++)
    {
        if (!isdigit(inputs[i])) return -1;
    }
    int ans = 0;
    for (i = 0; inputs[i] != '\0'; i ++)
    {
        ans *= 10;
        ans += inputs[i] - '0';
    }
    return ans;
}

unsigned long long read_account_no()
{
    char buffer[30];
    fgets(buffer, 29, stdin);
    int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
    }
    if(!strcmp(buffer, "-1")){
        return 1; // to break and return to main menu
    }
    int i;
    for (i = 0; buffer[i] != '\0'; i++)
    {
        if (!isdigit(buffer[i])) return 0;
    }
    len = strlen(buffer);
    if(len != 10)
        return 0;

    unsigned long long result = 0;

    for (i = 0; buffer[i] != '\0'; i ++)
    {
        result *= 10;
        result += buffer[i] - '0';
    }
    return result;
}

char *readPhone()
{
    int count=0,j;
    char inputs[100];
    char*result_p=malloc(30);
    fgets(inputs,99, stdin);
    int len = strlen(inputs);
    if(inputs[0] == '\n')
        return "NULL";
    if (len > 0 && inputs[len - 1] == '\n')
        inputs[len - 1] = '\0';
    int i;
    for (i = 0; inputs[i] != '\0'; i ++)
    {
        if(i==0&&inputs[i]=='+')
        {
            count++;
            if(count>1)
                return "NULL";
            continue;
        }
        if (!isdigit(inputs[i])) return "NULL";
    }

    strcpy(result_p,inputs);


    return result_p;
}

double readAmount()
{
    int count=0,j, neg = 0;
    char inputs[100];
    fgets(inputs,99, stdin);
    int len = strlen(inputs);
    if(inputs[0] == '\n')
        return -1;
    if (len > 0 && inputs[len - 1] == '\n')
    {
        inputs[len - 1] = '\0';
    }

    int i;
    for (i = 0; inputs[i] != '\0'; i ++)
    {
        if(inputs[i]=='.')
        {
            count++;
            if(count>1)
                return -1;
            continue;
        }
        if (i == 0 && inputs[i] == '-')
        {
            neg = 1;
            continue;
        }
        if (!isdigit(inputs[i])) return -1;
    }
    if (neg == 1)
        return -2; // -2 means amount entered is negative
    double num =0,ans=0,dec=0;
    int post=0;
    for (i = 0; (inputs[i] != '.')&&(inputs[i]!='\0'); i ++)
    {
        num *= 10;
        num += inputs[i] - '0';

    }
    if(inputs[i]=='.')
    {
        for(j=i+1; inputs[j]!='\0'; j++)
        {
            dec*= 10;
            dec += inputs[j] - '0';
            post++;

        }

    }
    dec/=pow(10,post);
    ans=num+dec;

    return ans;
}
bool cont_dig(char *x)
{
    int i=0;
    for(i; x[i]!='\0'; i++)
    {
        if(isdigit(x[i]))
            return 1;
    }
    return 0;
}
bool cont_spec(char *x, int mode)
{
    if(mode == 1){
        int i=0;
        for(i; x[i]!='\0'; i++)
        {
            if(ispunct(x[i]))
                return 1;
        }
        return 0;
    }
    else if(mode == 2){
        int i=0;
        for(i; x[i]!='\0'; i++)
        {
            if(!isdigit(x[i])&&!isalpha(x[i]))
            {
                if(x[i]!='+' && x[i]!='_' && x[i]!='-' && x[i]!='@' && x[i]!='.')
                    return 1;
            }
        }
        return 0;
    }
}
char *readEmail()
{
    char buffer[50];
    char *result_e = malloc(50);
    int i=0,count_at=0,count_dot=0,j;
    fgets(buffer, 49, stdin);
    if(buffer[0] == '\n')
        return "NULL";
    int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';
    if(cont_spec(buffer, 2))
        return "NULL";

    for(i; buffer[i]!='\0'; i++)
    {
        if (buffer[i] == ' ')
            return "NULL";
        if(buffer[i]=='@')
        {
            count_at++;
            if(count_at>1)
                return "NULL";
            for(j=i; buffer[j]!='\0'; j++)
            {
                if(buffer[j]=='.')
                {
                    count_dot++;
                    if(buffer[i+1]=='\0'||buffer[i+3]=='\0'||buffer[0]=='.')
                        return "NULL";
                }
            }
            if(buffer[i+1]=='.'||buffer[i+2]=='.'||buffer[0]=='@')
                return "NULL";
        }

        if(count_dot>1)
            return "NULL";

    }
    if(count_at!=1||count_dot!=1)
        return "NULL";
    strcpy(result_e,buffer);

    return result_e;
}

char *readName()
{
    char name[50];
    char *result = malloc(50);
    fgets(name, 49, stdin);
    if(name[0] == '\n')
        return "1";
    if(name[0] == ' ')
        return "2";
    int len = strlen(name);
    if (len > 0 && name[len - 1] == '\n')
        name[len - 1] = '\0';
    if(cont_dig(name) || cont_spec(name, 1))
        return "3";
    strcpy(result, name);
    return result;
}

int binary_search(unsigned long long account_no, int *mid)
{
    int low = 0, high = num_acc-1, found = 0;
    while (!found && low <= high)
    {
        *mid = (high + low) / 2;
        if (account_no == accounts[*mid]->account_no)
            found = 1;
        else if (account_no < accounts[*mid]->account_no)
            high = *mid - 1;
        else
            low = *mid + 1;
    }
    return found;
}

int SortByNum(const void *a,const void *b)
{
    if ((*(account**)a)->account_no > (*(account**)b)->account_no) return 1;
    else return -1;

    //return ((*(account**)a)->account_no - (*(account**)b)->account_no);  //another way
}

int SortByName(const void *a,const void *b)
{
// Cast pointers to account** before dereferencing and accessing the name field (dereferencing is getting at the pointed value)
    return strcmp((*(account**)a)->name, (*(account**)b)->name);
}

int SortByBalance(const void *a,const void *b)
{
    if ((*(account**)a)->balance > (*(account**)b)->balance) return 1;
    else if ((*(account**)a)->balance < (*(account**)b)->balance) return -1;
    else return 0;
}

int SortByDate(const void *a,const void *b)
{
    if ((*(account**)a)->date_opened.year > (*(account**)b)->date_opened.year) return 1;
    else if ((*(account**)a)->date_opened.year < (*(account**)b)->date_opened.year) return -1;
    else
    {
        if((*(account**)a)->date_opened.month > (*(account**)b)->date_opened.month) return 1;
        else if ((*(account**)a)->date_opened.month < (*(account**)b)->date_opened.month) return -1;
        else return 0;
    }
}
