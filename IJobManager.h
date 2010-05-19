/************************************************************************/
/**
 * @file IJobManager.h
 * @brief an interface for PAConsole plug-ins
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2008-10-09
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2008-2009 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef IJOBMANAGER_H_
#define IJOBMANAGER_H_

#include "PoDUserDefaultsOptions.h"

class QString;
class QWidget;
class QIcon;

class IJobManager
{
    public:
        virtual ~IJobManager()
        {}
        virtual QString getName() const = 0;
        virtual QWidget *getWidget() = 0;
        virtual QIcon getIcon() = 0;
        // argument in seconds; if 0 is given the timer should stop
        virtual void startUpdTimer(int _JobStatusUpdInterval) = 0;
        virtual int getJobsCount() const = 0;
        virtual void setUserDefaults(const PoD::CPoDUserDefaults &_ud)
        {

        }
        virtual void setEnvironment(const std::string &_envp)
        {

        }

        // The object of this interface has to provide the following signals:
        // void changeNumberOfJobs( int );
};

Q_DECLARE_INTERFACE( IJobManager,
                     "an interface for PAConsole plug-ins" );

#endif /* IJOBMANAGER_H_ */
