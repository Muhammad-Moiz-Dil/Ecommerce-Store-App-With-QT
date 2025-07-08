#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cmath>
#include <iomanip>
#include <limits>
#include <cstdio>

#include "Product.h"

using namespace std;

class Review {
private:
    int userID;
    int productID;
    int rating;
    char* comment;

    void allocateAndCopy(char*& dest, const char* src) {
        delete[] dest;
        if (src) {
            dest = new char[strlen(src) + 1];
            strcpy(dest, src);
        } else {
            dest = nullptr;
        }
    }

     static bool updateProductAverageRating(int productIDToUpdate) {
        ifstream reviewFile("data/reviews/reviews.txt");
        if (!reviewFile) {
            cerr << "Warning: Cannot open reviews.txt to calculate average rating." << endl;
            return false;
        }

        string line;
        double totalRatingSum = 0;
        int ratingCount = 0;

        while (getline(reviewFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            int currentProdID, currentUserID, currentRating;
            getline(ss, segment, ','); try { currentProdID = stoi(segment); } catch(...) { continue; }
            if (currentProdID == productIDToUpdate) {
                 getline(ss, segment, ',');
                 getline(ss, segment, ','); try { currentRating = stoi(segment); } catch(...) { continue; }
                 if (currentRating >= 1 && currentRating <= 5) {
                    totalRatingSum += currentRating;
                    ratingCount++;
                 }
            }
        }
        reviewFile.close();

        double averageRating = (ratingCount > 0) ? (totalRatingSum / ratingCount) : 0.0;
        averageRating = round(averageRating * 10.0) / 10.0;

        ifstream productInFile("data/products.txt");
        ofstream productTempFile("data/temp_products.txt");
        if (!productInFile || !productTempFile) {
            cerr << "Error: Could not open product files for rating update." << endl;
             productInFile.close(); productTempFile.close(); remove("data/temp_products.txt");
            return false;
        }

        bool foundProduct = false;
        while(getline(productInFile, line)) {
            if (line.empty()) { productTempFile << endl; continue; }
            stringstream ss(line);
            string segment;
            int currentProdID;
            getline(ss, segment, ',');
            try { currentProdID = stoi(segment); } catch (...) { productTempFile << line << endl; continue; }
            if (currentProdID == productIDToUpdate) {
                 foundProduct = true;
                 string name_str, cat_str, price_str, old_rating_ignored, stock_str;
                 getline(ss, name_str, ',');
                 getline(ss, cat_str, ',');
                 getline(ss, price_str, ',');
                 getline(ss, old_rating_ignored, ',');
                 getline(ss, stock_str, ',');
                 productTempFile << currentProdID << "," << name_str << "," << cat_str << "," \
                                 << price_str << "," \
                                 << fixed << setprecision(1) << averageRating << "," \
                                 << stock_str << endl;
            } else {
                productTempFile << line << endl;
            }
        }
        productInFile.close();
        productTempFile.close();

        if (!foundProduct) {
             cerr << "Warning: Product ID " << productIDToUpdate << " not found in products.txt during rating update." << endl;
             remove("data/temp_products.txt");
             return false;
        }

        if (remove("data/products.txt") != 0 || rename("data/temp_products.txt", "data/products.txt") != 0) {
            cerr << "Error: Failed to update products.txt with new average rating." << endl;
            return false;
        }
        cout << "Updated average rating for Product ID " << productIDToUpdate << " to: " << fixed << setprecision(1) << averageRating << endl;
        return true;
     }

public:
    Review() : userID(0), productID(0), rating(0), comment(nullptr) {}

    Review(int uid, int pid, int r, const char* cmt) :
        userID(uid), productID(pid), rating(r), comment(nullptr) {
        allocateAndCopy(comment, cmt);
    }
    
    Review(const Review& other) :
        userID(other.userID), productID(other.productID), rating(other.rating), comment(nullptr) {
        allocateAndCopy(comment, other.comment);
    }

    Review& operator=(const Review& other) {
        if (this != &other) {
            userID = other.userID;
            productID = other.productID;
            rating = other.rating;
            allocateAndCopy(comment, other.comment);
        }
        return *this;
    }

    ~Review() {
        delete[] comment;
    }

    int getUserID() const { return userID; }
    int getProductID() const { return productID; }
    int getRating() const { return rating; }
    const char* getComment() const { return comment; }

    static bool addReview(int productID, int userID, int rating, const char* commentText) {
        if (userID <= 0 || productID <= 0) {
            cerr << "Error: Invalid user ID or product ID provided for review." << endl;
            return false;
        }
        if (rating < 1 || rating > 5) {
            cerr << "Error: Invalid rating provided (" << rating << "). Must be 1-5." << endl;
            return false;
        }
         if (!commentText) {
             cerr << "Error: Comment text cannot be null." << endl;
             return false;
         }
        Product* tempProd = Product::getProductByID(productID);
        if (!tempProd) {
             cerr << "Error: Cannot review Product ID " << productID << " as it does not exist." << endl;
             return false;
        }
        delete tempProd;
         ifstream checkFile("data/reviews/reviews.txt");
         bool alreadyReviewed = false;
         if (checkFile) {
             string line;
             while(getline(checkFile, line)) {
                 if (line.empty()) continue;
                 stringstream ss(line);
                 string segment;
                 int currentProdID, currentUserID;
                 getline(ss, segment, ','); try { currentProdID = stoi(segment); } catch(...) { continue; }
                 getline(ss, segment, ','); try { currentUserID = stoi(segment); } catch(...) { continue; }
                 if (currentProdID == productID && currentUserID == userID) {
                     alreadyReviewed = true;
                     break;
                 }
             }
             checkFile.close();
         }
         if (alreadyReviewed) {
            cout << "Info: User " << userID << " has already submitted a review for Product ID " << productID << ". Cannot add another." << endl;
            return false;
         }
         char* safeComment = nullptr;
         if (commentText) {
             safeComment = new char[strlen(commentText) + 1];
             strcpy(safeComment, commentText);
              for (int i = 0; safeComment[i] != '\0'; ++i) {
                 if (safeComment[i] == ',') {
                     safeComment[i] = ';';
                 }
             }
         } else {
             safeComment = new char[1];
             safeComment[0] = '\0';
         }
        ofstream outFile("data/reviews/reviews.txt", ios::app);
        if (!outFile) {
            cerr << "Error: Could not open reviews.txt for writing." << endl;
            delete[] safeComment;
            return false;
        }
        outFile << productID << ","
                << userID << ","
                << rating << ","
                << safeComment << endl;
        outFile.close();
        delete[] safeComment;
        cout << "Review added successfully for Product ID " << productID << " by User ID " << userID << "." << endl;
        updateProductAverageRating(productID);
        return true;
    }

     static void getReviewsForProduct(int productIDToView) {
         ifstream inFile("data/reviews/reviews.txt");
         if (!inFile) {
             cerr << "Error: Could not open reviews.txt." << endl;
             return;
         }
         cout << "\n--- Reviews for Product ID: " << productIDToView << " ---" << endl;
         string line;
         int reviewCount = 0;
         double totalRating = 0;
         while (getline(inFile, line)) {
             if (line.empty()) continue;
             stringstream ss(line);
             string segment;
             int currentProdID, currentUserID, currentRating;
             string comment_str;
             getline(ss, segment, ','); try { currentProdID = stoi(segment); } catch(...) { continue; }
             if (currentProdID == productIDToView) {
                  reviewCount++;
                  getline(ss, segment, ','); try { currentUserID = stoi(segment); } catch(...) { currentUserID = 0; }
                  getline(ss, segment, ','); try { currentRating = stoi(segment); } catch(...) { currentRating = 0; }
                  getline(ss, comment_str);
                  if (currentRating >= 1 && currentRating <= 5) totalRating += currentRating;
                  cout << "User ID: " << currentUserID 
                       << " | Rating: " << currentRating << "/5" << endl;
                  cout << "Comment: " << comment_str << endl;
                  cout << "--------" << endl;
             }
         }
         inFile.close();
         if (reviewCount == 0) {
             cout << "No reviews found for this product yet." << endl;
         } else {
              double average = (reviewCount > 0) ? (totalRating / reviewCount) : 0.0;
              cout << "Average Rating based on displayed reviews: " << fixed << setprecision(1) << average << "/5.0" << endl;
         }
         cout << "------------------------------------" << endl;
     }

}; 