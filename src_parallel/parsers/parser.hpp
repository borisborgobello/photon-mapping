#ifndef PARSER_HPP_
#define PARSER_HPP_

/**
 * \file parser.hpp
 * \author B.BORGOBELLO
 * \brief Declaration of the Parser class
 */

#include <string>
#include <scene.hpp>

/**
 * \class Parser
 * \brief Base class for reading a formatted file (for the Scene)
 *
 * All scene parsers are supposed to be derived from this base class
 * It must read a file and create the scene
 */
template <typename T>
class Parser
{
public :
    Parser() {} ///< Empty constructor
    virtual ~Parser() = 0 ; ///< Empty destructor

    virtual Scene generate_scene() =0; ///< Creates the Scene described in the file
    virtual bool is_well_formed() = 0; ///< Verifies the parameters of everything in the files (recursively)
protected :
    T root; ///< The ROOT node of the file
};

#endif
