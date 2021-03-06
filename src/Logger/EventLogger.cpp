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
#include <execinfo.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <iostream>

// External

// Project
#include "./EventLogger.h"
#include "./Logger.h"

// Pre-defined
#ifndef MRH_CORE_EVENT_LOG_FILE_PATH
    #define MRH_CORE_EVENT_LOG_FILE_PATH "/var/log/mrh/ev_mrhcore.log"
#endif
#ifndef MRH_EVENT_LOGGER_PRINT_CLI
    #define MRH_EVENT_LOGGER_PRINT_CLI 0
#endif


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

EventLogger::EventLogger() noexcept : u64_EventID(0)
{
    f_EventLogFile.open(MRH_CORE_EVENT_LOG_FILE_PATH, std::ios::out | std::ios::trunc);
    
    if (f_EventLogFile.is_open() == false)
    {
        Logger::Singleton().Log(Logger::WARNING, "Failed to open backtrace file: " MRH_CORE_EVENT_LOG_FILE_PATH,
                                "Logger.cpp", __LINE__);
    }
}

EventLogger::~EventLogger() noexcept
{
    if (f_EventLogFile.is_open() == true)
    {
        f_EventLogFile.close();
    }
}

//*************************************************************************************
// Singleton
//*************************************************************************************

EventLogger& EventLogger::Singleton() noexcept
{
    static EventLogger c_EventLogger;
    return c_EventLogger;
}

//*************************************************************************************
// Log
//*************************************************************************************

void EventLogger::LogDataLine(std::string& s_Line, const MRH_Uint8* p_Data, MRH_Uint32 u32_Bytes) noexcept
{
    char p_Hex[32] = { '\0' };
    
    for (MRH_Uint32 i = 0; i < u32_Bytes; ++i)
    {
        snprintf(p_Hex, 32, "%X", p_Data[i]); // @NOTE: Anything wrong with the loop? check this hellspawn function
        s_Line += ("0x" + std::string(p_Hex, 2) + " ");
    }
}

void EventLogger::LogEventData(std::vector<std::string>& v_Message, const MRH_Uint8* p_Data, MRH_Uint32 u32_DataSize) noexcept
{
    MRH_Uint32 u32_Pos = 0;
    MRH_Uint32 u32_LineBytes;
    
    while (u32_Pos < u32_DataSize)
    {
        v_Message.emplace_back("");
        std::string& s_Line = v_Message[v_Message.size() - 1];
        
        if ((u32_LineBytes = (u32_DataSize - u32_Pos)) > 32)
        {
            u32_LineBytes = 32;
        }
        
        LogDataLine(s_Line, &(p_Data[u32_Pos]), u32_LineBytes);
        
        u32_Pos += u32_LineBytes;
    }
}

void EventLogger::Log(Event const& c_Event, std::string s_Message) noexcept
{
    std::vector<std::string> v_Message;
 
    time_t s_RawTime;
    time(&s_RawTime);
    std::string s_Time(std::ctime(&s_RawTime));
    s_Time.erase(s_Time.end() - 1); // ctime adds a newline, why?
    
    v_Message.emplace_back("===");
    v_Message.emplace_back("- " + std::to_string(u64_EventID) + ": " + GetEventNameString(c_Event.GetType()) + " -");
    v_Message.emplace_back("Time: " + s_Time);
    v_Message.emplace_back("Log Message: " + s_Message);
    v_Message.emplace_back("Event Group ID: " + std::to_string(c_Event.GetGroupID()));
    v_Message.emplace_back("Event Data Size: " + std::to_string(c_Event.GetDataSize()));
    v_Message.emplace_back("Event Data:");
    
    if (c_Event.GetData() != NULL && c_Event.GetDataSize() > 0)
    {
        LogEventData(v_Message, c_Event.GetData(), c_Event.GetDataSize());
    }
    else
    {
        v_Message.emplace_back("<No event data available>");
    }
    
    v_Message.emplace_back("===");
    
    c_Mutex.lock();
    
    for (auto& String : v_Message)
    {
        if (f_EventLogFile.is_open() == true)
        {
            f_EventLogFile << String << std::endl;
        }
        
        if (MRH_EVENT_LOGGER_PRINT_CLI > 0 && String.size() > 0)
        {
            std::cout << String << std::endl;
        }
    }
    
    ++u64_EventID;
    
    c_Mutex.unlock();
}

//*************************************************************************************
// Getters
//*************************************************************************************

std::string EventLogger::GetEventNameString(MRH_Uint32 u32_Type) noexcept
{
    std::string s_Result;
    
    switch (u32_Type)
    {
        /**
         *  Event Version 1
         */
            
        // Unknown
        case MRH_EVENT_UNK:
            s_Result = "MRH_EVENT_UNK";
            break;
            
        // Permission
        case MRH_EVENT_PERMISSION_DENIED:
            s_Result = "MRH_EVENT_PERMISSION_DENIED";
            break;
            
        // Password Required
        case MRH_EVENT_PASSWORD_REQUIRED:
            s_Result = "MRH_EVENT_PASSWORD_REQUIRED";
            break;
            
        // Not implemented
        case MRH_EVENT_NOT_IMPLEMENTED_S:
            s_Result = "MRH_EVENT_NOT_IMPLEMENTED_S";
            break;
            
        // Program state
        case MRH_EVENT_PS_RESET_REQUEST_U:
            s_Result = "MRH_EVENT_PS_RESET_REQUEST_U";
            break;
        case MRH_EVENT_PS_RESET_ACKNOLEDGED_U:
            s_Result = "MRH_EVENT_PS_RESET_ACKNOLEDGED_U";
            break;
            
        // Custom
        case MRH_EVENT_CUSTOM_AVAIL_U:
            s_Result = "MRH_EVENT_CUSTOM_AVAIL_U";
            break;
        case MRH_EVENT_CUSTOM_CUSTOM_COMMAND_U:
            s_Result = "MRH_EVENT_CUSTOM_CUSTOM_COMMAND_U";
            break;
            
        case MRH_EVENT_CUSTOM_AVAIL_S:
            s_Result = "MRH_EVENT_CUSTOM_AVAIL_S";
            break;
        case MRH_EVENT_CUSTOM_CUSTOM_COMMAND_S:
            s_Result = "MRH_EVENT_CUSTOM_CUSTOM_COMMAND_S";
            break;
            
        // Voice - Listen
        case MRH_EVENT_LISTEN_AVAIL_U:
            s_Result = "MRH_EVENT_LISTEN_AVAIL_U";
            break;
        case MRH_EVENT_LISTEN_GET_METHOD_U:
            s_Result = "MRH_EVENT_LISTEN_GET_METHOD_U";
            break;
        case MRH_EVENT_LISTEN_CUSTOM_COMMAND_U:
            s_Result = "MRH_EVENT_LISTEN_CUSTOM_COMMAND_U";
            break;
            
        case MRH_EVENT_LISTEN_AVAIL_S:
            s_Result = "MRH_EVENT_LISTEN_AVAIL_S";
            break;
        case MRH_EVENT_LISTEN_STRING_S:
            s_Result = "MRH_EVENT_LISTEN_STRING_S";
            break;
        case MRH_EVENT_LISTEN_GET_METHOD_S:
            s_Result = "MRH_EVENT_LISTEN_GET_METHOD_S";
            break;
        case MRH_EVENT_LISTEN_CUSTOM_COMMAND_S:
            s_Result = "MRH_EVENT_LISTEN_CUSTOM_COMMAND_S";
            break;
            
        // Voice - Say
        case MRH_EVENT_SAY_AVAIL_U:
            s_Result = "MRH_EVENT_SAY_AVAIL_U";
            break;
        case MRH_EVENT_SAY_STRING_U:
            s_Result = "MRH_EVENT_SAY_STRING_U";
            break;
        case MRH_EVENT_SAY_GET_METHOD_U:
            s_Result = "MRH_EVENT_SAY_GET_METHOD_U";
            break;
        case MRH_EVENT_SAY_NOTIFICATION_APP_U:
            s_Result = "MRH_EVENT_SAY_NOTIFICATION_APP_U";
            break;
        case MRH_EVENT_SAY_NOTIFICATION_SERVICE_U:
            s_Result = "MRH_EVENT_SAY_NOTIFICATION_SERVICE_U";
            break;
        case MRH_EVENT_SAY_CUSTOM_COMMAND_U:
            s_Result = "MRH_EVENT_SAY_CUSTOM_COMMAND_U";
            break;
        
        case MRH_EVENT_SAY_AVAIL_S:
            s_Result = "MRH_EVENT_SAY_AVAIL_S";
            break;
        case MRH_EVENT_SAY_STRING_S:
            s_Result = "MRH_EVENT_SAY_STRING_S";
            break;
        case MRH_EVENT_SAY_GET_METHOD_S:
            s_Result = "MRH_EVENT_SAY_GET_METHOD_S";
            break;
        case MRH_EVENT_SAY_NOTIFICATION_APP_S:
            s_Result = "MRH_EVENT_SAY_NOTIFICATION_APP_S";
            break;
        case MRH_EVENT_SAY_CUSTOM_COMMAND_S:
            s_Result = "MRH_EVENT_SAY_CUSTOM_COMMAND_S";
            break;
        
        // Password
        case MRH_EVENT_PASSWORD_AVAIL_U:
            s_Result = "MRH_EVENT_PASSWORD_AVAIL_U";
            break;
        case MRH_EVENT_PASSWORD_CHECK_U:
            s_Result = "MRH_EVENT_PASSWORD_CHECK_U";
            break;
        case MRH_EVENT_PASSWORD_SET_U:
            s_Result = "MRH_EVENT_PASSWORD_SET_U";
            break;
        case MRH_EVENT_PASSWORD_CUSTOM_COMMAND_U:
            s_Result = "MRH_EVENT_PASSWORD_CUSTOM_COMMAND_U";
            break;
        
        case MRH_EVENT_PASSWORD_AVAIL_S:
            s_Result = "MRH_EVENT_PASSWORD_AVAIL_S";
            break;
        case MRH_EVENT_PASSWORD_CHECK_S:
            s_Result = "MRH_EVENT_PASSWORD_CHECK_S";
            break;
        case MRH_EVENT_PASSWORD_SET_S:
            s_Result = "MRH_EVENT_PASSWORD_SET_S";
            break;
        case MRH_EVENT_PASSWORD_CUSTOM_COMMAND_S:
            s_Result = "MRH_EVENT_PASSWORD_CUSTOM_COMMAND_S";
            break;
        
        // User
        case MRH_EVENT_USER_AVAIL_U:
            s_Result = "MRH_EVENT_USER_AVAIL_U";
            break;
        case MRH_EVENT_USER_ACCESS_DOCUMENTS_U:
            s_Result = "MRH_EVENT_USER_ACCESS_DOCUMENTS_U";
            break;
        case MRH_EVENT_USER_ACCESS_PICTURES_U:
            s_Result = "MRH_EVENT_USER_ACCESS_PICTURES_U";
            break;
        case MRH_EVENT_USER_ACCESS_MUSIC_U:
            s_Result = "MRH_EVENT_USER_ACCESS_MUSIC_U";
            break;
        case MRH_EVENT_USER_ACCESS_VIDEOS_U:
            s_Result = "MRH_EVENT_USER_ACCESS_VIDEOS_U";
            break;
        case MRH_EVENT_USER_ACCESS_DOWNLOADS_U:
            s_Result = "MRH_EVENT_USER_ACCESS_DOWNLOADS_U";
            break;
        case MRH_EVENT_USER_ACCESS_CLIPBOARD_U:
            s_Result = "MRH_EVENT_USER_ACCESS_CLIPBOARD_U";
            break;
        case MRH_EVENT_USER_ACCESS_INFO_PERSON_U:
            s_Result = "MRH_EVENT_USER_ACCESS_INFO_PERSON_U";
            break;
        case MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_U:
            s_Result = "MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_U";
            break;
        case MRH_EVENT_USER_ACCESS_CLEAR_U:
            s_Result = "MRH_EVENT_USER_ACCESS_CLEAR_U";
            break;
        case MRH_EVENT_USER_GET_LOCATION_U:
            s_Result = "MRH_EVENT_USER_GET_LOCATION_U";
            break;
        case MRH_EVENT_USER_CUSTOM_COMMAND_U:
            s_Result = "MRH_EVENT_USER_CUSTOM_COMMAND_U";
            break;
        
        case MRH_EVENT_USER_AVAIL_S:
            s_Result = "MRH_EVENT_USER_AVAIL_S";
            break;
        case MRH_EVENT_USER_ACCESS_DOCUMENTS_S:
            s_Result = "MRH_EVENT_USER_ACCESS_DOCUMENTS_S";
            break;
        case MRH_EVENT_USER_ACCESS_PICTURES_S:
            s_Result = "MRH_EVENT_USER_ACCESS_PICTURES_S";
            break;
        case MRH_EVENT_USER_ACCESS_MUSIC_S:
            s_Result = "MRH_EVENT_USER_ACCESS_MUSIC_S";
            break;
        case MRH_EVENT_USER_ACCESS_VIDEOS_S:
            s_Result = "MRH_EVENT_USER_ACCESS_VIDEOS_S";
            break;
        case MRH_EVENT_USER_ACCESS_DOWNLOADS_S:
            s_Result = "MRH_EVENT_USER_ACCESS_DOWNLOADS_S";
            break;
        case MRH_EVENT_USER_ACCESS_CLIPBOARD_S:
            s_Result = "MRH_EVENT_USER_ACCESS_CLIPBOARD_S";
            break;
        case MRH_EVENT_USER_ACCESS_INFO_PERSON_S:
            s_Result = "MRH_EVENT_USER_ACCESS_INFO_PERSON_S";
            break;
        case MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_S:
            s_Result = "MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_S";
            break;
        case MRH_EVENT_USER_ACCESS_CLEAR_S:
            s_Result = "MRH_EVENT_USER_ACCESS_CLEAR_S";
            break;
        case MRH_EVENT_USER_GET_LOCATION_S:
            s_Result = "MRH_EVENT_USER_GET_LOCATION_S";
            break;
        case MRH_EVENT_USER_CUSTOM_COMMAND_S:
            s_Result = "MRH_EVENT_USER_CUSTOM_COMMAND_S";
            break;
        
        // Application
        case MRH_EVENT_APP_AVAIL_U:
            s_Result = "MRH_EVENT_APP_AVAIL_U";
            break;
        case MRH_EVENT_APP_LAUNCH_SOA_U:
            s_Result = "MRH_EVENT_APP_LAUNCH_SOA_U";
            break;
        case MRH_EVENT_APP_LAUNCH_SOA_TIMER_U:
            s_Result = "MRH_EVENT_APP_LAUNCH_SOA_TIMER_U";
            break;
        case MRH_EVENT_APP_LAUNCH_SOA_CLEAR_U:
            s_Result = "MRH_EVENT_APP_LAUNCH_SOA_CLEAR_U";
            break;
        case MRH_EVENT_APP_LAUNCH_SOA_CLEAR_TIMER_U:
            s_Result = "MRH_EVENT_APP_LAUNCH_SOA_CLEAR_TIMER_U";
            break;
        case MRH_EVENT_APP_CUSTOM_COMMAND_U:
            s_Result = "MRH_EVENT_APP_CUSTOM_COMMAND_U";
            break;
        
        case MRH_EVENT_APP_AVAIL_S:
            s_Result = "MRH_EVENT_APP_AVAIL_S";
            break;
        case MRH_EVENT_APP_LAUNCH_SOA_S:
            s_Result = "MRH_EVENT_APP_LAUNCH_SOA_S";
            break;
        case MRH_EVENT_APP_LAUNCH_SOA_TIMER_S:
            s_Result = "MRH_EVENT_APP_LAUNCH_SOA_TIMER_S";
            break;
        case MRH_EVENT_APP_LAUNCH_SOA_TIMER_REMINDER_S:
            s_Result = "MRH_EVENT_APP_LAUNCH_SOA_TIMER_REMINDER_S";
            break;
        case MRH_EVENT_APP_LAUNCH_SOA_CLEAR_S:
            s_Result = "MRH_EVENT_APP_LAUNCH_SOA_CLEAR_S";
            break;
        case MRH_EVENT_APP_LAUNCH_SOA_CLEAR_TIMER_S:
            s_Result = "MRH_EVENT_APP_LAUNCH_SOA_CLEAR_TIMER_S";
            break;
        case MRH_EVENT_APP_CUSTOM_COMMAND_S:
            s_Result = "MRH_EVENT_APP_CUSTOM_COMMAND_S";
            break;
            
        /**
         *  Unk
         */
            
        default:
            s_Result = "UNKNOWN EVENT";
            break;
    }
    
    s_Result += " (";
    s_Result += std::to_string(u32_Type);
    s_Result += ")";
    
    return s_Result;
}
