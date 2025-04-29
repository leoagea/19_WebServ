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

#define REQUEST_HTTP_SIZE 8192
#define R "\033[91m"
#define G "\033[92m"
#define B "\033[94m"
#define IT "\033[3m"
#define RES "\033[0m"

typedef struct sockaddr_in sockaddr_in;
typedef ServerBlock ServBlo;

class Cookies;

class TcpServer
{
public:
    TcpServer(const std::vector<int> &ports, const ConfigFile &config, std::map<std::string, std::string> envMap);
    ~TcpServer();

    void startServer();

    std::vector<int> getServerSockets();
    std::vector<int> getPorts();
    std::vector<pollfd> getPollFds();
    sockaddr_in getServerAddress();
    uint16_t getSocketPort(int socket);
    std::string getFullUrl(const std::string &requestBuffer);
    std::string getDirectoryFromFirstLine(const std::string &method, const std::string &fullUrl);
    std::string extractRequestedPath(const std::string &request);
    bool fileExists(const std::string &path);
    static void generateLog(std::string color, const std::string &message, const char *logType);
    static void closeFds(int sig);
    void handle_signal(void);

private:
    static const size_t _maxPollFds = 4096;
    std::vector<int> _serverSockets;
    std::vector<int> _ports;
    static std::vector<pollfd> _pollFds;
    std::map<int, ServBlo> _clientMap;
    sockaddr_in _serverAddress;
    std::string _isIndex;
    std::string _showPath;
    ConfigFile _config;
    std::map<std::string, std::string> _envMap;
    std::map<std::string, t_user> _cookiesMap;
    std::string _previousUser;

    std::string resolvePath(const std::string &requestedPath, int clientFd);
    void setupSocket();
    void makeNonBlocking(int socket);
    void acceptNewClient(int serverSocket);
    void handleClient(int clientFd);
    void cleanupClient(int fd);
    void exitCloseFds(std::vector<int> &serverSockets);
    t_user parseCookies(int clientFd, const std::string &, const std::string &);

    ServerBlock getServerBlockBySocket(int);
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
