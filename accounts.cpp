#include <iostream>

using std::cout;
using std::endl;

class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    Account(std::string number, std::string holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    virtual void displayDetails() {
        cout << "Account Details for Account (ID: " << accountNumber << "):" << endl;
        cout << "   Holder: " << accountHolder << endl;
        cout << "   Balance: $" << balance << endl;
    }

    void deposit(double amount) {
        balance += amount;
    }

    virtual void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
        } else {
            cout << "Insufficient funds for withdrawal." << endl;
        }
    }
    double getBalance() {
        return balance;
    }
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(std::string number, std::string holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    void displayDetails() override {
        Account::displayDetails();
        cout << "   Interest Rate: " << (interestRate * 100) << "%" << endl;
    }

    void withdraw(double amount) override {
        const double minBalance = 100; // Minimum balance required for withdrawal
        if (balance - amount >= minBalance) {
            balance -= amount;
        } else {
            cout << "Withdrawal failed. Minimum balance not maintained." << endl;
        }
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(std::string number, std::string holder, double initialBalance, double limit)
        : Account(number, holder, initialBalance), overdraftLimit(limit) {}

    void displayDetails() override {
        Account::displayDetails();
        cout << "   Overdraft Limit: $" << overdraftLimit << endl;
    }

    void withdraw(double amount) override {
        if (balance - amount >= -overdraftLimit) {
            balance -= amount;
        } else {
            cout << "Withdrawal failed. Overdraft limit exceeded." << endl;
        }
    }
    CurrentAccount operator+(SavingsAccount rhs) {
        CurrentAccount result = *this;
        result.deposit(rhs.getBalance());
        rhs.withdraw(rhs.getBalance());
        return result;
    }
};

std::ostream& operator<<(std::ostream& os, Account account) {
    account.displayDetails();
    return os;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    cout << savings;
    cout << current;

    savings.deposit(500);
    current.withdraw(1000);

    cout << "Account Details after deposit and withdrawal:" << endl;
    cout << savings;
    cout << current;

    // Transfer 300 from savings to current
    current = current + savings;

    cout << "Account Details after transfer:" << endl;
    cout << savings;
    cout << current;

    return 0;
}