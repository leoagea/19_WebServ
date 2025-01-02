#ifndef  __CGI_HANDLER__
# define __CGI_HANDLER__

# include "../server/Request.hpp"
# include <string>
# include <vector>
# include <map>
# include <cstdlib>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <iostream>
# include <sstream>

class CgiHandler
{
    public :
        CgiHandler(Request &request, std::string &scriptPath);
        ~CgiHandler();

        std::string     execute();

    private :
        std::map<std::string, std::string>  _envVar;
        std::string                         _scriptPath;
        std::string                         _scriptOutput;
};


#endif