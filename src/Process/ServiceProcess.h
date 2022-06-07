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

#ifndef ServiceProcess_h
#define ServiceProcess_h

// C / C++

// External

// Project
#include "./Process.h"
#include "../Event/EventQueue.h"


class ServiceProcess : public Process,
                       public EventQueue
{
public:
    
    //*************************************************************************************
    // Destructor
    //*************************************************************************************
    
    /**
     *  Default destructor.
     */
    
    virtual ~ServiceProcess() noexcept;
    
    //*************************************************************************************
    // Recieve
    //*************************************************************************************

    /**
     *  Recieve events. The events are read from C_W_P_R.
     *
     *  \param u32_EventLimit The max amount of event to be sent / recieved in a update.
     *  \param s32_TimeoutMS The recieve timeout in milliseconds.
     */

    virtual void RecieveEvents(MRH_Uint32 u32_EventLimit, MRH_Sint32 s32_TimeoutMS) noexcept;
    
    /**
     *  Retrieve recieved events.
     *
     *  \return A vector containing all read events.
     */
    
    virtual std::vector<Event>& RetrieveEvents() noexcept;

    //*************************************************************************************
    // Send
    //*************************************************************************************
    
    /**
     *  Add new events to send.
     *
     *  \param v_Event The events to send. The events will be moved an the vector cleared.
     */
    
    virtual void AddSendEvents(std::vector<Event>& v_Event) noexcept;
    
    /**
     *  Send remaining events.
     *
     *  \param u32_EventLimit The max amount of event to be sent / recieved in a update.
     */
    
    virtual void SendEvents(MRH_Uint32 u32_EventLimit) noexcept;
    
    /**
     *  Send new and remaining events.
     *
     *  \param v_Event The events to send. The events will be moved an the vector cleared.
     *  \param u32_EventLimit The max amount of event to be sent / recieved in a update.
     */

    virtual void SendEvents(std::vector<Event>& v_Event, MRH_Uint32 u32_EventLimit) noexcept;

    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Check if event sending to this process is allowed or not.
     *
     *  \return true if events can be sent, false if not.
     */
    
    bool GetCanRecieve() const noexcept;
    
    /**
     *  Check if event recieving from this process if allowed or not.
     *
     *  \return true if events can be recieved, false if not.
     */
    
    bool GetCanSend() const noexcept;
    
    /**
     *  Get the path of the last run process.
     *
     *  \return The full run path.
     */
    
    std::string GetRunPath() const noexcept;
    
private:

    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    bool b_CanRecieve;
    bool b_CanSend;
    
protected:
    
    //*************************************************************************************
    // Constructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     *
     *  \param e_Type The transmission source type.
     *  \param b_CanRecieve Wether event sending to this process is allowed or not.
     *  \param b_CanSend Wether event recieving from this process is allowed or not.
     */
    
    ServiceProcess(TransmissionSource::SourceType e_Type,
                   bool b_CanRecieve,
                   bool b_CanSend);
    
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_ServiceProcess ServiceProcess class source.
     */
    
    ServiceProcess(ServiceProcess const& c_ServiceProcess) = delete;

    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    std::string s_RunPath;
};

#endif /* ServiceProcess_h */
