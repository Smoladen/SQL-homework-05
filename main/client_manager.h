#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H
#include <iostream>
#include <pqxx/pqxx>

class ClientManager{
public:
    ClientManager(const std::string& connectionString) : conn(connectionString) {}

    void createTable();
    void addUser();
    void addTelephone();
    void updateUser();
    void deleteTelephone();
    void deleteUser();
    void findUser();
private:
    pqxx::connection conn;
};

#endif
