#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H
#include <iostream>
#include <pqxx/pqxx>
#include <vector>
struct Client{
    int id;
    std::string firstName;
    std::string lastName;
    std::string email;
};

class ClientManager{
public:
    ClientManager(const std::string& connectionString) : conn(connectionString) {}

    void initDbStructure();

    int addClient(const std::string& firstName, const std::string& lastName, const std::string& email);

    void addPhoneNumber(int clientId, const std::string& phoneNumber);

    void updateClient(int clientId, const std::string& firstName, const std::string& lastName, const std::string& email);

    void removePhoneNumber(int clientId, const std::string& phoneNumber);

    void removeClient(int clientId);

    std::vector<Client> findClient(const std::string& searchValue);

private:
    pqxx::connection conn;
};

#endif
