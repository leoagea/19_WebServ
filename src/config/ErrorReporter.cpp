/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorReporter.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea <lagea@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:23:57 by lagea             #+#    #+#             */
/*   Updated: 2025/01/03 15:07:04 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorReporter.hpp"
#include "configFileParser.hpp"

ErrorReporter::ErrorReporter()
{
}

ErrorReporter::ErrorReporter(ConfigFile *parser) : _parser(parser)
{
}

ErrorReporter::~ErrorReporter()
{
}
    
void ErrorReporter::operator()(unsigned int tokenIndex, const std::string &msg)
{
    if (_parser)
        _parser->reportError(tokenIndex, msg);
}
