/**
 *  UserProcess.cpp
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
#include <fstream>

// External

// Project
#include "./UserProcess.h"
#include "./UserPermission.h"
#include "../../FilePaths.h"
#include "../../Logger/Logger.h"
#include "../../Logger/EventLogger.h"

// Pre-defined
namespace
{
    constexpr int i_EventVerMin = 1;
    constexpr int i_EventVerMax = MRH_EVENT_VERSION;
}

#define MRH_USER_PROCESS_PID_FILE_PATH MRH_CORE_PID_FILE_DIR "" MRH_CORE_USER_PID_FILE
#define MRH_CORE_LAUNCH_INPUT_FILE_PATH MRH_CORE_LAUNCH_INPUT_DIR "" MRH_CORE_LAUNCH_INPUT_FILE


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

#ifdef __MRH_MRHCKM_SUPPORTED__
UserProcess::UserProcess() : EventQueue(TransmissionSource::SourceType::MRHCKM),
                             UserPermission(false)
#else
UserProcess::UserProcess() : EventQueue(TransmissionSource::SourceType::PIPE),
                             UserPermission(false)
#endif
{
    // Initial event group start
    u32_EventGroupID = rand() % RAND_MAX;
    u32_PreviousEventGroupID = u32_EventGroupID;
    
    // Initial event version
    i_EventVer = -1;
    
    // Initial reset state
    e_ResetState = REQUIRE_REQUEST;
    
    // Package
#if MRH_CORE_EVENT_LOGGING > 0
    s_PackagePath = "<undefined>";
#endif
}

UserProcess::~UserProcess() noexcept
{
    // Clear PID
    std::ofstream f_File(MRH_USER_PROCESS_PID_FILE_PATH, std::ios::trunc);
    f_File.close();
    
    // @NOTE: Process class terminates process on destruction!
}

//*************************************************************************************
// Run
//*************************************************************************************

void UserProcess::Run(Package const& c_Package, int i_LaunchCommandID, std::string s_LaunchInput, std::string s_AppParentBinaryPath, MRH_Uint32 u32_EventLimit, MRH_Sint32 s32_RecieveTimeoutMS)
{
    // Re-create event queue
    try
    {
        EventQueue::Reset();
    }
    catch (EventException& e)
    {
        throw ProcessException("Failed to reset event queue: " + e.what2());
    }
    
    // Get a new event group id
    while (u32_EventGroupID == u32_PreviousEventGroupID)
    {
        u32_EventGroupID = rand() % RAND_MAX;
    }
    
    u32_PreviousEventGroupID = u32_EventGroupID;
    
    // Basics reset, check package event ver for communication
    i_EventVer = c_Package.PackageApp::GetAppEventVersion();
    
    if (i_EventVer < i_EventVerMin || i_EventVer > i_EventVerMax)
    {
        throw ProcessException("Invalid package app event version: Got " +
                               std::to_string(i_EventVer) +
                               ", but bounds are " +
                               std::to_string(i_EventVerMin) +
                               " (Min) to " +
                               std::to_string(i_EventVerMax) +
                               " (Max)!");
    }
    
    // Set event limits
    this->u32_EventLimit = u32_EventLimit;
    this->s32_RecieveTimeoutMS = s32_RecieveTimeoutMS;
    
    // Set initial reset state
    e_ResetState = REQUIRE_REQUEST;
    Logger::Singleton().Log(Logger::INFO, "User process now requires a reset request.",
                            "UserProcess.cpp", __LINE__);
    
    // Set permissions for later, we lose s_Package given
    try
    {
        UpdatePermissions(c_Package);
    }
    catch (ProcessException& e)
    {
        throw ProcessException("Failed to set user process permissions: " + e.what2());
    }
    
    // Write launch input to file
    std::ofstream f_InputFile(MRH_CORE_LAUNCH_INPUT_FILE_PATH, std::ios::trunc);
    
    if (f_InputFile.is_open() == false)
    {
        throw ProcessException("Failed to open launch input file!");
    }
    
    f_InputFile << s_LaunchInput;
    f_InputFile.close();
    
    // Create arguments
    std::vector<std::vector<char>> v_Arg;
    
    try
    {
        v_Arg.emplace_back(GetArgumentBytes(c_Package.GetPackagePath()));
        v_Arg.emplace_back(GetArgumentBytes(std::to_string(GetPipeFD(QueueType::P_W_C_R, SourcePipe::PipeEnd::PIPE_END_READ))));
        v_Arg.emplace_back(GetArgumentBytes(std::to_string(GetPipeFD(QueueType::C_W_P_R, SourcePipe::PipeEnd::PIPE_END_WRITE))));
        v_Arg.emplace_back(GetArgumentBytes(std::to_string(u32_EventGroupID)));
        v_Arg.emplace_back(GetArgumentBytes(std::to_string(this->u32_EventLimit)));
        v_Arg.emplace_back(GetArgumentBytes(std::to_string(this->s32_RecieveTimeoutMS)));
        v_Arg.emplace_back(GetArgumentBytes(std::to_string(i_LaunchCommandID)));
        v_Arg.emplace_back(GetArgumentBytes(MRH_CORE_LAUNCH_INPUT_FILE_PATH));
        
        // Update the package path after a successfull launch
#if MRH_CORE_EVENT_LOGGING > 0
        s_PackagePath = c_Package.GetPackagePath();
#endif
    }
    catch (ProcessException& e)
    {
        throw ProcessException("Failed to setup user process launch arguments: " + e.what2());
    }
    catch (std::exception& e)
    {
        throw ProcessException("Failed to setup user process launch arguments: " + std::string(e.what()));
    }
    
    // Run
    try
    {
        Process::Run(s_AppParentBinaryPath, v_Arg);
    }
    catch (ProcessException& e)
    {
        throw;
    }
    
    // Write PID to file
    std::ofstream f_File(MRH_USER_PROCESS_PID_FILE_PATH, std::ios::trunc);
    
    if (f_File.is_open() == false)
    {
        f_File << std::to_string(GetProcessID());
        f_File.close();
    }
    else
    {
        Logger::Singleton().Log(Logger::WARNING, "Failed to write pid file: " MRH_USER_PROCESS_PID_FILE_PATH,
                                "UserProcess.cpp", __LINE__);
    }
    
    // @TODO: Add pid_t to MRHCKM if MRHCKM is used!
}

//*************************************************************************************
// Process Event Filter
//*************************************************************************************

void UserProcess::FilterEventsGroupID(std::vector<Event>& v_Event) noexcept
{
    for (auto Event = v_Event.begin(); Event != v_Event.end();)
    {
        switch (Event->GetType())
        {
            // Special events only sent by services without requesting them
            // should be inserted here.
            // These events would be sent to anyone, so we can ignore the group
            // ID here.
            case MRH_EVENT_LISTEN_STRING_S:
            case MRH_EVENT_SAY_STRING_S:
            case MRH_EVENT_APP_LAUNCH_SOA_TIMER_REMINDER_S:
                ++Event;
                break;
                
            default:
                if (Event->GetGroupID() != u32_EventGroupID)
                {
                    Logger::Singleton().Log(Logger::WARNING, "Cannot send user app event: Wrong process event group " +
                                                             std::to_string(Event->GetGroupID()) +
                                                             "!",
                                            "UserProcess.cpp", __LINE__);
                    
                    Event = v_Event.erase(Event);
                }
                else
                {
                    ++Event;
                }
                break;
        }
    }
}

bool UserProcess::FilterEventsResetRequest(std::vector<Event>& v_Event) noexcept
{
    while (v_Event.size() > 0)
    {
        if (v_Event[0].GetType() == MRH_EVENT_PS_RESET_REQUEST_U)
        {
            return true;
        }
        
        Logger::Singleton().Log(Logger::WARNING, "Cannot send user app event: Service reset not yet requested!",
                                "UserProcess.cpp", __LINE__);
        v_Event.erase(v_Event.begin());
    }
    
    return false;
}

//*************************************************************************************
// Recieve
//*************************************************************************************

#if MRH_CORE_EVENT_LOGGING > 0
void UserProcess::LogRecievedEvents(Event const& c_Event) noexcept
{
    EventLogger::Singleton().Log(c_Event, "Recieved event from user process: " + s_PackagePath);
}
#endif

void UserProcess::RecieveEvents() noexcept
{
    EventQueue::RecieveEvents(u32_EventLimit, s32_RecieveTimeoutMS);
}

std::vector<Event>& UserProcess::RetrieveEvents() noexcept
{
    std::vector<Event>& v_Event = EventQueue::RetrieveEvents();
    
    // Filter events by group id first
    FilterEventsGroupID(v_Event);
    
    // Next, filter on event version if needed
    if (i_EventVer < i_EventVerMax)
    {
        FilterEventsVersion(v_Event, i_EventVer);
    }
    
    // Now filter based on reset state
    switch (e_ResetState)
    {
        case REQUIRE_REQUEST:
            // Check for the event request and remove all events before it
            if (FilterEventsResetRequest(v_Event) == true)
            {
                Logger::Singleton().Log(Logger::INFO, "User process sent a service reset request.",
                                        "UserProcess.cpp", __LINE__);
                e_ResetState = SEND_RESPONSE;
            }
            break;
            
        default:
            // Filter based on app permissions and password
            FilterEventsPermission(v_Event, true);
            break;
    }
    
    return v_Event;
}

//*************************************************************************************
// Send
//*************************************************************************************

#if MRH_CORE_EVENT_LOGGING > 0
void UserProcess::LogSentEvents(Event const& c_Event) noexcept
{
    EventLogger::Singleton().Log(c_Event, "Sending event to user process: " + s_PackagePath);
}
#endif

void UserProcess::AddSendEvents(std::vector<Event>& v_Event) noexcept
{
    // Filter events by group id first
    FilterEventsGroupID(v_Event);
    
    // Next, filter on event version if needed
    if (i_EventVer < i_EventVerMax)
    {
        FilterEventsVersion(v_Event, i_EventVer);
    }
    
    // Now filter based on reset state
    switch (e_ResetState)
    {
        case SEND_RESPONSE:
            // Reset completed
            // @NOTE: We don't store events like speech input for instant recieval
            //        The app should inform the user (or the device - lights, etc.)
            //        once the app is available to use.
            v_Event.insert(v_Event.begin(), Event(u32_EventGroupID,
                                                  MRH_EVENT_PS_RESET_ACKNOLEDGED_U,
                                                  NULL,
                                                  0));
            e_ResetState = RESET_COMPLETE;
            Logger::Singleton().Log(Logger::INFO, "Sending reset request acknoledged response to user process.",
                                    "UserProcess.cpp", __LINE__);
        case RESET_COMPLETE:
            // Check app permissions and password, filter events
            // @NOTE: We don't add missing permission events here - some services always send without request
            FilterEventsPermission(v_Event, false);
            
            // We now add all no permission events in front of the other events
            if (v_PermissionDenied.size() > 0)
            {
                v_Event.reserve(v_Event.size() + v_PermissionDenied.size());
                v_Event.insert(v_Event.begin(), v_PermissionDenied.begin(), v_PermissionDenied.end());
                v_PermissionDenied.clear();
            }
            break;
            
        default:
            // Not allowed to recieve events
            v_Event.clear();
            break;
    }
    
    EventQueue::AddSendEvents(v_Event);
}

void UserProcess::SendEvents() noexcept
{
    EventQueue::SendEvents(u32_EventLimit);
}

void UserProcess::SendEvents(std::vector<Event>& v_Event) noexcept
{
    AddSendEvents(v_Event);
    EventQueue::SendEvents(v_Event, u32_EventLimit);
}
