cmake_minimum_required(VERSION 3.15)
project(TlsClient)

set(CMAKE_CXX_STANDARD 11)

set (SSL_LIBS -lssl -lcrypto)

set (CMAKE_CXX_FLAGS "--std=c++11 -finput-charset=utf8 -Wall")

include_directories (src/socket/include src/exception/include)

set (SOURCE_FILES
        src/main.cpp
        src/socket/src/TcpClient.cpp
        src/socket/src/TlsClient.cpp
        src/exception/src/IoException.cpp
        src/exception/src/NetworkException.cpp src/socket/src/TcpClient.cpp src/socket/include/TcpClient.h src/socket/src/TlsClient.cpp src/socket/include/TlsClient.h)

add_executable(TlsClient ${SOURCE_FILES})

target_link_libraries (TlsClient ${SSL_LIBS})