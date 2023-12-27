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
employee current_employee;

typedef struct {
    unsigned long long account_no;
    char *name, *email, *phone;
    double balance;
    date date_opened;
    transaction_details recent_transfers[5];
}account;

account** accounts = NULL;

void login();
void create_transaction_file(unsigned long long account_no);
account* constAcc(unsigned long long account_no, char *name, char *email, double balance, char *phone, date date_opened);
void distAcc(account *p);
account* decodeText(char* line);
void load();
void unload();
date *constDate(int month, int year);
void setDate(account *a, int month, int year);
int binary_search(unsigned long long account_no, int* acc_index);
void query_search();
void advanced_search();
int readInteger();
unsigned long long read_account_no();
double readDouble();
char*readEmail();
void print();
int SortByNum(const void *a,const void *b);
int SortByName(const void *a,const void *b);
int SortByDate(const void *a,const void *b);
int SortByBalance(const void *a,const void *b);
void modify_acc();
void printAccount(account *a);
void saveTransaction(unsigned long long account_no, unsigned long long from, unsigned long long to, double amount);
void deposit();
void delete_account();
void menu();
void report();
void save();
void log_out();
void quit();

int main()
{
    //load();  //loads accounts

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

    return 0;
}

void create_transaction_file(unsigned long long account_no) {

    char filename[15];  //10 for account_no 4 for .txt and 1 for \0    10 + 4 + 1 = 15

    snprintf(filename, sizeof(filename), "%010llu.txt", account_no);

    FILE *fp = fopen(filename, "r");
    if (!fp) fp = fopen(filename, "w");
    fclose(fp);
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

void distAcc(account *p)
{
    free(p->name);
    free(p->email);
    free(p->phone);
    free(p);
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

void unload()
{
    int i;
    for (i = 0; i < num_acc; i++)
    {
        distAcc((accounts)[i]);
    }
    free(accounts);
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
    do
    {
        printf("Enter account number: ");
        account_no = read_account_no();
        if (account_no == 0)
            printf("Invalid Account Number!\n");
    }
    while (account_no == 0);                       //doesn't work if account number is pasted with CTRL + v
    int acc_index;
    int found = binary_search(account_no,&acc_index);
    if (!found)
    {
        printf("The Specified Account is not found!\n\n");
        query_search();
    }

    else
        printAccount(accounts[acc_index]);
    printf("\n");
}

void advanced_search(){
    printf("Enter keyword: ");
    char key[50];
    fgets(key, 49, stdin);
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

void modify_acc()
{
    qsort(accounts, num_acc, sizeof(*accounts), SortByNum);
    unsigned long long account_no;
    do
    {
        printf("Enter account number: ");
        account_no = read_account_no();
        if (account_no == 0)
            printf("Invalid Account Number!\n");
    }
    while (account_no == 0);
    int acc_index;
    int found = binary_search(account_no,&acc_index);
    if (!found)
    {
        printf("The Specified Account is not found!\n\n");
        modify_acc();
        return;
    }

    else
    {
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
                    printf("Enter the new name: ");
                    fgets(str, 49, stdin);
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
                    printf("Enter the new e-mail address: ");
                    fgets(str, 49, stdin); // no validation for e-mail address
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
                    printf("Enter the new mobile: ");
                    fgets(str, 49, stdin);                                      // no validation for mobile number (11 numbers)
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
                printf("[1] Return to Main Menu \n");
                printf("[2] Modify Another Field\n");
                done = readInteger();
                if (done == 1)
                    menu();
                else if (done == 2)
                    ;
                else
                    printf("Invalid Input! Enter 1 or 2.\n");
            }
            while (done != 1 && done != 2);
        }
        while (done == 2);
    }
}

void printAccount(account *a){
    printf("\nAccount Number : %lld\n", a->account_no);
    printf("Name: %s\n", a->name);
    printf("E-mail : %s\n", a->email);
    printf("Balance: %.2lf $\n", a->balance);
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

int readInteger() {
    char inputs[100];
    fgets(inputs, 99, stdin);
    int len = strlen(inputs);
    if(inputs[0] == '\n')
        return -1;
    if (len > 0 && inputs[len - 1] == '\n') {
        inputs[len - 1] = '\0';
    }
    int i;
    for (i = 0; inputs[i] != '\0'; i ++) {
        if (!isdigit(inputs[i])) return -1;
    }
    int ans = 0;
    for (i = 0; inputs[i] != '\0'; i ++) {
        ans *= 10;
        ans += inputs[i] - '0';
    }
    return ans;
}

unsigned long long read_account_no(){
    char buffer[30];
    fgets(buffer, 29, stdin);
    int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    int i;
    for (i = 0; buffer[i] != '\0'; i++) {
        if (!isdigit(buffer[i])) return 0;
    }
    len = strlen(buffer);
    if(len != 10)
        return 0;

    unsigned long long result = 0;

    for (i = 0; buffer[i] != '\0'; i ++) {
        result *= 10;
        result += buffer[i] - '0';
    }
    return result;
}
double readDouble()
{
int count=0,j;
     char inputs[100];
    fgets(inputs,99, stdin);
    int len = strlen(inputs);
    if(inputs[0] == '\n')
        return -1;
    if (len > 0 && inputs[len - 1] == '\n') {
        inputs[len - 1] = '\0';
    }
    int i;
    for (i = 0; inputs[i] != '\0'; i ++) {
            if(inputs[i]=='.')
            {count++;
                if(count>1)
                    return -1;
                continue;
            }
        if (!isdigit(inputs[i])) return -1;
    }
    double num =0,ans=0,dec=0;
    int post=0;
    for (i = 0; inputs[i] != '.'; i ++) {
        num *= 10;
        num += inputs[i] - '0';
    }
    for(j=i+1;inputs[j]!='\0';j++)
    {
        dec*= 10;
        dec += inputs[j] - '0';
        post++;
    }

    dec/=pow(10,post);
    ans=num+dec;

    return ans;
}
char*readEmail()
{
    char buffer[30];
    char*result_e=malloc(30);
 int i=0,count=0;
    fgets(buffer, 29, stdin);
    for(i;buffer[i]!='\0';i++)
    {
        if(buffer[i]=='@')
        {
            count++;
       if(count>1)
            return "NULL";
            if(buffer[i+1]=='.'||buffer[i+2]=='.'||buffer[0]=='@')
                return "NULL";
        }
    }
            strcpy(result_e,buffer);

return result_e;
}

void print() {
    printf("\nSelect order to print the accounts (Enter 1, 2 or 3 accordingly)\n");
    printf("[1] Sort by name\n[2] Sort by balance\n[3] Sort by date opened\n");
    int sortType = readInteger();

    switch (sortType)
    {
            case 1: qsort(accounts, num_acc, sizeof(account*), SortByName); break;
            case 2: qsort(accounts, num_acc, sizeof(account*), SortByBalance); break;
            case 3: qsort(accounts, num_acc, sizeof(account*), SortByDate); break;
            default: printf("\nINVALID INPUT! (Enter 1, 2 or 3 only)\n"); print(); return;
    }
    //prints all accounts after sorting
    int i;
    for (i = 0; i < num_acc; i++)
    {
        printAccount(accounts[i]);
    }
    printf("\n");
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

void saveTransaction(unsigned long long account_no, unsigned long long from, unsigned long long to, double amount) {
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
    fprintf(fp, "%llu %llu %.2lf\n", from, to, amount);

    fclose(fp);
}

void deposit() {
    unsigned long long accNum;
    do {
        printf("\nEnter account number\nAccount Number: ");
        accNum = read_account_no();

            if (accNum == 0)
            printf("Invalid Account Number!\n");
    } while (accNum == 0);

    qsort(accounts, num_acc, sizeof(*accounts), SortByNum);
    int acc_index;
    int found = binary_search(accNum, &acc_index);
    if (!found) {
        printf("\nThe Specified Account is not found!\n\n");
        deposit();
        return;
    }

    printf("\nEnter amount to deposit to account [limit: 10,000$]\nAmount{$}: ");
    double amount;
    scanf("%lf", &amount);  //VALIDATE INPUT MAKE SURE ONLY FLOAT IS ENTERED {WILL DO LATER}

    while(amount > 10000.00 || amount <= 0) {
        printf("\nError: Amount entered is beyond limit\n");
        printf("\nEnter amount to deposit to account [limit: 10,000$]\nAmount{$}: ");
        scanf("%.2lf", &amount);  //VALIDATE INPUT MAKE SURE ONLY FLOAT IS ENTERED {WILL DO LATER}
    }
    fflush(stdin);

    printf("\nAccount Number: %llu\nDeposit Amount: $%.2lf\n\n", accNum, amount);
    printf("[1] Confirm the transaction\n[2] Cancel\n");
    int confirm;
    do {
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
        else if (confirm == 2){
            printf("\nTransaction Cancelled!\n");
            return;
        }
        else
            printf("Invalid Input! Enter 1 or 2.\n");
    } while (confirm != 1 && confirm != 2);
}

void delete_account(){
    printf("Enter account number: ");

    unsigned long long input;
    input = read_account_no();

    while(input == 0){
        printf("Invalid Input, Enter valid account number(10 digits): ");
        input = read_account_no();
    }

    bool found = 0;

    int i;
    for(i = 0;i<num_acc;i++){
        if(accounts[i] != NULL){
            if(accounts[i]->account_no == input){
                found = 1;
                if(accounts[i]->balance > 0){
                    printf("\nCan't delete an account with positive available balance.\n");
                    break;
                }
                else{
                    printf("\nAre you sure you want to delete this account: ");
                    printAccount(accounts[i]);
                    printf("\n[1] Confirm\n[2] Cancel and return to the Main Menu\n");
                    int confirm;
                    do {
                        confirm = readInteger();
                        if (confirm == 1)
                        {
                            distAcc(accounts[i]);
                            num_acc--;
                            int j;
                            for(j = i;j < num_acc;j++){
                                accounts[j] = accounts[j+1];
                            }
                            printf("\nAccount deleted Successfully.\n");
                            save();
                            break;
                        }
                        else if (confirm == 2){
                            printf("\nCancelled deletion!\n");
                            return;
                        }
                        else
                            printf("Invalid Input! Enter 1 or 2.\n");
                    } while (confirm != 1 && confirm != 2);
                }
            }
        }
    }
    if(!found)
        printf("\nCouldn't find account.\n");
}

void menu() {
    if (!logged_in) {
        printf("[1] Login\n");
        printf("[2] Quit\n");
        int input = readInteger();
        while(input != 1 && input != 2){
            printf("Invalid input! You must enter either 1 or 2.\n");
            printf("[1] Login\n");
            printf("[2] Quit\n");
            input = readInteger();
        }
        if (input == 1) {
            login();
            logged_in = 1;
            load();  //loads accounts
        }
        if (input == 2)
            quit();
        return ;
    }
    printf("\nWelcome to the main menu.\n");
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
    printf("[12] Quit the program entirely\n");
    int input = readInteger();
    while(!(input == 1 || input == 2 || input == 3
            || input == 4 || input == 5 || input == 6
            || input == 7 || input == 8 || input == 9
            || input == 10 || input == 11 || input == 12)){
            printf("Invalid input! Input should be between 1 and 12\n\n");
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
            printf("[12] Quit the program entirely\n");
            input = readInteger();
    }
    switch (input) {
        // all the print statements here are just temporary place holders for the actual functions
        // please replace them once the function has been made
        case 1: printf("\nAdding an account\n"); break;
        case 2: printf("\nDeleting an account\n"); delete_account(); break;
        case 3: printf("\nModifying account\n"); modify_acc(); break;
        case 4: printf("\nSearching for an account\n"); query_search(); break;
        case 5: printf("\nAdvanced searching\n"); advanced_search(); break;
        case 6: printf("\nWithdrawing money\n"); break;
        case 7: printf("\nDepositing money\n"); deposit(); break;
        case 8: printf("\nTransferring money\n"); break;
        case 9: printf("\nReporting\n"); report(); break;
        case 10: print(); break;
        case 11: log_out(); break;
        default : quit();
    }
}

void report() {
    unsigned long long acc_no;
    int i, tries_left = 5;
    printf("You must enter a correct account number within a maximum of 5 tries.\n");
    printf("Enter account number: ");
    acc_no = read_account_no();
    while((acc_no == 0) && (tries_left > 0)) {
        tries_left --;
        printf("Invalid input!\nYou have %d tries left.\n", tries_left);
        printf("Enter valid account number(10 digits): ");
        acc_no = read_account_no();
    }
    if (tries_left == 0) {
        printf("Too many invalid inputs. Returning to main menu.");
        return ;
    }
    bool found = 0;
    for(i = 0; i < num_acc; i ++){
        if(accounts[i] != NULL){
            if(accounts[i]->account_no == acc_no){
                found = 1;
            }
        }
    }
    if (!found) {
        printf("The account you requested can't be found.");
        return ;
    }
    char inputs[11] = "";
    for (i = 9; i >= 0; i --) {
        inputs[i] = '0' + acc_no % 10;
        acc_no /= 10;
    }
    strcat(inputs, ".txt");
    FILE *fp;
    fp = fopen(inputs, "r");
    if (fp == NULL) { // this shouldn't happen as we have found the account number in the accounts array
        printf("Error opening file %s\n", inputs);
        printf("Quitting the program.");
        fclose(fp);
        exit(2);
    }
    transaction_details transactions[5];
    for (i = 0; i < 5; i ++) {
        transactions[i].from = 1;
        transactions[i].to = 1;
        transactions[i].amount = 1;
    }
    while(!feof(fp)) {
        for (i = 4; i >= 1; i --) {
            transactions[i] = transactions[i - 1];
        }
        fscanf(fp, "%lld %lld %f\n", transactions[0].from, transactions[0].to, transactions[0].amount);
    }
    printf("Transactions sorted from most recent to oldest:\n");
    for (i = 0; i < 5; i ++) {
        printf("-->");
        if (transactions[i].to == 5) { // the number 5 symbolises the client, it is as if that the client has received the amount
            printf("$%.2f have been withdrawn from the account no.: %lld\n", transactions[i].amount, transactions[i].to);
        }
        else if (transactions[i].from == 5) {
            printf("$%.2f have been deposited into the account no.: %lld\n", transactions[i].amount, transactions[i].from);
        }
        else {
            printf("The account no.: %lld has tranferred $%.2f to the account no.: %lld\n", transactions[i].from, transactions[i].amount, transactions[i].to);
        }
    }
    fclose(fp);
}

void save(){
    qsort(accounts, num_acc, sizeof(*accounts), SortByNum);
    FILE *fp = fopen("accounts.txt", "w");
    if(fp == NULL)
        printf("Error: accounts.txt file not found");
    int i;
    for(i = 0;i<num_acc;i++){
        if(accounts[i] != NULL){
            fprintf(fp, "%llu,%s,%s,%.2f,%s,%d-%d\n", accounts[i]->account_no,
                    accounts[i]->name, accounts[i]->email, accounts[i]->balance,
                    accounts[i]->phone, accounts[i]->date_opened.month, accounts[i]->date_opened.year);
        }
    }
    fclose(fp);
}

void log_out() {
    printf("Are you sure you want to log out?\n");
    printf("[1] Confirm Logout\n[2] Cancel\n");
    int conf = readInteger();
    while(conf != 1 && conf != 2){
        printf("Invalid input! You must enter either 1 or 2.\n");
        printf("[1] Confirm Logout\n[2] Cancel\n");
        conf = readInteger();
    }
    if (conf == 1) {
        logged_in = 0;
        printf("Logging out\nGoodbye %s\n",current_employee.username); // should print the username of employee
        return;
    }
    printf("Logging out has been cancelled\n");
}

void quit() {
    printf("Are you sure you want to quit?\n");
    printf("[1] Confirm quit\n[2] Cancel\n");
    int conf = readInteger();
    while(conf != 1 && conf != 2){
        printf("Invalid input! You must enter either 1 or 2.\n");
        printf("[1] Confirm logout\n[2] Cancel\n");
        conf = readInteger();
    }
    if (conf == 1) {
        unload();  //unloads accounts
        printf("Quitting the program.\nGoodbye");
        exit(0);
    }
    printf("Quitting the program has been cancelled\n");
}
void login() {
    FILE *fptr;
    int i=0, flagu=0,flagp=0,inv_p=0,inv_u=0;
    char *c = malloc(100);
    char *x = malloc(50);
    char*pass=malloc(50);
    char*token;
    fptr = fopen("users.txt", "r");
    if (fptr == NULL) {
        printf("Error opening file");
        exit(1);
    }
    do { rewind(fptr);                //used to reset pointer to beginning of file
        printf("Enter username:");
        scanf("%s", x);
        i=1;
        while(i&&!feof(fptr)){
        fgets(c,100,fptr);
        token=strtok(c," ");           //used to separate password and username
        i=strcmp(token,x);}
        if(!i){
            flagu=1;
            current_employee.username=x;
            token=strtok(NULL," \n"); //checks second part of string
            do{
            printf("Enter password:");
            scanf("%s",pass);
            if(strcmp(token,pass))
            {
             printf("invalid password\n");
                inv_p++;
                printf("You have %d tries left\n", 5 - inv_p);
            }
            else{
          flagp=1;
                current_employee.password=pass;
            break;}}
            while(!flagp&&(inv_p<5));
            if(inv_p==5){
                printf("too many invalid passwords have been entered\nQuitting the program");
                exit(3);}
        }
        else {printf("Invalid username\n");
              inv_u++;
              printf("You have %d tries left\n", 5 - inv_u);}
    } while (!flagu&&(inv_u<5));
    if(inv_u==5){
    printf("too many invalid usernames have been entered\nQuitting the program");
    exit(3);}

    fclose(fptr);
    free(c);
    free(x);
    fflush(stdin);
}
