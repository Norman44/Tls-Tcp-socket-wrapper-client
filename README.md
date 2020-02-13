# Tls-Tcp-socket-wrapper-client
C++ Client tls tcp socket wrapper

## Requirements
Install cmake and openssl dev lib ...
```
sudo apt-get install libssl-dev
```

Create certificate and key ...

## Compile 
```
cmake CMakeLists.txt
```
```
make all
```
```
./TlsClient
```

## Example
```cpp
#include <iostream>
#include <string>

#include "TlsClient.h"

using namespace stream;

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

```

## Server
Client code available https://github.com/Norman44/Tls-Tcp-socket-wrapper