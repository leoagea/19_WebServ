/* TCP SERVER CLASS IMPLEMENTATION */
#ifndef  TCP_SERVER
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
# include <cctype>
# include <ctime>
# include <fcntl.h>
# include <stdexcept>
# include "../../inc/Color.h"
# include "Response.hpp"
# include "../config/serverBlock.hpp"
# include "../config/configFileParser.hpp"

# define REQUEST_HTTP_SIZE 8192
# define R                 "\033[91m"
# define G                 "\033[92m"
# define B                 "\033[94m"
# define IT                "\033[3m"
# define RES               "\033[0m"

typedef struct sockaddr_in sockaddr_in;
typedef        ServerBlock ServBlo;

typedef struct Client
{
        int             server_fd;
        int             client_fd;
        ServerBlock     server;
}       Client;

class TcpServer 
{
    public :
        TcpServer(const std::vector<int> & ports, const ConfigFile &config);
        ~TcpServer();

        void                    startServer();

        std::vector<int>        getServerSockets(); 
        std::vector<int>        getPorts();     
        std::vector<pollfd>     getPollFds();
        sockaddr_in             getServerAddress();
        uint16_t                getSocketPort(int socket);

		std::string             resolvePath(const std::string &requestedPath);
		std::string             extractRequestedPath(const std::string &request);
		bool                    fileExists(const std::string& path);
        void			        generateLog(std::string color, const std::string& message, const char *logType);
        
    private :
        static const size_t     _maxPollFds = 4096;
        std::vector<int>        _serverSockets;
        std::vector<int>        _ports;
        std::vector<pollfd>     _pollFds;
        std::map<int, ServBlo>  _clientMap;
        sockaddr_in             _serverAddress;
        ConfigFile              _config;

        void                    setupSocket();
        void                    makeNonBlocking(int socket);
        void                    acceptNewClient(int serverSocket);
        void                    handleClient(int clientFd);
        void                    cleanupClient(int fd);
        void                    exitCloseFds(std::vector<int> &serverSockets);

        ServerBlock             getServerBlockBySocket(int);
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
