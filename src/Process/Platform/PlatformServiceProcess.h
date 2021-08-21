/**
 *  PlatformServiceProcess.h
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
