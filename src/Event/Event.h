/**
 *  Event.h
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

#ifndef Event_h
#define Event_h

// C / C++
#include <memory>

// External
#include <MRH_Event.h>

// Project
#include "./EventException.h"


class Event
{
    friend class EventQueue;
    
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param u32_GroupID The event group.
     *  \param u32_Type The event type.
     *  \param p_Data The event data.
     *  \param u32_DataSize The event data size.
     */

    Event(MRH_Uint32 u32_GroupID,
          MRH_Uint32 u32_Type,
          const MRH_Uint8* p_Data,
          MRH_Uint32 u32_DataSize);
    
    /**
     *  No data constructor.
     *
     *  \param u32_GroupID The event group.
     *  \param u32_Type The event type.
     */

    Event(MRH_Uint32 u32_GroupID,
          MRH_Uint32 u32_Type) noexcept;

    /**
     *  Default destructor.
     */

    ~Event() noexcept;
    
    //*************************************************************************************
    // Cast
    //*************************************************************************************
    
    /**
     *  Cast event data to value type. This function is thread safe.
     *
     *  \param u32_Start The start in the data buffer.
     *
     *  \return The type casted value.
     */
    
    template <typename T> T CastValue(MRH_Uint32 u32_Start) const
    {
        if (u32_Start < u32_DataSize && u32_Start + sizeof(T) < u32_DataSize)
        {
            return static_cast<T>(*((T*)(&(p_Data.get()[u32_Start]))));
        }
        
        throw EventException("Invalid event data value cast!");
    }
    
    /**
     *  Cast event data to pointer type. This function is thread safe.
     *
     *  \param u32_Start The start in the data buffer.
     *
     *  \return The type casted pointer.
     */
    
    template <typename T> T CastPointer(MRH_Uint32 u32_Start) const
    {
        if (u32_Start < u32_DataSize)
        {
            return (T)(&(p_Data.get()[u32_Start]));
        }
        
        throw EventException("Invalid event data pointer cast!");
    }

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get the event group id. This function is thread safe.
     *
     *  \return The event group id.
     */
    
    MRH_Uint32 GetGroupID() const noexcept;
    
    /**
     *  Get the event type. This function is thread safe.
     *
     *  \return The event type.
     */

    MRH_Uint32 GetType() const noexcept;

    /**
     *  The the event data. This function is thread safe.
     *
     *  \return The event data.
     */

    const MRH_Uint8* GetData() const noexcept;
    
    /**
     *  Get the event data size. This function is thread safe.
     *
     *  \return The event data size.
     */
    
    MRH_Uint32 GetDataSize() const noexcept;

private:

    //*************************************************************************************
    // Data
    //*************************************************************************************

    MRH_Uint32 u32_GroupID;
    MRH_Uint32 u32_Type;
    std::shared_ptr<MRH_Uint8> p_Data;
    MRH_Uint32 u32_DataSize;

protected:

};

#endif /* Event_h */
