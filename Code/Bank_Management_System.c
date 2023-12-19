#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
    int month:4, year;
}date;

typedef struct {
    unsigned long long from, to;
    float amount;
}transfer_details;

typedef struct {
    unsigned long long account_no;
    char *username, password, email, phone;
    int balance;
    date date_opened;
    transfer_details recent_transfers[5];
}account;

int main()
{


    return 0;
}
