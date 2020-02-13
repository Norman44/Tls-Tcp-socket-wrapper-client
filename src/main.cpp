#include <iostream>
#include <string>

#include "TlsClient.h"

using namespace observit;

int main() {
    char buff[1024];
    std::string str = "Hello back there!";

    std::cout << "TLS client begin ..." << std::endl;

    TlsClient tls("10.2.1.58", 8080);

    if (tls.tlsConnect() == -1) {
        std::cerr << "Error TLS connect" << std::endl;
    }

    if (tls.tlsSendAll(str) == -1) {
        std::cerr << "Error TLS send" << std::endl;
    }

    if (tls.tlsReceive(buff, sizeof buff) == -1) {
        std::cerr << "Error TLS receive" << std::endl;
    }

    tls.shutdownSsl();

    std::cout << buff << std::endl;

    std::cout << "TLS client finish." << std::endl;
    return 0;
}
