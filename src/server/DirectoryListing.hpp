/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListing.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmailleu <kmailleu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 17:21:09 by lagea             #+#    #+#             */
/*   Updated: 2025/02/06 17:26:46 by kmailleu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __DIRECTORYLISTING_HPP__
# define __DIRECTORYLISTING_HPP__

#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <stdexcept>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "../../inc/Struct.h"

namespace DirectoryListing
{
    std::vector<s_info> listDirectory(std::string &path);
    std::string formatFileSize(off_t bytes);
    std::string generateDirectoryListingHTML(const std::string &directoryName, const std::string &showPath, const std::vector<s_info> &files);
}

#endif