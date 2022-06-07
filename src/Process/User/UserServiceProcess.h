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

#ifndef UserServiceProcess_h
#define UserServiceProcess_h

// C / C++

// External

// Project
#include "../ServiceProcess.h"
#include "./UserPermission.h"


class UserServiceProcess : public ServiceProcess,
                           public UserPermission
{
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     */
    
    UserServiceProcess();
    
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_UserServiceProcess UserServiceProcess class source.
     */
    
    UserServiceProcess(UserServiceProcess const& c_UserServiceProcess) = delete;
    
    /**
     *  Default destructor.
     */
    
    ~UserServiceProcess() noexcept;
    
    //*************************************************************************************
    // Run
    //*************************************************************************************
    
    /**
     *  Run a service process.
     *
     *  \param c_Package The package of the application service to run.
     *  \param s_BinaryPath The user service parent binary to start this application service with.
     *  \param u32_EventLimit The max amount of event to be sent / recieved in a update.
     */
    
    void Run(Package const& c_Package, std::string s_BinaryPath, MRH_Uint32 u32_EventLimit);
    
    //*************************************************************************************
    // Recieve
    //*************************************************************************************

    /**
     *  Log events recieved from the user service process.
     *
     *  \param c_Event The event to log.
     */
                
#if MRH_CORE_EVENT_LOGGING > 0
    void LogRecievedEvents(Event const& c_Event) noexcept override;
#endif
    
    /**
     *  Retrieve recieved events.
     *
     *  \return A vector containing all read events.
     */
    
    std::vector<Event>& RetrieveEvents() noexcept override;

    //*************************************************************************************
    // Send
    //*************************************************************************************
            
    /**
     *  Log events sent to the user service process.
     *
     *  \param c_Event The event to log.
     */
                
#if MRH_CORE_EVENT_LOGGING > 0
    void LogSentEvents(Event const& c_Event) noexcept override;
#endif
    
private:

    //*************************************************************************************
    // Data
    //*************************************************************************************

    // Package
#if MRH_CORE_EVENT_LOGGING > 0
    std::string s_PackagePath;
#endif
    
    // Event Version
    int i_EventVer;
    
protected:
    
};

#endif /* UserServiceProcess_h */
