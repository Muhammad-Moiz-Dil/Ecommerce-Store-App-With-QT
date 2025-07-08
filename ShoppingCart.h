#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <limits>

#include "Product.h"
#include "User.h"

using namespace std;

class ShoppingCart {
private:
    int userID;
    char* cartFilename;

    void generateCartFilename(int uid) {
        string prefix = "data/cart/cart_";
        string suffix = ".txt";
        string id_str = to_string(uid);
        string fullPath = prefix + id_str + suffix;
        delete[] cartFilename;
        cartFilename = new char[fullPath.length() + 1];
        strcpy(cartFilename, fullPath.c_str());
    }

    void allocateAndCopy(char*& dest, const char* src) {
        delete[] dest;
        if (src) {
            dest = new char[strlen(src) + 1];
            strcpy(dest, src);
        } else {
            dest = nullptr;
        }
    }

public:
    ShoppingCart(int uid) : userID(uid), cartFilename(nullptr) {
        if (userID <= 0) {
             cerr << "Warning: ShoppingCart created with invalid userID: " << uid << endl;
             return;
        }
        generateCartFilename(userID);
        ofstream outFile(cartFilename, ios::app);
        if (!outFile) {
            cerr << "Error: Could not create or open cart file: " << cartFilename << endl;
        }
        outFile.close();
    }

    ShoppingCart(const ShoppingCart& other) : userID(other.userID), cartFilename(nullptr) {
        allocateAndCopy(cartFilename, other.cartFilename);
    }

    ShoppingCart& operator=(const ShoppingCart& other) {
        if (this != &other) {
            userID = other.userID;
            allocateAndCopy(cartFilename, other.cartFilename);
        }
        return *this;
    }

    ~ShoppingCart() {
        delete[] cartFilename;
    }

    bool addToCart(int productID, int quantity) {
        if (quantity <= 0) {
            cerr << "Error: Quantity must be positive." << endl;
            return false;
        }
         if (!cartFilename) {
             cerr << "Error: Cart is not associated with a user (invalid filename)." << endl;
             return false;
         }
        Product* product = Product::getProductByID(productID);
        if (!product) {
            cerr << "Error: Product with ID " << productID << " not found." << endl;
            return false;
        }
        int currentStock = product->getStock();
        delete product;
        product = nullptr;
        int quantityAlreadyInCart = 0;
        ifstream checkFile(cartFilename);
        if (checkFile) {
            string line;
            while(getline(checkFile, line)) {
                if (line.empty()) continue;
                stringstream ss(line);
                string segment;
                int currentProdID, currentQuantity;
                getline(ss, segment, ',');
                try { currentProdID = stoi(segment); } catch(...) { continue; }
                getline(ss, segment, ',');
                try { currentQuantity = stoi(segment); } catch(...) { continue; }
                if (currentProdID == productID) {
                    quantityAlreadyInCart = currentQuantity;
                    break;
                }
            }
            checkFile.close();
        }
        if (quantity + quantityAlreadyInCart > currentStock) {
             cerr << "Error: Not enough stock for Product ID " << productID 
                  << ". Available: " << currentStock 
                  << ", In Cart: " << quantityAlreadyInCart
                  << ", Requested to Add: " << quantity << endl;
            return false;
        }
        ifstream inFile(cartFilename);
        string tempFilename = string(cartFilename) + ".tmp";
        ofstream tempFile(tempFilename);
        if (!inFile || !tempFile) {
            cerr << "Error: Could not open cart files for update." << endl;
            inFile.close();
            tempFile.close();
            remove(tempFilename.c_str());
            return false;
        }
        string line;
        bool found = false;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            int currentProdID, currentQuantity;
            getline(ss, segment, ',');
             try { currentProdID = stoi(segment); } catch(...) { tempFile << line << endl; continue; }
            if (currentProdID == productID) {
                found = true;
                 getline(ss, segment, ',');
                 try { currentQuantity = stoi(segment); } catch(...) { currentQuantity = 0; }
                tempFile << productID << "," << (currentQuantity + quantity) << endl;
                cout << "Updated quantity for Product ID " << productID << " in cart." << endl;
            } else {
                tempFile << line << endl;
            }
        }
        if (!found) {
            tempFile << productID << "," << quantity << endl;
            cout << "Added Product ID " << productID << " (Qty: " << quantity << ") to cart." << endl;
        }
        inFile.close();
        tempFile.close();
        if (remove(cartFilename) != 0) {
            cerr << "Error: Could not delete original cart file: " << cartFilename << endl;
            remove(tempFilename.c_str());
            return false;
        }
        if (rename(tempFilename.c_str(), cartFilename) != 0) {
            cerr << "Error: Could not rename temp cart file." << endl;
            return false;
        }
        return true;
    }

    bool removeFromCart(int productID) {
         if (!cartFilename) {
             cerr << "Error: Cart is not associated with a user (invalid filename)." << endl;
             return false;
         }
        ifstream inFile(cartFilename);
        string tempFilename = string(cartFilename) + ".tmp";
        ofstream tempFile(tempFilename);
        if (!inFile || !tempFile) {
            cerr << "Error: Could not open cart files for removal." << endl;
             inFile.close();
             tempFile.close();
             remove(tempFilename.c_str());
            return false;
        }
        string line;
        bool found = false;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            int currentProdID;
            getline(ss, segment, ',');
            try { currentProdID = stoi(segment); } catch(...) { tempFile << line << endl; continue; }
            if (currentProdID == productID) {
                found = true;
                cout << "Removed Product ID " << productID << " from cart." << endl;
            } else {
                tempFile << line << endl;
            }
        }
        inFile.close();
        tempFile.close();
        if (!found) {
            cerr << "Error: Product ID " << productID << " not found in cart." << endl;
            remove(tempFilename.c_str());
            return false;
        }
        if (remove(cartFilename) != 0) {
            cerr << "Error: Could not delete original cart file: " << cartFilename << endl;
            remove(tempFilename.c_str());
            return false;
        }
        if (rename(tempFilename.c_str(), cartFilename) != 0) {
            cerr << "Error: Could not rename temp cart file." << endl;
            return false;
        }
        return true;
    }

    void viewCart() {
        if (!cartFilename) {
             cerr << "Error: Cart is not associated with a user (invalid filename)." << endl;
             return;
         }
        ifstream inFile(cartFilename);
        if (!inFile) {
            cerr << "Error: Could not open cart file: " << cartFilename << endl;
            return;
        }
        cout << "\n--- Shopping Cart for User ID: " << userID << " ---" << endl;
        string line;
        double total = 0.0;
        int itemCount = 0;
        cout << left << setw(8) << "ProdID" 
             << setw(25) << "Name" 
             << setw(8) << "Qty" 
             << setw(12) << "Unit Price" 
             << setw(12) << "Subtotal" << endl;
        cout << setfill('-') << setw(65) << "" << setfill(' ') << endl;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            int prodID, quantity;
            getline(ss, segment, ','); try { prodID = stoi(segment); } catch (...) { continue; }
            getline(ss, segment, ','); try { quantity = stoi(segment); } catch (...) { continue; }
            Product* product = Product::getProductByID(prodID);
            if (product) {
                itemCount++;
                double price = product->getPrice();
                double subtotal = price * quantity;
                total += subtotal;
                 cout << left << setw(8) << prodID
                      << setw(25) << (product->getName() ? product->getName() : "N/A")
                      << setw(8) << quantity
                      << "$" << fixed << setprecision(2) << setw(11) << price
                      << "$" << fixed << setprecision(2) << setw(11) << subtotal << endl;
                delete product;
            } else {
                cout << left << setw(8) << prodID
                     << setw(25) << "<Product details not found>" 
                     << setw(8) << quantity 
                     << setw(12) << "N/A" 
                     << setw(12) << "N/A" << endl;
            }
        }
        inFile.close();
        if (itemCount == 0) {
            cout << "Your shopping cart is empty." << endl;
        } else {
            cout << setfill('-') << setw(65) << "" << setfill(' ') << endl;
            cout << right << setw(53) << "Total: " 
                 << "$" << fixed << setprecision(2) << setw(11) << total << endl;
        }
        cout << "------------------------------------" << endl;
    }

    double calculateTotal() {
        if (!cartFilename) {
             cerr << "Error: Cart is not associated with a user (invalid filename)." << endl;
             return 0.0;
         }
        ifstream inFile(cartFilename);
        if (!inFile) {
            return 0.0;
        }
        string line;
        double total = 0.0;
        while (getline(inFile, line)) {
             if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            int prodID, quantity;
             getline(ss, segment, ','); try { prodID = stoi(segment); } catch (...) { continue; }
             getline(ss, segment, ','); try { quantity = stoi(segment); } catch (...) { continue; }
            Product* product = Product::getProductByID(prodID);
            if (product) {
                total += product->getPrice() * quantity;
                delete product;
            }
        }
        inFile.close();
        return total;
    }
    
    bool clearCart() {
         if (!cartFilename) {
             cerr << "Error: Cart is not associated with a user (invalid filename)." << endl;
             return false;
         }
         ofstream outFile(cartFilename, ios::trunc);
          if (!outFile) {
            cerr << "Error: Could not open cart file to clear: " << cartFilename << endl;
            return false;
        }
        outFile.close();
        cout << "Cart for user " << userID << " cleared." << endl;
        return true;
    }
    
    bool isEmpty() {
         if (!cartFilename) {
             return true;
         }
         ifstream inFile(cartFilename);
         if (!inFile) {
             return true;
         }
         string line;
         bool hasItems = false;
         while(getline(inFile, line)) {
             if (!line.empty()) {
                 if (line.find(',') != string::npos) {
                      hasItems = true;
                      break;
                 }
             }
         }
         inFile.close();
         return !hasItems;
    }

    const char* getCartFilename() const { return cartFilename; }

}; 