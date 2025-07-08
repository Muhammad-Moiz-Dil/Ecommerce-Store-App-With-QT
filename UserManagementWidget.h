#ifndef USERMANAGEMENTWIDGET_H
#define USERMANAGEMENTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QList>
#include <QTextStream>
#include <fstream>
#include <sstream>
#include <string>
#include <QString>

#include "../../include/User.h"

// Simple data structure for the GUI
// This is only for the GUI and doesn't affect the backend
class UserDetails {
public:
    UserDetails() : m_id(0) {}
    
    // Getters
    int getId() const { return m_id; }
    const QString& getUsername() const { return m_username; }
    const QString& getEmail() const { return m_email; }
    const QString& getRole() const { return m_role; }
    
    // Setters
    void setId(int id) { m_id = id; }
    void setUsername(const QString& username) { m_username = username; }
    void setEmail(const QString& email) { m_email = email; }
    void setRole(const QString& role) { m_role = role; }

private:
    int m_id;
    QString m_username;
    QString m_email;
    QString m_role;
};

// Forward declarations
class EditUserDialog;

class UserManagementWidget : public QWidget
{
    Q_OBJECT

public:
    inline explicit UserManagementWidget(QWidget *parent = nullptr);
    inline ~UserManagementWidget();

signals:
    void userUpdated(int userId); // Signal when a user is updated
    void userRemoved(int userId); // Signal when a user is removed

private slots:
    inline void handleEditUser();
    inline void handleRemoveUser();
    inline void handleAddUser();
    inline void handleRefresh(); // New method to refresh user list

private:
    inline void loadUsers();
    inline void setupUi();

    QTableWidget *usersTable;
    QPushButton *editUserButton;
    QPushButton *removeUserButton;
    QPushButton *addUserButton;
    QPushButton *refreshButton; // New refresh button
    QList<UserDetails> displayedUserDetails;
};

inline UserManagementWidget::UserManagementWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadUsers(); 
}

inline UserManagementWidget::~UserManagementWidget() {}

inline void UserManagementWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    usersTable = new QTableWidget(this);
    usersTable->setColumnCount(4); // ID, Username, Email, Role
    usersTable->setHorizontalHeaderLabels({"ID", "Username", "Email", "Role"});
    usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    usersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(usersTable);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    editUserButton = new QPushButton("Edit Selected User", this);
    removeUserButton = new QPushButton("Remove Selected User", this);
    addUserButton = new QPushButton("Add User", this);
    refreshButton = new QPushButton("Refresh", this);
    buttonLayout->addWidget(editUserButton);
    buttonLayout->addWidget(removeUserButton);
    buttonLayout->addWidget(addUserButton);
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(editUserButton, &QPushButton::clicked, this, &UserManagementWidget::handleEditUser);
    connect(removeUserButton, &QPushButton::clicked, this, &UserManagementWidget::handleRemoveUser);
    connect(addUserButton, &QPushButton::clicked, this, &UserManagementWidget::handleAddUser);
    connect(refreshButton, &QPushButton::clicked, this, &UserManagementWidget::handleRefresh);
}

inline void UserManagementWidget::loadUsers() {
    usersTable->setRowCount(0);
    displayedUserDetails.clear();

    // Read users directly from the file
    std::ifstream inFile("data/users.txt");
    if (!inFile.is_open()) {
        QMessageBox::warning(this, "User Management", "No users found or could not open users file.");
        return;
    }
    
    std::string line;
    int row = 0;
    
    // First count the number of rows to set the table size
    int userCount = 0;
    while (std::getline(inFile, line)) {
        if (!line.empty()) userCount++;
    }
    
    // Reset file position to beginning
    inFile.clear();
    inFile.seekg(0, std::ios::beg);
    
    if (userCount == 0) {
        QMessageBox::information(this, "User Management", "No users found in the system.");
        inFile.close();
        return;
    }
    
    usersTable->setRowCount(userCount);
    
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string id_str, username_str, password_str /* ignored */, email_str, is_admin_str;
        
        std::getline(ss, id_str, ',');
        std::getline(ss, username_str, ',');
        std::getline(ss, password_str, ','); // Skip password
        std::getline(ss, email_str, ',');
        std::getline(ss, is_admin_str, ',');
        
        try {
            int userId = std::stoi(id_str);
            
            // Create UserDetails for display
            UserDetails userDetails;
            userDetails.setId(userId);
            userDetails.setUsername(QString::fromStdString(username_str));
            userDetails.setEmail(QString::fromStdString(email_str));
            userDetails.setRole(QString::fromStdString(is_admin_str == "1" ? "Admin" : "Customer"));
            displayedUserDetails.append(userDetails);
            
            // Set table items
            usersTable->setItem(row, 0, new QTableWidgetItem(QString::number(userId)));
            usersTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(username_str)));
            usersTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(email_str)));
            usersTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(is_admin_str == "1" ? "Admin" : "Customer")));
            
            row++;
        } catch (...) {
            // Skip invalid entries
        }
    }
    
    inFile.close();
    usersTable->resizeColumnsToContents();
}

// Include EditUserDialog.h at the end to resolve circular dependency
#include "EditUserDialog.h"

inline void UserManagementWidget::handleEditUser() {
    QList<QTableWidgetItem*> selectedItems = usersTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Edit User", "Please select a user to edit.");
        return;
    }

    int selectedRow = selectedItems.first()->row();
    UserDetails currentUserDetails;
    currentUserDetails.setId(usersTable->item(selectedRow, 0)->text().toInt());
    currentUserDetails.setUsername(usersTable->item(selectedRow, 1)->text());
    currentUserDetails.setEmail(usersTable->item(selectedRow, 2)->text());
    currentUserDetails.setRole(usersTable->item(selectedRow, 3)->text());

    EditUserDialog dialog(currentUserDetails, this);
    if (dialog.exec() == QDialog::Accepted) {
        UserDetails updatedDetails = dialog.getUserDetails();
        
        // Use User class for editing
        User* userObj = User::getUserByID(updatedDetails.getId());
        if (!userObj) {
            QMessageBox::critical(this, "Edit User", "Could not find user to edit.");
            return;
        }
        
        // Update user properties
        userObj->setEmail(updatedDetails.getEmail().toStdString().c_str());
        // Note: Username typically shouldn't change, but we're allowing it here
        userObj->setUsername(updatedDetails.getUsername().toStdString().c_str());
        // Note: Role changes might require more complex logic in a real app
        
        // Set the admin status from updatedDetails
        bool newAdminStatus = (updatedDetails.getRole() == "Admin");
        userObj->setIsAdmin(newAdminStatus);

        if (User::updateUserInFile(*userObj)) {
            QMessageBox::information(this, "Edit User", "User details updated successfully.");
            loadUsers(); // Refresh the table
            emit userUpdated(updatedDetails.getId());
        } else {
            QMessageBox::critical(this, "Edit User", "Failed to update user details.");
        }
        
        delete userObj;
    }
}

inline void UserManagementWidget::handleRemoveUser() {
    QList<QTableWidgetItem*> selectedItems = usersTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Remove User", "Please select a user to remove.");
        return;
    }

    int selectedRow = selectedItems.first()->row();
    int userId = usersTable->item(selectedRow, 0)->text().toInt();
    QString username = usersTable->item(selectedRow, 1)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Remove User", 
                                  QString("Are you sure you want to remove user '%1' (ID: %2)?").arg(username).arg(userId),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (User::removeUser(userId)) {
            QMessageBox::information(this, "Remove User", QString("User '%1' removed successfully.").arg(username));
            loadUsers(); // Refresh the table
            emit userRemoved(userId);
        } else {
            QMessageBox::critical(this, "Remove User", QString("Failed to remove user '%1'.").arg(username));
        }
    }
}

inline void UserManagementWidget::handleAddUser() {
    // For now, admin adding users directly can be complex (e.g., password setting).
    // This can be expanded later if required by the project scope.
    QMessageBox::information(this, "Add User", "User self-registration is preferred. Admin direct add not implemented.");
}

inline void UserManagementWidget::handleRefresh() {
    loadUsers();
}

#endif // USERMANAGEMENTWIDGET_H 