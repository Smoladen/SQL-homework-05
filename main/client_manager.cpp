#include "client_manager.h"



void ClientManager::initDbStructure(){

    pqxx::work W(conn);

    std::string users =
        "CREATE TABLE IF NOT EXISTS users ("
        "ID SERIAL PRIMARY KEY, "
        "firstName TEXT NOT NULL, "
        "lastName TEXT NOT NULL, "
        "\"email\" TEXT NOT NULL"
        ");";
    std::string phones =
        "CREATE TABLE IF NOT EXISTS user_telephones ("
        "telephoneId SERIAL PRIMARY KEY, "
        "userId INTEGER REFERENCES users(ID) ON DELETE CASCADE, "
        "telephone TEXT"
        ");";;

    W.exec(users);
    W.exec(phones);
    W.commit();
    }

int ClientManager::addClient(const std::string& firstName, const std::string& lastName, const std::string& email) {
    pqxx::work W(conn);

    pqxx::result r = W.exec(
        "INSERT INTO users (firstName, lastName, \"email\") "
        "VALUES (" + W.quote(firstName) + ", " + W.quote(lastName) + ", " + W.quote(email) + ") "
        "RETURNING ID;"
        );

    W.commit();
    return r[0][0].as<int>();
}


void ClientManager::addPhoneNumber(int clientId, const std::string& phoneNumber) {

    pqxx::work W(conn);

    W.exec(
        "INSERT INTO user_telephones (userId, telephone)"
        "VALUES (" + W.quote(clientId) + ", " + W.quote(phoneNumber) + ");"
    );
    W.commit();

};
void ClientManager::updateClient(int clientId, const std::string& firstName, const std::string& lastName, const std::string& email) {
    pqxx::work W(conn);
    W.exec(
        "UPDATE users SET firstName = " + W.quote(firstName)+
        ", lastName = " + W.quote(lastName) +
        ", \"email\" = " + W.quote(email) +
        "WHERE ID = " + W.quote(clientId) + ";"
    );
    W.commit();

};


void ClientManager::removePhoneNumber(int clientId, const std::string& phoneNumber) {
    pqxx::work W(conn);

    W.exec(
        "DELETE FROM user_telephones WHERE userId = " + W.quote(clientId) +
        " AND telephone = " + W.quote(phoneNumber) + ";"
    );
    W.commit();
};

void ClientManager::removeClient(int clientId) {
    pqxx::work W(conn);
    W.exec("DELETE FROM users WHERE ID = " + W.quote(clientId) + ";");
    W.commit();
};

std::vector<Client> ClientManager::findClient(const std::string& searchValue) {
    pqxx::work W(conn);

    pqxx::result r = W.exec(
        "SELECT * FROM users WHERE firstName = " + W.quote(searchValue) +
        "OR lastName = " + W.quote(searchValue) +
        "OR \"email\" = " + W.quote(searchValue) +
        "OR ID IN (SELECT userId FROM user_telephones WHERE telephone = " + W.quote(searchValue) + ");"
    );

    std::vector<Client> clients;
    for (const auto& row : r) {
        clients.push_back(Client{
            row["ID"].as<int>(),
            row["firstName"].as<std::string>(),
            row["lastName"].as<std::string>(),
            row["email"].as<std::string>()
        });
    }
    return clients;
};
