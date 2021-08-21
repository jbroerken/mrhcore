/**
 *  ServiceProcess.h
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
