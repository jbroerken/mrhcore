/**
 *  PoolEvents.h
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
