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
    std::string connectionString = "host=localhost port=5432 dbname=ClientManager user=postgres password= ";

    ClientManager dbManager(connectionString);
    dbManager.createTable();
    dbManager.addUser();
    dbManager.addTelephone();
    dbManager.updateUser();
    dbManager.deleteTelephone();
    dbManager.deleteUser();
    dbManager.findUser();


    return 0;
}
