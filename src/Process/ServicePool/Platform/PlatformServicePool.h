/**
 *  PlatformServicePool.h
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

#ifndef PlatformServicePool_h
#define PlatformServicePool_h

// C / C++
#include <unordered_map>

// External

// Project
#include "../ServicePool.h"
#include "./PlatformService.h"


class PlatformServicePool : public ServicePool
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     */
    
    PlatformServicePool();
    
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_PlatformServicePool PlatformServicePool class source.
     */
    
    PlatformServicePool(PlatformServicePool const& c_PlatformServicePool) = delete;
    
    /**
     *  Default destructor.
     */
    
    ~PlatformServicePool() noexcept;

    //*************************************************************************************
    // Getters
    //*************************************************************************************
     
    /**
     *  Check if all services are running.
     *
     *  \return true if all services are running, false if not.
     */

    bool GetAllRunning() const noexcept;
    
private:

    //*************************************************************************************
    // Send
    //*************************************************************************************
     
    /**
     *  Add sendable events to the servies.
     */

    void DistributeSendEvents() noexcept override;

    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Check if a service event route can recieve a event.
     *
     *  \param u32_RouteID The service event route id.
     *  \param u32_Type The event type.
     *
     *  \return true if the event is part of this route, false if not.
     */
    
    bool GetEventInRoute(MRH_Uint32 u32_RouteID, MRH_Uint32 u32_Type) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    // <Route ID, Event Vector<Event ID>>
    std::unordered_map<MRH_Uint32, std::vector<MRH_Uint32>> m_EventRoute;
    
protected:

};

#endif /* PlatformServicePool_h */
