/**
 *  PlatformService.h
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

#ifndef PlatformService_h
#define PlatformService_h

// C / C++

// External

// Project
#include "../PoolService.h"


class PlatformService : public PoolService
{
    friend class PlatformServicePool;
    
public:

    //*************************************************************************************
    // Destructor
    //*************************************************************************************
    
    /**
     *  Default destructor.
     */
    
    ~PlatformService() noexcept;

    //*************************************************************************************
    // Send
    //*************************************************************************************
    
    /**
     *  Add a event to send to the service. This function is thread safe.
     *
     *  \param c_Event The event to send.
     */

    void SendEvent(Event& c_Event) noexcept;
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Get the service event route id. This function is thread safe.
     *
     *  \return The service event route id.
     */
    
    MRH_Uint32 GetRouteID() const noexcept;
    
private:

    //*************************************************************************************
    // Constructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     *
     *  \param p_Process The process for this service.
     *  \param p_Condition The service pool condition for notification.
     *  \param u32_EventLimit The max amount of event to be sent / recieved in a update.
     *  \param s32_TimeoutMS The event recieve timeout in milliseconds.  
     *  \param b_Essential Wether the service is essential or not.
     *  \param u32_RouteID The service event route id.
     */
    
    PlatformService(std::shared_ptr<ServiceProcess>& p_Process,
                    std::shared_ptr<PoolCondition>& p_Condition,
                    MRH_Uint32 u32_EventLimit,
                    MRH_Sint32 s32_TimeoutMS,
                    bool b_Essential,
                    MRH_Uint32 u32_RouteID) noexcept;
                    
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_PlatformService PlatformService class source.
     */
                    
    PlatformService(PlatformService const& c_PlatformService) = delete;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    MRH_Uint32 u32_RouteID;
    
protected:
    
};

#endif /* PlatformService_h */
