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
