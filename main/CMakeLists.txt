cmake_minimum_required(VERSION 3.20)

project(main LANGUAGES CXX)

set(PostgreSQL_ADDITIONAL_VERSIONS "16")

add_subdirectory("C:\\Den\\libpq++\\libpqxx-7.9.1" libpqxx-build)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp client_manager.cpp client_manager.h)

target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main PRIVATE pqxx)

include(GNUInstallDirs)
install(TARGETS main
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
