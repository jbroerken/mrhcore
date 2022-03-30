/**
 *  PlatformServicePool.cpp
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
#include <memory>

// External

// Project
#include "./PlatformServicePool.h"
#include "../../Platform/PlatformServiceProcess.h"
#include "../../../Configuration/PlatformServiceList.h"
#include "../../../Configuration/UserEventRoute.h"
#include "../../../Configuration/CoreConfiguration.h"
#include "../../../FilePaths.h"
#include "../../../Logger/Logger.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PlatformServicePool::PlatformServicePool()
{
    CoreConfiguration& c_CoreConfiguration = CoreConfiguration::Singleton();
    MRH_Uint32 u32_EventLimit = c_CoreConfiguration.GetEventLimit(CoreConfiguration::PLATFORM_SERVICE);
    MRH_Sint32 s32_RecieveTimeoutMS = c_CoreConfiguration.GetRecieveTimeoutMS(CoreConfiguration::PLATFORM_SERVICE);
    
    try // Giant block, but all depends on service list being read successfully!
    {
        // Load event routes
        UserEventRoute c_EventRoute;
        
        // Create each service to populate vector
        PlatformServiceList c_ServiceList;
        size_t us_ServiceCount = c_ServiceList.GetServiceCount();
        std::vector<pid_t> v_Pid;
        
        if (us_ServiceCount == 0)
        {
            throw ProcessException("No platform services in list!");
        }
        
        for (size_t i = 0; i < us_ServiceCount; ++i)
        {
            PlatformServiceList::Service const& c_Service = c_ServiceList.GetService(i);
            
            // Simply skip disabled services, essential or not
            if (c_Service.b_Disabled == true)
            {
                Logger::Singleton().Log(Logger::INFO, "Platform service " +
                                                      c_Service.s_BinaryPath +
                                                      " is disabled.",
                                        "PlatformServicePool.cpp", __LINE__);
                continue;
            }
            
            // Add the event route for this service
            auto Route = m_EventRoute.find(c_Service.u32_RouteID);
            if (Route == m_EventRoute.end())
            {
                if (m_EventRoute.insert(std::make_pair(c_Service.u32_RouteID, c_EventRoute.GetRoute(c_Service.u32_RouteID))).second == false)
                {
                    throw ProcessException("Failed to add event route!");
                }
            }
            
            // We rather cast the shared_ptr to have a guarantee that this instance
            // deallocates on exception by going out of scope. This will also
            // terminate the running process.
            std::shared_ptr<ServiceProcess> p_Process(new PlatformServiceProcess());
            std::dynamic_pointer_cast<PlatformServiceProcess>(p_Process)->Run(c_Service.s_BinaryPath,
                                                                              u32_EventLimit,
                                                                              s32_RecieveTimeoutMS);
            
            v_Service.emplace_back(std::shared_ptr<PlatformService>(new PlatformService(p_Process,
                                                                                        p_Condition,
                                                                                        u32_EventLimit,
                                                                                        s32_RecieveTimeoutMS,
                                                                                        c_Service.b_Essential,
                                                                                        c_Service.u32_RouteID)));
            v_Pid.emplace_back((*(--(v_Service.end())))->GetProcess()->GetProcessID());
        }
        
        // Write pid list
        WritePidList(MRH_CORE_PLATFORM_SERVICE_PID_FILE, v_Pid);
    }
    catch (ProcessException& e) // No individual service process check, one crashes -> All failed
    {
        throw;
    }
    catch (ConfigurationException& e)
    {
        throw ProcessException(e.what2() + " (" + e.filepath2() + ")");
    }
    catch (std::exception& e) // Catches all other exceptions
    {
        throw ProcessException("Failed to create platform service: " + std::string(e.what()));
    }
    
    // Finally, start
    StartUpdate(s32_RecieveTimeoutMS);
}

PlatformServicePool::~PlatformServicePool() noexcept
{
    // Clear pid list
    WritePidList(MRH_CORE_PLATFORM_SERVICE_PID_FILE, {});
    
    // Stop and clear handled by base destructor
}

//*************************************************************************************
// Send
//*************************************************************************************

void PlatformServicePool::DistributeSendEvents() noexcept
{
    // Lock send queue
    // NOTE: This lock stops the service pool update thread. The caller
    //       would have to wait anyway until all events are processed.
    p_Mutex[SEND].lock();
    
    // Loop event list and service list. This makes sure events are properly
    // duplicated for each service with a valid source.
    // NOTE: Event list is the outer loop for shorter inner loops
    for (auto& Event : p_Queue[SEND])
    {
        // Is this a valid event number?
        if (Event.GetType() > MRH_EVENT_TYPE_MAX)
        {
            Logger::Singleton().Log(Logger::WARNING, "Event " +
                                                     std::to_string(Event.GetType()) +
                                                     " is unknown!",
                                    "PlatformServicePool.cpp", __LINE__);
            continue;
        }
        
        // Check which type of event this is
        switch (Event.GetType())
        {
            /**
             *  Event Version 1
             */
 
            // Unknown
            case MRH_EVENT_UNK:
                
            // Permission
            case MRH_EVENT_PERMISSION_DENIED:
                
            // Password Required
            case MRH_EVENT_PASSWORD_REQUIRED:
                
            // Not implemented
            case MRH_EVENT_NOT_IMPLEMENTED_S:
                
            // Program state
            case MRH_EVENT_PS_RESET_ACKNOLEDGED_U: // Not sent by the user, block
                
            // Custom
            case MRH_EVENT_CUSTOM_AVAIL_S:
            case MRH_EVENT_CUSTOM_CUSTOM_COMMAND_S:
                
            // Voice - Listen
            case MRH_EVENT_LISTEN_AVAIL_S:
            case MRH_EVENT_LISTEN_STRING_S:
            case MRH_EVENT_LISTEN_GET_METHOD_S:
            case MRH_EVENT_LISTEN_CUSTOM_COMMAND_S:
                
            // Voice - Say
            case MRH_EVENT_SAY_AVAIL_S:
            case MRH_EVENT_SAY_STRING_S:
            case MRH_EVENT_SAY_GET_METHOD_S:
            case MRH_EVENT_SAY_NOTIFICATION_APP_S:
            case MRH_EVENT_SAY_CUSTOM_COMMAND_S:
                
            // Password
            case MRH_EVENT_PASSWORD_AVAIL_S:
            case MRH_EVENT_PASSWORD_CHECK_S:
            case MRH_EVENT_PASSWORD_SET_S:
            case MRH_EVENT_PASSWORD_CUSTOM_COMMAND_S:
                
            // User
            case MRH_EVENT_USER_AVAIL_S:
            case MRH_EVENT_USER_ACCESS_DOCUMENTS_S:
            case MRH_EVENT_USER_ACCESS_PICTURES_S:
            case MRH_EVENT_USER_ACCESS_MUSIC_S:
            case MRH_EVENT_USER_ACCESS_VIDEOS_S:
            case MRH_EVENT_USER_ACCESS_DOWNLOADS_S:
            case MRH_EVENT_USER_ACCESS_CLIPBOARD_S:
            case MRH_EVENT_USER_ACCESS_INFO_PERSON_S:
            case MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_S:
            case MRH_EVENT_USER_ACCESS_CLEAR_S:
            case MRH_EVENT_USER_GET_LOCATION_S:
            case MRH_EVENT_USER_CUSTOM_COMMAND_S:
                
            // Application
            case MRH_EVENT_APP_AVAIL_S:
            case MRH_EVENT_APP_LAUNCH_SOA_S:
            case MRH_EVENT_APP_LAUNCH_SOA_TIMER_S:
            case MRH_EVENT_APP_LAUNCH_SOA_TIMER_REMINDER_S:
            case MRH_EVENT_APP_LAUNCH_SOA_CLEAR_S:
            case MRH_EVENT_APP_LAUNCH_SOA_CLEAR_TIMER_S:
            case MRH_EVENT_APP_CUSTOM_COMMAND_S:
                Logger::Singleton().Log(Logger::WARNING, "Event " +
                                                         std::to_string(Event.GetType()) +
                                                         " is not allowed to be sent to platform services!",
                                        "PlatformServicePool.cpp", __LINE__);
                break;
                
            /**
             *  All - User Event
             */
                
            // The event is not from a service, give it to all services requiring it
            default:
                for (auto& Service : v_Service) // No service locking, not meant to be changed
                {
                    try
                    {
                        auto p_Service = std::dynamic_pointer_cast<PlatformService>(Service);
                        
                        if (GetEventInRoute(p_Service->GetRouteID(), Event.GetType()) == true)
                        {
                            p_Service->SendEvent(Event);
                        }
                    }
                    catch (std::exception& e) // dynamic cast
                    {
                        Logger::Singleton().Log(Logger::WARNING, e.what(), "PlatformServicePool.cpp", __LINE__);
                    }
                }
                break;
        }
    }
    
    // All sent, clear original
    p_Queue[SEND].clear();
    p_Mutex[SEND].unlock();
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool PlatformServicePool::GetEventInRoute(MRH_Uint32 u32_RouteID, MRH_Uint32 u32_Type) noexcept
{
    auto Route = m_EventRoute.find(u32_RouteID);
    
    if (Route != m_EventRoute.end())
    {
        for (auto& Type : Route->second)
        {
            if (Type == u32_Type)
            {
                return true;
            }
        }
    }
    
    return false;
}

bool PlatformServicePool::GetAllRunning() const noexcept
{
    for (auto& Service : v_Service)
    {
        if (Service->GetProcess()->GetRunning() == false)
        {
            return false;
        }
    }
    
    return true;
}
