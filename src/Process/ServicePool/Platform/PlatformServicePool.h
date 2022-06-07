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
