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
