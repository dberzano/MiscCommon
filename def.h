/************************************************************************/
/**
 * @file def.h
 * @brief Helpers and definitions (typedefs)
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*
 
        version number:    $LastChangedRevision$
        created by:        Anar Manafov
                           2006-03-28
        last changed by:   $LastChangedBy$ $LastChangedDate$
 
        Copyright (c) 2006 GSI GridTeam. All rights reserved.
************************************************************************/
#ifndef DEF_H
#define DEF_H

// STD
#include <string>
#include <set>
#include <vector>
#include <map>

namespace MiscCommon
{

    /// Long Pointer to Constant STRing
    typedef const char * LPCSTR;

    /*! \typedef StringSet_t
    \brief STL set of strings
     */
    typedef std::set<std::string> StringSet_t;
    /*! \typedef StringVector_t
    \brief STL vector of strings
     */
    typedef std::vector<std::string> StringVector_t;
    /*! \typedef CHARVector_t
    \brief STL vector of char(s)
     */
    typedef std::vector<char> CHARVector_t;
    typedef std::vector<unsigned char> BYTEVector_t;
    /*! \typedef UIntStringMap_t
    \brief STL map, which is mapping pair of size_t (as a key) and string (as a value)
     */
    typedef std::map<size_t, std::string> UIntStringMap_t;
    /*! \typedef StringUIntMap_t
    \brief STL map, which is mapping pair of string (as a key) and size_t (as a value)
     */
    typedef std::map<std::string, size_t> StringUIntMap_t;

};

#endif
