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

        void            m_delete();
	void		m_post();
	void		m_get();

        int             getMethod();

    private :
        const int       _method;
        const char*     _path;
	std::string	_response;
};


#endif
