/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListing.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 17:24:55 by lagea             #+#    #+#             */
/*   Updated: 2025/04/29 15:42:31 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DirectoryListing.hpp"

std::vector<s_info> DirectoryListing::listDirectory(std::string &path)
{
    std::vector<s_info> results;

    DIR *dir = opendir(path.c_str());
    if (!dir)
    {
        throw std::runtime_error("Can't open the directory");
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {

        std::string fullPath = path;
        if (!fullPath.empty() && fullPath[fullPath.size() - 1] != '/')
            fullPath += '/';
        fullPath += entry->d_name;

        struct stat st;
        if (stat(fullPath.c_str(), &st) == 0)
        {
            s_info info;
            info.name = entry->d_name;
            info.fullpath = fullPath;
            info.raw_size = st.st_size;
            info.mtime = st.st_mtime;
            info.format_size = DirectoryListing::formatFileSize(info.raw_size);

            char dateBuffer[32];
            struct tm *ltime = std::localtime(&(info.mtime));
            std::strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d %H:%M:%S", ltime);
            info.format_time = dateBuffer;

            results.push_back(info);
        }
        else
        {
            throw std::runtime_error("Can't get the infos from" + fullPath);
        }
    }

    if (closedir(dir) == -1)
    {
        throw std::runtime_error("Can't close the directory " + path);
    }

    return results;
}

std::string DirectoryListing::formatFileSize(off_t bytes)
{
    static const char *units[] = {"B", "KB", "MB", "GB", "TB"};
    const int maxIndex = 4;

    double size = static_cast<double>(bytes);
    int i = 0;

    while (size >= 1024.0 && i < maxIndex)
    {
        size /= 1024.0;
        i++;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << size << " " << units[i];
    return oss.str();
}

std::string DirectoryListing::generateDirectoryListingHTML(const std::string &directoryName, const std::string &showPath, const std::vector<s_info> &files)
{
    std::ostringstream html;
    (void)directoryName;
    html << "<!DOCTYPE html>\n"
         << "<html lang=\"fr\">\n"
         << "<head>\n"
         << "  <meta charset=\"utf-8\">\n"
         << "  <title>Index of " << showPath << "</title>\n"
         << "  <style>\n"
         << "    body {\n"
         << "      font-family: Arial, sans-serif;\n"
         << "      background-color: #f8f8f8;\n"
         << "      color: #333;\n"
         << "      margin: 0;\n"
         << "      padding: 20px;\n"
         << "    }\n"
         << "    h1 {\n"
         << "      background: #2c3e50;\n"
         << "      color: #ecf0f1;\n"
         << "      padding: 15px;\n"
         << "      margin-top: 0;\n"
         << "      border-radius: 4px;\n"
         << "    }\n"
         << "    table {\n"
         << "      width: 100%;\n"
         << "      max-width: 960px;\n"
         << "      border-collapse: collapse;\n"
         << "      margin: 20px auto;\n"
         << "      box-shadow: 0 2px 5px rgba(0,0,0,0.1);\n"
         << "    }\n"
         << "    th, td {\n"
         << "      padding: 12px 15px;\n"
         << "      border-bottom: 1px solid #ccc;\n"
         << "      text-align: left;\n"
         << "    }\n"
         << "    th {\n"
         << "      background: #e74c3c;\n"
         << "      color: #fff;\n"
         << "      font-weight: normal;\n"
         << "    }\n"
         << "    tr:nth-child(even) {\n"
         << "      background: #fafafa;\n"
         << "    }\n"
         << "    tr:hover {\n"
         << "      background: #f1f1f1;\n"
         << "    }\n"
         << "    a {\n"
         << "      text-decoration: none;\n"
         << "      font-weight: bold;\n"
         << "    }\n"
         << "    .file-link {\n"
         << "      color: #3498db;\n"
         << "    }\n"
         << "    .file-link:hover {\n"
         << "      text-decoration: underline;\n"
         << "    }\n"
         << "    .dir-link {\n"
         << "      color: #1b4f72;\n"
         << "    }\n"
         << "    .dir-link:hover {\n"
         << "      text-decoration: underline;\n"
         << "    }\n"
         << "  </style>\n"
         << "</head>\n"
         << "<body>\n"
         << "  <h1>Index of " << showPath << "</h1>\n"
         << "  <table>\n"
         << "    <thead>\n"
         << "      <tr>\n"
         << "        <th>Nom</th>\n"
         << "        <th>Taille</th>\n"
         << "        <th>Dernière modification</th>\n"
         << "      </tr>\n"
         << "    </thead>\n"
         << "    <tbody>\n";

    for (std::vector<s_info>::const_iterator it = files.begin(); it != files.end(); ++it)
    {
        bool isDirectory = false;
        struct stat st;
        if (stat(it->fullpath.c_str(), &st) == 0)
        {
            isDirectory = S_ISDIR(st.st_mode);
        }

        html << "    <tr>\n"
             << "      <td>";

        if (isDirectory)
        {
            html << "<a class=\"dir-link\" href=\"" << it->name << "/\">" << it->name << "/</a>";
        }
        else
        {
            html << "<a class=\"file-link\" href=\"" << it->name 
                 << "\" download=\"" << it->name << "\">" << it->name << "</a>";
        }

        html << "</td>\n"
             << "      <td>" << it->format_size << "</td>\n"
             << "      <td>" << it->format_time << "</td>\n"
             << "    </tr>\n";
    }

    html << "    </tbody>\n"
         << "  </table>\n"
         << "</body>\n"
         << "</html>\n";

    return html.str();
}
