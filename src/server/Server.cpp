/* TCP SERVER IMPLEMENTATION */
#include "TcpServer.hpp"

int main() {
    std::vector<int> ports = {8080, 9090, 10101, 2341, 5643, 2135, 9999, 9001, 65000};
    TcpServer server(ports);
    server.startServer();

    return 0;
}