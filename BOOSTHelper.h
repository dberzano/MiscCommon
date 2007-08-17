/************************************************************************/
/**
 * @file BOOSTHelper.h
 * @brief Helpers for BOOST libraries
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2007-04-20
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef BOOSTHELPER_H_
#define BOOSTHELPER_H_

// BOOST
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/program_options/variables_map.hpp>

// MiscCommon
#include "MiscUtils.h"


namespace MiscCommon
{

    /**
     * @brief A typedef wrapper for boost::thread pointer.
     **/
    typedef boost::shared_ptr<boost::thread> Thread_PTR_t;


    // Function used to check that 'opt1' and 'opt2' are not specified at the same time.
    inline void conflicting_options( const boost::program_options::variables_map& _vm,
                                     const char* _opt1, const char* _opt2 ) throw(std::exception)
    {
        if ( _vm.count(_opt1) && !_vm[_opt1].defaulted() && _vm.count(_opt2) && !_vm[_opt2].defaulted() )
        {
            std::string str("Command line parameter \"%1\" conflicts with \"%2\"");
            MiscCommon::replace<std::string>( &str, "%1", _opt1 );
            MiscCommon::replace<std::string>( &str, "%2", _opt2 );
            throw std::runtime_error( str );
        }
    }
    // Function used to check that of 'for_what' is specified, then 'required_option' is specified too.
    inline void option_dependency( const boost::program_options::variables_map &_vm,
                                   const char *_for_what, const char *_required_option ) throw(std::exception)
    {
        if ( _vm.count(_for_what) && !_vm[_for_what].defaulted() && ( !_vm.count(_required_option) || _vm[_required_option].defaulted() ) )
        {
            std::string str("Command line parameter \"%1\" must be used with \"%2\"");
            MiscCommon::replace<std::string>( &str, "%1", _for_what );
            MiscCommon::replace<std::string>( &str, "%2", _required_option );
            throw std::runtime_error( str );
        }
    }

};

#endif /*BOOSTHELPER_H_*/
