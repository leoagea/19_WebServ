#include "Response.hpp"

Response::Response() : _method(DELETE), _path("var/www/html/deletableFile") {}

Response::~Response() {}

void Response::deleteMethod()
{
    remove(_path);
}

int Response::getMethod() { return _method; }