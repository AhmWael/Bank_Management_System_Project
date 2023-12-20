#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct
{
    unsigned month:4, year;
} date;

typedef struct
{
    unsigned long long from, to;
    float amount;
} transfer_details;

typedef struct
{
    char *username, *password;
} employee;

typedef struct
{
    unsigned long long account_no;
    char *name, *email, *phone;
    double balance;
    date date_opened;
    transfer_details recent_transfers[5];
} account;

void printAccount(account *a)
{
    printf("\nAccount Number : %lld\n", a->account_no);
    printf("Name: %s\n", a->name);
    printf("E-mail : %s\n", a->email);
    printf("Balance: %.2lf\n", a->balance);
    printf("Mobile: %s\n", a->phone);
    printf("Date Opened: %d-%d\n", a->date_opened.month, a->date_opened.year);// change format of printing date
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

int main()
{
    account** accounts;
    int num_acc;

    load(accounts, &num_acc);

    int i;
    for (i = 0; i < num_acc; i++)
    {
        printAccount(accounts[i]);
    }

    unload(accounts, num_acc);

    return 0;
}
