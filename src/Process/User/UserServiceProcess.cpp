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

// External

// Project
#include "./UserServiceProcess.h"
#include "../../Logger/Logger.h"
#include "../../Logger/EventLogger.h"

// Pre-defined
namespace
{
    constexpr int i_EventVerMin = 1;
    constexpr int i_EventVerMax = MRH_EVENT_VERSION;
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

#ifdef __MRH_MRHCKM_SUPPORTED__
UserServiceProcess::UserServiceProcess() : ServiceProcess(TransmissionSource::SourceType::MRHCKM,
                                                          false,
                                                          true),
                                           UserPermission(true)
#else
UserServiceProcess::UserServiceProcess() : ServiceProcess(TransmissionSource::SourceType::PIPE,
                                                          false,
                                                          true),
                                           UserPermission(true)
#endif
{
    // Package
#if MRH_CORE_EVENT_LOGGING > 0
    s_PackagePath = "<undefined>";
#endif
    
    // Event version
    i_EventVer = -1;
}

UserServiceProcess::~UserServiceProcess() noexcept
{
    // @NOTE: ServiceProcess class terminates process on destruction!
}

//*************************************************************************************
// Run
//*************************************************************************************

void UserServiceProcess::Run(Package const& c_Package, std::string s_BinaryPath, MRH_Uint32 u32_EventLimit)
{
    // Set run path
    this->s_RunPath = c_Package.GetPackagePath();
    
    // Re-create event queue
    try
    {
        EventQueue::Reset();
    }
    catch (EventException& e)
    {
        throw ProcessException("Failed to reset event queue: " + e.what2()); // Convert to process exception
    }
    
    // Basics reset, check package event ver for communication
    i_EventVer = c_Package.PackageService::GetServiceEventVersion();
    
    if (i_EventVer < i_EventVerMin || i_EventVer > i_EventVerMax)
    {
        throw ProcessException("Invalid package service event version: Got " +
                               std::to_string(i_EventVer) +
                               ", but bounds are " +
                               std::to_string(i_EventVerMin) +
                               " (Min) to " +
                               std::to_string(i_EventVerMax) +
                               " (Max)!");
    }
    
    // Set permissions for later, we lose s_Package given
    try
    {
        UpdatePermissions(c_Package);
    }
    catch (ProcessException& e)
    {
        throw ProcessException("Failed to set user service process permissions: " + e.what2());
    }
    
    // Create arguments
    std::vector<std::vector<char>> v_Arg;
    
    try
    {
        v_Arg.emplace_back(GetArgumentBytes(this->s_RunPath));
        v_Arg.emplace_back(GetArgumentBytes(std::to_string(GetPipeFD(QueueType::C_W_P_R, SourcePipe::PipeEnd::PIPE_END_WRITE))));
        v_Arg.emplace_back(GetArgumentBytes(std::to_string(u32_EventLimit)));
        
        // Update the package path after a successfull launch
#if MRH_CORE_EVENT_LOGGING > 0
        s_PackagePath = c_Package.GetPackagePath();
#endif
    }
    catch (ProcessException& e)
    {
        throw ProcessException("Failed to setup user service process launch arguments: " + e.what2()); // More info
    }
    catch (std::exception& e)
    {
        throw ProcessException("Failed to setup user service process launch arguments: " + std::string(e.what()));
    }
    
    // Run
    try
    {
        Process::Run(s_BinaryPath, v_Arg);
    }
    catch (ProcessException& e)
    {
        throw;
    }
}

//*************************************************************************************
// Recieve
//*************************************************************************************

#if MRH_CORE_EVENT_LOGGING > 0
void UserServiceProcess::LogRecievedEvents(Event const& c_Event) noexcept
{
    EventLogger::Singleton().Log(c_Event, "Recieved event from user service process: " + s_PackagePath);
}
#endif

std::vector<Event>& UserServiceProcess::RetrieveEvents() noexcept
{
    // Filter events based on service permissions
    std::vector<Event>& v_Event = EventQueue::RetrieveEvents();
    
    // Run filters
    FilterEventsPermission(v_Event, false); // No response for services
    
    if (i_EventVer < i_EventVerMax)
    {
        FilterEventsVersion(v_Event, i_EventVer);
    }
    
    return v_Event;
}

//*************************************************************************************
// Send
//*************************************************************************************

#if MRH_CORE_EVENT_LOGGING > 0
void UserServiceProcess::LogSentEvents(Event const& c_Event) noexcept
{
    EventLogger::Singleton().Log(c_Event, "Sending event to user service process: " +
                                          s_PackagePath +
                                          " (User servies are not allowed to have events sent to them!)");
}
#endif
