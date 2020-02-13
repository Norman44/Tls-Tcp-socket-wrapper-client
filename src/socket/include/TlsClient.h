#ifndef TCPTLSCLIENTSOCKET_H
#define TCPTLSCLIENTSOCKET_H

#include "TcpClient.h"

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
#include <cstring>

namespace stream {

    class TlsClient : public TcpClient {
    public:
        TlsClient(const std::string& host, int port);

        ~TlsClient();

        int tlsConnect();

        int tlsReceive(char* buffer, size_t bufSize);

        int tlsReceive(char* buffer, size_t bufSize, size_t size);

        int tlsSend(const char* buffer, size_t bufSize);

        int tlsSendAll(const char *buffer, int size);

        int tlsSendAll(const std::string& msg);

        void shutdownSsl();

    private:
        SSL* ssl_ = nullptr;
        SSL_CTX* ctx_ = nullptr;

        SSL_CTX* createContext();

        /**
         * Initialize TLS
         * @return 0 on success or -1 on error
         */
        int tlsInitialize();




    };

} // namespace

#endif
