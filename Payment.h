#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <iomanip>

using namespace std;

class Payment {
private:
    int paymentID;
    int orderID;
    int userID;
    double amount;
    char* method;
    char* status;

    void allocateAndCopy(char*& dest, const char* src) {
        delete[] dest;
        if (src) {
            dest = new char[strlen(src) + 1];
            strcpy(dest, src);
        } else {
            dest = nullptr;
        }
    }

    static int getNextPaymentID() {
        ifstream inFile("data/payments.txt");
        string line;
        int maxID = 0;
        int currentID = 0;

        if (!inFile) {
            return 5001;
        }

        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;

            if (getline(ss, segment, ',')) {
                try {
                     if (!segment.empty()) {
                        currentID = stoi(segment);
                        if (currentID > maxID) {
                            maxID = currentID;
                        }
                    }
                } catch (...) {
                }
            }
        }
        inFile.close();
        return (maxID == 0) ? 5001 : maxID + 1;
    }

public:
    Payment() : paymentID(0), orderID(0), userID(0), amount(0.0), method(nullptr), status(nullptr) {
         static bool seeded = false;
         if (!seeded) {
             srand(time(0));
             seeded = true;
         }
    }

    Payment(int pid, int oid, int uid, double amt, const char* mthd, const char* stat) :
        paymentID(pid), orderID(oid), userID(uid), amount(amt), method(nullptr), status(nullptr) {
        allocateAndCopy(method, mthd);
        allocateAndCopy(status, stat);
    }
    
    Payment(const Payment& other) : 
        paymentID(other.paymentID), orderID(other.orderID), userID(other.userID), amount(other.amount), method(nullptr), status(nullptr) {
        allocateAndCopy(method, other.method);
        allocateAndCopy(status, other.status);
    }

    Payment& operator=(const Payment& other) {
        if (this != &other) {
            paymentID = other.paymentID;
            orderID = other.orderID;
            userID = other.userID;
            amount = other.amount;
            allocateAndCopy(method, other.method);
            allocateAndCopy(status, other.status);
        }
        return *this;
    }

    ~Payment() {
        delete[] method;
        delete[] status;
    }

    int getPaymentID() const { return paymentID; }
    int getOrderID() const { return orderID; }
    int getUserID() const { return userID; }
    double getAmount() const { return amount; }
    const char* getMethod() const { return method; }
    const char* getStatus() const { return status; }
  bool simulatePayment(int oid, int uid, double amt) {
        if (oid <= 0 || uid <= 0 || amt <= 0) {
            cerr << "Error: Invalid user or amount details for payment simulation." << endl;
            return false;
        }
        
        this->orderID = oid;
        this->userID = uid;
        this->amount = amt;
        this->paymentID = getNextPaymentID();
        delete[] method; method = nullptr;
        delete[] status; status = nullptr;

        cout << "\n--- Payment Simulation --- Order ID: " << orderID << " Amount: $" 
             << fixed << setprecision(2) << amount << endl;

        int choice;
        string selectedMethod;
        while (true) {
            cout << "Select Payment Method:" << endl;
            cout << "1. VISA" << endl;
            cout << "2. Mastercard" << endl;
            cout << "3. JazzCash" << endl;
            cout << "4. EasyPaisa" << endl;
            cout << "5. PayPak" << endl;
            cout << "Enter choice (1-5): ";
            
            if (!(cin >> choice) || choice < 1 || choice > 5) {
                 cout << "Invalid choice. Please enter a number between 1 and 5." << endl;
                 cin.clear();
                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                switch (choice) {
                    case 1: selectedMethod = "VISA"; break;
                    case 2: selectedMethod = "Mastercard"; break;
                    case 3: selectedMethod = "JazzCash"; break;
                    case 4: selectedMethod = "EasyPaisa"; break;
                    case 5: selectedMethod = "PayPak"; break;
                }
                break;
            }
        }
        allocateAndCopy(this->method, selectedMethod.c_str());

        string cardNumber, expiryDate, cvv, holderName, accountNumber, pin;

        if (choice == 1 || choice == 2 || choice == 5) {
            cout << "Enter Card Number (simulated): ";
            getline(cin, cardNumber);
            cout << "Enter Expiry Date (MM/YY) (simulated): ";
            getline(cin, expiryDate);
            cout << "Enter CVV (simulated): ";
            getline(cin, cvv);
            cout << "Enter Card Holder Name (simulated): ";
            getline(cin, holderName);
        } else {
            cout << "Enter Account Number (simulated): ";
            getline(cin, accountNumber);
            cout << "Enter PIN (simulated): ";
            getline(cin, pin);
        }

        cout << "\nProcessing payment..." << endl;
        bool success = true;

        if (success) {
            cout << "Payment Successful!" << endl;
            allocateAndCopy(this->status, "Completed");
        } else {
            cout << "Payment Failed. Please try again or use a different method." << endl;
            allocateAndCopy(this->status, "Failed");
        }

        ofstream logFile("data/payments.txt", ios::app);
        if (!logFile) {
            cerr << "Error: Could not open payments.txt to log transaction." << endl;
             cerr << "Warning: Payment log failed. Status was: " << this->status << endl;
        } else {
            logFile << this->paymentID << ","
                    << this->orderID << ","
                    << this->userID << ","
                    << fixed << setprecision(2) << this->amount << ","
                    << this->method << ","
                    << this->status << endl;
            logFile.close();
        }

        return success;
    }

}; 