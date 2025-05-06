#ifndef __CGI_HANDLER__
#define __CGI_HANDLER__

#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>

typedef unsigned int uint;

class CgiHandler
{
public:
    CgiHandler(std::map<std::string, std::string> envMap);
    ~CgiHandler();

    int  executepy(std::string cgi_path);
    int  executego(std::string cgi_path);

    void setMinPrice(uint &minPrice);
    void setMaxPrice(uint &maxPrice);
    void setCurrentDir(std::string &dir);

private:
    std::map<std::string, std::string> _envpMap;
    std::vector<char *> _envpVect;
    int _minPrice;
    int _maxPrice;
    std::string _currentDir;

    int waitProcessWithTimeout(pid_t pid, int timeoutSeconds);
};

#endif