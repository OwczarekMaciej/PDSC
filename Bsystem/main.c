#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME 21
#define MAX_SURNAME 31
#define MAX_PESEL 20
#define MAX_ADDRESS 50
#define PESEL 11
#define MAX_NO 100

FILE *fptr;

typedef struct Account
{
    long int accNo;
    char name[MAX_NAME];
    char surname[MAX_SURNAME];
    char pesel[MAX_PESEL];
    char address[MAX_ADDRESS];
    int regularBalance;
    int savingsBalance;
} ACCOUNT;

ACCOUNT search_client;

void flush_input_buffer()
{
    char clear;
    while ((clear = getchar()) != '\n' && clear != EOF)
        ;
}

void fill_account_with_zeros(ACCOUNT *account)
{
    memset(account, 0, sizeof(ACCOUNT));
}

void open_or_create_file()
{
    fptr = fopen("clients.dat", "r+");

    if (fptr == NULL)
    {
        printf("Database cannot be found or does not exist\nCreating new one...\n");
        fptr = fopen("clients.dat", "w+");
    }
    printf("Correct access to the database.\n");
    fclose(fptr);
}

int choose_operation()
{
    int operation;
    while (1)
    {
        printf("\n\n");
        printf("What would you like to do?\n");
        printf("1 - create new account\n");
        printf("2 - show list of accounts\n");
        printf("3 - search for an account\n");
        printf("4 - make a deposit\n");
        printf("5 - make a withdrawal\n");
        printf("6 - make a money transfer between two accounts\n");
        printf("7 - make a money transfer between regular and saving sub-accounts\n");
        printf("0 - quit\n");
        int input = scanf("%d", &operation);
        if (input != 1)
        {
            printf("Wrong operation\n");
            flush_input_buffer();
        }
        flush_input_buffer();
        if ((operation >= 1 && operation <= 7) || operation == 0)
        {
            return operation;
        }
        else
            printf("Wrong operation\n");
    }
}

int check_name(char *array)
{
    int c = 0;
    while (array[c] != '\0')
    {
        if (!isalpha(array[c]))
        {
            printf("This text should contain only letters.\n");
            return 0;
        }
        c++;
    }
    if (c < 2)
    {
        printf("Input is too short!\n");
        return 0;
    }
    return 1;
}

int check_pesel(char *array)
{
    int c = 0;
    int len = 0;
    while (array[c] != '\0')
    {
        if (!isdigit(array[c]))
        {
            printf("Pesel should contain only numbers!\n");
            return 0;
        }
        c++;
        len++;
    }
    if (len == PESEL)
    {
        return 1;
    }
    else
    {
        printf("The length of the pesel is wrong.\n");
    }
    return 0;
}

int check_address(char *array)
{
    int c = 0;
    if (!isalnum(array[c]))
    {
        printf("First letter of address must be a letter!\n");
        return 0;
    }

    while (array[c] != '\0')
    {
        if (!isalnum(array[c]) && array[c] != ' ' && array[c] != ',' && array[c] != '.')
        {
            printf("Address contains invalid characters!\n");
            return 0;
        }
        c++;
    }
    return 1;
}

int get_money()
{
    float money = 0;
    char number[100];
    if (!scanf("%99s", number))
        return -1;
    char *end;
    money = strtod(number, &end);
    if (*end != '\0')
    {
        return -1;
    }
    if (money < 0)
        return 0;
    return money * 100;
}

int confirm()
{
    char confromation;
    printf("\nDo you want to confirm? Y / N:\n");
    confromation = getchar();
    if (tolower(confromation) == 'y')
    {
        return 1;
    }
    return 0;
}

void get_input(char array[], int size)
{
    int true = 1;
    while (true)
    {
        fgets(array, size, stdin);
        int index = 0;
        int length = strlen(array);
        for (index = 0; index < length; index++)
        {
            if (array[index] == '\n')
            {
                array[index] = '\0';
                true = 0;
            }
        }
    }
}

void create_account()
{
    ACCOUNT new_account;
    ACCOUNT last_account;
    int pass = 0;
    fill_account_with_zeros(&new_account);
    fill_account_with_zeros(&last_account);

    printf("\n\n\n");
    printf("CREATING NEW ACCOUNT\n\n");
    while (!pass)
    {
        printf("Name: ");
        get_input(new_account.name, MAX_NAME);
        if (check_name(new_account.name) == 1)
            pass = 1;
    }
    pass = 0;
    while (!pass)
    {
        printf("Surname: ");
        get_input(new_account.surname, MAX_SURNAME);
        if (check_name(new_account.surname) == 1)
            pass = 1;
    }
    pass = 0;
    while (!pass)
    {
        printf("Pesel: ");
        get_input(new_account.pesel, MAX_PESEL);
        if (check_pesel(new_account.pesel) == 1)
            pass = 1;
    }
    pass = 0;
    while (!pass)
    {
        printf("Address: ");
        get_input(new_account.address, MAX_ADDRESS);
        if (check_address(new_account.address) == 1)
            pass = 1;
    }
    printf("\n\n\n");
    printf("NEW ACCOUNT DETAILS:\n\n");
    printf("NAME: %s\nSURNAME: %s\nADDRESS: %s\nPESEL: %s\n", new_account.name, new_account.surname, new_account.address, new_account.pesel);

    if (confirm())
    {
        int accountNumber;

        fptr = fopen("clients.dat", "r+");

        fseek(fptr, -1 * sizeof(ACCOUNT), SEEK_END);
        fread(&last_account, sizeof(ACCOUNT), 1, fptr);
        accountNumber = last_account.accNo;
        new_account.accNo = 1 + accountNumber;
        fseek(fptr, (accountNumber) * sizeof(ACCOUNT), SEEK_SET);

        if (fwrite(&new_account, sizeof(ACCOUNT), 1, fptr) == 1)
        {
            printf("Account has been created\n");
        }
        else
        {
            printf("Account creation failed\n");
        }
        fclose(fptr);
    }
}

void print_account(ACCOUNT account)
{
    float regBalance = (float)(account.regularBalance) / 100;
    float savBalance = (float)(account.savingsBalance) / 100;

    printf("Account No: %ld\t", account.accNo);
    printf("Name: %s\t", account.name);
    printf("Surname: %s\t", account.surname);
    printf("Address: %s\t", account.address);
    printf("Pesel: %s\t", account.pesel);
    printf("Reg. Balance: %.2f\t", regBalance);
    printf("Sav. Balance: %.2f\t", savBalance);
    printf("\n\n");
}

void show_accounts()
{
    ACCOUNT account;
    fptr = fopen("clients.dat", "r");
    printf("\n\nACCOUNTS: \n\n");

    while (fread(&account, sizeof(ACCOUNT), 1, fptr) == 1)
        print_account(account);

    fclose(fptr);
}

ACCOUNT *search_for_account(long int accNo, char text[MAX_ADDRESS], int operation)
{
    fptr = fopen("clients.dat", "r");
    int acc_found = 0;

    while (fread(&search_client, sizeof(ACCOUNT), 1, fptr) == 1)
    {
        if (operation == 1 && accNo == search_client.accNo)
        {
            acc_found = 1;
            print_account(search_client);
            fclose(fptr);
            return &search_client;
        }
        else if (operation == 2 && !strcmp(text, search_client.name))
        {
            acc_found = 1;
            print_account(search_client);
            fclose(fptr);
            return &search_client;
        }

        else if (operation == 3 && !strcmp(text, search_client.surname))
        {
            acc_found = 1;
            print_account(search_client);
            fclose(fptr);
            return &search_client;
        }

        else if (operation == 4 && !strcmp(text, search_client.address))
        {
            acc_found = 1;
            print_account(search_client);
            fclose(fptr);
            return &search_client;
        }

        else if (operation == 5 && !strcmp(text, search_client.pesel))
        {
            acc_found = 1;
            print_account(search_client);
            fclose(fptr);
            return &search_client;
        }
    }
    fclose(fptr);
    if (acc_found == 0)
        printf("Account does not exist.\n");
    return NULL;
}

ACCOUNT *choose_search_filter()
{
    int operation;
    long int accNo;
    char text_key[MAX_ADDRESS];
    while (1)
    {
        printf("\n\nSearch account using one of the following fields: \n");
        printf("1 - account number\n");
        printf("2 - name\n");
        printf("3 - surname\n");
        printf("4 - address\n");
        printf("5 - pesel\n");

        int input = scanf("%d", &operation);
        if (input != 1)
        {
            printf("Wrong operation\n");
            flush_input_buffer();
        }
        flush_input_buffer();

        int pass = 0;
        if (operation == 1)
        {
            while (!pass)
            {
                printf("\nEnter account number: ");
                int acc_input = scanf("%ld", &accNo);
                if (acc_input != 1)
                {
                    printf("Wrong input\n");
                    flush_input_buffer();
                }
                else
                {
                    pass = 1;
                }
            }
            break;
        }
        else if (operation == 2 || operation == 3)
        {
            pass = 0;
            while (!pass)
            {
                printf("Enter name or surname: ");
                get_input(text_key, MAX_SURNAME);
                if (check_name(text_key) == 1)
                    pass = 1;
            }
            break;
        }
        else if (operation == 4)
        {
            pass = 0;
            while (!pass)
            {
                printf("Enter address ");
                get_input(text_key, MAX_ADDRESS);
                if (check_address(text_key) == 1)
                    pass = 1;
            }
            break;
        }
        else if (operation == 5)
        {
            pass = 0;
            while (!pass)
            {
                printf("Enter pesel ");
                get_input(text_key, MAX_PESEL);
                if (check_pesel(text_key) == 1)
                    pass = 1;
            }
            break;
        }
        else
        {
            printf("Wrong operation.");
            return NULL;
        }
    }
    return search_for_account(accNo, text_key, operation);
}

int compare(ACCOUNT account1, ACCOUNT account2)
{
    if (account1.accNo != account2.accNo)
        return 0;

    else if (strcmp(account1.name, account2.name) != 0)
        return 0;

    else if (strcmp(account1.surname, account2.surname) != 0)
        return 0;

    else if (strcmp(account1.pesel, account2.pesel) != 0)
        return 0;

    else if (strcmp(account1.address, account2.address) != 0)
        return 0;

    else if (account1.regularBalance != account2.regularBalance)
        return 0;

    else if (account1.savingsBalance != account2.savingsBalance)
        return 0;

    return 1;
}

void update_account(ACCOUNT old_ccount, ACCOUNT new_account)
{
    ACCOUNT account;
    fptr = fopen("clients.dat", "r+");

    while (fread(&account, sizeof(ACCOUNT), 1, fptr) == 1)
    {
        if (compare(old_ccount, account))
        {

            fseek(fptr, -1 * sizeof(ACCOUNT), SEEK_CUR);
            if (fwrite(&new_account, sizeof(ACCOUNT), 1, fptr) == 1)
            {
                printf("Account has been updated\n");
                fclose(fptr);
            }
            else
            {
                printf("Error writing data to clients.dat");
            }
            return;
        }
    }
    fclose(fptr);
}

void deposit()
{
    printf("\n\nDEPOSIT\n");
    ACCOUNT *temp_ptr = choose_search_filter();
    if (!temp_ptr)
        return;

    ACCOUNT old_account = *temp_ptr;
    ACCOUNT new_account = old_account;

    int deposit;
    int pass = 0;
    while (!pass)
    {
        printf("How much money do you want to deposit?\n");
        deposit = get_money();
        if (deposit == -1)
        {
            printf("Wrong input\n");
            flush_input_buffer();
        }
        if (deposit == 0)
        {
            printf("Deposit value cannot be negative\n");
            flush_input_buffer();
        }
        else
            pass = 1;
    }
    flush_input_buffer();
    if (confirm())
    {
        int relDeposit = deposit;
        new_account.regularBalance += relDeposit;
        update_account(old_account, new_account);
        print_account(new_account);
    }
}

void withdrawal()
{
    printf("\n\nWITHDRAWAL\n");
    ACCOUNT *temp_ptr = choose_search_filter();
    if (!temp_ptr)
        return;

    ACCOUNT old_account = *temp_ptr;
    ACCOUNT new_account = old_account;

    int transfer;
    int pass = 0;
    while (!pass)
    {
        printf("How much money do you want to withdraw?\n");
        transfer = get_money();
        if (transfer == -1)
        {
            printf("Wrong input\n");
            flush_input_buffer();
        }
        if (transfer == 0)
        {
            printf("Withdrawal value cannot be negative\n");
            flush_input_buffer();
        }
        else
            pass = 1;
    }
    int relWithdrawal = transfer;
    flush_input_buffer();
    if (confirm())
    {
        if (old_account.regularBalance >= relWithdrawal)
        {
            new_account.regularBalance -= relWithdrawal;
            update_account(old_account, new_account);
            print_account(new_account);
        }
        else
            printf("There is not enough funds in the balance of regular account\n");
    }
}

void money_transfer()
{
    printf("\nSelect the account from which you want to transfer money");
    ACCOUNT *temp_ptr = choose_search_filter();
    if (!temp_ptr)
        return;

    ACCOUNT old_account_from = *temp_ptr;
    ACCOUNT new_account_form = old_account_from;

    printf("\nSelect the account to which you want to transfer money");
    temp_ptr = choose_search_filter();
    if (!temp_ptr)
        return;

    ACCOUNT old_account_to = *temp_ptr;
    ACCOUNT new_account_to = old_account_to;

    int transfer;
    int pass = 0;
    while (!pass)
    {
        printf("How much money do you want to transfer?\n");
        transfer = get_money();
        if (transfer == -1)
        {
            printf("Wrong input\n");
            flush_input_buffer();
        }
        if (transfer == 0)
        {
            printf("Transfer value cannot be negative\n");
            flush_input_buffer();
        }
        else
            pass = 1;
    }
    flush_input_buffer();
    if (confirm())
    {

        int relTransfer = transfer;
        if (old_account_from.regularBalance >= relTransfer)
        {
            new_account_form.regularBalance -= relTransfer;
            update_account(old_account_from, new_account_form);
            print_account(new_account_form);

            new_account_to.regularBalance += relTransfer;
            update_account(old_account_to, new_account_to);
            print_account(new_account_to);
        }
        else
            printf("There is not enough funds in the balance of regular account\n");
    }
}

void subaccount_transfer()
{
    printf("\n\nSUBACCOUNTS TRANSFER\n\n");
    ACCOUNT *temp_ptr = choose_search_filter();
    if (!temp_ptr)
        return;

    ACCOUNT old_account = *temp_ptr;
    ACCOUNT new_account = old_account;

    printf("1 - to savings sub-account\n");
    printf("2 - from savings sub-account\n");

    int operation;
    int pass = 0;
    int input = scanf("%d", &operation);
    if (input != 1)
    {
        printf("Wrong operation\n");
        flush_input_buffer();
    }
    flush_input_buffer();
    int transfer;
    if (operation == 1)
    {
        while (!pass)
        {
            printf("How much money do you want to transfer?\n");
            transfer = get_money();
            if (transfer == -1)
            {
                printf("Wrong input\n");
                flush_input_buffer();
            }
            if (transfer == 0)
            {
                printf("Transfer value cannot be negative\n");
                flush_input_buffer();
            }
            else
                pass = 1;
        }
        flush_input_buffer();
        if (confirm())
        {
            if (old_account.regularBalance >= transfer)
            {
                new_account.savingsBalance += transfer;
                new_account.regularBalance -= transfer;
                update_account(old_account, new_account);
                print_account(new_account);
            }
            else
                printf("There is not enough funds in the balance of regular account\n");
        }
    }
    else if (operation == 2)
    {
        int transfer;
        while (!pass)
        {
            printf("How much money do you want to transfer?\n");
            transfer = get_money();
            if (transfer == -1)
            {
                printf("Wrong input\n");
                flush_input_buffer();
            }
            if (transfer == 0)
            {
                printf("Transfer value cannot be negative\n");
                flush_input_buffer();
            }
            else
                pass = 1;
        }
        flush_input_buffer();
        if (confirm())
        {
            int relTransfer = transfer;
            if (old_account.savingsBalance >= relTransfer)
            {
                new_account.regularBalance += relTransfer;
                new_account.savingsBalance -= relTransfer;
                update_account(old_account, new_account);
                print_account(new_account);
            }
            else
                printf("There is not enough funds in the balance of saving account\n");
        }
    }
    else
        printf("Wrong operation\n");
}

int main()
{
    open_or_create_file();
    while (1)
    {
        int operation = choose_operation();

        switch (operation)
        {
        case 1:
            create_account();
            break;
        case 2:
            show_accounts();
            break;
        case 3:
            choose_search_filter();
            break;
        case 4:
            deposit();
            break;
        case 5:
            withdrawal();
            break;
        case 6:
            money_transfer();
            break;
        case 7:
            subaccount_transfer();
            break;
        case 0:
            printf("\nProgram closed successfully.\n");
            exit(9);
            break;
        }
    }
    return 0;
}