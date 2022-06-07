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
#include "./InputLaunch.h"
#include "../../Package/PackageContainer.h"

// Pre-defined
namespace
{
    typedef enum
    {
        APP_LAUNCH_S_SIZE_PACKAGE_LENGTH = sizeof(MRH_Uint32),
        APP_LAUNCH_S_SIZE_INPUT_LENGTH = sizeof(MRH_Uint32),
        APP_LAUNCH_S_SIZE_LAUNCH_COMMAND_ID = sizeof(MRH_Sint32),
    
    }Event_Launch_SOA_S_Size;

    typedef enum
    {
        APP_LAUNCH_S_PACKAGE_LENGTH = 0,
        APP_LAUNCH_S_INPUT_LENGTH = APP_LAUNCH_S_PACKAGE_LENGTH + APP_LAUNCH_S_SIZE_PACKAGE_LENGTH,
        APP_LAUNCH_S_LAUNCH_COMMAND_ID = APP_LAUNCH_S_INPUT_LENGTH + APP_LAUNCH_S_SIZE_INPUT_LENGTH,
        APP_LAUNCH_S_PACKAGE_PATH = APP_LAUNCH_S_LAUNCH_COMMAND_ID + APP_LAUNCH_S_SIZE_LAUNCH_COMMAND_ID,
        APP_LAUNCH_S_INPUT_STRING = APP_LAUNCH_S_LAUNCH_COMMAND_ID + APP_LAUNCH_S_SIZE_LAUNCH_COMMAND_ID, // Depending on package string length
    
    }Event_Launch_SOA_S;

    constexpr MRH_Uint32 u32_MinDataSize = APP_LAUNCH_S_PACKAGE_PATH + 1; // terminator
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

InputLaunch::InputLaunch() noexcept : c_LaunchRequest("",
                                                      "",
                                                      -1), // Dummy
                                      b_LaunchRequested(false)
{}

InputLaunch::~InputLaunch() noexcept
{}

InputLaunch::LaunchRequest::LaunchRequest(std::string s_PackagePath,
                                          std::string s_LaunchInput,
                                          int i_LaunchCommand) noexcept : s_PackagePath(s_PackagePath),
                                                                          s_LaunchInput(s_LaunchInput),
                                                                          i_LaunchCommand(i_LaunchCommand)
{}

InputLaunch::LaunchRequest::LaunchRequest(LaunchRequest const& c_LaunchRequest) noexcept : s_PackagePath(c_LaunchRequest.s_PackagePath),
                                                                                           s_LaunchInput(c_LaunchRequest.s_LaunchInput),
                                                                                           i_LaunchCommand(c_LaunchRequest.i_LaunchCommand)
{}

InputLaunch::LaunchRequest::~LaunchRequest() noexcept
{}

//*************************************************************************************
// Clear
//*************************************************************************************

void InputLaunch::ClearLaunchRequest() noexcept
{
    // No need to clear the package info
    b_LaunchRequested = false;
}

//*************************************************************************************
// Update
//*************************************************************************************

void InputLaunch::UpdateLaunch(Event const& c_Event)
{
    if (c_Event.GetData() == NULL ||
        c_Event.GetDataSize() < u32_MinDataSize ||
        c_Event.CastValue<MRH_Uint32>(APP_LAUNCH_S_PACKAGE_LENGTH) == 0)
    {
        throw InputException("Invalid launch event data (Basic length!)", "Launch");
    }
    
    // Grab string info first
    MRH_Uint32 u32_PathLength = c_Event.CastValue<MRH_Uint32>(APP_LAUNCH_S_PACKAGE_LENGTH);
    MRH_Uint32 u32_InputLength = c_Event.CastValue<MRH_Uint32>(APP_LAUNCH_S_INPUT_LENGTH); // Event info = byte length
    
    if (c_Event.GetDataSize() < APP_LAUNCH_S_PACKAGE_PATH + u32_PathLength + u32_InputLength)
    {
        throw InputException("Invalid launch event data (Total buffer length)!", "Launch");
    }
    
    std::string s_PackagePath = std::string(c_Event.CastPointer<const char*>(APP_LAUNCH_S_PACKAGE_PATH),
                                            u32_PathLength);
    int i_LaunchCommandID = c_Event.CastValue<MRH_Sint32>(APP_LAUNCH_S_LAUNCH_COMMAND_ID);
    std::string s_LaunchInput = "";
    
    if (u32_InputLength > 0)
    {
        s_LaunchInput = std::string(c_Event.CastPointer<const char*>(APP_LAUNCH_S_INPUT_STRING + u32_PathLength), // See enum
                                    u32_InputLength);
    }
    
    // Use public for launch, checks package list
    try
    {
        UpdateLaunch(s_PackagePath, s_LaunchInput, i_LaunchCommandID);
    }
    catch (InputException& e)
    {
        throw;
    }
}

void InputLaunch::UpdateLaunch(std::string s_PackagePath, std::string s_LaunchInput, int i_LaunchCommand)
{
    // Only allow requests which exists, stop unlisted packages
    if (PackageContainer::Singleton().GetPackageExists(s_PackagePath) == false)
    {
        throw InputException("Tried to launch unknown package " + s_PackagePath, "Launch");
    }
    
    // Overwrite launch, only 1 active at a given time
    if (*((c_LaunchRequest.s_PackagePath = s_PackagePath).end() - 1) != '/')
    {
        c_LaunchRequest.s_PackagePath += "/";
    }
    
    c_LaunchRequest.s_LaunchInput = s_LaunchInput;
    c_LaunchRequest.i_LaunchCommand = i_LaunchCommand;
    
    b_LaunchRequested = true;
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool InputLaunch::GetLaunchRequested() noexcept
{
    return b_LaunchRequested;
}

InputLaunch::LaunchRequest InputLaunch::GetLaunchRequest(bool b_Reset)
{
    if (b_LaunchRequested == false)
    {
        throw InputException("No launch request available!", "Launch");
    }
    
    if (b_Reset == true)
    {
        b_LaunchRequested = false;
    }
    
    return c_LaunchRequest;
}
