#pragma once

#include <iostream>
#include <fstream>
#include <string>           
#include <cstring>   
#include <sstream>   
#include <iomanip>   
#include <limits>    

using namespace std;

class Product {
private:
    int productID;
    char* name;
    char* category;
    char* description;
    double price;
    double rating; 
    int stock;

    void allocateAndCopy(char*& dest, const char* src) {
        delete[] dest; 
        if (src) {
            dest = new char[strlen(src) + 1];
            strcpy(dest, src);
        } else {
            dest = nullptr;
        }
    }

    static int getNextProductID() {
        ifstream inFile("data/products.txt");
        string line;
        int maxID = 0;
        int currentID = 0;

        if (!inFile) {
            return 101; 
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
                } catch (const std::invalid_argument& ia) {
                    cerr << "Warning: Invalid Product ID format found in products.txt: " << segment << endl;
                } catch (const std::out_of_range& oor) {
                    cerr << "Warning: Product ID out of range in products.txt: " << segment << endl;
                }
            }
        }
        inFile.close();
        return (maxID < 100) ? 101 : maxID + 1;
    }

    static void displayProductList(Product* products, int count, const string& title) {
        cout << "\n--- " << title << " --- (" << count << " items found)" << endl;
        if (count == 0) {
            cout << "No products match your criteria." << endl;
            cout << "------------------------------------" << endl;
            return;
        }
        
        cout << left << setw(8) << "ID" 
             << setw(25) << "Name" 
             << setw(15) << "Category" 
             << setw(10) << "Price" 
             << setw(8) << "Rating" 
             << setw(8) << "Stock" << endl;
        cout << setfill('-') << setw(74) << "" << setfill(' ') << endl;

        for (int i = 0; i < count; ++i) {
             cout << left << setw(8) << products[i].getProductID()
                  << setw(25) << (products[i].getName() ? products[i].getName() : "N/A")
                  << setw(15) << (products[i].getCategory() ? products[i].getCategory() : "N/A")
                  << "$" << fixed << setprecision(2) << setw(9) << products[i].getPrice()
                  << fixed << setprecision(1) << setw(8) << products[i].getRating()
                  << fixed << setprecision(0) << setw(8) << products[i].getStock() << endl;
        }
        cout << setfill('-') << setw(74) << "" << setfill(' ') << endl;
    }

public:
    Product() : productID(0), name(nullptr), category(nullptr), description(nullptr), price(0.0), rating(0.0), stock(0) {}

    Product(int id, const char* n, const char* cat, const char* desc, double p, double r, int s) :
        productID(id), name(nullptr), category(nullptr), description(nullptr), price(p), rating(r), stock(s) {
        allocateAndCopy(name, n);
        allocateAndCopy(category, cat);
        allocateAndCopy(description, desc);
    }

    Product(int id, const char* n, const char* cat, double p, double r, int s) :
        productID(id), name(nullptr), category(nullptr), description(nullptr), price(p), rating(r), stock(s) {
        allocateAndCopy(name, n);
        allocateAndCopy(category, cat);
    }

    Product(const Product& other) :
        productID(other.productID), name(nullptr), category(nullptr), description(nullptr),
        price(other.price), rating(other.rating), stock(other.stock) {
        allocateAndCopy(name, other.name);
        allocateAndCopy(category, other.category);
        allocateAndCopy(description, other.description);
    }

    Product& operator=(const Product& other) {
        if (this != &other) {
            productID = other.productID;
            price = other.price;
            rating = other.rating;
            stock = other.stock;
            allocateAndCopy(name, other.name);
            allocateAndCopy(category, other.category);
            allocateAndCopy(description, other.description);
        }
        return *this;
    }

    ~Product() {
        delete[] name;
        delete[] category;
        delete[] description;
    }

    int getProductID() const { return productID; }
    const char* getName() const { return name; }
    const char* getCategory() const { return category; }
    const char* getDescription() const { return description; }
    double getPrice() const { return price; }
    double getRating() const { return rating; }
    int getStock() const { return stock; }

    void setName(const char* n) { allocateAndCopy(name, n); }
    void setCategory(const char* cat) { allocateAndCopy(category, cat); }
    void setDescription(const char* desc) { allocateAndCopy(description, desc); }
    void setPrice(double p) { price = p; }
    void setRating(double r) { rating = r; }
    void setStock(int s) { stock = s; }

    static Product* getProductByID(int productID) {
        ifstream inFile("data/products.txt");
        if (!inFile) {
            return nullptr;
        }

        string line;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            int currentID;

            getline(ss, segment, ','); 
            try {
                 if (segment.empty()) continue; 
                currentID = stoi(segment);
                if (currentID == productID) {
                    string name_str, category_str, price_str, rating_str, stock_str;
                    double price_val, rating_val;
                    int stock_val;

                    getline(ss, name_str, ',');
                    getline(ss, category_str, ',');
                    getline(ss, price_str, ',');
                    getline(ss, rating_str, ',');
                    getline(ss, stock_str);

                    try {
                        price_val = stod(price_str);
                        rating_val = stod(rating_str);
                        stock_val = stoi(stock_str);
                    } catch (...) {
                         cerr << "Warning: Found Product ID " << productID << " but failed to parse details." << endl;
                         inFile.close();
                         return nullptr; 
                    }
                    
                    inFile.close();
                    return new Product(currentID, name_str.c_str(), category_str.c_str(), price_val, rating_val, stock_val);
                }
            } catch (...) {
                continue;
            }
        }
        
        inFile.close();
        return nullptr;
    }

    void displayProduct() const {
        if (!name) {
            cout << "Product data not loaded." << endl;
            return;
        }
        cout << "------------------------------------" << endl;
        cout << "ID:       " << productID << endl;
        cout << "Name:     " << name << endl;
        cout << "Category: " << category << endl;
        if (description) {
            cout << "Desc:     " << description << endl;
        }
        cout << fixed << setprecision(2);
        cout << "Price:    $" << price << endl;
        cout << fixed << setprecision(1);
        cout << "Rating:   " << rating << " / 5.0" << endl;
        cout << fixed << setprecision(0);
        cout << "Stock:    " << stock << endl;
        cout << "------------------------------------" << endl;
    }

    static bool addProduct();
    static Product* loadAllProducts(int& outCount);
    static void searchByName(const string& query) {
        if (query.empty()) {
            cout << "Search query cannot be empty." << endl;
             loadAllProductsAndDisplay("All Products (Empty Search)");
            return;
        }

        string lowerQuery = query;
        for (int i = 0; i < lowerQuery.length(); ++i) {
             lowerQuery[i] = tolower(lowerQuery[i]);
        }
        
        ifstream inFile("data/products.txt");
        if (!inFile) {
            cerr << "Error: Cannot open products.txt for searching." << endl;
            return;
        }
        
        string line;
        int matchCount = 0;
        streampos initialPos = inFile.tellg();

        while (getline(inFile, line)) {
             if (line.empty()) continue;
             stringstream ss(line);
             string segment, name_str;
             getline(ss, segment, ',');
             getline(ss, name_str, ',');

             string lowerName = name_str;
             for (int i = 0; i < lowerName.length(); ++i) {
                  lowerName[i] = tolower(lowerName[i]);
             }

             if (lowerName.find(lowerQuery) != string::npos) {
                 matchCount++;
             }
        }

        if (matchCount == 0) {
             inFile.close();
             displayProductList(nullptr, 0, "Search Results for \"" + query + "\"");
             return;
        }

        Product* matchedProducts = new Product[matchCount];
        inFile.clear();
        inFile.seekg(initialPos);
        int currentIndex = 0;

        while (getline(inFile, line) && currentIndex < matchCount) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment, name_str, cat_str, price_str, rating_str, stock_str;
            int id, stock_val;
            double price_val, rating_val;

            getline(ss, segment, ','); try { id = stoi(segment); } catch(...) { continue; }
            getline(ss, name_str, ','); 
            getline(ss, cat_str, ',');
            getline(ss, price_str, ','); try { price_val = stod(price_str); } catch(...) { continue; }
            getline(ss, rating_str, ','); try { rating_val = stod(rating_str); } catch(...) { continue; }
            getline(ss, stock_str); try { stock_val = stoi(stock_str); } catch(...) { continue; }

            string lowerName = name_str;
             for (int i = 0; i < lowerName.length(); ++i) {
                  lowerName[i] = tolower(lowerName[i]);
             }

            if (lowerName.find(lowerQuery) != string::npos) {
                matchedProducts[currentIndex] = Product(id, name_str.c_str(), cat_str.c_str(), price_val, rating_val, stock_val);
                currentIndex++;
            }
        }
        inFile.close();

        displayProductList(matchedProducts, currentIndex, "Search Results for \"" + query + "\"");
        delete[] matchedProducts;
    }

    static void filterByCategory(const string& categoryQuery) {
         if (categoryQuery.empty()) {
            cout << "Category filter cannot be empty." << endl;
             loadAllProductsAndDisplay("All Products (Empty Filter)");
            return;
        }
        string lowerQuery = categoryQuery;
        for (int i = 0; i < lowerQuery.length(); ++i) { lowerQuery[i] = tolower(lowerQuery[i]); }

        ifstream inFile("data/products.txt");
        if (!inFile) { cerr << "Error: Cannot open products.txt." << endl; return; }
        
        string line;
        int matchCount = 0;
        streampos initialPos = inFile.tellg();

        while (getline(inFile, line)) {
             if (line.empty()) continue;
             stringstream ss(line);
             string segment, cat_str;
             getline(ss, segment, ',');
             getline(ss, segment, ',');
             getline(ss, cat_str, ',');

             string lowerCat = cat_str;
             for (int i = 0; i < lowerCat.length(); ++i) { lowerCat[i] = tolower(lowerCat[i]); }

             if (lowerCat == lowerQuery) {
                 matchCount++;
             }
        }

        if (matchCount == 0) {
             inFile.close();
             displayProductList(nullptr, 0, "Filter Results for Category: " + categoryQuery);
             return;
        }

        Product* matchedProducts = new Product[matchCount];
        inFile.clear();
        inFile.seekg(initialPos);
        int currentIndex = 0;

        while (getline(inFile, line) && currentIndex < matchCount) {
             if (line.empty()) continue;
            stringstream ss(line);
            string segment, name_str, cat_str, price_str, rating_str, stock_str;
            int id, stock_val;
            double price_val, rating_val;

            getline(ss, segment, ','); try { id = stoi(segment); } catch(...) { continue; }
            getline(ss, name_str, ','); 
            getline(ss, cat_str, ',');
            getline(ss, price_str, ','); try { price_val = stod(price_str); } catch(...) { continue; }
            getline(ss, rating_str, ','); try { rating_val = stod(rating_str); } catch(...) { continue; }
            getline(ss, stock_str); try { stock_val = stoi(stock_str); } catch(...) { continue; }

            string lowerCat = cat_str;
             for (int i = 0; i < lowerCat.length(); ++i) { lowerCat[i] = tolower(lowerCat[i]); }

            if (lowerCat == lowerQuery) {
                matchedProducts[currentIndex] = Product(id, name_str.c_str(), cat_str.c_str(), price_val, rating_val, stock_val);
                currentIndex++;
            }
        }
        inFile.close();

        displayProductList(matchedProducts, currentIndex, "Filter Results for Category: " + categoryQuery);
        delete[] matchedProducts;
    }

    static void filterByPriceRange(double minPrice, double maxPrice) {
         if (minPrice < 0 || maxPrice < 0 || minPrice > maxPrice) {
             cout << "Invalid price range specified." << endl; return;
         }
         ifstream inFile("data/products.txt");
         if (!inFile) { cerr << "Error: Cannot open products.txt." << endl; return; }
        
        string line;
        int matchCount = 0;
        streampos initialPos = inFile.tellg();

        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment, price_str;
             getline(ss, segment, ',');
             getline(ss, segment, ',');
             getline(ss, segment, ',');
             getline(ss, price_str, ',');
             try {
                 double currentPrice = stod(price_str);
                 if (currentPrice >= minPrice && currentPrice <= maxPrice) {
                     matchCount++;
                 }
             } catch(...) { continue; }
        }
        
        if (matchCount == 0) {
             inFile.close();
             string title = "Filter Results for Price: $" + to_string(minPrice) + " - $" + to_string(maxPrice);
             displayProductList(nullptr, 0, title);
             return;
        }

        Product* matchedProducts = new Product[matchCount];
        inFile.clear();
        inFile.seekg(initialPos);
        int currentIndex = 0;

        while (getline(inFile, line) && currentIndex < matchCount) {
             if (line.empty()) continue;
            stringstream ss(line);
            string segment, name_str, cat_str, price_str, rating_str, stock_str;
            int id, stock_val;
            double price_val, rating_val;

            getline(ss, segment, ','); try { id = stoi(segment); } catch(...) { continue; }
            getline(ss, name_str, ','); 
            getline(ss, cat_str, ',');
            getline(ss, price_str, ','); try { price_val = stod(price_str); } catch(...) { continue; }
            getline(ss, rating_str, ','); try { rating_val = stod(rating_str); } catch(...) { continue; }
            getline(ss, stock_str); try { stock_val = stoi(stock_str); } catch(...) { continue; }

            if (price_val >= minPrice && price_val <= maxPrice) {
                 matchedProducts[currentIndex] = Product(id, name_str.c_str(), cat_str.c_str(), price_val, rating_val, stock_val);
                 currentIndex++;
             }
        }
        inFile.close();
        string title = "Filter Results for Price: $" + to_string(minPrice) + " - $" + to_string(maxPrice);
        displayProductList(matchedProducts, currentIndex, title);
        delete[] matchedProducts;
    }

    static void filterByRating(double minRating) {
         if (minRating < 0.0 || minRating > 5.0) {
             cout << "Invalid minimum rating specified (must be 0.0-5.0)." << endl; return;
         }
         ifstream inFile("data/products.txt");
         if (!inFile) { cerr << "Error: Cannot open products.txt." << endl; return; }
        
        string line;
        int matchCount = 0;
        streampos initialPos = inFile.tellg();

        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment, rating_str;
             getline(ss, segment, ',');
             getline(ss, segment, ',');
             getline(ss, segment, ',');
             getline(ss, segment, ',');
             getline(ss, rating_str, ',');
             try {
                 double currentRating = stod(rating_str);
                 if (currentRating >= minRating) {
                     matchCount++;
                 }
             } catch(...) { continue; }
        }
        
        if (matchCount == 0) {
             inFile.close();
             string title = "Filter Results for Rating >= " + to_string(minRating);
             displayProductList(nullptr, 0, title);
             return;
        }

        Product* matchedProducts = new Product[matchCount];
        inFile.clear();
        inFile.seekg(initialPos);
        int currentIndex = 0;

         while (getline(inFile, line) && currentIndex < matchCount) {
             if (line.empty()) continue;
            stringstream ss(line);
            string segment, name_str, cat_str, price_str, rating_str, stock_str;
            int id, stock_val;
            double price_val, rating_val;

            getline(ss, segment, ','); try { id = stoi(segment); } catch(...) { continue; }
            getline(ss, name_str, ','); 
            getline(ss, cat_str, ',');
            getline(ss, price_str, ','); try { price_val = stod(price_str); } catch(...) { continue; }
            getline(ss, rating_str, ','); try { rating_val = stod(rating_str); } catch(...) { continue; }
            getline(ss, stock_str); try { stock_val = stoi(stock_str); } catch(...) { continue; }

            if (rating_val >= minRating) {
                 matchedProducts[currentIndex] = Product(id, name_str.c_str(), cat_str.c_str(), price_val, rating_val, stock_val);
                 currentIndex++;
             }
        }
        inFile.close();
        string title = "Filter Results for Rating >= " + to_string(minRating);
        displayProductList(matchedProducts, currentIndex, title);
        delete[] matchedProducts;
    }

    static void loadAllProductsAndDisplay(const string& title = "All Products") {
         int count = 0;
         Product* all = loadAllProducts(count);
         displayProductList(all, count, title);
         delete[] all;
    }

    static bool addProduct(const Product& productData);
    static bool editProduct(int productId, const Product& productData);
    static bool removeProduct(int productId);

};

inline bool Product::addProduct(const Product& productData) {
    int newID = getNextProductID();
    
    std::ofstream outFile("data/products.txt", std::ios::app);
    if (!outFile) {
        std::cerr << "Error: Could not open products.txt for writing in Product::addProduct." << std::endl;
        return false;
    }
    
    outFile << newID << ","
            << (productData.getName() ? productData.getName() : "") << ","
            << (productData.getCategory() ? productData.getCategory() : "") << ","
            << std::fixed << std::setprecision(2) << productData.getPrice() << ","
            << std::fixed << std::setprecision(1) << 0.0 << ","
            << productData.getStock() << std::endl;
    
    outFile.close();
    std::cout << "Product '" << (productData.getName() ? productData.getName() : "N/A") 
              << "' added successfully with ID: " << newID << std::endl;
    return true;
}

inline bool Product::editProduct(int productId, const Product& productData) {
    std::ifstream inFile("data/products.txt");
    if (!inFile) {
        std::cerr << "Error: Could not open products.txt for reading." << std::endl;
        return false;
    }

    std::ofstream tempFile("data/products_temp.txt");
    if (!tempFile) {
        std::cerr << "Error: Could not create temporary file for product update." << std::endl;
        inFile.close();
        return false;
    }

    std::string line;
    bool productFound = false;
    
    while (std::getline(inFile, line)) {
        if (line.empty()) {
            tempFile << std::endl;
            continue;
        }
        
        std::stringstream ss(line);
        std::string segment;
        std::getline(ss, segment, ',');
        
        int currentId;
        try {
            currentId = std::stoi(segment);
        } catch (const std::exception& e) {
            tempFile << line << std::endl;
            continue;
        }
        
        if (currentId == productId) {
            productFound = true;
            double originalRating = 0.0;
            std::getline(ss, segment, ',');
            std::getline(ss, segment, ',');
            std::getline(ss, segment, ',');
            if (std::getline(ss, segment, ',')) {
                try {
                    originalRating = std::stod(segment);
                } catch (...) {
                }
            }
            
            tempFile << productId << ","
                     << (productData.getName() ? productData.getName() : "") << ","
                     << (productData.getCategory() ? productData.getCategory() : "") << ","
                     << std::fixed << std::setprecision(2) << productData.getPrice() << ","
                     << std::fixed << std::setprecision(1) << originalRating << ","
                     << productData.getStock() << std::endl;
        } else {
            tempFile << line << std::endl;
        }
    }
    
    inFile.close();
    tempFile.close();
    
    if (!productFound) {
        std::cerr << "Error: Product ID " << productId << " not found for editing." << std::endl;
        std::remove("data/products_temp.txt");
        return false;
    }
    
    if (std::remove("data/products.txt") != 0) {
        std::cerr << "Error: Could not remove original products.txt file." << std::endl;
        return false;
    }
    
    if (std::rename("data/products_temp.txt", "data/products.txt") != 0) {
        std::cerr << "Error: Could not rename temporary file to products.txt." << std::endl;
        return false;
    }
    
    std::cout << "Product ID " << productId << " updated successfully." << std::endl;
    return true;
}

inline bool Product::removeProduct(int productId) {
    std::ifstream inFile("data/products.txt");
    if (!inFile) {
        std::cerr << "Error: Could not open products.txt for reading." << std::endl;
        return false;
    }

    std::ofstream tempFile("data/products_temp.txt");
    if (!tempFile) {
        std::cerr << "Error: Could not create temporary file for product removal." << std::endl;
        inFile.close();
        return false;
    }

    std::string line;
    bool productFound = false;
    
    while (std::getline(inFile, line)) {
        if (line.empty()) {
            tempFile << std::endl;
            continue;
        }
        
        std::stringstream ss(line);
        std::string segment;
        std::getline(ss, segment, ',');
        
        int currentId;
        try {
            currentId = std::stoi(segment);
        } catch (const std::exception& e) {
            tempFile << line << std::endl;
            continue;
        }
        
        if (currentId != productId) {
            tempFile << line << std::endl;
        } else {
            productFound = true;
        }
    }
    
    inFile.close();
    tempFile.close();
    
    if (!productFound) {
        std::cerr << "Error: Product ID " << productId << " not found for removal." << std::endl;
        std::remove("data/products_temp.txt");
        return false;
    }
    
    if (std::remove("data/products.txt") != 0) {
        std::cerr << "Error: Could not remove original products.txt file." << std::endl;
        return false;
    }
    
    if (std::rename("data/products_temp.txt", "data/products.txt") != 0) {
        std::cerr << "Error: Could not rename temporary file to products.txt." << std::endl;
        return false;
    }
    
    std::cout << "Product ID " << productId << " removed successfully." << std::endl;
    return true;
}

inline Product* Product::loadAllProducts(int& outCount) {
    outCount = 0;
    ifstream inFile("data/products.txt");
    
    if (!inFile) {
        cerr << "Error: Could not open products.txt file." << endl;
        return nullptr;
    }
    
    string line;
    int validProductCount = 0;
    
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string segment;
        
        if (getline(ss, segment, ',')) {
            try {
                int id = stoi(segment);
                validProductCount++;
            } catch (...) {
            }
        }
    }
    
    if (validProductCount == 0) {
        inFile.close();
        return nullptr;
    }
    
    Product* products = new Product[validProductCount];
    
    inFile.clear();
    inFile.seekg(0, ios::beg);
    
    int index = 0;
    
    while (getline(inFile, line) && index < validProductCount) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string segment;
        
        int id = 0;
        string name, category;
        double price = 0.0, rating = 0.0;
        int stock = 0;
        
        if (getline(ss, segment, ',')) {
            try {
                id = stoi(segment);
            } catch (...) {
                continue;
            }
        } else {
            continue;
        }
        
        if (!getline(ss, name, ',')) {
            name = "";
        }
        
        if (!getline(ss, category, ',')) {
            category = "";
        }
        
        if (getline(ss, segment, ',')) {
            try {
                price = stod(segment);
            } catch (...) {
                price = 0.0;
            }
        }
        
        if (getline(ss, segment, ',')) {
            try {
                rating = stod(segment);
            } catch (...) {
                rating = 0.0;
            }
        }
        
        if (getline(ss, segment, ',')) {
            try {
                stock = stoi(segment);
            } catch (...) {
                stock = 0;
            }
        }
        
        products[index] = Product(id, name.c_str(), category.c_str(), price, rating, stock);
        index++;
    }
    
    inFile.close();
    outCount = index;
    return products;
}

inline bool Product::addProduct() {
    string name_str, category_str, description_str;
    double price_val, rating_val = 0.0;
    int stock_val;
    int nextID = getNextProductID();

    cout << "\n--- Add New Product ---" << endl;
    cout << "Product ID will be: " << nextID << endl;

    cout << "Enter Product Name: ";
    getline(cin, name_str);

    cout << "Enter Product Category: ";
    getline(cin, category_str);

    cout << "Enter Product Description (optional, press enter to skip): ";
    getline(cin, description_str);

    cout << "Enter Product Price: ";
    while (!(cin >> price_val) || price_val < 0) {
        cout << "Invalid price. Please enter a non-negative number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Stock Quantity: ";
    while (!(cin >> stock_val) || stock_val < 0) {
        cout << "Invalid stock. Please enter a non-negative integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    ofstream outFile("data/products.txt", ios::app);
    if (!outFile) {
        cerr << "Error: Could not open products.txt for writing." << endl;
        return false;
    }

    outFile << nextID << "," 
            << name_str << "," 
            << category_str << "," 
            << fixed << setprecision(2) << price_val << "," 
            << fixed << setprecision(1) << rating_val << "," 
            << stock_val << endl;

    outFile.close();
    cout << "Product '" << name_str << "' added successfully!" << endl;
    return true;
}

