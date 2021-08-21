/**
 *  CoreConfiguration.h
 *
 *  This file is part of the MRH project.
 *  See the AUTHORS file for Copyright information.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef CoreConfiguration_h
#define CoreConfiguration_h

// C / C++
#include <mutex>

// External
#include <MRH_Typedefs.h>

// Project
#include "./ConfigurationException.h"


class CoreConfiguration
{
public:
    
    //*************************************************************************************
    // Types
    //*************************************************************************************
    
    typedef enum
    {
        USER_APP = 0,
        USER_SERVICE = 1,
        PLATFORM_SERVICE = 2,
        
        QUEUE_MAX = PLATFORM_SERVICE,
        
        QUEUE_COUNT = QUEUE_MAX + 1
        
    }Queue;
    
    //*************************************************************************************
    // Constructor
    //*************************************************************************************

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_CoreConfiguration CoreConfiguration class source.
     */

    CoreConfiguration(CoreConfiguration const& c_CoreConfiguration) = delete;
    
    //*************************************************************************************
    // Singleton
    //*************************************************************************************

    /**
     *  Get the class instance. This function is thread safe.
     *
     *  \return The class instance.
     */

    static CoreConfiguration& Singleton() noexcept;
    
    //*************************************************************************************
    // Load
    //*************************************************************************************

    /**
     *  Load the configuration.
     */
    
    void Load();

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get the app parent binary for shared objects. This function is thread safe.
     *
     *  \return A string containing the binary path.
     */

    std::string GetAppParentBinaryPath() const noexcept;
    
    /**
     *  Get the app service parent binary for shared objects. This function is thread safe.
     *
     *  \return A string containing the binary path.
     */
    
    std::string GetAppServiceParentBinaryPath() const noexcept;
    
    /**
     *  Get the process force stop timer. This function is thread safe.
     *
     *  \return The timer in seconds.
     */
    
    MRH_Uint32 GetForceStopTimerS() const noexcept;
    
    /**
     *  Get the thread wait sleep timer. This function is thread safe.
     *
     *  \return The timer in milliseconds.
     */
    
    MRH_Uint32 GetWaitSleepTimerMS() const noexcept;
    
    /**
     *  Get the event recieve timeout.
     *
     *  \param e_Queue The queue this timeout is for.
     *
     *  \return The timeout in milliseconds.
     */
    
    MRH_Sint32 GetRecieveTimeoutMS(Queue e_Queue) const;
    
    /**
     *  Get the service event limit.
     *
     *  \param e_Queue The queue this event limit is for.
     *
     *  \return The service event limit.
     */
    
    MRH_Uint32 GetEventLimit(Queue e_Queue) const;

    /**
     *  Get the default package path.
     *
     *  \return A string containing the home package path.
     */

    std::string GetHomePackagePath() const noexcept;
    
    /**
     *  Get the default home package launch command id.
     *
     *  \return The default home package launch command id.
     */

    int GetHomePackageDefaultLaunchCommandID() const noexcept;
    
    /**
     *  Get the startup home package launch command id.
     *
     *  \return The startup home package launch command id.
     */

    int GetHomePackageStartupLaunchCommandID() const noexcept;
    
private:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    CoreConfiguration() noexcept;

    /**
     *  Default destructor.
     */

    ~CoreConfiguration() noexcept;

    //*************************************************************************************
    // Data
    //**************************************************************************************

    // Paths
    std::string s_AppParentBinaryPath;
    std::string s_AppServiceParentBinaryPath;
    
    // Timers
    MRH_Uint32 u32_ForceStopTimerS;
    MRH_Uint32 u32_WaitSleepTimerMS;
    MRH_Sint32 p_RecieveTimeoutMS[QUEUE_COUNT];
    
    // Events
    MRH_Uint32 p_EventLimit[QUEUE_COUNT];

    // App
    std::string s_HomePackagePath;
    int i_HomePackageDefaultLaunchCommandID;
    int i_HomePackageStartupLaunchCommandID;
    
protected:

};

#endif /* CoreConfiguration_h */
