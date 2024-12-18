/* TCP SERVER IMPLEMENTATION */
#include "TcpServer.hpp"

int main() {
    std::vector<int> ports = {8080, 9090};
    TcpServer server(ports);
    server.startServer();
    return 0;
}