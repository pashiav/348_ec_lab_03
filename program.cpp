#include <iostream>
#include <iomanip>

class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    virtual void displayDetails() const {
        std::cout << "Account Details for " << getType() << " (ID: " << accountNumber << "):" << std::endl;
        std::cout << "   Holder: " << accountHolder << std::endl;
        std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
    }

    virtual void deposit(double amount) {
        balance += amount;
    }

    virtual void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
        } else {
            std::cout << "Insufficient funds!" << std::endl;
        }
    }

    virtual std::string getType() const {
        return "Generic Account";
    }

    double getBalance() const {
        return balance;
    }
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << (interestRate * 100) << "%" << std::endl;
    }

    void withdraw(double amount) override {
        const double minBalance = 100; 
        if (balance - amount >= minBalance) {
            balance -= amount;
        } else {
            std::cout << "Withdrawal not allowed. Minimum balance should be maintained." << std::endl;
        }
    }

    std::string getType() const override {
        return "Savings Account";
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
        : Account(number, holder, initialBalance), overdraftLimit(limit) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << std::endl;
    }

    void withdraw(double amount) override {
        if (balance - amount >= -overdraftLimit) {
            balance -= amount;
        } else {
            std::cout << "Withdrawal not allowed. Overdraft limit exceeded." << std::endl;
        }
    }

    std::string getType() const override {
        return "Current Account";
    }
};

CurrentAccount operator+(CurrentAccount& current, SavingsAccount& savings) {
    double transferAmount = 300;
    if (savings.getBalance() >= transferAmount) {
        savings.withdraw(transferAmount);
        current.deposit(transferAmount);
    } else {
        std::cout << "Transfer failed. Insufficient funds in the source account." << std::endl;
    }
    return current;
}

std::ostream& operator<<(std::ostream& os, const Account& account) {
    account.displayDetails();
    return os;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    std::cout << savings;
    std::cout << current;

    savings.deposit(500);
    current.withdraw(1000);

    std::cout << "Account Details after deposit and withdrawal:" << std::endl;
    std::cout << savings;
    std::cout << current;

    current = current + savings;

    std::cout << "Account Details after transfer:" << std::endl;
    std::cout << savings;
    std::cout << current;

    return 0;
}
