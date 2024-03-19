#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void display_details();
void open_account();
void deposit();
void withdraw();
void view_statement();
void change_password();
void manage_payee();
void add_payee(char*);
void display_payee_list(char*);
int validate(char*);
void acc_num_generate(char*);
void password_encrypt(char*);
void password_decrypt(char*);

struct User {
    char name[20], address[50], phone_number[11], email[25], username[25], account_num[20], password[16];
};

struct Transanction {
    char username[25], time[100], transaction[10];
    int amount;
};

struct Balance {
    char username[25];
    int balance;
};

struct Payee {
    char username[25], payee_name[20], payee_accn_no[11], payee_address[50];
};
void initial() {
    printf("\n\tWelcome to ABC Bank\n");
    printf("___________________________________\n");
    printf("\nPlease select an option- \n");
    printf("1: View Account Details\n");
    printf("2: Open new account\n");
    printf("3: Change Password\n");
    printf("4: View Statement\n");
    printf("5: Deposit\n");
    printf("6: Withdrawal\n");
    printf("7: Manage Payee\n");
    printf("\nYour selection: ");
    int choice;
    scanf("%d", &choice);
    switch(choice) {
        case 1: display_details();
                break;
        case 2: open_account();
                break;
        case 3: change_password();
                break;
        case 4: view_statement();
                break;
        case 5: deposit();
                break;
        case 6: withdraw();
                break;
        case 7: manage_payee();
                break;
        default: printf("Invalid Selection");
                 break;
    }
}


void display_details() {
    FILE *file1 = fopen("User_Detail.bin", "rb");
    FILE *file2 = fopen("Balance.bin", "rb");
    if(file1 == NULL || file2 == NULL) {
        printf("FILE ERROR");
        return;
    }
    printf("\n   ---Login to your account---\n");
    printf("\nEnter your username: ");
    char user_name[25];
    scanf("%s", user_name);
    int ans = validate(user_name);
    if(ans == 1) {
        struct User u;
        struct Balance b;
        while(fread(&u, sizeof(struct User), 1, file1) == 1) {
            if(strcmp(u.username, user_name) == 0) {
                printf("\n   ---Details of the Account Holder---   \n");
                printf("\n     Name:        %s", u.name);
                printf("\b     Address:     %s", u.address);
                printf("\b     Phone No:    %s", u.phone_number);
                printf("\n     Email Id:    %s", u.email);
                printf("\n     Account No:  %s", u.account_num);
                fclose(file1);
                while(fread(&b, sizeof(struct Balance), 1, file2)) {
                    if(strcmp(b.username, user_name) == 0) {
                        printf("\n     Balance:     Rs. %d", b.balance);
                        fclose(file2);
                        break;
                    }
                }
                break;
            }
        }
    }
}

void open_account() {
    FILE *file1 = fopen("User_Detail.bin", "ab");
    FILE *file2 = fopen("Balance.bin", "ab");
    if(file1 == NULL || file2 == NULL) {
        printf("FILE ERROR");
        return;
    }
    struct User u;
    printf("\nPlease enter your details: \n");
    char tname[20];
    getchar();
    printf("\nName: ");
    fgets(tname, 20, stdin);
    strcpy(u.name, tname);
    char taddress[50];
    printf("Address: ");
    fgets(taddress, 50, stdin);
    strcpy(u.address, taddress);
    printf("Phone number: ");
    scanf("%s", u.phone_number);
    printf("Email: ");
    scanf("%s", u.email);
    char account_num[20];
    strcpy(u.username, u.email);
    printf("\nYour username is your email id: %s", u.username);
    acc_num_generate(account_num);
    printf("\nYour account number is: %s", account_num);
    strcpy(u.account_num, account_num);
    char pass[16];
    printf("\nPlease set your password: ");
    scanf("%s", pass);
    password_encrypt(pass);
    strcpy(u.password, pass);
    fwrite(&u, sizeof(struct User), 1, file1);
    printf("\nCongratulations! Your account has been successfully created.");
    fclose(file1);

    
    struct Balance b;
    strcpy(b.username, u.username);
    b.balance = 0;
    fwrite(&b, sizeof(struct Balance), 1, file2);
    fclose(file2);
}

void change_password() {
    char user_name[25];
    printf("\n   ---Login to your account---\n");
    printf("\nEnter your username: ");
    scanf("%s", user_name);
    int ans = validate(user_name);
    if(ans == 1) {
        FILE *file1 = fopen("User_Detail.bin", "rb+");
        if(file1 == NULL) {
            printf("FILE ERROR");
            return;
        }
        struct User u;
        char newpass[16];
        printf("\nEnter your new password: ");
        scanf("%s", newpass);
        int z=0;
        while(fread(&u, sizeof(struct User), 1, file1) == 1) {
            if(strcmp(u.username, user_name) == 0) {
                password_encrypt(newpass);
                strcpy(u.password, newpass);
                fseek(file1, z*sizeof(struct User),  SEEK_SET);
                fwrite(&u, sizeof(struct User), 1, file1);
                fclose(file1);
                printf("\nPassword changed successfully!");
                break;
            }
            z++;
        }
    }
}

void view_statement() {
    char user_name[25];
    // int amt;
    struct Transanction t1;
    printf("\n   ---Login to your account---\n");
    printf("\nEnter username: ");
    scanf("%s", user_name);
    int ans = validate(user_name);
    int check = 0;
    if(ans == 1) {
        FILE *file1 = fopen("Transanction.bin", "rb");
        if(file1 == NULL) {
            printf("FILE ERROR");
            return;
        }
        while(fread(&t1, sizeof(struct Transanction), 1, file1) == 1) {
            if(strcmp(t1.username, user_name) == 0) {
                if(check == 0) {
                    printf("\n\t\tTransactions");
                    printf("\n__________________________________________________");
                    printf("\n     Amount\t  DR/CR\t\t  Time\n");
                    printf("__________________________________________________\n");
                }
                printf("\n   Rs. %-8d\t   %s\t   %s", t1.amount, t1.transaction, t1.time);
                check++;
            }
        }
        fclose(file1);
        if(check == 0)
            printf("\nNo Transactions Yet!");
    }
}

void deposit() {
    char user_name[25];
    int amt;
    struct Transanction t1;
    printf("\n   ---Login to your account---\n");
    printf("\nEnter username: ");
    scanf("%s", user_name);
    int ans = validate(user_name);
    if(ans == 1) {
        FILE *file1 = fopen("Transanction.bin", "ab");
        FILE *file2 = fopen("Balance.bin", "rb+");
        if(file1 == NULL || file2 == NULL) {
            printf("FILE ERROR");
            return;
        }
        printf("\nEnter amount to be deposited: Rs. ");
        time_t t;
        time(&t);
        scanf("%d", &amt);
        strcpy(t1.username, user_name);
        strcpy(t1.transaction, "CR");
        strcpy(t1.time, ctime(&t));
        t1.amount = amt;
        fwrite(&t1, sizeof(struct Transanction), 1, file1);
        fclose(file1);
        struct Balance b; 
        int z = 0;
        while(fread(&b, sizeof(struct Balance), 1, file2) == 1) {
            if(strcmp(b.username, user_name) == 0) {
                b.balance += amt;
                fseek(file2, z * sizeof(struct Balance), SEEK_SET);
                fwrite(&b, sizeof(struct Balance), 1, file2);
                fclose(file2);
                printf("\nAmount Credited Successfully !");
                printf("\nUpdated Balance: %d", b.balance);
                break;
            }
            z++;
        }
    }
}

void withdraw() {
    char user_name[25];
    int amt;
    struct Transanction t1;
    printf("\n   ---Login to your account---\n");
    printf("\nEnter username: ");
    scanf("%s", user_name);
    int ans = validate(user_name);
    if(ans == 1) {
        FILE *file1 = fopen("Transanction.bin", "ab");
        FILE *file2 = fopen("Balance.bin", "rb+");
        if(file1 == NULL || file2 == NULL) {
            printf("FILE ERROR");
            return;
        }
        printf("\nEnter amount to be withdrawn: Rs. ");
        time_t t;
        time(&t);
        scanf("%d", &amt);
        struct Balance b; 
        int z = 0;
        while(fread(&b, sizeof(struct Balance), 1, file2) == 1) {
            if(strcmp(b.username, user_name) == 0) {
                if(b.balance >= amt) {
                    b.balance -= amt;
                    fseek(file2, z * sizeof(struct Balance), SEEK_SET);
                    fwrite(&b, sizeof(struct Balance), 1, file2);
                    fclose(file2);
                    strcpy(t1.username, user_name);
                    strcpy(t1.transaction, "DR");
                    strcpy(t1.time, ctime(&t));
                    t1.amount = amt;
                    fwrite(&t1, sizeof(struct Transanction), 1, file1);
                    printf("\nAmount Debited Successfully!");
                    printf("\nUpdated Balance: %d", b.balance);
                    fclose(file1);
                    break;
                }
                else {
                    printf("Insufficient Balance!");
                    break;
                }
            }
            z++;
        }
    }
}

void manage_payee() {
    char user_name[25];
    printf("\n   ---Login to your account---\n");
    printf("\nEnter username: ");
    scanf("%s", user_name);
    int ans = validate(user_name);
    if(ans == 1) {
        int choice;
        printf("\nSelect an option: ");
        printf("\n1. Add payee");
        printf("\n2. Display payee list\n");
        printf("\nYour selection: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1: add_payee(user_name);
                    break;
            case 2: display_payee_list(user_name);
                    break;
            default: printf("\nInvalid selection");
                     break;
        }
    }
}

void add_payee(char *username) {
    FILE *file1 = fopen("Payee.bin", "ab");
    if(file1 == NULL) {
        printf("FILE ERROR");
        return;
    }
    struct Payee p;
    char tname[20], taccn[11], taddress[50];
    printf("\nPlease enter the details-\n");
    getchar();
    printf("Name of payee: ");
    fgets(tname, 21, stdin);
    printf("Account No of payee: ");
    fgets(taccn, 11, stdin);
    getchar();
    printf("Address of payee: ");
    fgets(taddress, 51, stdin);
    strcpy(p.username, username);
    strcpy(p.payee_name, tname);
    strcpy(p.payee_accn_no, taccn);
    strcpy(p.payee_address, taddress);
    fwrite(&p, sizeof(struct Payee), 1, file1);
    printf("\nPayee added successfully!");
    fclose(file1);
}

void display_payee_list(char *username) {
    FILE *file1 = fopen("Payee.bin", "rb");
    if(file1 == NULL) {
        printf("FILE ERROR");
        return;
    }
    struct Payee p;
    int count = 1;
    while(fread(&p, sizeof(struct Payee), 1, file1) == 1) {
        if(strcmp(p.username, username) == 0) {
            printf("\n\tPayee %d\n", count);
            printf("________________________\n");
            printf("\nName:        %s", p.payee_name);
            printf("Address:     %s", p.payee_address);
            printf("Account No:  %s\n\n", p.payee_accn_no);
            count++;
        }
    }
    if(count == 1) 
        printf("\nPayee list is empty!");
    fclose(file1);
}

int validate(char *user_name) {
    FILE *file1 = fopen("User_Detail.bin", "rb");
    if(file1 == NULL) {
        printf("FILE ERROR");
        return 0;
    }
    char pass[16];
    struct User u;
    while(fread(&u, sizeof(struct User), 1, file1) == 1) {
        if(strcmp(u.username, user_name) == 0) {
            printf("Enter your password: ");
            scanf("%s", pass);
            char correct_pass[16];
            strcpy(correct_pass, u.password);
            password_decrypt(correct_pass);
            if(strcmp(correct_pass, pass) == 0) {
                fclose(file1);
                printf("Login Successful\n");
                return 1;
            }
            else {
                printf("\n***** Wrong Password *****");
                fclose(file1);
                return 0;
            } 
        }
    }
    printf("\nUser not found!");
    fclose(file1);
    return -1;
}

void acc_num_generate(char *account_num) {
    srand(time(NULL));
    char str1[20], str2[20];
    int upper = 99999, lower = 10000;
    int p1 = (rand() % (upper - lower +1)) + lower;
    int p2 = (rand() % (upper - lower +1)) + lower;
    itoa(p1, str1, 10);
    itoa(p2, str2, 10);
    strcat(str1, str2);
    strcpy(account_num, str1);
}

void password_encrypt(char *pass) {
    char ch;
    char new_pass[16];
    for(int i=0; i<16; i++) {
        ch = *(pass+i);
        if(ch != '\0') {
            ch += 100;
            new_pass[i] = ch;
        }
        else {
            new_pass[i] = '\0';
            break;
        }
    }
    strcpy(pass, new_pass);
}

void password_decrypt(char *pass) {
    char ch;
    char new_pass[16];
    for(int i=0; i<16; i++) {
        ch = *(pass + i);
        if(ch != '\0') {
            ch -= 100;
            new_pass[i] = ch;
        }
        else {
            new_pass[i] = '\0';
            break;
        }
    }
    strcpy(pass, new_pass);
}

int main() {
    initial();
    printf("\n\n\n-----------Thanku for choosing ABC Bank------------");
}