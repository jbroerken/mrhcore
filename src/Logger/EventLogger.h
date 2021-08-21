/**
 *  EventLogger.h
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

#ifndef EventLogger_h
#define EventLogger_h

// C / C++
#include <mutex>
#include <fstream>
#include <string>

// External

// Project
#include "../Event/Event.h"


class EventLogger
{
public:
    
    //*************************************************************************************
    // Types
    //*************************************************************************************
    
    typedef enum
    {
        USER = 0,
        USER_SERVICE = 1,
        PLATFORM_SERVICE = 2,
        
        EVENT_SOURCE_MAX = PLATFORM_SERVICE,
        
        EVENT_SOURCE_COUNT = EVENT_SOURCE_MAX + 1
        
    }EventSource;

    //*************************************************************************************
    // Singleton
    //*************************************************************************************

    /**
     *  Get the class instance. This function is thread safe.
     *
     *  \return The class instance.
     */

    static EventLogger& Singleton() noexcept;
    
    //*************************************************************************************
    // Log
    //*************************************************************************************
    
    /**
     *  Log an event. This function is thread safe.
     *
     *  \param c_Event The event to log.
     *  \param s_Message The message describing the event.
     */
    
    void Log(Event const& c_Event, std::string s_Message) noexcept;
    
private:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     */
    
    EventLogger() noexcept;
    
    /**
     *  Default destructor.
     */
    
    ~EventLogger() noexcept;
    
    //*************************************************************************************
    // Log
    //*************************************************************************************
     
    /**
     *  Log a line of the event data buffer.
     *
     *  \param s_Line The line string to write to.
     *  \param p_Data The start of the event data to log.
     *  \param u32_Bytes The amount of bytes to log
     */
     
    void LogDataLine(std::string& s_Line, const MRH_Uint8* p_Data, MRH_Uint32 u32_Bytes) noexcept;
    
    /**
     *  Log an event data buffer.
     *
     *  \param v_Message The message vector to append to.
     *  \param p_Data The event data to log.
     *  \param u32_DataSize The total event data size.
     */
    
    void LogEventData(std::vector<std::string>& v_Message, const MRH_Uint8* p_Data, MRH_Uint32 u32_DataSize) noexcept;
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Get the event name string. This function is thread safe.
     *
     *  \param u32_Type The event type.
     *
     *  \return The event name string.
     */
    
    std::string GetEventNameString(MRH_Uint32 u32_Type) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    std::mutex c_Mutex;
    
    std::ofstream f_EventLogFile;
    
    MRH_Uint64 u64_EventID; // The id is defined by the amount sent, first event 0, second 1, etc...
    
protected:
    
};

#endif /* EventLogger_h */
