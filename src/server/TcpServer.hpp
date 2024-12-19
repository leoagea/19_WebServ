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
# include <algorithm>
# include <fcntl.h>
# include "../../inc/Color.h"

# define REQUEST_HTTP_SIZE 4096
# define R                 "\033[91m"
# define G                 "\033[92m"
# define B                 "\033[94m"
# define IT                "\033[3m"
# define RES               "\033[0m"

typedef struct sockaddr_in sockaddr_in;

class TcpServer 
{
    public :
        TcpServer(const std::vector<int> & ports);
        ~TcpServer();

        void startServer();

        static size_t getPollCount();
        std::vector<int> getServerSockets(); 
        std::vector<int> getPorts();     
        std::vector<pollfd> getPollFds();
        sockaddr_in getServerAddress();
        
    private :
        static const size_t _maxPollFds = 4096;
        static size_t _pollCount;
        std::vector<int> _serverSockets;    // Fd's from server socket only
        std::vector<int> _ports;
        std::vector<pollfd> _pollFds;       // Basically all fd's server and client
        sockaddr_in _serverAddress;

        void setupSocket();
        void makeNonBlocking(int socket);
        void acceptNewClient(int serverSocket);
        void handleClient(int clientFd);
        void cleanupClient(int fd);
        void exitCloseFds(std::vector<int> &serverSockets);
};

#endif

/*      From <netinet/in.h> :
*
*       struct sockaddr_in {
*           sa_family_t     sin_family; IPv4/IPv6
*           in_port_t       sin_port;   Port de connexion
*           struct in_addr  sin_addr;
*       };
*       
*       struct in_addr {
*           uint32_t        s_addr;     Representation en entier d'une addresse IP entrante
*       };
*
*
*
*       struct pollfd {
*           int   fd;          File descriptor 
*           short events;      Requested events 
*           short revents;     Returned events 
*       };
*/