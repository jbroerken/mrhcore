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
