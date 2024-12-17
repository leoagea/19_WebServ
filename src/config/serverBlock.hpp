/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 13:28:47 by lagea             #+#    #+#             */
/*   Updated: 2024/12/16 22:38:01 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVERBLOCK_HPP__
# define __SERVERBLOCK_HPP__

#include <vector>
#include <iostream>
#include <map>
#include <set>

#include "locationBlock.hpp"

class locationBlock;

class serverBlock 
{
    public:
        serverBlock();
        ~serverBlock();

    private:
       std::set<unsigned int> _listeningports;
       std::string _servername;
       std::string _rootdir;
       std::string _index;
       std::string _acceslogdir;
       std::string _errorlogdir;
       unsigned int _bodysizelimit;
       std::map<std::string ,class locationBlock> _locationblock;
       std::map<std::pair<int, std::string>, class locationBlock> _errorpages;
};

#endif