#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int month:4, year;
} date;

typedef struct
{
    unsigned long long from, to;
    float amount;
} transfer_details;

typedef struct
{
    unsigned long long account_no;
    char *name;
    char *email;
    char *phone;
    float balance;
    date date_opened;
    transfer_details recent_transfers[5];
} account;

account* constAcc(unsigned long long account_no, char *name, char *email/*, char *phone, float balance*/)
{
    account* p = malloc(sizeof(account));

    p->account_no = account_no;

    p->name = malloc(strlen(name) + 1);
    strcpy(p->name, name);

    p->email = malloc(strlen(email) + 1);
    strcpy(p->email, email);

    return p;
}

void distAcc(account *p)
{
    free(p->name);
    free(p->email);
    free(p);
}

account* decodeText(char* line)
{
    unsigned long long accNum = atoi(strtok(line, ","));
    char *name = strtok(NULL, ",");
    char *email = strtok(NULL, ",");

    return constAcc(accNum, name, email);
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
    while(fgets(record, sizeof(record), fp))  (*numRec)++;

    *accounts = malloc((*numRec) * sizeof(account));

    rewind(fp);
    int i;
    for (i = 0; i < *numRec; i++)
    {
        if(!fgets(record, sizeof(record), fp)) break;
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
    free(*accounts);
}

int main()
{
    account** accounts;
    int num_acc;

    load(accounts, &num_acc);

    int i;
    for (i = 0; i < num_acc; i++)
    {
        printf("Acc num = %llu, name = %s, email = %s\n", accounts[i]->account_no, accounts[i]->name, accounts[i]->email);
    }

    unload(accounts, num_acc);

    return 0;
}
