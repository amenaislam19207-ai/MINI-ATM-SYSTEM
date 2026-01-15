#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* =====================================================
   CONSTANT DEFINITIONS
   ===================================================== */
#define MAX_ATTEMPTS 3
#define ENCRYPT_KEY 11
#define DAILY_LIMIT 5000

/* =====================================================
   GLOBAL VARIABLES (SIMULATED SECURE STORAGE)
   ===================================================== */
int encryptedBalance;
int encryptedPIN;
int dailyWithdrawTotal;

/* =====================================================
   ENCRYPTION / DECRYPTION FUNCTIONS
   ===================================================== */
int encryptValue(int value) {
    return value + ENCRYPT_KEY;
}

int decryptValue(int value) {
    return value - ENCRYPT_KEY;
}

/* =====================================================
   RANDOM OTP GENERATION
   ===================================================== */
int generateOTP() {
    return rand() % 9000 + 1000;
}

/* =====================================================
   INITIALIZATION FUNCTIONS
   ===================================================== */
void initializeBalance() {
    encryptedBalance = encryptValue(1000);
}

void initializePIN() {
    encryptedPIN = encryptValue(1234);
}

void initializeWithdrawLimit() {
    dailyWithdrawTotal = 0;
}

void initializeAccount() {
    initializeBalance();
    initializePIN();
    initializeWithdrawLimit();
}

/* =====================================================
   DISPLAY FUNCTIONS
   ===================================================== */
void displayLine() {
    printf("---------------------------------\n");
}

void displayWelcome() {
    displayLine();
    printf("        MINI ATM SYSTEM\n");
    displayLine();
}

void displayExit() {
    displayLine();
    printf("Thank you for using the ATM.\n");
    displayLine();
}

void displayLocked() {
    displayLine();
    printf("Account locked due to failed attempts.\n");
    displayLine();
}

/* =====================================================
   PIN HANDLING FUNCTIONS
   ===================================================== */
int readPIN() {
    int pin;
    printf("Enter PIN: ");
    scanf("%d", &pin);
    return pin;
}

int isPINCorrect(int pin) {
    if (encryptValue(pin) == encryptedPIN) {
        return 1;
    }
    return 0;
}

int verifyPIN() {
    int attempts = 0;
    int pin;

    while (attempts < MAX_ATTEMPTS) {
        pin = readPIN();

        if (isPINCorrect(pin)) {
            return 1;
        } else {
            printf("Wrong PIN.\n");
            attempts++;
        }
    }
    return 0;
}

/* =====================================================
   OTP HANDLING FUNCTIONS
   ===================================================== */
int readOTP() {
    int otp;
    printf("Enter OTP: ");
    scanf("%d", &otp);
    return otp;
}

int verifyOTP() {
    int otp = generateOTP();
    int userOTP;

    printf("Your OTP is: %d\n", otp);
    userOTP = readOTP();

    if (otp == userOTP) {
        return 1;
    }
    return 0;
}

/* =====================================================
   BALANCE FUNCTIONS
   ===================================================== */
int getBalance() {
    return decryptValue(encryptedBalance);
}

void showBalance() {
    printf("Current Balance: %d\n", getBalance());
}

/* =====================================================
   DEPOSIT FUNCTIONS
   ===================================================== */
int isDepositValid(int amount) {
    if (amount > 0) {
        return 1;
    }
    return 0;
}

void updateBalanceAfterDeposit(int amount) {
    encryptedBalance = encryptValue(getBalance() + amount);
}

void depositMoney() {
    int amount;

    printf("Enter deposit amount: ");
    scanf("%d", &amount);

    if (isDepositValid(amount)) {
        updateBalanceAfterDeposit(amount);
        printf("Deposit successful.\n");
    } else {
        printf("Invalid deposit amount.\n");
    }
}

/* =====================================================
   WITHDRAW FUNCTIONS
   ===================================================== */
int isAmountAvailable(int amount) {
    if (amount <= getBalance()) {
        return 1;
    }
    return 0;
}

int isWithinDailyLimit(int amount) {
    if ((dailyWithdrawTotal + amount) <= DAILY_LIMIT) {
        return 1;
    }
    return 0;
}

void updateBalanceAfterWithdraw(int amount) {
    encryptedBalance = encryptValue(getBalance() - amount);
    dailyWithdrawTotal += amount;
}

void withdrawMoney() {
    int amount;

    printf("Enter withdrawal amount: ");
    scanf("%d", &amount);

    if (!isAmountAvailable(amount)) {
        printf("Insufficient balance.\n");
        return;
    }

    if (!isWithinDailyLimit(amount)) {
        printf("Daily withdrawal limit exceeded.\n");
        return;
    }

    updateBalanceAfterWithdraw(amount);
    printf("Withdrawal successful.\n");
}

/* =====================================================
   MENU DISPLAY FUNCTIONS
   ===================================================== */
void displayMenuHeader() {
    displayLine();
    printf("ATM MENU\n");
    displayLine();
}

void displayMenuOptions() {
    printf("1. Check Balance\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Exit\n");
}

void displayMenu() {
    displayMenuHeader();
    displayMenuOptions();
    displayLine();
}

/* =====================================================
   MENU PROCESSING FUNCTIONS
   ===================================================== */
int readMenuChoice() {
    int choice;
    printf("Enter choice: ");
    scanf("%d", &choice);
    return choice;
}

void processMenuChoice(int choice) {
    if (choice == 1) {
        showBalance();
    } 
    else if (choice == 2) {
        depositMoney();
    } 
    else if (choice == 3) {
        withdrawMoney();
    } 
    else if (choice == 4) {
        displayExit();
    } 
    else {
        printf("Invalid choice.\n");
    }
}

/* =====================================================
   MAIN ATM LOOP
   ===================================================== */
void startATM() {
    int choice;

    do {
        displayMenu();
        choice = readMenuChoice();
        processMenuChoice(choice);
    } while (choice != 4);
}

/* =====================================================
   AUTHENTICATION CONTROLLER
   ===================================================== */
int authenticateUser() {
    if (!verifyPIN()) {
        displayLocked();
        return 0;
    }

    if (!verifyOTP()) {
        printf("OTP verification failed.\n");
        return 0;
    }

    return 1;
}

/* =====================================================
   MAIN FUNCTION
   ===================================================== */
int main() {
    srand(time(NULL));

    initializeAccount();
    displayWelcome();

    if (authenticateUser()) {
        startATM();
    }

    return 0;
}
