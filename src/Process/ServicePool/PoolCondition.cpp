/**
 *  PoolCondition.cpp
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
#include "./PoolCondition.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PoolCondition::PoolCondition() noexcept
{}

PoolCondition::~PoolCondition() noexcept
{}

//*************************************************************************************
// Notify
//*************************************************************************************

void PoolCondition::Notify() noexcept
{
    c_Condition.notify_one();
}

//*************************************************************************************
// Wait
//*************************************************************************************

void PoolCondition::Wait() noexcept
{
    std::unique_lock<std::mutex> c_UniqueLock(c_Mutex);
    c_Condition.wait(c_UniqueLock);
}

void PoolCondition::Wait(MRH_Sint32 s32_TimeoutMS) noexcept
{
    if (s32_TimeoutMS <= 0)
    {
        return;
    }
    
    std::unique_lock<std::mutex> c_UniqueLock(c_Mutex);
    c_Condition.wait_for(c_UniqueLock, std::chrono::milliseconds(s32_TimeoutMS));
}
