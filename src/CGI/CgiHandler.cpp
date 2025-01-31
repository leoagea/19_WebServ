#include "CgiHandler.hpp"

CgiHandler::CgiHandler(std::map<std::string, std::string> envMap) { (void)envMap; }

CgiHandler::~CgiHandler() {}

void     CgiHandler::setMinPrice(uint & minPrice) { _minPrice = minPrice; }

void     CgiHandler::setMaxPrice(uint & maxPrice) { _maxPrice = maxPrice; }

void     CgiHandler::setCurrentDir(std::string &dir) { _currentDir = dir; }

void CgiHandler::executepy(std::string cgi_path)
{
    // int     pipeFd[2];

    // if (pipe(pipeFd) < 0) 
    // {
    //     std::cerr << "Pipe creation failed" << std::endl;
    //     return;
    // }

    std::stringstream ss;

    ss << "MINPRICE=" << _minPrice;
    std::string min = ss.str();
    ss.str("");

    ss << "MAXPRICE=" << _maxPrice;
    std::string max = ss.str();
    ss.str("");

    ss <<"PWD=" << _currentDir;
    std::string dir = ss.str();

    pid_t pid = fork();

    char *envp[] = 
    {
            const_cast<char *>(min.c_str()),
            const_cast<char *>(max.c_str()),
            const_cast<char *>(dir.c_str()),
            NULL
    };

    if (pid < 0) 
    {
        std::cerr << "Fork failed" << std::endl;
        return;
    }

    if (pid == 0)
    {

        std::vector<char *> argv;

        argv.push_back(const_cast<char *>("/usr/bin/python3"));
        argv.push_back(const_cast<char *>(cgi_path.c_str()));
        argv.push_back(NULL);

        execve("/usr/bin/python3", argv.data(), envp);
        
        std::cerr << "Execve failed" << std::endl;
        return;
    }

    waitpid(pid, NULL, 0);
}


void CgiHandler::executego(std::string cgi_path)
{
    pid_t pid = fork();

    if (pid < 0) 
    {
        std::cerr << "Fork failed" << std::endl;
        return;
    }
    if (pid == 0)
    {
        std::string gopath = std::string("GOPATH=") + std::getenv("GOPATH");
        std::string gomodcache = std::string("GOMODCACHE=") + std::getenv("GOMODCACHE");
        std::string qrpath = std::string("QRPATH=") + std::getenv("QRPATH");
        std::string qrhtml = std::string("QRHTML=") + std::getenv("QRHTML");

        char *envp[] = {
            const_cast<char *>(gopath.c_str()),
            const_cast<char *>(gomodcache.c_str()),
            const_cast<char *>("HOME=/tmp"),
            const_cast<char *>("XDG_CACHE_HOME=/tmp"),
            const_cast<char *>("GOCACHE=/tmp/go-build"),
            const_cast<char *>(qrpath.c_str()),
            const_cast<char *>(qrhtml.c_str()),
            NULL
        };

        // std::cout << const_cast<char *>(std::getenv("QRPATH")) << std::endl;
        // std::cout << const_cast<char *>(std::getenv("QRHTML")) << std::endl;
        // std::cout << cgi_path << std::endl;

        std::vector<char *> argv;

        argv.push_back(const_cast<char *>(cgi_path.c_str()));
        argv.push_back(NULL);
        execve(cgi_path.c_str(), argv.data(), envp);

        std::cerr << "Execve failed" << std::endl;
        return;
    }
    
    waitpid(pid, NULL, 0);
}