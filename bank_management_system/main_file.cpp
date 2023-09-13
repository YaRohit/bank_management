#include<bits/stdc++.h>
#include<fstream>
#include<cctype>
#include<iomanip>
using namespace std;

class Transaction {
private:
    int accountNumber;
    int amount;
    char type;

public:
    Transaction(int accNo, int amt, char transType) : accountNumber(accNo), amount(amt), type(transType) {}
    int getAccountNumber() const { return accountNumber; }
    int getAmount() const { return amount; }
    char getType() const { return type; }
};

class Account {
private:
    int accountNumber;
    char name[50];
    int balance;
    char type;

public:
    Account() : accountNumber(0), balance(0), type(' ') { strcpy(name, "");
    }

    void createAccount() {
        cout << "\nEnter The account No. : ";
        cin >> accountNumber;
        cout << "\nEnter The Name of The account Holder : ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "\nEnter Type of The account (C/S) : ";
        cin >> type;
        type = toupper(type);
        cout << "\nEnter The Initial amount (>=500 for Saving and >=1000 for current) : ";
        cin >> balance;
        cout << "\n\n\nAccount Created..";
    }

    void showAccount() const {
        cout << "\nAccount No. : " << accountNumber;
        cout << "\nAccount Holder Name : " << name;
        cout << "\nType of Account : " << type;
        cout << "\nBalance amount : " << balance;
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    int getBalance() const {
        return balance;
    }

    char getType() const {
        return type;
    }

    void modify() {
        cout << "\nAccount No. : " << accountNumber;
        cout << "\nModify Account Holder Name : ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "\nModify Type of Account : ";
        cin >> type;
        type = toupper(type);
        cout << "\nModify Balance amount : ";
        cin >> balance;
    }

    void deposit(int amount) {
        balance += amount;
    }

    void withdraw(int amount) {
        balance -= amount;
    }

    void report() const {
        cout << accountNumber << setw(10) << " " << name << setw(10) << " " << type << setw(6) << balance << endl;
    }
};

class Bank {
public:
    void writeAccount();
    void displaySpecificAccount(int accountNumber);
    void modifyAccount(int accountNumber);
    void deleteAccount(int accountNumber);
    void displayAllAccounts();
    void depositOrWithdraw(int accountNumber, int transactionType);
    void start();
    void introduction();

private:
    ifstream inFile;
    ofstream outFile;
};

void Bank::writeAccount() {
    Account account;
    outFile.open("account.dat", ios::binary | ios::app);
    account.createAccount();
    outFile.write(reinterpret_cast<char*>(&account), sizeof(Account));
    outFile.close();
}

void Bank::displaySpecificAccount(int accountNumber) {
    Account account;
    bool found = false;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    cout << "\nBALANCE DETAILS\n";

    while (inFile.read(reinterpret_cast<char*>(&account), sizeof(Account))) {
        if (account.getAccountNumber() == accountNumber) {
            account.showAccount();
            found = true;
        }
    }
    inFile.close();
    if (!found)
        cout << "\n\nAccount number does not exist";
}

void Bank::modifyAccount(int accountNumber) {
    bool found = false;
    Account account;
    fstream file;
    file.open("account.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    while (!file.eof() && !found) {
        file.read(reinterpret_cast<char*>(&account), sizeof(Account));
        if (account.getAccountNumber() == accountNumber) {
            account.showAccount();
            cout << "\n\nEnter The New Details of account" << endl;
            account.modify();
            int pos = (-1) * static_cast<int>(sizeof(Account));
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char*>(&account), sizeof(Account));
            cout << "\n\n\t Record Updated";
            found = true;
        }
    }
    file.close();
    if (!found)
        cout << "\n\n Record Not Found ";
}

void Bank::deleteAccount(int accountNumber) {
    Account account;
    ifstream inFile;
    ofstream outFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    outFile.open("Temp.dat", ios::binary);
    inFile.seekg(0, ios::beg);
    while (inFile.read(reinterpret_cast<char*>(&account), sizeof(Account))) {
        if (account.getAccountNumber() != accountNumber) {
            outFile.write(reinterpret_cast<char*>(&account), sizeof(Account));
        }
    }
    inFile.close();
    outFile.close();
    remove("account.dat");
    rename("Temp.dat", "account.dat");
    cout << "\n\n\tRecord Deleted ..";
}

void Bank::displayAllAccounts() {
    Account account;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    cout << "\n\n\t\tACCOUNT HOLDER LIST\n\n";
    cout << "====================================================\n";
    cout << "A/c no.      NAME           Type  Balance\n";
    cout << "====================================================\n";
    while (inFile.read(reinterpret_cast<char*>(&account), sizeof(Account))) {
        account.report();
    }
    inFile.close();
}

void Bank::depositOrWithdraw(int accountNumber, int transactionType) {
    int amount;
    bool found = false;
    Account account;
    fstream file;
    file.open("account.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    while (!file.eof() && !found) {
        file.read(reinterpret_cast<char*>(&account), sizeof(Account));
        if (account.getAccountNumber() == accountNumber) {
            account.showAccount();
            if (transactionType == 1) {
                cout << "\n\n\tTO DEPOSIT AMOUNT ";
                cout << "\n\nEnter The amount to be deposited: ";
                cin >> amount;
                account.deposit(amount);
            }
            if (transactionType == 2) {
                cout << "\n\n\tTO WITHDRAW AMOUNT ";
                cout << "\n\nEnter The amount to be withdrawn: ";
                cin >> amount;
                int balance = account.getBalance() - amount;
                if ((balance < 500 && account.getType() == 'S') || (balance < 1000 && account.getType() == 'C'))
                    cout << "Insufficient balance";
                else
                    account.withdraw(amount);
            }
            int pos = (-1) * static_cast<int>(sizeof(account));
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char*>(&account), sizeof(Account));
            cout << "\n\n\t Record Updated";
            found = true;
        }
    }
    file.close();
    if (!found)
        cout << "\n\n Record Not Found ";
}

void Bank::start() {
    char choice;
    int num;
    introduction();
    do {
        system("cls");
        cout << "\n\n\n\tMAIN MENU";
        cout << "\n\n\t01. NEW ACCOUNT";
        cout << "\n\n\t02. DEPOSIT AMOUNT";
        cout << "\n\n\t03. WITHDRAW AMOUNT";
        cout << "\n\n\t04. BALANCE ENQUIRY";
        cout << "\n\n\t05. ALL ACCOUNT HOLDER LIST";
        cout << "\n\n\t06. CLOSE AN ACCOUNT";
        cout << "\n\n\t07. MODIFY AN ACCOUNT";
        cout << "\n\n\t08. EXIT";
        cout << "\n\n\tSelect Your Option (1-8) ";
        cin >> choice;
        system("cls");
        switch (choice) {
        case '1':
            writeAccount();
            break;
        case '2':
            cout << "\n\n\tEnter The account No. : ";
            cin >> num;
            depositOrWithdraw(num, 1);
            break;
        case '3':
            cout << "\n\n\tEnter The account No. : ";
            cin >> num;
            depositOrWithdraw(num, 2);
            break;
        case '4':
            cout << "\n\n\tEnter The account No. : ";
            cin >> num;
            displaySpecificAccount(num);
            break;
        case '5':
            displayAllAccounts();
            break;
        case '6':
            cout << "\n\n\tEnter The account No. : ";
            cin >> num;
            deleteAccount(num);
            break;
        case '7':
            cout << "\n\n\tEnter The account No. : ";
            cin >> num;
            modifyAccount(num);
            break;
        case '8':
            cout << "\n\n\tThanks for using bank management system";
            break;
        default:
            cout << "\a";
        }
        cin.ignore();
        cin.get();
    } while (choice != '8');
}

void Bank::introduction() {
    cout << "\n\n\n\t  BANK";
    cout << "\n\n\tMANAGEMENT";
    cout << "\n\n\t  SYSTEM";
    cout << "\n\n\n\nMADE BY : Rohit Yadav";
    cout << "\n\nSCHOOL : Army Institute Of Technology";
    cin.get();
}

int main() {
    Bank bank;
    bank.start();
    return 0;
}
