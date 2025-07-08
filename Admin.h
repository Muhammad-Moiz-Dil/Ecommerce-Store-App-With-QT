#pragma once

#include "User.h"    
#include "Product.h" 
#include <iostream>

using namespace std;

class Admin : public User {
public:
    Admin(int id, const char* uname, const char* pwd, const char* mail) :
        User(id, uname, pwd, mail, true) { 
            if (!getIsAdmin()) {
                 cerr << "Warning: Creating Admin object but isAdmin flag is false!" << endl;
            }
        }

    Admin() : User() {
                 }

    bool checkAdminStatus() const {
        return getIsAdmin(); 
    }
    
   
    void adminAddProduct() {
        cout << "\n[Admin Action] Adding a new product..." << endl;
        Product::addProduct(); 
    }

    void adminEditProduct(int productId, const Product& productData) {
        cout << "\n[Admin Action] Editing an existing product..." << endl;
        Product::editProduct(productId, productData); 
    }

    void adminRemoveProduct(int productId) {
        cout << "\n[Admin Action] Removing a product..." << endl;
        Product::removeProduct(productId); 
    }
    
    void viewInventory() {
         cout << "\n[Admin Action] Viewing Inventory..." << endl;
         int productCount = 0;
         Product* allProducts = Product::loadAllProducts(productCount);

         if (!allProducts || productCount == 0) {
             cout << "No products found in the inventory or error loading." << endl;
             delete[] allProducts; 
             return;
         }

         cout << "\n--- Current Inventory --- (" << productCount << " products)" << endl;
         cout << left << setw(8) << "ID" 
              << setw(25) << "Name" 
              << setw(15) << "Category" 
              << setw(10) << "Price" 
              << setw(8) << "Rating" 
              << setw(8) << "Stock" << endl;
         cout << setfill('-') << setw(74) << "" << setfill(' ') << endl; 

         for (int i = 0; i < productCount; ++i) {
             cout << left << setw(8) << allProducts[i].getProductID()
                  << setw(25) << (allProducts[i].getName() ? allProducts[i].getName() : "N/A")
                  << setw(15) << (allProducts[i].getCategory() ? allProducts[i].getCategory() : "N/A")
                  << "$" << fixed << setprecision(2) << setw(9) << allProducts[i].getPrice()
                  << fixed << setprecision(1) << setw(8) << allProducts[i].getRating()
                  << fixed << setprecision(0) << setw(8) << allProducts[i].getStock() << endl;
             if (allProducts[i].getStock() < 5 && allProducts[i].getStock() > 0) { 
                 cout << "  -> LOW STOCK!" << endl;
             } else if (allProducts[i].getStock() == 0) {
                 cout << "  -> OUT OF STOCK!" << endl;
             }
         }
         cout << setfill('-') << setw(74) << "" << setfill(' ') << endl;

         delete[] allProducts; 
    }

    void viewOrders() {
        cout << "\n[Admin Action] Viewing orders... (Not implemented yet)" << endl;
    }

    void updateOrderStatus() {
        cout << "\n[Admin Action] Updating order status... (Not implemented yet)" << endl;
    }
    
     static void viewAllUsers() {
        cout << "\n[Admin Action] Viewing all registered users..." << endl;
        ifstream inFile("data/users.txt");
        if (!inFile) {
            cerr << "Error: Could not open users.txt." << endl;
            return;
        }

        string line;
        cout << left << setw(8) << "User ID" << setw(20) << "Username" << setw(30) << "Email" << setw(10) << "Is Admin" << endl;
        cout << setfill('-') << setw(68) << "" << setfill(' ') << endl;

        int userCount = 0;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            string id_str, uname, pwd_ignored, email, isAdmin_str;
            
            getline(ss, id_str, ',');
            getline(ss, uname, ',');
            getline(ss, pwd_ignored, ',');
            getline(ss, email, ',');
            getline(ss, isAdmin_str, ',');

            cout << left << setw(8) << id_str 
                 << setw(20) << uname 
                 << setw(30) << email 
                 << setw(10) << (isAdmin_str == "1" ? "Yes" : "No") << endl;
            userCount++;
        }
        cout << setfill('-') << setw(68) << "" << setfill(' ') << endl;
        cout << "Total users found: " << userCount << endl;
        inFile.close();
    }

}; 