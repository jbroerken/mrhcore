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
#include <cstring>

// External
#include <libmrhbf.h>

// Project
#include "./CoreConfiguration.h"
#include "../Logger/Logger.h"
#include "../FilePaths.h"

// Pre-defined
namespace
{
    enum Identifier
    {
        // Block Name
        BLOCK_CONFIGURATION = 0,
        
        // Configuration Key
        APP_PARENT_BINARY_PATH = 1,
        APP_SERVICE_PARENT_BINARY_PATH = 2,
        TIMER_FORCE_STOP = 3,
        TIMER_WAIT_SLEEP = 4,
        USER_APP_RECIEVE_TIMEOUT = 5,
        USER_SERVICE_RECIEVE_TIMEOUT = 6,
        PLATFORM_SERVICE_RECIEVE_TIMEOUT = 7,
        USER_APP_EVENT_LIMIT,
        USER_SERVICE_EVENT_LIMIT,
        PLATFORM_SERVICE_EVENT_LIMIT,
        HOME_PACKAGE_PATH,
        HOME_LAUNCH_COMMAND_ID_DEFAULT,
        HOME_LAUNCH_COMMAND_ID_STARTUP,

        // Bounds
        IDENTIFIER_MAX = HOME_LAUNCH_COMMAND_ID_STARTUP,

        IDENTIFIER_COUNT = IDENTIFIER_MAX + 1
    };

    const char* p_Identifier[IDENTIFIER_COUNT] =
    {
        // Block Name
        "Core",
        
        // Configuration Key
        "AppParentBinaryPath",
        "AppServiceParentBinaryPath",
        "ForceStopProcessS",
        "ThreadWaitSleepMS",
        "UserAppRecieveTimeoutMS",
        "UserServiceRecieveTimeoutMS",
        "PlatformServiceRecieveTimeoutMS",
        "UserAppEventLimit",
        "UserServiceEventLimit",
        "PlatformServiceEventLimit",
        "HomePackagePath",
        "HomePackageDefaultLaunchCommandID",
        "HomePackageStartupLaunchCommandID"
    };
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

CoreConfiguration::CoreConfiguration() noexcept : s_AppParentBinaryPath(""),
                                                  s_AppServiceParentBinaryPath(""),
                                                  u32_ForceStopTimerS(3),
                                                  u32_WaitSleepTimerMS(100),
                                                  s_HomePackagePath(""),
                                                  i_HomePackageDefaultLaunchCommandID(0),
                                                  i_HomePackageStartupLaunchCommandID(0)
{
    for (size_t i = 0; i < QUEUE_COUNT; ++i)
    {
        p_RecieveTimeoutMS[i] = 100;
        p_EventLimit[i] = 10;
    }
}

CoreConfiguration::~CoreConfiguration() noexcept
{}

//*************************************************************************************
// Singleton
//*************************************************************************************

CoreConfiguration& CoreConfiguration::Singleton() noexcept
{
    static CoreConfiguration c_CoreConfiguration;
    return c_CoreConfiguration;
}

//*************************************************************************************
// Load
//*************************************************************************************

void CoreConfiguration::Load()
{
    try
    {
        MRH_BlockFile c_File(MRH_CORE_CONFIGURATION_FILE_PATH);
        
        for (auto& Block : c_File.l_Block)
        {
            if (Block.GetName().compare(p_Identifier[BLOCK_CONFIGURATION]) != 0)
            {
                continue;
            }
            
            // Paths
            s_AppParentBinaryPath = Block.GetValue(p_Identifier[APP_PARENT_BINARY_PATH]);
            s_AppServiceParentBinaryPath = Block.GetValue(p_Identifier[APP_SERVICE_PARENT_BINARY_PATH]);
        
            // Timers
            u32_ForceStopTimerS = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[TIMER_FORCE_STOP])));
            u32_WaitSleepTimerMS = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[TIMER_WAIT_SLEEP])));
            p_RecieveTimeoutMS[USER_APP] = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[USER_APP_RECIEVE_TIMEOUT])));
            p_RecieveTimeoutMS[USER_SERVICE] = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[USER_SERVICE_RECIEVE_TIMEOUT])));
            p_RecieveTimeoutMS[PLATFORM_SERVICE] = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[PLATFORM_SERVICE_RECIEVE_TIMEOUT])));
            
            // Event
            if ((p_EventLimit[USER_APP] = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[USER_APP_EVENT_LIMIT])))) == 0)
            {
                p_EventLimit[USER_APP] = 1;
            }
            
            if ((p_EventLimit[USER_SERVICE] = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[USER_SERVICE_EVENT_LIMIT])))) == 0)
            {
                p_EventLimit[USER_SERVICE] = 1;
            }
            
            if ((p_EventLimit[PLATFORM_SERVICE] = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[PLATFORM_SERVICE_EVENT_LIMIT])))) == 0)
            {
                p_EventLimit[PLATFORM_SERVICE] = 1;
            }
            
            // Home (default) package
            s_HomePackagePath = Block.GetValue(p_Identifier[HOME_PACKAGE_PATH]);
            
            if (s_HomePackagePath.size() > 0 && *(s_HomePackagePath.end() - 1) != '/')
            {
                s_HomePackagePath += "/";
            }
            
            i_HomePackageDefaultLaunchCommandID = std::stoi(Block.GetValue(p_Identifier[HOME_LAUNCH_COMMAND_ID_DEFAULT]));
            i_HomePackageStartupLaunchCommandID = std::stoi(Block.GetValue(p_Identifier[HOME_LAUNCH_COMMAND_ID_STARTUP]));
            
            break;
        }
    }
    catch (std::exception& e)
    {
        throw ConfigurationException("Could not read core configuration: " + std::string(e.what()), MRH_CORE_CONFIGURATION_FILE_PATH);
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

std::string CoreConfiguration::GetAppParentBinaryPath() const noexcept
{
    return s_AppParentBinaryPath;
}

std::string CoreConfiguration::GetAppServiceParentBinaryPath() const noexcept
{
    return s_AppServiceParentBinaryPath;
}

MRH_Uint32 CoreConfiguration::GetForceStopTimerS() const noexcept
{
    return u32_ForceStopTimerS;
}

MRH_Uint32 CoreConfiguration::GetWaitSleepTimerMS() const noexcept
{
    return u32_WaitSleepTimerMS;
}

MRH_Sint32 CoreConfiguration::GetRecieveTimeoutMS(Queue e_Queue) const
{
    if (e_Queue > QUEUE_MAX)
    {
        throw ConfigurationException("Invalid queue: " + std::to_string(e_Queue), MRH_CORE_CONFIGURATION_FILE_PATH);
    }
    
    return p_RecieveTimeoutMS[e_Queue];
}

MRH_Uint32 CoreConfiguration::GetEventLimit(Queue e_Queue) const
{
    if (e_Queue > QUEUE_MAX)
    {
        throw ConfigurationException("Invalid queue: " + std::to_string(e_Queue), MRH_CORE_CONFIGURATION_FILE_PATH);
    }
    
    return p_EventLimit[e_Queue];
}

std::string CoreConfiguration::GetHomePackagePath() const noexcept
{
    return s_HomePackagePath;
}

int CoreConfiguration::GetHomePackageDefaultLaunchCommandID() const noexcept
{
    return i_HomePackageDefaultLaunchCommandID;
}

int CoreConfiguration::GetHomePackageStartupLaunchCommandID() const noexcept
{
    return i_HomePackageStartupLaunchCommandID;
}
