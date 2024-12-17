/* TCP SERVER IMPLEMENTATION */
#include "TcpServer.hpp"


int main() {
    std::vector<int> vect;
    vect.push_back(1024);
    vect.push_back(5250);
    vect.push_back(3453);
    TcpServer server(vect);

    for (int i = 0; i < server.getServerSockets().size(); ++i) {
        std::cout << server.getServerSockets()[i] << std::endl;
    }
}