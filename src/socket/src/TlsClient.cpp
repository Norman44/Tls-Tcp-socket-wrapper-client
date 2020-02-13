#include "TlsClient.h"

namespace stream {

    TlsClient::
    TlsClient(const std::string& host, int port)
    : TcpClient(host, port) {
        SSL_load_error_strings();
        OpenSSL_add_ssl_algorithms();
        createContext();
    }

    TlsClient::
    ~TlsClient() {
        shutdownSsl(); /* If not shutdown from another thread */

        TcpClient::closeSocket();

        if(ctx_ != nullptr) {
            SSL_CTX_free(ctx_);
        }

        EVP_cleanup();
    }

    int TlsClient::
    tlsConnect() {
        if (tlsInitialize() == -1) { return ERROR; }

        ssl_ = SSL_new(ctx_);

        if (!ssl_) {
            return ERROR;
        }

        SSL_set_fd(ssl_, TcpClient::sock);

        int err = SSL_connect(ssl_);

        if (err <= 0) {
            return ERROR;
        }

        return TcpClient::sock;

    }

    int TlsClient::
    tlsReceive(char *buffer, size_t bufSize) {
        if (ssl_ == nullptr || TcpClient::sock < 0) {
            return ERROR;
        }

        memset(buffer, 0, bufSize);
        int bytes = SSL_read(ssl_, buffer, bufSize);

        return bytes;
    }

    int TlsClient::
    tlsReceive(char* buffer, size_t bufSize, size_t size) {
        if (size > bufSize) { return ERROR; }

        if (ssl_ == nullptr || sock < 0) {
            return ERROR;
        }

        int bytesReceived = 0, bytes = 0;
        memset(buffer, 0, bufSize);

        while (bytesReceived < (int)size) {
            bytes = SSL_read(ssl_, &buffer[bytesReceived], (int)size-bytesReceived);
            bytesReceived += bytes;
            if (bytes == -1) { return bytes; }
            if (bytes == 0) { return bytes; }
        }

        return bytesReceived;
    }

    int TlsClient::
    tlsSend(const char* buffer, size_t bufSize) {
        if (ssl_ == nullptr || TcpClient::sock < 0) {
            return ERROR;
        }
        return SSL_write(ssl_, buffer, bufSize);
    }

    int TlsClient::
    tlsSendAll(const char *buffer, int size) {
        if (ssl_ == nullptr || TcpClient::sock < 0) {
            return ERROR;
        }

        int *len = &size;
        int total = 0;  //How many bytes we've sent
        int bytesleft = *len;   //How many we have left to send
        int n;

        while(total < *len) {
            n = SSL_write(ssl_, buffer+total, bytesleft);
            if (n == -1) { return ERROR; }
            total += n;
            bytesleft -= n;
        }
        *len = total; // Return number actually sent here

        return n == -1 ? -1 : 0; // return -1 on fail, 0 on success
    }

    int TlsClient::
    tlsSendAll(const std::string& str) {
        const char *buff = str.c_str();
        return tlsSendAll(buff, strlen(buff));
    }

    void TlsClient::
    shutdownSsl() {
        if (ssl_ != nullptr) {
            SSL_shutdown(ssl_);
            SSL_free(ssl_);
            ssl_ = nullptr;
        }
    }

    SSL_CTX* TlsClient::
    createContext() {
        const SSL_METHOD *method;
        method = SSLv23_client_method();

        ctx_ = SSL_CTX_new(method);

        return ctx_;
    }

    int TlsClient::
    tlsInitialize() {
        int success = 0;

        if (!ctx_) { return ERROR; }

        if (TcpClient::tcpConnect() == -1) { return ERROR; }

        return success;
    }

} // namespace