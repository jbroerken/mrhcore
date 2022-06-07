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

// External

// Project
#include "./UserServicePool.h"
#include "./UserService.h"
#include "../../User/UserServiceProcess.h"
#include "../../../Configuration/UserServiceList.h"
#include "../../../Configuration/CoreConfiguration.h"
#include "../../../Package/PackageContainer.h"
#include "../../../FilePaths.h"
#include "../../../Logger/Logger.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

UserServicePool::UserServicePool()
{
    try
    {
        // Create each service to populate vector
        UserServiceList c_ServiceList;
        size_t us_PackageCount = c_ServiceList.GetPackageCount();
        std::vector<pid_t> v_Pid;
        
        if (PackageContainer::Singleton().GetPackageCount() == 0 || us_PackageCount == 0)
        {
            // No user services, maybe none installed or none enabled
            Logger::Singleton().Log(Logger::INFO, "No user service packages.", "UserServiceProcess.cpp", __LINE__);
            return;
        }
        
        for (size_t i = 0; i < us_PackageCount; ++i)
        {
            try
            {
                v_Pid.emplace_back(AddService(c_ServiceList.GetPackage(i)));
            }
            catch (std::exception& e) // Catches all other exceptions
            {
                Logger::Singleton().Log(Logger::WARNING, "Failed to add user process: " +
                                                         std::string(e.what()),
                                        "UserServicePool.cpp", __LINE__);
            }
        }
        
        // All Added, write pid list
        WritePidList(MRH_CORE_USER_SERVICE_PID_FILE, v_Pid);
    }
    catch (ConfigurationException& e)
    {
        throw ProcessException(e.what());
    }
    catch (std::exception& e) // Catches all other exceptions
    {
        throw ProcessException("Failed to create user service: " + std::string(e.what()));
    }
    
    // Finally, start
    StartUpdate(CoreConfiguration::Singleton().GetRecieveTimeoutMS(CoreConfiguration::USER_SERVICE));
}

UserServicePool::~UserServicePool() noexcept
{
    // Clear pid list
    WritePidList(MRH_CORE_PLATFORM_SERVICE_PID_FILE, {});
    
    // Stop and clear handled by base destructor
}

//*************************************************************************************
// Update
//*************************************************************************************

pid_t UserServicePool::AddService(std::string const& s_PackageName)
{
    CoreConfiguration& c_CoreConfiguration = CoreConfiguration::Singleton();
    MRH_Uint32 u32_EventLimit = c_CoreConfiguration.GetEventLimit(CoreConfiguration::USER_SERVICE);
    
    try
    {
        Package const& s_Package = PackageContainer::Singleton().GetPackage(s_PackageName);
        
        // We rather cast the shared_ptr to have a guarantee that this instance
        // deallocates on exception by going out of scope. This will also
        // terminate the running process.
        std::shared_ptr<ServiceProcess> p_Process(new UserServiceProcess());
        std::dynamic_pointer_cast<UserServiceProcess>(p_Process)->Run(s_Package,
                                                                      c_CoreConfiguration.GetAppServiceParentBinaryPath(),
                                                                      u32_EventLimit);

        v_Service.emplace_back(std::shared_ptr<UserService>(new UserService(p_Process,
                                                                            p_Condition,
                                                                            u32_EventLimit,
                                                                            c_CoreConfiguration.GetRecieveTimeoutMS(CoreConfiguration::USER_SERVICE))));
        
        // Oof
        return (*(--(v_Service.end())))->GetProcess()->GetProcessID();
    }
    catch (std::exception& e) // Catches all other exceptions
    {
        throw ProcessException(e.what());
    }
}

void UserServicePool::Reload()
{
    // Stop the service pool update
    if (GetRunning() == true)
    {
        StopUpdate();
    }
    
    try // Giant block, but all depends on first element: the configuration
    {
        UserServiceList c_ServiceList;
        
        // Remove all services no longer on the list
        // @NOTE: The thread is stopped, no locking needed for service
        //        operations.
        for (auto Service = v_Service.begin(); Service != v_Service.end();)
        {
            if ((*Service)->GetProcess()->GetRunning() == false || c_ServiceList.GetUserService((*Service)->GetProcess()->GetRunPath()) == false)
            {
                Service = v_Service.erase(Service);
            }
        }
        
        // Add all new services in the list
        size_t us_PackageCount = c_ServiceList.GetPackageCount();
        std::string s_PackageName;
        bool b_AddService;
        std::vector<pid_t> v_Pid;
        
        for (size_t i = 0; i < us_PackageCount; ++i)
        {
            // Setup initial service info for adding or removing
            try
            {
                s_PackageName = c_ServiceList.GetPackage(i);
                b_AddService = true;
            }
            catch (...) // Don't enter outer exception body, keep going!
            {
                // Somehow out of bounds, config error, etc. so we skip
                Logger::Singleton().Log(Logger::WARNING, "Failed to get user service " +
                                                         std::to_string(i) +
                                                         " from the user service configuration.",
                                        "UserServicePool.cpp", __LINE__);
                continue;
            }
            
            // Now check if the service is already added to the thread pool
            for (auto& Service : v_Service)
            {
                if (Service->GetProcess()->GetRunPath().compare(s_PackageName) == 0)
                {
                    b_AddService = false;
                    break;
                }
            }
            
            if (b_AddService == true)
            {
                
                try
                {
                    AddService(s_PackageName);
                }
                catch (std::exception& e) // Catches all other exceptions
                {
                    Logger::Singleton().Log(Logger::WARNING, "Failed to add user process: " +
                                                             std::string(e.what()),
                                            "UserServicePool.cpp", __LINE__);
                }
            }
        }
        
        // Grab current pids
        for (auto& Service : v_Service)
        {
            v_Pid.emplace_back(Service->GetProcess()->GetProcessID());
        }
        
        WritePidList(MRH_CORE_USER_SERVICE_PID_FILE, v_Pid);
    }
    catch (ConfigurationException& e)
    {
        throw ProcessException(e.what());
    }
    catch (std::exception& e)
    {
        throw ProcessException(e.what());
    }
    
    // Start update again
    StartUpdate(CoreConfiguration::Singleton().GetRecieveTimeoutMS(CoreConfiguration::USER_SERVICE));
}

//*************************************************************************************
// Send
//*************************************************************************************

void UserServicePool::DistributeSendEvents() noexcept
{
    // User services cannot have events sent to them,
    // so we dummy out this function.
    return;
}
