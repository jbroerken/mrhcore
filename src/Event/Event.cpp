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
#include <cstring>
#include <new>

// External

// Project
#include "./Event.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

Event::Event(MRH_Uint32 u32_GroupID,
             MRH_Uint32 u32_Type,
             const MRH_Uint8* p_Data,
             MRH_Uint32 u32_DataSize) : u32_GroupID(u32_GroupID),
                                        u32_Type(u32_Type),
                                        u32_DataSize(u32_DataSize)
{
    // Check and create data buffer
    if (this->u32_DataSize > 0)
    {
        if (p_Data == NULL)
        {
            throw EventException("Event data size > 0, but event data is NULL!");
        }
        
        try
        {
            // < C++17 = Add special delete
            if ((this->p_Data = std::shared_ptr<MRH_Uint8>(new MRH_Uint8[u32_DataSize], std::default_delete<MRH_Uint8[]>())) == NULL)
            {
                throw EventException("Failed to allocate event data!");
            }
        }
        catch (std::exception& e)
        {
            throw EventException(e.what());
        }
        
        std::memcpy(this->p_Data.get(), p_Data, u32_DataSize);
    }
    else
    {
        this->p_Data = NULL;
    }
}

Event::Event(MRH_Uint32 u32_GroupID,
             MRH_Uint32 u32_Type) noexcept
{
    this->u32_GroupID = u32_GroupID;
    this->u32_Type = u32_Type;
    
    // Event has no data
    u32_DataSize = 0;
    p_Data = NULL;
}

Event::~Event() noexcept
{}

//*************************************************************************************
// Getters
//*************************************************************************************

MRH_Uint32 Event::GetGroupID() const noexcept
{
    return u32_GroupID;
}

MRH_Uint32 Event::GetType() const noexcept
{
    return u32_Type;
}

const MRH_Uint8* Event::GetData() const noexcept
{
    return p_Data.get();
}

MRH_Uint32 Event::GetDataSize() const noexcept
{
    return u32_DataSize;
}
