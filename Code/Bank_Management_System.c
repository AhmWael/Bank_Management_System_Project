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
int num_acc = 0;

typedef struct {
    unsigned month:4, year;
}date;

typedef struct {
    unsigned long long from, to;
    float amount;
}transaction_details;

typedef struct {
    char *username, *password;
}employee;

typedef struct {
    unsigned long long account_no;
    char *name, *email, *phone;
    double balance;
    date date_opened;
    transaction_details recent_transfers[5];
}account;

account** accounts = NULL;

void login();
account* constAcc(unsigned long long account_no, char *name, char *email, double balance, char *phone, date date_opened);
void distAcc(account *p);
account* decodeText(char* line);
void load(account*** accounts, int *numRec);
void unload(account*** accounts, int numRec);
date *constDate(int month, int year);
void setDate(account *a, int month, int year);
int binary_search(unsigned long long account_no, int* acc_index);
void query_search();
void advanced_search();
int readInteger();
void print();
int SortByNum(const void *a,const void *b);
int SortByName(const void *a,const void *b);
int SortByDate(const void *a,const void *b);
int SortByBalance(const void *a,const void *b);
void printAccount(account *a);
void menu();
void report();
void log_out();
void quit();

int main()
{
    load(&accounts, &num_acc);  //loads accounts

    /*
    //{FOR TESTING LOAD}
    int i;
    for (i = 0; i < num_acc; i++)
    {
        printAccount(accounts[i]);  //prints all accounts
    }
    */
    while (1) {
        menu();
    }

    unload(&accounts, num_acc);  //unloads accounts

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

void load(account*** accounts, int *numRec)
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

    //printf("num of acc = %d\n", *numRec);  //for testing delete later

    *accounts = (account**)malloc((*numRec) * sizeof(account*));

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
        (*accounts)[i] = decodeText(record);
    }

    fclose(fp);
}

void unload(account*** accounts, int numRec)
{
    int i;
    for (i = 0; i < numRec; i++)
    {
        distAcc((*accounts)[i]);
    }
    free(*accounts);
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

int binary_search(unsigned long long account_no, int* mid)
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

void query_search()
{
    unsigned long long account_no;

    qsort(accounts, num_acc, sizeof(*accounts), SortByNum); //should we use this better? does the same thing below
    /*
    account* temp;
    int i, pass, sorted=0;
    for (pass = 1; pass < num_acc && !sorted; pass++)
    {
        sorted = 1;
        for (i = 0; i < num_acc - pass; i++)
        {
            if (accounts[i]->account_no > accounts[i+1]->account_no)
            {
                temp = accounts[i];
                accounts[i] = accounts[i+1];
                accounts[i+1] = temp;
                sorted = 0;
            }

        }
    }
    //distAcc(temp);     //fix later
    */
    printf("Enter account number: ");
    scanf("%llu", &account_no);
    int acc_index;
    int found = binary_search(account_no,&acc_index);
    if (!found)
        printf("The Specified Account is not found!");
    else
        printAccount(accounts[acc_index]);
}

void advanced_search(){
    printf("Enter keyword: ");
    char key[30];
    gets(key);
    while(!((key[0] >= 'a' && key[0] <= 'z') || (key[0] >= 'A' && key[0] <= 'Z') || (key[0] >= '0' && key[0] <= '9'))){
        printf("Invalid input, please enter keyword starting with a letter or a number only: ");
        gets(key);
    }
    printf("\nSearching for \"%s\"\n", key);
    int i, char_count = 0, search_crit = 0;
    if(key[0] >= '0' && key[0] <= '9'){
        for(i = 0;i<strlen(key);i++)
            if(!(key[i] >= '0' && key[i] <= '9')){
                char_count = 1;
                break;
            }
        if(!char_count)
            search_crit = 1;
    }
    else
        search_crit = 2;

    if(search_crit == 1){ // Search in account_number, phone
        bool found = 0;
        printf("Search Results: \n\n");

        int i;
        for(i = 0;i<num_acc;i++){
            char account_no_str[11];
            snprintf(account_no_str, sizeof(account_no_str), "%llu", accounts[i]->account_no);

            if(strstr(account_no_str, key) || strstr(accounts[i]->phone, key)){
                printAccount(accounts[i]);
                printf("\n");
                found = 1;
            }
        }
        if(!found)
            printf("\nNo match found.\nTry entering different keyword.\n");
    }
    else if(search_crit == 2){ // Search in name, email
        bool found = 0;

        int i;
        for(i = 0;i<num_acc;i++){
            char *name_str = accounts[i]->name;
            char *mail_str = accounts[i]->email;

            int j;
            for(j = 0; name_str[j]; j++){
              name_str[j] = tolower(name_str[j]);
            }

            for(j = 0; mail_str[j]; j++){
              mail_str[j] = tolower(mail_str[j]);
            }

            if(strstr(name_str, key) || strstr(mail_str, key)){
                printAccount(accounts[i]);
                printf("\n");
                found = 1;
            }
        }
        if(!found)
            printf("\nNo match found.\nTry entering different keyword.\n");
    }
}

void printAccount(account *a){
    printf("\nAccount Number : %lld\n", a->account_no);
    printf("Name: %s\n", a->name);
    printf("E-mail : %s\n", a->email);
    printf("Balance: %.2lf\n $", a->balance);
    printf("Mobile: %s\n", a->phone);
    printf("Date Opened: ");
    switch (a->date_opened.month)
        {
            case 1: printf("January"); break;
            case 2: printf("February"); break;
            case 3: printf("March"); break;
            case 4: printf("April"); break;
            case 5: printf("May"); break;
            case 6: printf("June"); break;
            case 7: printf("July"); break;
            case 8: printf("August"); break;
            case 9: printf("September"); break;
            case 10: printf("October"); break;
            case 11: printf("November"); break;
            case 12: printf("December"); break;
        }
    printf(" %d\n", a->date_opened.year);
}

//fekra used from chatgpt tarsh manest5dmo? very efficient
int readInteger() {
    char buffer[50];
    int userInput;

    // Read a line of input
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }

    // Uses sscanf to convert the string to an integer and check for additional characters
    if (sscanf(buffer, "%d", &userInput) != 1 || buffer[1] != '\n') {
        return -1;
    }

    return userInput;
}

void print() {
    printf("\nSelect order to print the accounts (Enter 1, 2 or 3 accordingly)\n");
    printf("1- sort by name\n2- sort by balance\n3- sort by date opened\n");
    int sortType = readInteger();

    switch (sortType)
    {
            case 1: qsort(accounts, num_acc, sizeof(account*), SortByName); break;
            case 2: qsort(accounts, num_acc, sizeof(account*), SortByBalance); break;
            case 3: qsort(accounts, num_acc, sizeof(account*), SortByDate); break;
            default: printf("\nINVALID INPUT\n"); print(); return;
    }
    //prints all accounts after sorting
    int i;
    for (i = 0; i < num_acc; i++)
    {
        printAccount(accounts[i]);
    }
}

int SortByNum(const void *a,const void *b) {
    if ((*(account**)a)->account_no > (*(account**)b)->account_no) return 1;
    else return -1;

    //return ((*(account**)a)->account_no - (*(account**)b)->account_no);  //another way
}

int SortByName(const void *a,const void *b) {
// Cast pointers to account** before dereferencing and accessing the name field (dereferencing is getting at the pointed value)
    return strcmp((*(account**)a)->name, (*(account**)b)->name);
}

int SortByBalance(const void *a,const void *b) {
    if ((*(account**)a)->balance > (*(account**)b)->balance) return 1;
    else if ((*(account**)a)->balance < (*(account**)b)->balance) return -1;
    else return 0;
}

int SortByDate(const void *a,const void *b) {
    if ((*(account**)a)->date_opened.year > (*(account**)b)->date_opened.year) return 1;
    else if ((*(account**)a)->date_opened.year < (*(account**)b)->date_opened.year) return -1;
    else
    {
        if((*(account**)a)->date_opened.month > (*(account**)b)->date_opened.month) return 1;
        else if ((*(account**)a)->date_opened.month < (*(account**)b)->date_opened.month) return -1;
        else return 0;
    }
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
            login(); // should be replaced with the function login();
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
        case 5: printf("Advanced searching\n"); advanced_search(); break;
        case 6: printf("Withdrawing money\n"); break;
        case 7: printf("Depositing money\n"); break;
        case 8: printf("Transferring money\n"); break;
        case 9: printf("Reporting\n"); report(); break;
        case 10: print(); break;
        case 11: log_out(); break;
        default : quit();
    }
}

void report() {
    unsigned long long acc_no;
    char inputs[100];
    long position;
    bool bad_input = 1, cancel = 0;
    int i, tries_left = 5;
    printf("Please note that you only have 5 tries to enter a correct account number, otherwise the program will be forcibly closed\n");
    while ((bad_input) && (tries_left)) {
        printf("Enter the account number: ");
        gets(inputs);
        if (strcmp(inputs, "0") == 0) {
            printf("The report has been cancelled\n");
            return ;
        }
        if (strlen(inputs) != 10) {
            printf("Invalid input! The account number must have 10 digits.\n");
            printf("Please check the account number again\n");
            printf("If you want to cancel the report, please enter 0 when asked about the account number\n");
            tries_left --;
            printf("You only have %d tries!!\n", tries_left);
            continue ;
        }
        for (i = 0; inputs[i] != '\0'; i ++) {
            if (!isdigit(inputs[i])) {
                printf("Invalid input! The account number can not contain any characters.\n");
                printf("Please check the account number again\n");
                printf("If you want to cancel the report, please enter 0 when asked about the account number\n");
                tries_left --;
                printf("You only have %d tries!!\n", tries_left);
                continue ;
            }
        }
        FILE *fp;
        fp = fopen("accounts.txt", "r");
        if(!fp)
        {
            printf("accounts.txt file not found");
            exit(2);
        }
        rewind(fp);
        char record[100];
        while(!feof(fp)) {
            //printf("Searching in file ");
            position = ftell(fp);
            //printf("position: %ld\n", position);
            fgets(record, 99, fp);
            if (strstr(record, inputs) != NULL) {
                bad_input = 0;
                fclose(fp);
                //printf("Found the record\n");
                break;
            }
        }
        if (feof(fp)) {
            printf("Invalid input! The account number you have entered does not exist\n");
            tries_left --;
            printf("You only have %d tries!!\n", tries_left);
            continue ;
        }
        fclose(fp);
    }
    if (!tries_left) {
        printf("Too many invalid inputs\nQuitting the program\n");
        exit(3);
    }
    FILE *fp;
    strcat(inputs, ".txt");
    fp = fopen(inputs, "r");
    if(!fp)
    {
        printf("%s can not be found", inputs);
        fclose(fp);
        exit(2);
    }
    char latest_transactions[100];
    fgets(latest_transactions, 99, fp);
    //printf("string: %s", latest_transactions);
    while(strcmp(latest_transactions, "Latest transactions:\n") != 0) {
        //printf("strcmp: %d\n", strcmp(latest_transactions, "Latest transactions:"));
        fgets(latest_transactions, 99, fp);
        //printf("string: %s", latest_transactions);
    }
    printf("The latest transactions are: \n");
    transaction_details transactions;
    //char to_get_to_the_next_line[100]; // this has no purpose other than getting the pointer to the next line as fscanf doesn't move the pointer
    while (!feof(fp)) {
        printf("-->");
        fscanf(fp, "%lld %lld %f\n", &transactions.from, &transactions.to, &transactions.amount);
        //fgets(to_get_to_the_next_line, 99, fp);
        if (transactions.to == 5) { // the number 5 symbolises the bank, it is as if that the bank has received the amount
            printf("$%.2f have been deposited into the account no.: %lld\n", transactions.amount, transactions.from);
        }
        else if (transactions.from == 5) {
            printf("$%.2f have been withdrawn from the account no.: %lld\n", transactions.amount, transactions.to);
        }
        else {
            printf("The account no.: %lld has tranferred $%.2f to the account no.: %lld\n", transactions.from, transactions.amount, transactions.to);
        }
    }
    fclose(fp);
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
void login() {
    FILE *fptr;
    int i=0, flagu=0,flagp=0;
    char *c = malloc(100);
    char *x = malloc(50);
    char*pass=malloc(50);
    char*token;
    fptr = fopen("users.txt", "r");
    if (fptr == NULL) {
        printf("Error opening file");
        exit(1);
    }
    do { rewind(fptr);                //used to reset pointer to beggining of file
        printf("Enter username:");
        scanf("%s", x);
        i=1;
        while(i&&!feof(fptr)){
        fgets(c,100,fptr);
        token=strtok(c," ");           //used to seperate password and username
        i=strcmp(token,x);}
        if(!i){
            flagu=1;
            token=strtok(NULL," \n");   //checks second part of string
            do{
            printf("Enter password:");
            scanf("%s",pass);
            if(strcmp(token,pass))
            {
             printf("invalid password\n");
            }
            else{
          flagp=1;
            break;}}
            while(!flagp);
        }
        else printf("invalid username\n");
    } while (!flagu);


    fclose(fptr);
    free(c);
    free(x);
}
