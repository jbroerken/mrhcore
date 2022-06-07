/**
 *  Copyright (C) 2021 - 2022 The MRH Project Authors.
 * 
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
