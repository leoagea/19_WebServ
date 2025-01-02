#ifndef __REPSONSE__
# define __RESPONSE__

# include <cstdio>
# include "Request.hpp"

enum
{
    GET    = 1,
    POST   = 2,
    DELETE = 3
};

class Response
{
    public :
        Response();
        ~Response();

        void            deleteMethod();

        int             getMethod();

    private :
        const int       _method;
        const char*    _path;
};


#endif