/**
 *  Event.cpp
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
