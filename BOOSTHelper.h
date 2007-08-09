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

namespace MiscCommon
{
    /**
     * @brief A typedef wrapper for boost::thread pointer.
     **/
    typedef boost::shared_ptr<boost::thread> Thread_PTR_t;
};

#endif /*BOOSTHELPER_H_*/
