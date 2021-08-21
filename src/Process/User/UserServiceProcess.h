/**
 *  UserServiceProcess.h
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
    
protected:
    
};

#endif /* UserServiceProcess_h */
