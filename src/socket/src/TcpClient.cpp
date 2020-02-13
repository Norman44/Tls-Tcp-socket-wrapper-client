#include "TcpClient.h"

#include <unistd.h>
#include <cstring>
#include <chrono>
#include <iostream>

namespace stream {
    const int TcpClient::ERROR = -1;

    TcpClient::
    TcpClient(const std::string& host, int port)
    :   sock(-1),
        host_(host),
        port_(std::to_string(port)),
        logString_(host + ":" + std::to_string(port)) {}

    TcpClient::
    ~TcpClient() {}

    int TcpClient::
    tcpConnect() {
        closeSocket();
        struct addrinfo hints{};
        struct addrinfo *ai;
        struct addrinfo *p;
        int res;
        int sfd;

        closeSocket();
        bzero(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        res = getaddrinfo(host_.c_str(), port_.c_str(), &hints, &ai);
        if (res != 0) {
            return (-1);
        }

        for (p = ai; p != nullptr; p = p->ai_next) {
            sfd = socket(p->ai_family, p->ai_socktype,
                         p->ai_protocol);
            if (sfd == -1) {
                continue;
            }

            if (::connect(sfd, p->ai_addr, p->ai_addrlen) != -1) {
                sock = sfd;
                break;
            } else {
                ::close(sfd);
            }
        }

        freeaddrinfo(ai);

        if (p == nullptr) {               /* No address succeeded */
            return (-1);
        }

        return sock;
    }

    int TcpClient::
    tcpReceive(char* buffer, size_t bufSize) {
        memset(buffer, 0, bufSize);

        return recv(sock, buffer, bufSize, 0);
    }

    int TcpClient::
    tcpReceive(int sock, char* buffer, size_t bufSize) {
        memset(buffer, 0, bufSize);

        return recv(sock, buffer, bufSize, 0);
    }

    int TcpClient::
    tcpReceive(char* buffer, size_t bufSize, size_t size) {
        int bytesReceived = 0, bytes = 0;
        memset(buffer, 0, bufSize);

        while (bytesReceived < (int)size) {
            bytes = recv(sock, &buffer[bytesReceived], size - bytesReceived, 0);
            bytesReceived += bytes;
            if (bytes == -1) { return bytes; }
            if (bytes == 0) { return bytes; }
        }

        return bytesReceived;
    }

    int TcpClient::
    tcpReceive(int sock, char* buffer, size_t bufSize, size_t size) {
        int bytesReceived = 0, bytes = 0;
        memset(buffer, 0, bufSize);

        while (bytesReceived < (int)size) {
            bytes = recv(sock, &buffer[bytesReceived], size - bytesReceived, 0);
            bytesReceived += bytes;
            if (bytes == -1) { return bytes; }
            if (bytes == 0) { return bytes; }
        }

        return bytesReceived;
    }

    int TcpClient::
    tcpSend(const char *buffer, size_t size) {
        size_t *len = &size;
        int total = 0;  //How many bytes we've sent
        int bytesLeft = *len;   //How many we have left to send
        int n;

        while(total < (int)*len) {
            n = send(sock, buffer+total, bytesLeft, MSG_NOSIGNAL);
            if (n == -1) break;
            total += n;
            bytesLeft -= n;
        }
        *len = total; // Return number actually sent here

        return n == -1 ? -1 : 0; // return -1 on fail, 0 on success
    }

    int TcpClient::
    tcpSend(int sock, const char *buffer, int size) {
        int *len = &size;
        int total = 0;  //How many bytes we've sent
        int bytesLeft = *len;   //How many we have left to send
        int n;

        while(total < *len) {
            n = send(sock, buffer+total, bytesLeft, MSG_NOSIGNAL);
            if (n == -1) break;
            total += n;
            bytesLeft -= n;
        }
        *len = total; // Return number actually sent here

        return n == -1 ? -1 : 0; // return -1 on fail, 0 on success
    }


    int TcpClient::
    tcpSend(const std::string& str) {
        const char *buff = str.c_str();
        return tcpSend(buff, strlen(buff));
    }

    int TcpClient::
    tcpSend(int sock, const std::string& str) {
        const char *buff = str.c_str();
        return TcpClient::tcpSend(sock, buff, strlen(buff));
    }

    void TcpClient::
    setSocketTimeout(int time_s) {
        tv_.tv_sec = time_s;
        tv_.tv_usec = 0;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_, sizeof tv_);
    }

    void TcpClient::
    setSocketTimeout(int sock, int time_s) {
        struct timeval tv{};
        tv.tv_sec = time_s;
        tv.tv_usec = 0;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    }

    int TcpClient::
    isSocketActive() {
        int error = 0;
        socklen_t len = sizeof (error);
        int retval = getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &len);

        if (retval != 0) {
            return -1;
        }
        if (error != 0) {
            return -1;
        }

        return 0;
    }

    int TcpClient::
    isSocketActive(int sock) {
        int error = 0;
        socklen_t len = sizeof (error);
        int ret = getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &len);

        if (ret != 0) {
            return -1;
        }
        if (error != 0) {
            return -1;
        }

        return 0;
    }

    int TcpClient::
    closeSocket() {
        if(sock != -1) {
            ::close(sock);
            sock = -1;
            return 0;
        }
        return -1;
    }

} // namespace
