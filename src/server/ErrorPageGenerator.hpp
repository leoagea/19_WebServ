/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageGenerator.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 14:48:53 by lagea             #+#    #+#             */
/*   Updated: 2025/02/06 14:52:29 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ERRORPAGEGENERATOR__
# define __ERRORPAGEGENERATOR__ 

# include <string>
# include <map>
# include <sstream>
# include <fstream>

# include "../config/PathChecking.hpp"

namespace ErrorPageGenerator
{
    std::string generateErrorPageCode(std::map<int, std::string> &errormap, int error);
}

#endif