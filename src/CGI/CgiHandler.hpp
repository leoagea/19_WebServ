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
        CgiHandler(std::map<std::string, std::string> envMap);
        ~CgiHandler();

        void     executepy(std::string cgi_path);
        void     executego();

    private :
        std::map<std::string, std::string>  _envpMap;
        std::vector<char *>                 _envpVect;

};


#endif