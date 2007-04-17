#ifndef BOOSTHELPER_H_
#define BOOSTHELPER_H_

// BOOST
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>

namespace MiscCommon
{
    typedef boost::shared_ptr<boost::thread> Thread_PTR_t;
};

#endif /*BOOSTHELPER_H_*/
