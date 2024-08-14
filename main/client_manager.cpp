#include "client_manager.h"

void ClientManager::createTable(){
        try {
            pqxx::work W(conn);

            std::string users =
                "CREATE TABLE IF NOT EXISTS users ("
                "ID SERIAL PRIMARY KEY, "
                "NAME TEXT NOT NULL, "
                "SURNAME TEXT NOT NULL, "
                "\"email\" TEXT NOT NULL"
                ");";
            std::string usertelephone =
                "CREATE TABLE IF NOT EXISTS user_telephones ("
                "telephoneId SERIAL PRIMARY KEY, "
                "userId INTEGER REFERENCES users(ID) ON DELETE CASCADE, "
                "telephone TEXT"
                ");";;

            W.exec(users);
            W.exec(usertelephone);
            W.commit();
            std::cout << "Table created." << std::endl;
        } catch (const std::exception &e) {
            std::cout<< "Exception Happened: " << e.what() << std::endl;
        }
    };

void ClientManager::addUser(){
        try {
            std::string name, surname, email;

            std::cout << "Enter name: ";
            std::cin >> name;

            std::cout << "Enter surname: ";
            std::cin >> surname;

            std::cout << "Enter email: ";
            std::cin >> email;

            pqxx::work W(conn);

            std::string sql =
                "INSERT INTO users (name, surname, \"email\") "
                "VALUES (" + W.quote(name) + ", " + W.quote(surname) + ", " + W.quote(email) + ");";

            W.exec(sql);

            W.commit();
            std::cout << "User added successfully!" << std::endl;
        } catch (const std::exception &e) {
            std::cout << "Error adding user: " << e.what() << std::endl;
        }
};

void ClientManager::addTelephone() {
        try {
            int userId;
            std::string telephone;

            std::cout << "Enter user ID: ";
            std::cin >> userId;

            std::cout << "Enter telephone number: ";
            std::cin >>  telephone;

            pqxx::work W(conn);

            std::string findUserById =
                "SELECT ID FROM users WHERE ID = " + W.quote(userId) + ";";

            pqxx::result r = W.exec(findUserById);

            if (r.empty()) {
                std::cout << "No user found with the given ID." << std::endl;
                return;
            }

            std::string addTel =
                "INSERT INTO user_telephones (userId, telephone) "
                "VALUES (" + W.quote(userId) + ", " + W.quote(telephone) + ");";

            W.exec(addTel);
            W.commit();

            std::cout << "Telephone added successfully!" << std::endl;
        } catch (const std::exception &e) {
            std::cout << "Error adding telephone: " << e.what() << std::endl;
        }
};
void ClientManager::updateUser() {
        try {
            int userId;
            std::string field, newValue;

            std::cout << "Enter user ID: ";
            std::cin >> userId;

            std::cout << "Which field do you want to update? (name/surname/email): ";
            std::cin >> field;

            if (field != "name" && field != "surname" && field != "email") {
                std::cout << "Invalid field. Please choose 'name', 'surname', or 'email'." << std::endl;
                return;
            }

            std::cout << "Enter new value for " << field << ": ";
            std::cin.ignore();
            std::getline(std::cin, newValue);

            pqxx::work W(conn);

            std::string updateQuery;
            if (field == "email") {
                updateQuery = "UPDATE users SET \"email\" = " + W.quote(newValue) + " WHERE ID = " + W.quote(userId) + ";";
            } else {
                updateQuery = "UPDATE users SET " + field + " = " + W.quote(newValue) + " WHERE ID = " + W.quote(userId) + ";";
            }

            W.exec(updateQuery);
            W.commit();

            std::cout << "User's " << field << " updated successfully!" << std::endl;
        } catch (const std::exception &e) {
            std::cout << "Error updating user: " << e.what() << std::endl;
        }
};


void ClientManager::deleteTelephone() {
        try {
            int userId;
            std::string telephone;

            std::cout << "Enter user ID: ";
            std::cin >> userId;

            pqxx::work W(conn);

            std::string deleteQuery =
                "DELETE FROM user_telephones WHERE userId = " + W.quote(userId) +";";

            W.exec(deleteQuery);
            W.commit();

            std::cout << "Telephone deleted successfully!" << std::endl;
        } catch (const std::exception &e) {
            std::cout << "Error deleting telephone: " << e.what() << std::endl;
        }
};
void ClientManager::deleteUser() {
        try {
            int userId;

            std::cout << "Enter user ID to delete: ";
            std::cin >> userId;

            pqxx::work W(conn);

            std::string deleteQuery =
                "DELETE FROM users WHERE ID = " + W.quote(userId) + ";";

            W.exec(deleteQuery);
            W.commit();

            std::cout << "User and all associated telephones deleted successfully!" << std::endl;
        } catch (const std::exception &e) {
            std::cout << "Error deleting user: " << e.what() << std::endl;
        }
};

void ClientManager::findUser() {
        try {
            std::string searchField, searchValue;

            std::cout << "Search by (name/surname/email/telephone): ";
            std::cin >> searchField;

            if (searchField != "name" && searchField != "surname" && searchField != "email" && searchField != "telephone") {
                std::cout << "Invalid search field. Please choose 'name', 'surname', 'email', or 'telephone'." << std::endl;
                return;
            }

            std::cout << "Enter " << searchField << " to search: ";
            std::cin.ignore();
            std::getline(std::cin, searchValue);

            pqxx::work W(conn);
            pqxx::result r;


            if (searchField == "name") {
                r = W.exec("SELECT * FROM users WHERE name = " + W.quote(searchValue) + ";");
            } else if (searchField == "surname") {
                r = W.exec("SELECT * FROM users WHERE surname = " + W.quote(searchValue) + ";");
            } else if (searchField == "email") {
                r = W.exec("SELECT * FROM users WHERE \"email\" = " + W.quote(searchValue) + ";");
            } else if (searchField == "telephone") {
                r = W.exec("SELECT u.* FROM users u "
                           "JOIN user_telephones ut ON u.ID = ut.userId "
                           "WHERE ut.telephone = " + W.quote(searchValue) + ";");
            }

            if (r.empty()) {
                std::cout << "No user found with the given " << searchField << "." << std::endl;
            } else {
                for (const auto& row : r) {
                    std::cout << "User found: " << std::endl;
                    std::cout << "ID: " << row["id"].as<int>() << std::endl;
                    std::cout << "Name: " << row["name"].as<std::string>() << std::endl;
                    std::cout << "Surname: " << row["surname"].as<std::string>() << std::endl;
                    std::cout << "Email: " << row["email"].as<std::string>() << std::endl;
                    std::cout << "--------" << std::endl;
                }
            }

            W.commit();
        } catch (const std::exception &e) {
            std::cout << "Error searching for user: " << e.what() << std::endl;
        }
};
