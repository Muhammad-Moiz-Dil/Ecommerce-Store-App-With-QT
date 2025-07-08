#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <iomanip>

#include "Product.h"
#include "ShoppingCart.h"

using namespace std;

class Wishlist {
private:
    int userID;
    char* wishlistFilename;

    void generateWishlistFilename(int uid) {
        string prefix = "data/wishlist/wishlist_";
        string suffix = ".txt";
        string id_str = to_string(uid);
        string fullPath = prefix + id_str + suffix;
        
        delete[] wishlistFilename;
        wishlistFilename = new char[fullPath.length() + 1];
        strcpy(wishlistFilename, fullPath.c_str());
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
    Wishlist(int uid) : userID(uid), wishlistFilename(nullptr) {
        if (userID <= 0) {
             cerr << "Warning: Wishlist created with invalid userID: " << uid << endl;
             return;
        }
        generateWishlistFilename(userID);
        ofstream outFile(wishlistFilename, ios::app);
        if (!outFile) {
            cerr << "Error: Could not create or open wishlist file: " << wishlistFilename << endl;
        }
        outFile.close();
    }

    Wishlist(const Wishlist& other) : userID(other.userID), wishlistFilename(nullptr) {
        allocateAndCopy(wishlistFilename, other.wishlistFilename);
    }

    Wishlist& operator=(const Wishlist& other) {
        if (this != &other) {
            userID = other.userID;
            allocateAndCopy(wishlistFilename, other.wishlistFilename);
        }
        return *this;
    }

    ~Wishlist() {
        delete[] wishlistFilename;
    }

    bool addToWishlist(int productID) {
         if (!wishlistFilename) {
             cerr << "Error: Wishlist is not associated with a user." << endl;
             return false;
         }
        Product* product = Product::getProductByID(productID);
        if (!product) {
            cerr << "Error: Product ID " << productID << " does not exist and cannot be added to wishlist." << endl;
            return false;
        }
        delete product;
        ifstream checkFile(wishlistFilename);
        bool alreadyExists = false;
        if (checkFile) {
            string line;
            int currentID;
            while (getline(checkFile, line)) {
                if (line.empty()) continue;
                try {
                    currentID = stoi(line);
                    if (currentID == productID) {
                        alreadyExists = true;
                        break;
                    }
                } catch (...) { }
            }
            checkFile.close();
        }
        if (alreadyExists) {
            cout << "Product ID " << productID << " is already in your wishlist." << endl;
            return true;
        }
        ofstream outFile(wishlistFilename, ios::app);
        if (!outFile) {
            cerr << "Error: Could not open wishlist file for writing: " << wishlistFilename << endl;
            return false;
        }
        outFile << productID << endl;
        outFile.close();
        cout << "Product ID " << productID << " added to your wishlist." << endl;
        return true;
    }

    bool removeFromWishlist(int productID) {
        if (!wishlistFilename) {
             cerr << "Error: Wishlist is not associated with a user." << endl;
             return false;
         }
        ifstream inFile(wishlistFilename);
        string tempFilename = string(wishlistFilename) + ".tmp";
        ofstream tempFile(tempFilename);
        if (!inFile || !tempFile) {
            cerr << "Error: Could not open wishlist files for removal." << endl;
             inFile.close(); tempFile.close(); remove(tempFilename.c_str());
            return false;
        }
        string line;
        bool found = false;
        int currentID;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            try {
                 currentID = stoi(line);
                 if (currentID == productID) {
                     found = true;
                     cout << "Removed Product ID " << productID << " from wishlist." << endl;
                 } else {
                     tempFile << line << endl;
                 }
            } catch (...) {
                tempFile << line << endl;
            }
        }
        inFile.close();
        tempFile.close();
        if (!found) {
            cerr << "Error: Product ID " << productID << " not found in wishlist." << endl;
            remove(tempFilename.c_str());
            return false;
        }
        if (remove(wishlistFilename) != 0 || rename(tempFilename.c_str(), wishlistFilename) != 0) {
            cerr << "Error: Failed to update wishlist file." << endl;
            return false;
        }
        return true;
    }

    void viewWishlist() {
         if (!wishlistFilename) {
             cerr << "Error: Wishlist is not associated with a user." << endl;
             return;
         }
        ifstream inFile(wishlistFilename);
        if (!inFile) {
            cerr << "Error: Could not open wishlist file: " << wishlistFilename << endl;
            return;
        }
        cout << "\n--- Wishlist for User ID: " << userID << " ---" << endl;
        string line;
        int itemCount = 0;
        cout << left << setw(8) << "ProdID" 
             << setw(30) << "Name" 
             << setw(15) << "Category" 
             << setw(12) << "Price" 
             << setw(8) << "Rating" << endl;
        cout << setfill('-') << setw(73) << "" << setfill(' ') << endl;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            int prodID;
            try {
                prodID = stoi(line);
            } catch (...) {
                cout << "<Invalid item entry in wishlist>" << endl;
                continue;
            }
            Product* product = Product::getProductByID(prodID);
            if (product) {
                itemCount++;
                 cout << left << setw(8) << prodID
                      << setw(30) << (product->getName() ? product->getName() : "N/A")
                      << setw(15) << (product->getCategory() ? product->getCategory() : "N/A")
                      << "$" << fixed << setprecision(2) << setw(11) << product->getPrice()
                      << fixed << setprecision(1) << setw(8) << product->getRating() << endl;
                delete product;
            } else {
                 cout << left << setw(8) << prodID
                      << setw(30) << "<Product details not found>" << endl;
            }
        }
        inFile.close();
        if (itemCount == 0) {
            cout << "Your wishlist is empty." << endl;
        }
        cout << "------------------------------------" << endl;
    }
    
    bool isEmpty() {
         if (!wishlistFilename) {
             return true;
         }
         ifstream inFile(wishlistFilename);
         if (!inFile) {
             return true;
         }
         string line;
         bool hasItems = false;
         while(getline(inFile, line)) {
             if (!line.empty()) {
                 try {
                     stoi(line);
                     hasItems = true;
                     break;
                 } catch(...) { }
             }
         }
         inFile.close();
         return !hasItems;
    }


};
