/* TCP SERVER CLASS IMPLEMENTATION */
#ifndef TCP_SERVER
#define TCP_SERVER

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <signal.h>
#include <vector>
#include <poll.h>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <fcntl.h>
#include <cstring>
#include <stdexcept>
#include "../../inc/Color.h"
#include "Response.hpp"
#include "../config/serverBlock.hpp"
#include "../config/configFileParser.hpp"
#include "../CGI/CgiHandler.hpp"
#include "Cookies.hpp"
#include "ErrorPageGenerator.hpp"

# define REQUEST_HTTP_SIZE 4096
# define R                 "\033[91m"
# define G                 "\033[92m"
# define B                 "\033[94m"
# define IT                "\033[3m"
# define RES               "\033[0m"
 
typedef struct sockaddr_in sockaddr_in;
typedef ServerBlock ServBlo;

class Cookies;

class TcpServer
{
public:
    TcpServer(const std::vector<int> &ports, const ConfigFile &config, std::map<std::string, std::string> envMap);
    ~TcpServer();

    void startServer();

    std::vector<int>        getServerSockets() const; 
    std::vector<int>        getPorts() const;     
    std::vector<pollfd>     getPollFds() const;
    sockaddr_in             getServerAddress() const;
    uint16_t                getSocketPort(int socket);
    std::string             getFullUrl(const std::string& requestBuffer);
    std::string             getDirectoryFromFirstLine(const std::string & method, const std::string & fullUrl);
    int                     getClientFd() const;
    std::string             extractRequestedPath(const std::string &request);
    static void			    generateLog(std::string color, const std::string& message, const char *logType);
    static void             closeFds(int sig);
    void                    handle_signal(void);
    void                    clearIsIndex();
    void                    handleClient(int clientFd);

private :
    int                     _clientFd;
    static const size_t     _maxPollFds = 4096;
    std::vector<int>        _serverSockets;
    std::vector<int>        _ports;
    static std::vector<pollfd>     _pollFds;
    std::map<int, ServBlo>  _clientMap;
    sockaddr_in             _serverAddress;
    std::string             _isIndex;
    std::string             _showPath;
    ConfigFile              _config;
    std::map<std::string, std::string> _envMap;
    std::map<std::string, t_user>      _cookiesMap;
    std::string             _previousUser;
    static bool       _shouldExit;
    
    struct ClientData 
    {
        std::string responseToSend;
        size_t      bytesSent;
        bool        responseReady;
        
        ClientData() : bytesSent(0), responseReady(false) {}
    };
	
    struct ClientRequest 
    {
        std::string buffer;
        bool        headerComplete;
        size_t      contentLength;
        size_t      contentReceived;
        
        ClientRequest() : headerComplete(false), contentLength(0), contentReceived(0) {}
    };
    
    std::map<int, ClientData> _clientResponseMap;
    std::map<int, ClientRequest> _clientRequestMap;

    std::string     resolvePath(const std::string &requestedPath, int clientFd);
    void            setupSocket();
    void            makeNonBlocking(int socket);
    void            acceptNewClient(int serverSocket);

    void            handleClientWrite(int clientFd);
    void            cleanupClient(int fd);
    void            exitCloseFds(std::vector<int> &serverSockets);
    t_user          parseCookies(int clientFd, const std::string &, const std::string &);
    ServerBlock     getServerBlockBySocket(int);
    size_t          getRequestBody(const std::string &request);
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
