/* TCP SERVER CLASS IMPLEMENTATION */
#ifndef TCP_SERVER
# define TCP_SERVER

# include <iostream>
# include <string>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <cstdlib>
# include <vector>
# include <poll.h>
# include <fcntl.h>
# include "../../inc/Color.h"

class TcpServer 
{
    public :
        TcpServer(const std::vector<int> & ports);
        ~TcpServer();

        int startServer();

        std::vector<int> getServerSockets();
        
    private :
        std::vector<int> _serverSockets;
        std::vector<int> _ports;
        std::vector<pollfd> _pollFds;
        struct sockaddr_in _serverAddress;
  

        void setupSocket();
        void makeNonBlocking(int socket);
        void acceptNewClient();
        void handleClient(int clientFd);
        void cleanupClient(int index);
};

#endif