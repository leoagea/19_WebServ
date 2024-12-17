#ifndef __PATHCHECKING_CPP__
# define __PATHCHECKING_CPP__

#include <string>
#include <sys/stat.h>

namespace PathChecking
{
    bool exist(std::string &path){
        struct stat fileInfo;
        return stat(path.c_str(), &fileInfo) == 0;
    }

    bool isFile(std::string &path){
        struct stat fileInfo;
        stat(path.c_str(), &fileInfo);
        return S_ISREG(fileInfo.st_mode);
    }

    bool isDirectory(std::string &path){
        struct stat fileInfo;
        stat(path.c_str(), &fileInfo);
        return S_ISDIR(fileInfo.st_mode);
    }

    bool getReadPermission(std::string &path){
        struct stat fileInfo;
        stat(path.c_str(), &fileInfo);
        return S_IRUSR & fileInfo.st_mode;
    }

    bool getWritePermission(std::string &path){
        struct stat fileInfo;
        stat(path.c_str(), &fileInfo);
        return S_IWUSR & fileInfo.st_mode;
    }

    bool getExecPermission(std::string &path){
        struct stat fileInfo;
        stat(path.c_str(), &fileInfo);
        return S_IXUSR & fileInfo.st_mode;
    }
}


#endif