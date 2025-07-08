#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <limits>
#include <iomanip>

using namespace std;

class User {
private:
    int userID;
    char* username;
    char* password;
    char* email;
    bool isAdmin;

    void allocateAndCopy(char*& dest, const char* src) {
        delete[] dest;
        if (src) {
            dest = new char[strlen(src) + 1];
            strcpy(dest, src);
        } else {
            dest = nullptr;
        }
    }
    
    int getNextUserID() {
        ifstream inFile("data/users.txt");
        string line;
        int maxID = 0;
        if (!inFile) {
            return 1;
        }
        while (getline(inFile, line)) {
            size_t first = line.find_first_not_of(" \t\n\r");
            if (string::npos == first) continue;
            size_t last = line.find_last_not_of(" \t\n\r");
            line = line.substr(first, (last - first + 1));
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            if (getline(ss, segment, ',')) {
                 size_t seg_first = segment.find_first_not_of(" \t\n\r");
                 size_t seg_last = segment.find_last_not_of(" \t\n\r");
                 if (string::npos != seg_first) {
                     segment = segment.substr(seg_first, (seg_last - seg_first + 1));
                 } else {
                      segment = "";
                 }
                 if (!segment.empty()) {
                     try {
                        int currentID = stoi(segment);
                        if (currentID > maxID) {
                            maxID = currentID;
                        }
                     } catch (const std::invalid_argument& ia) {
                        cerr << "Warning: Invalid User ID format found in users.txt (line ignored): " << line << endl;
                     } catch (const std::out_of_range& oor) {
                        cerr << "Warning: User ID out of range in users.txt (line ignored): " << line << endl;
                     }
                 }
            }
        }
        inFile.close();
        return (maxID == 0) ? 1 : maxID + 1; 
    }

    static char substituteChar(char c) {
        if (c >= 'a' && c <= 'z') {
            return 'a' + (c - 'a' + 3) % 26;
        } else if (c >= 'A' && c <= 'Z') {
            return 'A' + (c - 'A' + 3) % 26;
        } else if (c >= '0' && c <= '9') {
            return '0' + (c - '0' + 3) % 10;
        } else {
            return c;
        }
    }

    static char* hashPassword(const char* plainPassword) {
        if (!plainPassword) return nullptr;
        int len = strlen(plainPassword);
        char* hashedPassword = new char[len + 1];
        for (int i = 0; i < len; ++i) {
            hashedPassword[i] = substituteChar(plainPassword[i]);
        }
        hashedPassword[len] = '\0';
        return hashedPassword;
    }

    static bool usernameExists(const char* usernameToCheck) {
        ifstream inFile("data/users.txt");
        string line;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            getline(ss, segment, ',');
            getline(ss, segment, ',');
            if (segment == usernameToCheck) {
                return true;
            }
        }
        inFile.close();
        return false;
    }

public:
    User() : userID(0), username(nullptr), password(nullptr), email(nullptr), isAdmin(false) {
    }

    User(int id, const char* uname, const char* pwd, const char* mail, bool adminStatus) : 
        userID(id), username(nullptr), password(nullptr), email(nullptr), isAdmin(adminStatus) {
        allocateAndCopy(username, uname);
        allocateAndCopy(password, pwd);
        allocateAndCopy(email, mail);
    }

    User(const User& other) : 
        userID(other.userID), username(nullptr), password(nullptr), email(nullptr), isAdmin(other.isAdmin) {
        allocateAndCopy(username, other.username);
        allocateAndCopy(password, other.password);
        allocateAndCopy(email, other.email);
    }

    User& operator=(const User& other) {
        if (this != &other) {
            userID = other.userID;
            isAdmin = other.isAdmin;
            allocateAndCopy(username, other.username);
            allocateAndCopy(password, other.password);
            allocateAndCopy(email, other.email);
        }
        return *this;
    }

    ~User() {
        delete[] username;
        delete[] password;
        delete[] email;
    }

    int getUserID() const { return userID; }
    const char* getUsername() const { return username; }
    const char* getEmail() const { return email; }
    bool getIsAdmin() const { return isAdmin; }

    void setIsAdmin(bool adminStatus) {
        isAdmin = adminStatus;
    }

    bool registerUser() {
        string uname_str, email_str, pwd_str;
        
        cout << "\n--- User Registration ---" << endl;
        cout << "Enter Username: ";
        cin >> uname_str;
        cout << "Enter Email: ";
        cin >> email_str;
        cout << "Enter Password: ";
        cin >> pwd_str;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        char* hashedPwd = hashPassword(pwd_str.c_str());
        if (!hashedPwd) {
             cerr << "Error: Password processing failed." << endl;
             return false;
        }
        ifstream inFile("data/users.txt");
        string line;
        bool exists = false;
        while (getline(inFile, line)) {
             if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            getline(ss, segment, ',');
            getline(ss, segment, ',');
            if (segment == uname_str) {
                exists = true;
                break;
            }
        }
        inFile.close();
        if (exists) {
            cerr << "Error: Username '" << uname_str << "' already exists. Please try a different username." << endl;
            delete[] hashedPwd;
            return false;
        }
        this->userID = getNextUserID(); 
        allocateAndCopy(this->username, uname_str.c_str());
        allocateAndCopy(this->password, hashedPwd);
        allocateAndCopy(this->email, email_str.c_str());
        this->isAdmin = false;
        ofstream outFile("data/users.txt", ios::app);
        if (!outFile) {
            cerr << "Error: Could not open users.txt for writing." << endl;
            delete[] this->username; this->username = nullptr;
            delete[] this->password; this->password = nullptr;
            delete[] this->email;    this->email = nullptr;
            delete[] hashedPwd;
            this->userID = 0;
            return false;
        }
        outFile << this->userID << "," 
                << this->username << "," 
                << this->password << "," 
                << this->email << "," 
                << (this->isAdmin ? "1" : "0") << endl;
        outFile.close();
        delete[] hashedPwd;
        cout << "Registration successful! User ID: " << this->userID << endl;
        return true;
    }

    bool loginUser() {
        string uname_str, pwd_str;
        
        cout << "\n--- User Login ---" << endl;
        cout << "Enter Username: ";
        cin >> uname_str;
        cout << "Enter Password: ";
        cin >> pwd_str;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        char* inputHashed = hashPassword(pwd_str.c_str());
        if (!inputHashed) {
            cerr << "Error processing login password." << endl;
            return false;
        }
        ifstream inFile("data/users.txt");
        if (!inFile) {
            cerr << "Error: Could not open users.txt for reading." << endl;
            delete[] inputHashed;
            return false;
        }
        string line;
        bool found = false;
        while (getline(inFile, line)) {
             if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            string file_id_str, file_uname, file_pwd_hashed, file_email, file_isAdmin_str;
            getline(ss, file_id_str, ',');
            getline(ss, file_uname, ',');
            getline(ss, file_pwd_hashed, ',');
            getline(ss, file_email, ',');
            getline(ss, file_isAdmin_str, ',');
            if (file_uname == uname_str && strcmp(inputHashed, file_pwd_hashed.c_str()) == 0) {
                try {
                    this->userID = stoi(file_id_str);
                } catch (...) {
                     cerr << "Warning: Invalid User ID format encountered during login for user: " << file_uname << endl;
                     continue; 
                }
                allocateAndCopy(this->username, file_uname.c_str());
                allocateAndCopy(this->password, file_pwd_hashed.c_str());
                allocateAndCopy(this->email, file_email.c_str());
                 try {
                    this->isAdmin = (stoi(file_isAdmin_str) == 1);
                 } catch (...) {
                     cerr << "Warning: Invalid isAdmin format encountered for user: " << file_uname << ". Defaulting to non-admin." << endl;
                     this->isAdmin = false;
                 }
                found = true;
                break;
            }
        }
        inFile.close();
        delete[] inputHashed;
        if (found) {
            cout << "Login successful! Welcome, " << this->username << "." << endl;
            return true;
        } else {
            cerr << "Login failed: Invalid username or password." << endl;
            delete[] this->username; this->username = nullptr;
            delete[] this->password; this->password = nullptr;
            delete[] this->email;    this->email = nullptr;
            this->userID = 0;
            this->isAdmin = false;
            return false;
        }
    }
    
    void logoutUser() {
        cout << "Logging out " << (username ? username : "user") << "." << endl;
        delete[] username; username = nullptr;
        delete[] password; password = nullptr;
        delete[] email; email = nullptr;
        userID = 0;
        isAdmin = false;
    }

    void displayUserInfo() const {
         if (!username) {
             cout << "User data is not loaded." << endl;
             return;
         }
         cout << "------------------------" << endl;
         cout << "User ID:   " << userID << endl;
         cout << "Username:  " << username << endl;
         cout << "Email:     " << email << endl;
         cout << "Admin:     " << (isAdmin ? "Yes" : "No") << endl;
         cout << "------------------------" << endl;
    }

    static void viewAllUsers() {
        ifstream inFile("data/users.txt");
        if (!inFile) {
            cerr << "Error: Could not open users.txt for viewing." << endl;
            return;
        }
        cout << "\n--- All Registered Users ---" << endl;
        string line;
        int userCount = 0;
        cout << left << setw(8) << "User ID" 
             << setw(20) << "Username" 
             << setw(30) << "Email" 
             << setw(8) << "IsAdmin" << endl;
        cout << setfill('-') << setw(66) << "" << setfill(' ') << endl;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            userCount++;
            stringstream ss(line);
            string segment;
            string uid_str, uname_str, pwd_hash_ignored, email_str, admin_str;
            getline(ss, uid_str, ',');
            getline(ss, uname_str, ',');
            getline(ss, pwd_hash_ignored, ',');
            getline(ss, email_str, ',');
            getline(ss, admin_str, ',');
            cout << left << setw(8) << uid_str 
                 << setw(20) << uname_str 
                 << setw(30) << email_str 
                 << setw(8) << (admin_str == "1" ? "Yes" : "No") << endl;
        }
        inFile.close();
        if (userCount == 0) {
            cout << "No users found." << endl;
        } else {
            cout << setfill('-') << setw(66) << "" << setfill(' ') << endl;
        }
        cout << "------------------------------------" << endl;
    }

    static bool editUser() {
         cout << "\n--- Edit User ---" << endl;
         int userIDToEdit;
         cout << "Enter the User ID to edit (or 0 to cancel): ";
         while (!(cin >> userIDToEdit)) {
             cout << "Invalid ID. Please enter an integer: ";
             cin.clear();
             cin.ignore(numeric_limits<streamsize>::max(), '\n');
         }
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         if (userIDToEdit == 0) {
             cout << "Edit operation cancelled." << endl;
             return true;
         }
         if (userIDToEdit == 1) {
              cout << "Error: Cannot edit the primary admin account (User ID 1)." << endl;
              return false;
         }
        ifstream inFile("data/users.txt");
        ofstream tempFile("data/temp_users.txt"); 
        if (!inFile || !tempFile) {
            cerr << "Error: Could not open user files for editing." << endl;
            inFile.close(); tempFile.close(); remove("data/temp_users.txt");
            return false;
        }
        string line;
        bool found = false;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            int currentID;
            getline(ss, segment, ',');
            try {
                if (segment.empty()) continue; 
                currentID = stoi(segment);
            } catch (...) {
                tempFile << line << endl;
                continue;
            }
            if (currentID == userIDToEdit) {
                found = true;
                cout << "Found User ID: " << currentID << ". Enter new details:" << endl;
                string uname_orig, pwd_hash, email_orig, admin_str;
                stringstream ss_original(line);
                getline(ss_original, segment, ',');
                getline(ss_original, uname_orig, ',');
                getline(ss_original, pwd_hash, ',');
                getline(ss_original, email_orig, ',');
                getline(ss_original, admin_str, ',');
                string newUsername, newEmail;
                cout << "Enter NEW Username (leave blank to keep '" << uname_orig << "'): ";
                getline(cin, newUsername);
                if (newUsername.empty()) newUsername = uname_orig;
                else {
                    if (newUsername != uname_orig && usernameExists(newUsername.c_str())) {
                         cout << "Error: Username '" << newUsername << "' already exists. Edit cancelled." << endl;
                         tempFile << line << endl;
                         found = false;
                         break;
                    }
                }
                cout << "Enter NEW Email (leave blank to keep '" << email_orig << "'): ";
                getline(cin, newEmail);
                if (newEmail.empty()) newEmail = email_orig;
                tempFile << currentID << "," 
                         << newUsername << "," 
                         << pwd_hash << "," 
                         << newEmail << "," 
                         << admin_str << endl;
                cout << "User ID " << currentID << " updated." << endl;
            } else {
                tempFile << line << endl;
            }
        }
        inFile.close();
        tempFile.close();
        if (!found) {
            if (userIDToEdit != 1) {
                 cerr << "Error: User ID " << userIDToEdit << " not found." << endl;
            }
            remove("data/temp_users.txt");
            return false;
        }
        if (remove("data/users.txt") != 0 || rename("data/temp_users.txt", "data/users.txt") != 0) {
            cerr << "Error: Failed to update users.txt." << endl;
            return false;
        }
        return true;
    }

    static bool removeUser(int currentAdminUserID) {
         cout << "\n--- Remove User ---" << endl;
         int userIDToRemove;
         cout << "Enter the User ID to remove (or 0 to cancel): ";
         while (!(cin >> userIDToRemove)) {
             cout << "Invalid ID. Please enter an integer: ";
             cin.clear();
             cin.ignore(numeric_limits<streamsize>::max(), '\n');
         }
         cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
         if (userIDToRemove == 0) {
             cout << "Remove operation cancelled." << endl;
             return true;
         }
         if (userIDToRemove == currentAdminUserID) {
             cout << "Error: You cannot remove your own admin account." << endl;
             return false;
         }
          if (userIDToRemove == 1) {
              cout << "Error: Cannot remove the primary admin account (User ID 1)." << endl;
              return false;
         }
        ifstream inFile("data/users.txt");
        ofstream tempFile("data/temp_users.txt");
        if (!inFile || !tempFile) {
            cerr << "Error: Could not open user files for removal." << endl;
            inFile.close(); tempFile.close(); remove("data/temp_users.txt");
            return false;
        }
        string line;
        bool found = false;
        int currentID;
        string removedUsername = ""; 
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            getline(ss, segment, ',');
            try {
                if (segment.empty()) continue; 
                currentID = stoi(segment);
            } catch (...) {
                tempFile << line << endl;
                continue;
            }
            if (currentID == userIDToRemove) {
                found = true;
                getline(ss, segment, ',');
                removedUsername = segment;
                cout << "User ID " << currentID << " ('" << removedUsername << "') marked for removal." << endl;
            } else {
                tempFile << line << endl;
            }
        }
        inFile.close();
        tempFile.close();
        if (!found) {
            cerr << "Error: User ID " << userIDToRemove << " not found." << endl;
            remove("data/temp_users.txt");
            return false;
        }
        if (remove("data/users.txt") != 0 || rename("data/temp_users.txt", "data/users.txt") != 0) {
            cerr << "Error: Failed to update users.txt." << endl;
            return false;
        }
        cout << "User '" << removedUsername << "' (ID: " << userIDToRemove << ") removed successfully." << endl;
        return true;
    }

    static int getUserIdByUsername(const char* username) {
        if (!username) return -1;
        ifstream inFile("data/users.txt");
        if (!inFile) {
            cerr << "Error: Could not open users.txt to get user ID." << endl;
            return -1;
        }
        string line;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            string id_str, uname, pwd_ignored, email_ignored, isAdmin_ignored;
            getline(ss, id_str, ',');
            getline(ss, uname, ',');
            if (uname == username) {
                inFile.close();
                return stoi(id_str);
            }
        }
        inFile.close();
        return -1;
    }
    
    static bool getUserEmailById(int userId, std::string& email) {
        if (userId <= 0) return false;
        ifstream inFile("data/users.txt");
        if (!inFile) {
            cerr << "Error: Could not open users.txt to get user email." << endl;
            return false;
        }
        string line;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            string id_str, uname_ignored, pwd_ignored, email_str, isAdmin_ignored;
            getline(ss, id_str, ',');
            getline(ss, uname_ignored, ',');
            getline(ss, pwd_ignored, ',');
            getline(ss, email_str, ',');
            if (stoi(id_str) == userId) {
                email = email_str;
                inFile.close();
                return true;
            }
        }
        inFile.close();
        return false;
    }

    static User* getUserByID(int userId) {
        if (userId <= 0) return nullptr;
        ifstream inFile("data/users.txt");
        if (!inFile) {
            cerr << "Error: Could not open users.txt to get user." << endl;
            return nullptr;
        }
        string line;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string segment;
            string id_str, uname, pwd, email, isAdmin_str;
            getline(ss, id_str, ',');
            if (stoi(id_str) == userId) {
                getline(ss, uname, ',');
                getline(ss, pwd, ',');
                getline(ss, email, ',');
                getline(ss, isAdmin_str, ',');
                bool isAdmin = (isAdmin_str == "admin");
                User* user = new User(userId, uname.c_str(), pwd.c_str(), email.c_str(), isAdmin);
                inFile.close();
                return user;
            }
        }
        inFile.close();
        return nullptr;
    }
    
    static bool updateUserInFile(User& user) {
        if (user.getUserID() <= 0) return false;
        ifstream inFile("data/users.txt");
        ofstream tempFile("data/temp_users.txt");
        if (!inFile || !tempFile) {
            cerr << "Error: Could not open user files for updating." << endl;
            inFile.close(); tempFile.close(); remove("data/temp_users.txt");
            return false;
        }
        string line;
        bool found = false;
        while (getline(inFile, line)) {
            if (line.empty()) {
                tempFile << endl;
                continue;
            }
            stringstream ss(line);
            string segment;
            int currentId;
            getline(ss, segment, ',');
            try {
                if (segment.empty()) continue; 
                currentId = stoi(segment);
            } catch (...) {
                tempFile << line << endl;
                continue;
            }
            if (currentId == user.getUserID()) {
                found = true;
                string username_orig, pwd_hash, email_orig, admin_str;
                getline(ss, username_orig, ',');
                getline(ss, pwd_hash, ',');
                getline(ss, email_orig, ',');
                getline(ss, admin_str, ',');
                tempFile << user.getUserID() << "," 
                         << user.getUsername() << "," 
                         << pwd_hash << "," 
                         << user.getEmail() << "," 
                         << (user.getIsAdmin() ? "1" : "0") << endl;
            } else {
                tempFile << line << endl;
            }
        }
        inFile.close();
        tempFile.close();
        if (!found) {
            cerr << "Error: User ID " << user.getUserID() << " not found for update." << endl;
            remove("data/temp_users.txt");
            return false;
        }
        if (remove("data/users.txt") != 0 || rename("data/temp_users.txt", "data/users.txt") != 0) {
            cerr << "Error: Failed to update users.txt." << endl;
            return false;
        }
        return true;
    }
    
    void setUsername(const char* newUsername) {
        if (newUsername && !usernameExists(newUsername)) {
            allocateAndCopy(username, newUsername);
        }
    }
    
    void setEmail(const char* newEmail) {
        if (newEmail) {
            allocateAndCopy(email, newEmail);
        }
    }

}; 