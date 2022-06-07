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

#ifndef UserProcess_h
#define UserProcess_h

// C / C++
#include <string>

// External

// Project
#include "../Process.h"
#include "./UserPermission.h"
#include "../../Event/EventQueue.h"
#include "../../Package/Package.h"


class UserProcess : public Process,
                    private EventQueue,
                    public UserPermission
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    UserProcess();

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_UserProcess UserProcess class source.
     */

    UserProcess(UserProcess const& c_UserProcess) = delete;

    /**
     *  Default destructor.
     */

    ~UserProcess() noexcept;

    //*************************************************************************************
    // Run
    //*************************************************************************************

    /**
     *  Run a user process.
     *
     *  \param c_Package The package of the application to run.
     *  \param i_LaunchCommandID The launch command identifier for this application.
     *  \param s_LaunchInput The UTF-8 launch input for this application.
     *  \param s_AppParentBinaryPath The app parent binary to start this application with.
     *  \param u32_EventLimit The max amount of event to be sent / recieved in a update.
     *  \param s32_RecieveTimeoutMS The event recieve timeout in milliseconds.
     */

    void Run(Package const& c_Package, int i_LaunchCommandID, std::string s_LaunchInput, std::string s_AppParentBinaryPath, MRH_Uint32 u32_EventLimit, MRH_Sint32 s32_RecieveTimeoutMS);
    
    //*************************************************************************************
    // Recieve
    //*************************************************************************************

    /**
     *  Log events recieved from the user process.
     *
     *  \param c_Event The event to log.
     */
        
#if MRH_CORE_EVENT_LOGGING > 0
    void LogRecievedEvents(Event const& c_Event) noexcept override;
#endif
    
    /**
     *  Recieve events. The events are read from C_W_P_R.
     */

    void RecieveEvents() noexcept;
    
    /**
     *  Retrieve recieved events.
     *
     *  \return A vector containing all read events.
     */
    
    std::vector<Event>& RetrieveEvents() noexcept;

    //*************************************************************************************
    // Send
    //*************************************************************************************
    
    /**
     *  Log events sent to the user process.
     *
     *  \param c_Event The event to log.
     */
        
#if MRH_CORE_EVENT_LOGGING > 0
    void LogSentEvents(Event const& c_Event) noexcept override;
#endif
    
    /**
     *  Add new events to send.
     *
     *  \param v_Event The events to send. The events will be moved an the vector cleared.
     */
    
    void AddSendEvents(std::vector<Event>& v_Event) noexcept;
    
    /**
     *  Send remaining events.
     */
    
    void SendEvents() noexcept;
    
    /**
     *  Send new and remaining events.
     *
     *  \param v_Event The events to send. The events will be moved an the vector cleared.
     */

    void SendEvents(std::vector<Event>& v_Event) noexcept;
    
private:

    //*************************************************************************************
    // Process Event Filter
    //*************************************************************************************
    
    /**
     *  Filter events based on the current group id.
     *
     *  \param v_Event The event vector to check.
     */
    
    void FilterEventsGroupID(std::vector<Event>& v_Event) noexcept;
    
    /**
     *  Filter events while looking for a reset request.
     *
     *  \param v_Event The event vector to check.
     *
     *  \return true if the reset request was found, false if not.
     */
    
    bool FilterEventsResetRequest(std::vector<Event>& v_Event) noexcept;

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get the event permission for a permission type.
     *
     *  \param u32_Type The event type to get the permission for.
     *
     *  \return The permission type permissions.
     */
    
    Package::EventPermission GetPermission(MRH_Uint32 u32_Type) const noexcept;
    
    //*************************************************************************************
    // Types
    //*************************************************************************************
    
    typedef enum
    {
        REQUIRE_REQUEST = 0,
        SEND_RESPONSE = 1,
        RESET_COMPLETE = 2,
        
        RESET_STATE_MAX = RESET_COMPLETE,
        
        RESET_STATE_COUNT = RESET_STATE_MAX + 1
        
    }ResetState;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    // Event group id
    MRH_Uint32 u32_EventGroupID;
    MRH_Uint32 u32_PreviousEventGroupID;
    
    // Event limits
    MRH_Uint32 u32_EventLimit;
    MRH_Sint32 s32_RecieveTimeoutMS;
    
    // Event Version
    int i_EventVer;
    
    // Reset state
    ResetState e_ResetState;
    
    // Package
#if MRH_CORE_EVENT_LOGGING > 0
    std::string s_PackagePath;
#endif

protected:

};

#endif /* UserProcess_h */
