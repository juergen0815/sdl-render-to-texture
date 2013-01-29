/*
 * assert.cpp
 *
 *  Created on: 2013-01-15
 *      Author: jurgens
 */

#include <boost/lexical_cast.hpp>

#include <string>
#include <stdexcept>

namespace boost
{
    void assertion_failed(char const * expr, char const * function, char const * file, long line)
    {
        std::string what;
        what += "Expression: <";
        what += expr;
        what += ">\nIn function: '";
        what += function;
        what += "'\nFile: ";
        what += file;
        what += "\nLine: ";
        what += boost::lexical_cast<std::string>(line);

        throw std::runtime_error( what );
    }
}



