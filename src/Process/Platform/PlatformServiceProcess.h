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

#ifndef PlatformServiceProcess_h
#define PlatformServiceProcess_h

// C / C++

// External

// Project
#include "../ServiceProcess.h"


class PlatformServiceProcess : public ServiceProcess
{
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     */
    
    PlatformServiceProcess();
    
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_PlatformServiceProcess PlatformServiceProcess class source.
     */
    
    PlatformServiceProcess(PlatformServiceProcess const& c_PlatformServiceProcess) = delete;
    
    /**
     *  Default destructor.
     */
    
    ~PlatformServiceProcess() noexcept;
    
    //*************************************************************************************
    // Run
    //*************************************************************************************
    
    /**
     *  Run a service process.
     *
     *  \param s_BinaryPath The platform service parent binary to start the platform service with.
     *  \param u32_EventLimit The max amount of event to be sent / recieved in a update.
     *  \param s32_RecieveTimeoutMS The event recieve timeout in milliseconds.
     */
    
    void Run(std::string s_BinaryPath, MRH_Uint32 u32_EventLimit, MRH_Sint32 s32_RecieveTimeoutMS);
    
    //*************************************************************************************
    // Recieve
    //*************************************************************************************

    /**
     *  Log events recieved from the platform service process.
     *  
     *  \param c_Event The event to log.
     */
            
#if MRH_CORE_EVENT_LOGGING > 0
    void LogRecievedEvents(Event const& c_Event) noexcept override;
#endif

    //*************************************************************************************
    // Send
    //*************************************************************************************
        
    /**
     *  Log events sent to the platform service process.
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
    
protected:
    
};

#endif /* PlatformServiceProcess_h */
