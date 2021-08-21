/**
 *  EventLogger.cpp
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
        snprintf(p_Hex, 32, "%X", p_Data[i]); // NOTE: Anything wrong with the loop? check this hellspawn function
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
    
    v_Message.emplace_back(std::to_string(u64_EventID) + ": " + GetEventNameString(c_Event.GetType()));
    v_Message.emplace_back(v_Message[0].length(), '=');
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
    
    v_Message.emplace_back(""); // Empty line to separate
    
    c_Mutex.lock();
    
    for (auto& String : v_Message)
    {
        if (f_EventLogFile.is_open() == true)
        {
            f_EventLogFile << String << std::endl;
        }
        
        if (MRH_EVENT_LOGGER_PRINT_CLI > 0)
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
        case MRH_EVENT_CUSTOM_CUSTOM_COMMAND_U:
            s_Result = "MRH_EVENT_CUSTOM_CUSTOM_COMMAND_U";
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
        case MRH_EVENT_APP_GET_DEFAULT_PACKAGE_U:
            s_Result = "MRH_EVENT_APP_GET_DEFAULT_PACKAGE_U";
            break;
        case MRH_EVENT_APP_SET_DEFAULT_PACKAGE_U:
            s_Result = "MRH_EVENT_APP_SET_DEFAULT_PACKAGE_U";
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
        case MRH_EVENT_APP_GET_DEFAULT_PACKAGE_S:
            s_Result = "MRH_EVENT_APP_GET_DEFAULT_PACKAGE_S";
            break;
        case MRH_EVENT_APP_SET_DEFAULT_PACKAGE_S:
            s_Result = "MRH_EVENT_APP_SET_DEFAULT_PACKAGE_S";
            break;
        case MRH_EVENT_APP_CUSTOM_COMMAND_S:
            s_Result = "MRH_EVENT_APP_CUSTOM_COMMAND_S";
            break;
            
        // Notifications
        case MRH_EVENT_NOTIFICATION_AVAIL_U:
            s_Result = "MRH_EVENT_NOTIFICATION_AVAIL_U";
            break;
        case MRH_EVENT_NOTIFICATION_CREATE_APP_U:
            s_Result = "MRH_EVENT_NOTIFICATION_CREATE_APP_U";
            break;
        case MRH_EVENT_NOTIFICATION_CREATE_SERVICE_U:
            s_Result = "MRH_EVENT_NOTIFICATION_CREATE_SERVICE_U";
            break;
        case MRH_EVENT_NOTIFICATION_DESTROY_APP_U:
            s_Result = "MRH_EVENT_NOTIFICATION_DESTROY_APP_U";
            break;
        case MRH_EVENT_NOTIFICATION_GET_WAITING_U:
            s_Result = "MRH_EVENT_NOTIFICATION_GET_WAITING_U";
            break;
        case MRH_EVENT_NOTIFICATION_GET_NEXT_U:
            s_Result = "MRH_EVENT_NOTIFICATION_GET_NEXT_U";
            break;
        case MRH_EVENT_NOTIFICATION_CUSTOM_COMMAND_U:
            s_Result = "MRH_EVENT_NOTIFICATION_CUSTOM_COMMAND_U";
            break;
            
        case MRH_EVENT_NOTIFICATION_AVAIL_S:
            s_Result = "MRH_EVENT_NOTIFICATION_AVAIL_S";
            break;
        case MRH_EVENT_NOTIFICATION_CREATE_APP_S:
            s_Result = "MRH_EVENT_NOTIFICATION_CREATE_APP_S";
            break;
        case MRH_EVENT_NOTIFICATION_DESTROY_APP_S:
            s_Result = "MRH_EVENT_NOTIFICATION_DESTROY_APP_S";
            break;
        case MRH_EVENT_NOTIFICATION_GET_WAITING_S:
            s_Result = "MRH_EVENT_NOTIFICATION_GET_WAITING_S";
            break;
        case MRH_EVENT_NOTIFICATION_GET_NEXT_S:
            s_Result = "MRH_EVENT_NOTIFICATION_GET_NEXT_S";
            break;
        case MRH_EVENT_NOTIFICATION_CUSTOM_COMMAND_S:
            s_Result = "MRH_EVENT_NOTIFICATION_CUSTOM_COMMAND_S";
            break;
            
        default:
            s_Result = "UNKNOWN EVENT";
            break;
    }
    
    s_Result += " (";
    s_Result += std::to_string(u32_Type);
    s_Result += ")";
    
    return s_Result;
}
