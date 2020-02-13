#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>

#include "NetworkException.h"

namespace stream {

/**
 * Tcp client socket class socket wrapper
 */
    class TcpClient {
    public:
        static const int ERROR;

        /**
         * Tcp server socket
         * @param port
         */
        TcpClient(const std::string& host, int port);

        ~TcpClient();

        /**
         * TCP client server connect
         * @return socket fd, -1 on error
         */
        int tcpConnect();

        /**
         * Tcp receive
         * @param  buffer  - buffer to receive to
         * @param  bufSize - buffer size
         * @return bytes received or -1 on fail
         */
        int tcpReceive(char* buffer, size_t bufSize);

        static int tcpReceive(int sock, char* buffer, size_t bufSize);

        /**
         * Tcp receive bytes of size. "size" must not exceed bufSize value.
         * @param  buffer  - buffer to receive to
         * @param  bufSize - size of a receiving buffer
         * @param  size     - bytes to receive
         * @return bytes received
         */
        int tcpReceive(char* buffer, size_t bufSize, size_t size);

        static int tcpReceive(int sock, char* buffer, size_t bufSize, size_t size);

        /**
         * TCP send bytes
         * @param  buffer
         * @param  size - number of bytes to send
         * @return bytes sent on success -1 on error
         */
        int tcpSend(const char *buffer, size_t size);

        static int tcpSend(int sock, const char *buffer, int size);

        /**
         * TCP send string container
         * @param  str - string container to send
         * @return bytes sent on success -1 on error
         */
        int tcpSend(const std::string& str);

        static int tcpSend(int sock, const std::string& str);

        /**
         * Set timeout on socket
         * @param time_s - time in seconds
         */
        void setSocketTimeout(int time_s);

        static void setSocketTimeout(int sock, int time_s);

        /**
         * Check if new socket is active
         * @return -1 if socket not active, else 0
         */
        int isSocketActive();

        static int isSocketActive(int sock);

        /**
         * Close socket
         * @return 0 on succes, -1 otherwise
         */
        int closeSocket();

        const char* c_str() { return logString_.c_str(); }

        int sock;

    private:
        std::string host_;
        std::string port_;
        std::string logString_;

        struct timeval tv_;

    };

} // namespace

#endif
