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

// C / C++

// External

// Project
#include "./PoolEvents.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PoolEvents::PoolEvents() noexcept
{}

PoolEvents::~PoolEvents() noexcept
{}

//*************************************************************************************
// Recieve
//*************************************************************************************

void PoolEvents::LockRecievedEvents() noexcept
{
    p_Mutex[RECIEVED].lock();
}

void PoolEvents::UnlockRecievedEvents() noexcept
{
    p_Mutex[RECIEVED].unlock();
}

std::vector<Event>& PoolEvents::RetrieveEvents() noexcept
{
    return p_Queue[RECIEVED];
}

//*************************************************************************************
// Send
//*************************************************************************************

void PoolEvents::SendEvents(std::vector<Event>& v_Event) noexcept
{
    p_Mutex[SEND].lock();
    std::move(v_Event.begin(), v_Event.end(), std::back_inserter(p_Queue[SEND]));
    p_Mutex[SEND].unlock();
    
    v_Event.clear();
}
