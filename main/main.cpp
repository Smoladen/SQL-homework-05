#include <iostream>
#include <pqxx/pqxx>
#include "client_manager.h"

//#include <Windows.h>

//#pragma execution_character_set( "uft-8" )

#include <tuple>

int main()
{

  /*  setlocale(LC_ALL, "Russian");
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);*/
    try {
    std::string connectionString = "host=localhost port=5432 dbname=ClientManager user=postgres password= Smolkin@2";

    ClientManager dbManager(connectionString);
    dbManager.initDbStructure();
    int clientId = 5;
    dbManager.addClient("Ivan", "Smal", "Vano@gmail.com");
    std::cout << "Client added with" << std::endl;



    dbManager.addPhoneNumber(clientId, "1234567890");

    dbManager.updateClient(clientId, "Victor", "Smal", "smolan222@gmail.com");

    dbManager.removePhoneNumber(clientId, "1234567890");

    std::vector<Client> clients = dbManager.findClient("Denis");
    for (const Client& client : clients) {
        std::cout << "Found client: " << client.firstName << " " << client.lastName << " (" << client.email << ")" << std::endl;
    }

    dbManager.removeClient(clientId);
    std::cout << "Client removed." << std::endl;

    } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
