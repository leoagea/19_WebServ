/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListing.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 17:24:55 by lagea             #+#    #+#             */
/*   Updated: 2025/01/02 17:51:02 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DirectoryListing.hpp"

//Throw and runtime error if something failed
std::vector<s_info> DirectoryListing::listDirectory(std::string &path) 
{ 
    std::vector<s_info> results;

    DIR* dir = opendir(path.c_str());
    if (!dir) {
        throw std::runtime_error("Can't open the directory");
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {

        std::string fullPath = path;
        if (!fullPath.empty() && fullPath[fullPath.size()-1] != '/')
            fullPath += '/';
        fullPath += entry->d_name;

        struct stat st;
        if (stat(fullPath.c_str(), &st) == 0) {
            s_info info;
            info.name  = entry->d_name;
            info.raw_size  = st.st_size;
            info.mtime = st.st_mtime;
			info.format_size = DirectoryListing::formatFileSize(info.raw_size);
            
			char dateBuffer[32];
        	struct tm *ltime = std::localtime(&(info.mtime));
			std::strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M:%S", ltime);
			info.format_time = dateBuffer;

            results.push_back(info);
        } else {
            throw std::runtime_error("Can't get the infos from" + fullPath);
        }
    }

    if (closedir(dir) == -1) {
        throw std::runtime_error("Can't close the directory " + path);
    }

    return results;
}

std::string DirectoryListing::formatFileSize(off_t bytes) 
{
    static const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    const int maxIndex = 4;

    double size = static_cast<double>(bytes);
    int i = 0;

    while (size >= 1024.0 && i < maxIndex) {
        size /= 1024.0;
        i++;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << size << " " << units[i];
    return oss.str();
}