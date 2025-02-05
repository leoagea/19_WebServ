/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageGenerator.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 14:34:04 by lagea             #+#    #+#             */
/*   Updated: 2025/02/05 18:32:56 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ERRORPAGEGENERATOR_HPP__
# define __ERRORPAGEGENERATOR_HPP__

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../config/serverBlock.hpp"

namespace ErrorPageGenerator
{
    std::string generateErrorPageCode(std::map<int, std::string> &, int);
}

#endif