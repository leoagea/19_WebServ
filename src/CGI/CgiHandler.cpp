#include "CgiHandler.hpp"

CgiHandler::CgiHandler(std::map<std::string, std::string> envMap) { (void)envMap; }

CgiHandler::~CgiHandler() {}

void CgiHandler::executepy(std::string cgi_path)
{
    // int     pipeFd[2];

    // if (pipe(pipeFd) < 0) 
    // {
    //     std::cerr << "Pipe creation failed" << std::endl;
    //     return;
    // }

    pid_t pid = fork();

    if (pid < 0) 
    {
        std::cerr << "Fork failed" << std::endl;
        return;
    }

    if (pid == 0)
    {
        // close(pipeFd[0]);
        // dup2(pipeFd[1], STDOUT_FILENO);
        // close(pipeFd[1]);

        std::vector<char *> argv;

        argv.push_back(const_cast<char *>("/usr/bin/python3"));
        argv.push_back(const_cast<char *>(cgi_path.c_str()));
        argv.push_back(NULL);

        execve("/usr/bin/python3", argv.data(), NULL);
        
        std::cerr << "Execve failed" << std::endl;
        return;
    }

    // if (pid > 0)
    // {
    //     close(pipeFd[1]);

    //     char    buffer[8096];
    //     int     bytesRead = read(pipeFd[0], buffer, sizeof(buffer) - 1);

    //     if (bytesRead > 0)
    //     {
    //         buffer[bytesRead] = '\0';
    //         _scriptOutput     = buffer;
    //     }

    //     close(pipeFd[0]);
    //     waitpid(pid, nullptr, 0);
    // }

    // return _scriptOutput;
}


void CgiHandler::executego(std::string cgi_path)
{
    // int     pipeFd[2];

    // if (pipe(pipeFd) < 0) 
    // {
    //     std::cerr << "Pipe creation failed" << std::endl;
    //     return;
    // }

    pid_t pid = fork();

    if (pid < 0) 
    {
        std::cerr << "Fork failed" << std::endl;
        return;
    }

    if (pid == 0)
    {
        // close(pipeFd[0]);
        // dup2(pipeFd[1], STDOUT_FILENO);
        // close(pipeFd[1]);

        std::vector<char *> argv;

        argv.push_back(const_cast<char *>("rungo"));
        argv.push_back(const_cast<char *>(cgi_path.c_str()));
        argv.push_back(NULL);

        execve("/usr/bin/go", argv.data(), NULL);

        std::cerr << "Execve failed" << std::endl;

        return;
    }

    // if (pid > 0)
    // {
    //     close(pipeFd[1]);

    //     char    buffer[8096];
    //     int     bytesRead = read(pipeFd[0], buffer, sizeof(buffer) - 1);

    //     if (bytesRead > 0)
    //     {
    //         buffer[bytesRead] = '\0';
    //         _scriptOutput     = buffer;
    //     }

    //     close(pipeFd[0]);
    //     waitpid(pid, nullptr, 0);
    // }

    // return _scriptOutput;
}