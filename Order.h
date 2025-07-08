#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <iomanip>

#include "ShoppingCart.h"
#include "Product.h"

using namespace std;

class Order {
private:
    int orderID;
    int userID;
    char* orderItems;
    char* orderDate;
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

    static int getNextOrderID() {
        ifstream inFile("data/orders/orders.txt");
        string line;
        int maxID = 0;
        int currentID = 0;

        if (!inFile) {
            return 1001;
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
        return (maxID == 0) ? 1001 : maxID + 1;
    }

    static char* getCurrentDateTimeString() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char* dt = new char[20];
        strftime(dt, 20, "%Y-%m-%d %H:%M:%S", ltm);
        return dt;
    }

public:
    Order() : orderID(0), userID(0), orderItems(nullptr), orderDate(nullptr), status(nullptr) {}

    Order(int oid, int uid, const char* items, const char* date, const char* stat) :
        orderID(oid), userID(uid), orderItems(nullptr), orderDate(nullptr), status(nullptr) {
        allocateAndCopy(orderItems, items);
        allocateAndCopy(orderDate, date);
        allocateAndCopy(status, stat);
    }

    Order(const Order& other) :
        orderID(other.orderID), userID(other.userID), orderItems(nullptr), orderDate(nullptr), status(nullptr) {
        allocateAndCopy(orderItems, other.orderItems);
        allocateAndCopy(orderDate, other.orderDate);
        allocateAndCopy(status, other.status);
    }

    Order& operator=(const Order& other) {
        if (this != &other) {
            orderID = other.orderID;
            userID = other.userID;
            allocateAndCopy(orderItems, other.orderItems);
            allocateAndCopy(orderDate, other.orderDate);
            allocateAndCopy(status, other.status);
        }
        return *this;
    }

    ~Order() {
        delete[] orderItems;
        delete[] orderDate;
        delete[] status;
    }

    int getOrderID() const { return orderID; }
    int getUserID() const { return userID; }
    const char* getOrderItems() const { return orderItems; }
    const char* getOrderDate() const { return orderDate; }
    const char* getStatus() const { return status; }
    
    int placeOrder(ShoppingCart& cart) {
        if (cart.isEmpty()) {
            cerr << "Error: Cannot place order. Shopping cart is empty." << endl;
            return 0;
        }

        this->orderID = getNextOrderID();
        char* currentDateTime = getCurrentDateTimeString();
        allocateAndCopy(this->orderDate, currentDateTime);
        delete[] currentDateTime;

        if (this->userID <= 0) {
             cerr << "Error: Order cannot be placed without a valid User ID." << endl;
             return 0;
        }

        ifstream cartFile(cart.getCartFilename());
        
        if (!cartFile) {
            cerr << "Error: Could not open cart file: " << cart.getCartFilename() << endl;
            return 0;
        }

        string itemsString = "";
        string line;
        bool firstItem = true;

        int itemCount = 0;
        while(getline(cartFile, line)) { if (!line.empty()) itemCount++; }
        
        if (itemCount == 0) {
            cartFile.close();
            cerr << "Error: Cart file seems empty after counting." << endl;
            return 0;
        }

        int* productIDs = new int[itemCount];
        int* quantities = new int[itemCount];
        int currentItemIndex = 0;

        cartFile.clear();
        cartFile.seekg(0, ios::beg);

        while (getline(cartFile, line) && currentItemIndex < itemCount) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            int prodID, qty;

            try {
                getline(ss, segment, ','); prodID = stoi(segment);
                getline(ss, segment, ','); qty = stoi(segment);
            } catch (...) {
                 cerr << "Warning: Skipping invalid line in cart file: " << line << endl;
                 continue;
            }

            if (!firstItem) {
                itemsString += "|";
            }
            itemsString += to_string(prodID) + ":" + to_string(qty);
            firstItem = false;

            productIDs[currentItemIndex] = prodID;
            quantities[currentItemIndex] = qty;
            currentItemIndex++;
        }
        cartFile.close();
        allocateAndCopy(this->orderItems, itemsString.c_str());

        ifstream productInFile("data/products.txt");
        ofstream productTempFile("data/temp_products.txt");
        if (!productInFile || !productTempFile) {
            cerr << "Error: Could not open product files for stock update." << endl;
             productInFile.close(); productTempFile.close(); remove("data/temp_products.txt");
             delete[] productIDs; delete[] quantities;
            return 0;
        }
        
        bool stockUpdateSuccessful = true;
        while(getline(productInFile, line)) {
             if (line.empty()) { productTempFile << endl; continue; }
             stringstream ss(line);
             string segment;
             int currentProdID;
             getline(ss, segment, ',');
             try { currentProdID = stoi(segment); } catch (...) { productTempFile << line << endl; continue; }

             bool matched = false;
             for(int i=0; i < currentItemIndex; ++i) {
                 if (currentProdID == productIDs[i]) {
                     string name_str, cat_str, price_str, rating_str, stock_str;
                     int oldStock;
                     getline(ss, name_str, ',');
                     getline(ss, cat_str, ',');
                     getline(ss, price_str, ',');
                     getline(ss, rating_str, ',');
                     getline(ss, stock_str, ',');
                     try { oldStock = stoi(stock_str); } catch(...) { oldStock = 0; }

                     int newStock = oldStock - quantities[i];
                     if (newStock < 0) {
                         cerr << "Critical Error: Stock became negative during order placement for Product ID: " << currentProdID << ". Order cancelled." << endl;
                         stockUpdateSuccessful = false;
                         break;
                     }
                     
                     productTempFile << currentProdID << "," << name_str << "," << cat_str << "," 
                                     << price_str << "," << rating_str << "," << newStock << endl;
                     matched = true;
                     break;
                 }
             }
             
             if (!stockUpdateSuccessful) break;

             if (!matched) {
                 productTempFile << line << endl;
             }
        }
        productInFile.close();
        productTempFile.close();
        delete[] productIDs;
        delete[] quantities;

        if (!stockUpdateSuccessful) {
            remove("data/temp_products.txt");
             delete[] this->orderItems; this->orderItems = nullptr;
             delete[] this->orderDate; this->orderDate = nullptr;
             this->orderID = 0;
            return 0;
        }

        if (remove("data/products.txt") != 0 || rename("data/temp_products.txt", "data/products.txt") != 0) {
            cerr << "Error: Failed to update products.txt with new stock levels. Order placement failed." << endl;
            delete[] this->orderItems; this->orderItems = nullptr;
             delete[] this->orderItems; this->orderItems = nullptr;
             delete[] this->orderDate; this->orderDate = nullptr;
             this->orderID = 0;
            return 0;
        }

        ofstream orderFile("data/orders/orders.txt", ios::app);
        if (!orderFile) {
            cerr << "Error: Could not open orders.txt to save order." << endl;
            delete[] this->orderItems; this->orderItems = nullptr;
             delete[] this->orderDate; this->orderDate = nullptr;
             this->orderID = 0;
            return 0;
        }
        allocateAndCopy(this->status, "Complete");
        
        orderFile << this->orderID << "," 
                  << this->userID << "," 
                  << this->orderItems << "," 
                  << this->orderDate << "," 
                  << this->status << endl;
        orderFile.close();
        
        if (!cart.clearCart()) {
            cerr << "Warning: Order placed (ID: " << this->orderID << ") but failed to clear the shopping cart." << endl;
        }

        cout << "Order placed successfully! Order ID: " << this->orderID << endl;
        return this->orderID;
    }

    static void trackOrder(int orderIDToTrack) {
        ifstream inFile("data/orders/orders.txt");
        if (!inFile) {
            cerr << "Error: Could not open orders file to track order." << endl;
            return;
        }

        cout << "\n--- Tracking Order ID: " << orderIDToTrack << " ---" << endl;
        string line;
        bool found = false;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            int currentOrderID;

            getline(ss, segment, ',');
            try { currentOrderID = stoi(segment); } catch (...) { continue; }

            if (currentOrderID == orderIDToTrack) {
                found = true;
                string uid_str, items_str, date_str, status_str;
                getline(ss, uid_str, ',');
                getline(ss, items_str, ',');
                getline(ss, date_str, ',');
                getline(ss, status_str, ',');

                cout << "Order ID:    " << currentOrderID << endl;
                cout << "User ID:     " << uid_str << endl;
                cout << "Order Date:  " << date_str << endl;
                cout << "Status:      " << status_str << endl;
                cout << "Items:       " << endl;

                stringstream item_ss(items_str);
                string item_segment;
                cout << left << "  " << setw(8) << "ProdID" << setw(8) << "Qty" << setw(25) << "Name" << endl;
                 cout << "  " << setfill('-') << setw(41) << "" << setfill(' ') << endl;

                while (getline(item_ss, item_segment, '|')) {
                    stringstream pair_ss(item_segment);
                    string id_part, qty_part;
                    int itemID, itemQty;
                    getline(pair_ss, id_part, ':');
                    getline(pair_ss, qty_part, ':');
                    try {
                         itemID = stoi(id_part);
                         itemQty = stoi(qty_part);
                         Product* p = Product::getProductByID(itemID);
                         cout << left << "  " << setw(8) << itemID 
                              << setw(8) << itemQty 
                              << setw(25) << (p ? (p->getName() ? p->getName() : "N/A") : "<Details N/A>") << endl;
                          delete p;
                    } catch (...) {
                        cout << "  <Error parsing item: " << item_segment << ">" << endl;
                    }
                }
                break;
            }
        }
        inFile.close();

        if (!found) {
            cout << "Order ID " << orderIDToTrack << " not found." << endl;
        }
        cout << "------------------------------------" << endl;
    }

    static bool updateStatus(int orderIDToUpdate, const char* newStatus) {
         if (!newStatus || strlen(newStatus) == 0) {
             cerr << "Error: New status cannot be empty." << endl;
             return false;
         }

        cout << "[Debug] updateStatus called for Order ID: " << orderIDToUpdate << " with Status: " << newStatus << endl;

        ifstream inFile("data/orders/orders.txt");
        string tempFilenameStr = "data/orders/temp_orders.txt";
        ofstream tempFile(tempFilenameStr);
        if (!inFile || !tempFile) {
            cerr << "Error: Could not open order files for status update." << endl;
             inFile.close(); tempFile.close(); remove(tempFilenameStr.c_str());
            return false;
        }

        string line;
        bool found = false;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            int currentOrderID;
            
            streampos lineStartPos = ss.tellg();
            getline(ss, segment, ',');
            try { currentOrderID = stoi(segment); } catch (...) { tempFile << line << endl; continue; }

            if (currentOrderID == orderIDToUpdate) {
                found = true;
                cout << "[Debug] Found matching Order ID: " << currentOrderID << endl;
                 string uid_str, items_str, date_str, old_status_ignored;
                 getline(ss, uid_str, ',');
                 getline(ss, items_str, ',');
                 getline(ss, date_str, ',');

                 string updatedLine = to_string(currentOrderID) + "," + uid_str + "," + items_str + "," + date_str + "," + newStatus;
                 tempFile << updatedLine << endl;
                 cout << "[Debug] Writing updated line to temp file: " << updatedLine << endl;
            } else {
                tempFile << line << endl;
            }
        }
        inFile.close();
        tempFile.close();

        if (!found) {
            cerr << "Error: Order ID " << orderIDToUpdate << " not found for status update." << endl;
            remove(tempFilenameStr.c_str());
            return false;
        }

        cout << "[Debug] Attempting to replace original file..." << endl;
        if (remove("data/orders/orders.txt") != 0) {
             cerr << "Error: Could not delete original orders file." << endl;
             remove(tempFilenameStr.c_str());
             return false;
        }
        if (rename(tempFilenameStr.c_str(), "data/orders/orders.txt") != 0) {
            cerr << "Error: Could not rename temp file to orders.txt." << endl;
            return false;
        }

        cout << "[Debug] File replaced successfully." << endl;
        return true;
    }
    
    static void viewAllOrders() {
         ifstream inFile("data/orders/orders.txt");
        if (!inFile) {
            cerr << "Error: Could not open orders file." << endl;
            return;
        }

        cout << "\n--- All Orders ---" << endl;
        string line;
        int orderCount = 0;

        cout << left << setw(8) << "Order ID" 
             << setw(8) << "User ID" 
             << setw(20) << "Date" 
             << setw(40) << "Items (Summary)" 
             << setw(15) << "Status" << endl;
        cout << setfill('-') << setw(91) << "" << setfill(' ') << endl;

        while (getline(inFile, line)) {
            if (line.empty()) continue;
            orderCount++;
            stringstream ss(line);
            string oid_str, uid_str, items_str, date_str, status_str;
            
            getline(ss, oid_str, ',');
            getline(ss, uid_str, ',');
            getline(ss, items_str, ',');
            getline(ss, date_str, ',');
            getline(ss, status_str, ',');
            
            string items_summary = items_str.substr(0, 35) + (items_str.length() > 35 ? "..." : "");

            cout << left << setw(8) << oid_str 
                 << setw(8) << uid_str 
                 << setw(20) << date_str 
                 << setw(40) << items_summary
                 << setw(15) << status_str << endl;
        }
        inFile.close();
        
        if (orderCount == 0) {
            cout << "No orders found." << endl;
        } else {
            cout << setfill('-') << setw(91) << "" << setfill(' ') << endl;
        }
        cout << "------------------------------------" << endl;
    }

    static void viewOrdersForUser(int userIDToView) {
         ifstream inFile("data/orders/orders.txt");
        if (!inFile) {
            cerr << "Error: Could not open orders file." << endl;
            return;
        }

        cout << "\n--- Your Order History (User ID: " << userIDToView << ") ---" << endl;
        string line;
        int orderCount = 0;
        bool foundOrders = false;

        cout << left << setw(8) << "Order ID" 
             << setw(20) << "Date" 
             << setw(40) << "Items (Summary)" 
             << setw(15) << "Status" << endl;
        cout << setfill('-') << setw(83) << "" << setfill(' ') << endl;

        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string oid_str, uid_str, items_str, date_str, status_str;
            int currentUserID;
            
            getline(ss, oid_str, ',');
            getline(ss, uid_str, ',');
            getline(ss, items_str, ',');
            getline(ss, date_str, ',');
            getline(ss, status_str, ',');

            try {
                 currentUserID = stoi(uid_str);
                 if (currentUserID == userIDToView) {
                     foundOrders = true;
                     orderCount++;
                     string items_summary = items_str.substr(0, 35) + (items_str.length() > 35 ? "..." : "");

                     cout << left << setw(8) << oid_str 
                          << setw(20) << date_str 
                          << setw(40) << items_summary
                          << setw(15) << status_str << endl;
                 }
            } catch (...) {
                continue;
            }
        }
        inFile.close();
        
        if (!foundOrders) {
            cout << "No orders found for your account." << endl;
        } else {
            cout << setfill('-') << setw(83) << "" << setfill(' ') << endl;
        }
        cout << "------------------------------------" << endl;
    }

        static int* getProductIDsForOrder(int orderID, int& count) {
        count = 0;
        ifstream file("data/orders/orders.txt");
        if (!file.is_open()) {
            cerr << "Error: Could not open orders file for reading." << endl;
            return nullptr;
        }

        string line;
        string productIDsStr;
        bool orderFound = false;

        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            int currentOrderID = -1;

            if (getline(ss, segment, ',')) {
                 try {
                    currentOrderID = stoi(segment);
                } catch (const invalid_argument& e) {
                     cerr << "Warning: Could not parse order ID in getProductIDsForOrder: " << segment << endl;
                     continue;
                 }
            } else continue;

            if (currentOrderID == orderID) {
                if (!getline(ss, segment, ',')) continue;

                if (getline(ss, productIDsStr, ',')) {
                    orderFound = true;
                    break;
                }
                continue;
            }
        }
        file.close();

        if (!orderFound || productIDsStr.empty()) {
            return nullptr;
        }

        stringstream count_ss(productIDsStr);
        string itemSegmentForCount;
        int tempCount = 0;
        while (getline(count_ss, itemSegmentForCount, '|')) {
             if (!itemSegmentForCount.empty()) {
                 tempCount++;
             }
        }

        if (tempCount == 0) {
            return nullptr;
        }

       
        int* productIDs = new int[tempCount];
        stringstream parse_ss(productIDsStr);
        string itemSegmentForParse;
        int index = 0;
        while (index < tempCount && getline(parse_ss, itemSegmentForParse, '|')) {
             if (!itemSegmentForParse.empty()) {
                 stringstream pair_ss(itemSegmentForParse);
                 string idPart;
                 if (getline(pair_ss, idPart, ':')) {
                    try {
                        productIDs[index++] = stoi(idPart);
                    } catch (const invalid_argument& e) {
                        cerr << "Warning: Could not parse product ID from segment: " << idPart << " in item " << itemSegmentForParse << endl;
                    } catch (const out_of_range& e) {
                         cerr << "Warning: Product ID out of range from segment: " << idPart << " in item " << itemSegmentForParse << endl;
                    }
                 } else {
                      cerr << "Warning: Malformed item segment (missing ':'): " << itemSegmentForParse << endl;
                 }
             }
        }

         count = index;
         if (count == 0) {
            delete[] productIDs;
            return nullptr;
         }

         return productIDs;
    }

}; 