/**
 *  PlatformService.cpp
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

// C / C++

// External

// Project
#include "./PlatformService.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PlatformService::PlatformService(std::shared_ptr<ServiceProcess>& p_Process,
                                 std::shared_ptr<PoolCondition>& p_Condition,
                                 MRH_Uint32 u32_EventLimit,
                                 MRH_Sint32 s32_TimeoutMS,
                                 bool b_Essential,
                                 MRH_Uint32 u32_RouteID) : PoolService(p_Process,
                                                                       p_Condition,
                                                                       u32_EventLimit,
                                                                       s32_TimeoutMS,
                                                                       b_Essential),
                                                           u32_RouteID(u32_RouteID)
{}

PlatformService::~PlatformService() noexcept
{}

//*************************************************************************************
// Send
//*************************************************************************************

void PlatformService::SendEvent(Event& c_Event) noexcept
{
    p_Mutex[SEND].lock();
    p_Queue[SEND].emplace_back(c_Event);
    p_Mutex[SEND].unlock();
}

//*************************************************************************************
// Getters
//*************************************************************************************

MRH_Uint32 PlatformService::GetRouteID() const noexcept
{
    return u32_RouteID;
}
