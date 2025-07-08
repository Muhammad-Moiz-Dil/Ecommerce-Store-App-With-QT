#include "../include/UserProfileWidget.h"
#include <fstream>
#include <sstream>
#include "../../include/User.h"

UserProfileWidget::UserProfileWidget(int userId, const QString& username, QWidget *parent)
    : QWidget(parent), userId(userId), username(username)
{
    setupUI();
    loadUserData();
}

UserProfileWidget::~UserProfileWidget()
{
    // Qt handles deletion of child widgets
}

void UserProfileWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // User Info Group
    QGroupBox *userInfoGroup = new QGroupBox("User Information", this);
    QFormLayout *userInfoLayout = new QFormLayout(userInfoGroup);
    
    usernameLabel = new QLabel(username, this);
    emailLabel = new QLabel("Loading...", this);
    
    userInfoLayout->addRow(new QLabel("Username:", this), usernameLabel);
    userInfoLayout->addRow(new QLabel("Email:", this), emailLabel);
    
    // Email Update Group
    QGroupBox *emailGroup = new QGroupBox("Update Email", this);
    QFormLayout *emailLayout = new QFormLayout(emailGroup);
    
    newEmailEdit = new QLineEdit(this);
    updateEmailButton = new QPushButton("Update Email", this);
    
    emailLayout->addRow(new QLabel("New Email:", this), newEmailEdit);
    emailLayout->addRow(updateEmailButton);
    
    // Password Update Group
    QGroupBox *passwordGroup = new QGroupBox("Change Password", this);
    QFormLayout *passwordLayout = new QFormLayout(passwordGroup);
    
    currentPasswordEdit = new QLineEdit(this);
    currentPasswordEdit->setEchoMode(QLineEdit::Password);
    
    newPasswordEdit = new QLineEdit(this);
    newPasswordEdit->setEchoMode(QLineEdit::Password);
    
    confirmPasswordEdit = new QLineEdit(this);
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    
    updatePasswordButton = new QPushButton("Update Password", this);
    
    passwordLayout->addRow(new QLabel("Current Password:", this), currentPasswordEdit);
    passwordLayout->addRow(new QLabel("New Password:", this), newPasswordEdit);
    passwordLayout->addRow(new QLabel("Confirm Password:", this), confirmPasswordEdit);
    passwordLayout->addRow(updatePasswordButton);
    
    // Add groups to main layout
    mainLayout->addWidget(userInfoGroup);
    mainLayout->addWidget(emailGroup);
    mainLayout->addWidget(passwordGroup);
    mainLayout->addStretch();
    
    // Connect signals
    connect(updateEmailButton, &QPushButton::clicked, this, &UserProfileWidget::handleUpdateEmail);
    connect(updatePasswordButton, &QPushButton::clicked, this, &UserProfileWidget::handleUpdatePassword);
    
    setLayout(mainLayout);
}

void UserProfileWidget::loadUserData()
{
    // Get user email for display
    std::string userEmail;
    bool emailFound = User::getUserEmailById(userId, userEmail);
    emailLabel->setText(emailFound ? QString::fromStdString(userEmail) : "Email not found");
}

void UserProfileWidget::handleUpdateEmail()
{
    QString newEmail = newEmailEdit->text().trimmed();
    
    if (newEmail.isEmpty()) {
        QMessageBox::warning(this, "Update Email", "Please enter a new email address.");
        return;
    }
    
    // Update the email in the users.txt file
    std::ifstream inFile("data/users.txt");
    std::ofstream tempFile("data/temp_users.txt");
    
    if (!inFile.is_open() || !tempFile.is_open()) {
        QMessageBox::warning(this, "Error", "Failed to open user files for update.");
        return;
    }
    
    std::string line;
    bool updated = false;
    
    while (std::getline(inFile, line)) {
        if (line.empty()) {
            tempFile << line << std::endl;
            continue;
        }
        
        std::stringstream ss(line);
        std::string segment;
        std::string userIdStr, usernameStr, passwordStr, emailStr, adminStr;
        
        // Parse user details
        std::getline(ss, userIdStr, ',');
        std::getline(ss, usernameStr, ',');
        std::getline(ss, passwordStr, ',');
        std::getline(ss, emailStr, ',');
        std::getline(ss, adminStr, ',');
        
        int currentUserId = -1;
        try {
            currentUserId = std::stoi(userIdStr);
        } catch (...) {
            // Write invalid line as-is
            tempFile << line << std::endl;
            continue;
        }
        
        // Check if this is the user we want to update
        if (currentUserId == userId) {
            // Update email
            tempFile << userIdStr << ","
                    << usernameStr << ","
                    << passwordStr << ","
                    << newEmail.toStdString() << ","
                    << adminStr << std::endl;
            updated = true;
        } else {
            // Write line as-is
            tempFile << line << std::endl;
        }
    }
    
    inFile.close();
    tempFile.close();
    
    if (updated) {
        // Replace original file
        if (std::remove("data/users.txt") != 0 || std::rename("data/temp_users.txt", "data/users.txt") != 0) {
            QMessageBox::warning(this, "Error", "Failed to update users file.");
            std::remove("data/temp_users.txt"); // Clean up if rename fails
            return;
        }
        
        email = newEmail;
        emailLabel->setText(email);
        newEmailEdit->clear();
        QMessageBox::information(this, "Update Email", "Email updated successfully.");
    } else {
        QMessageBox::warning(this, "Update Email", "Failed to find user for update.");
        std::remove("data/temp_users.txt"); // Clean up temp file
    }
}

void UserProfileWidget::handleUpdatePassword()
{
    QString currentPassword = currentPasswordEdit->text();
    QString newPassword = newPasswordEdit->text();
    QString confirmPassword = confirmPasswordEdit->text();
    
    if (currentPassword.isEmpty() || newPassword.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "Update Password", "All password fields are required.");
        return;
    }
    
    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, "Update Password", "New password and confirmation do not match.");
        return;
    }
    
    // First, verify current password
    std::ifstream checkFile("data/users.txt");
    if (!checkFile.is_open()) {
        QMessageBox::warning(this, "Error", "Failed to open user file to verify password.");
        return;
    }
    
    std::string line;
    std::string storedPassword;
    bool userFound = false;
    
    while (std::getline(checkFile, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string segment;
        std::string userIdStr;
        
        // Parse user ID
        std::getline(ss, userIdStr, ',');
        
        int currentUserId = -1;
        try {
            currentUserId = std::stoi(userIdStr);
        } catch (...) {
            continue;
        }
        
        // Check if this is the user we're looking for
        if (currentUserId == userId) {
            userFound = true;
            
            // Skip username
            std::getline(ss, segment, ',');
            
            // Get password
            std::getline(ss, storedPassword, ',');
            break;
        }
    }
    
    checkFile.close();
    
    if (!userFound) {
        QMessageBox::warning(this, "Error", "User not found.");
        return;
    }
    
    // Hash the provided current password to compare with stored password
    // Using the substitution cipher algorithm from User class
    std::string hashedCurrentPassword = currentPassword.toStdString();
    for (char& c : hashedCurrentPassword) {
        if (c >= 'a' && c <= 'z') {
            c = 'a' + (c - 'a' + 3) % 26;
        } else if (c >= 'A' && c <= 'Z') {
            c = 'A' + (c - 'A' + 3) % 26;
        } else if (c >= '0' && c <= '9') {
            c = '0' + (c - '0' + 3) % 10;
        }
    }
    
    // Check if current password matches
    if (hashedCurrentPassword != storedPassword) {
        QMessageBox::warning(this, "Error", "Current password is incorrect.");
        return;
    }
    
    // Hash the new password
    std::string hashedNewPassword = newPassword.toStdString();
    for (char& c : hashedNewPassword) {
        if (c >= 'a' && c <= 'z') {
            c = 'a' + (c - 'a' + 3) % 26;
        } else if (c >= 'A' && c <= 'Z') {
            c = 'A' + (c - 'A' + 3) % 26;
        } else if (c >= '0' && c <= '9') {
            c = '0' + (c - '0' + 3) % 10;
        }
    }
    
    // Now update the password
    std::ifstream inFile("data/users.txt");
    std::ofstream tempFile("data/temp_users.txt");
    
    if (!inFile.is_open() || !tempFile.is_open()) {
        QMessageBox::warning(this, "Error", "Failed to open user files for update.");
        return;
    }
    
    bool updated = false;
    
    while (std::getline(inFile, line)) {
        if (line.empty()) {
            tempFile << line << std::endl;
            continue;
        }
        
        std::stringstream ss(line);
        std::string userIdStr, usernameStr, passwordStr, emailStr, adminStr;
        
        // Parse user details
        std::getline(ss, userIdStr, ',');
        std::getline(ss, usernameStr, ',');
        std::getline(ss, passwordStr, ',');
        std::getline(ss, emailStr, ',');
        std::getline(ss, adminStr, ',');
        
        int currentUserId = -1;
        try {
            currentUserId = std::stoi(userIdStr);
        } catch (...) {
            // Write invalid line as-is
            tempFile << line << std::endl;
            continue;
        }
        
        // Check if this is the user we want to update
        if (currentUserId == userId) {
            // Update password
            tempFile << userIdStr << ","
                    << usernameStr << ","
                    << hashedNewPassword << ","
                    << emailStr << ","
                    << adminStr << std::endl;
            updated = true;
        } else {
            // Write line as-is
            tempFile << line << std::endl;
        }
    }
    
    inFile.close();
    tempFile.close();
    
    if (updated) {
        // Replace original file
        if (std::remove("data/users.txt") != 0 || std::rename("data/temp_users.txt", "data/users.txt") != 0) {
            QMessageBox::warning(this, "Error", "Failed to update users file.");
            std::remove("data/temp_users.txt"); // Clean up if rename fails
            return;
        }
        
        currentPasswordEdit->clear();
        newPasswordEdit->clear();
        confirmPasswordEdit->clear();
        QMessageBox::information(this, "Update Password", "Password updated successfully.");
    } else {
        QMessageBox::warning(this, "Update Password", "Failed to find user for update.");
        std::remove("data/temp_users.txt"); // Clean up temp file
    }
} 