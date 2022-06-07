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

#ifndef PoolEvents_h
#define PoolEvents_h

// C / C++
#include <mutex>
#include <vector>

// External
#include "../../Event/Event.h"

// Project


class PoolEvents
{
public:

    //*************************************************************************************
    // Destructor
    //*************************************************************************************
    
    /**
     *  Default destructor.
     */
    
    virtual ~PoolEvents() noexcept;
    
    //*************************************************************************************
    // Recieve
    //*************************************************************************************

    /**
     *  Lock the recieved event queue. This function is thread safe.
     */
    
    void LockRecievedEvents() noexcept;
    
    /**
     *  Unlock the recieved event queue. This function is thread safe.
     */
    
    void UnlockRecievedEvents() noexcept;
    
    /**
     *  Retrieve recieved events. This function is thread safe.
     *
     *  \return A vector containing all read events.
     */
    
    std::vector<Event>& RetrieveEvents() noexcept;

    //*************************************************************************************
    // Send
    //*************************************************************************************
    
    /**
     *  Add events to send to the services. This function is thread safe.
     *
     *  \param v_Event The events to send. The events will be moved an the vector cleared.
     */

    void SendEvents(std::vector<Event>& v_Event) noexcept;
    
private:

    //*************************************************************************************
    // Data
    //*************************************************************************************
    
protected:
    
    //*************************************************************************************
    // Types
    //*************************************************************************************

    typedef enum
    {
        SEND = 0,
        RECIEVED = 1,
        
        QUEUE_TYPE_MAX = RECIEVED,
        
        QUEUE_TYPE_COUNT = QUEUE_TYPE_MAX + 1
        
    }QueueType;
    
    //*************************************************************************************
    // Constructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     */
    
    PoolEvents() noexcept;
    
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_PoolEvents PoolEvents class source.
     */
    
    PoolEvents(PoolEvents const& c_PoolEvents) = delete;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    // Thread pool safety
    std::mutex p_Mutex[QUEUE_TYPE_COUNT];
    
    // Event storage
    // @NOTE: We store both here instead of giving them directly to the destination
    //        This is done to minimize wait times for mutexes
    std::vector<Event> p_Queue[QUEUE_TYPE_COUNT];
};

#endif /* PoolEvents_h */
