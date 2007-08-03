/************************************************************************/
/**
 * @file BOOST_FILESYSTEM.h
 * @brief BOOST filesystem lib. helper
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:   $LastChangedRevision$
        created by:          Anar Manafov
                                  2007-08-02
        last changed by:   $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
************************************************************************/
#ifndef BOOST_FILESYSTEM_H_
#define BOOST_FILESYSTEM_H_
// BOOST
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/exception.hpp"

// MiscCommon
#include "MiscUtils.h"

namespace fs = boost::filesystem ;

namespace MiscCommon
{

    /**
     * Removes '/' characters at the end of the of the input pathname
     */
    std::string normalize_path( const std::string &_path )
    {
        std::string path( _path );
        MiscCommon::trim_right<std::string>( &path, "/" );
        return path;
    }

    /*
    * Checks whether the pathname represents a file
    */
    inline bool is_file (const std::string &_pathname)
    {
        bool is_valid = false;

        try
        {
            fs::path cp ( normalize_path(_pathname), fs::native );
            is_valid = !( fs::is_directory(cp) ) ;
        }
        catch (const fs::filesystem_error &_ex)
            {}

        return is_valid;
    }
    /*
    * Checks whether the pathname represents a directory
    */
    inline bool is_directory (const std::string &_pathname)
    {
        bool is_valid = false;
        try
        {
            fs::path cp ( normalize_path(_pathname), fs::native );
            is_valid =  fs::is_directory(cp) ;
        }
        catch ( const fs::filesystem_error &_ex)
            {}

        return is_valid;
    }

};
#endif /*BOOST_FILESYSTEM_H_*/
