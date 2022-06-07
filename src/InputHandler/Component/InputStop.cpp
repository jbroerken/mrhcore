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
#include <libmrhvt/String/MRH_Convert.h>
#include <libmrhvt/String/MRH_LocalisedPath.h>

// Project
#include "./InputStop.h"
#include "../../FilePaths.h"

// Pre-defined
namespace
{
    typedef enum
    {
        LISTEN_STRING_S_SIZE_STRING_ID = sizeof(MRH_Uint32)
        
    }Event_Listen_String_S_Size;
    
    constexpr MRH_Uint32 u32_MinDataSize = LISTEN_STRING_S_SIZE_STRING_ID + 1; // terminator
    
    constexpr MRH_Sint32 s32_StopTriggerValue = 0;
    constexpr MRH_Uint32 u32_StopTriggerWeight = 300;
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

InputStop::InputStop() : c_StopTrigger(MRH_LocalisedPath::GetPath(MRH_CORE_INPUT_STOP_TRIGGER_PATH, 
                                                                  MRH_CORE_INPUT_STOP_TRIGGER_FILE)),
                         b_StopPackage(false)
{
    if (c_StopTrigger.GetTriggers().size() == 0)
    {
        throw InputException("Stop commands trigger file has no triggers to use!", "Stop");
    }
}

InputStop::~InputStop() noexcept
{}

//*************************************************************************************
// Update
//*************************************************************************************

bool InputStop::UpdateStopCommand(Event const& c_Event) noexcept
{
    if (c_Event.GetDataSize() < u32_MinDataSize)
    {
        return false;
    }
    
    std::string s_String((const char*)&(c_Event.GetData()[LISTEN_STRING_S_SIZE_STRING_ID]),
                         c_Event.GetDataSize() - LISTEN_STRING_S_SIZE_STRING_ID);
    
    MRH_StringConvert::ToLower(s_String);
    MRH_InputTrigger::Evaluation c_Evaluation = c_StopTrigger.Evaluate(s_String, u32_StopTriggerWeight);
    
    if (c_Evaluation.first == s32_StopTriggerValue && c_Evaluation.second >= u32_StopTriggerWeight)
    {
        b_StopPackage = true;
        return true;
    }
    
    return false;
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool InputStop::GetStopPackage(bool b_Reset) noexcept
{
    if (b_Reset == false)
    {
        return b_StopPackage;
    }
    else
    {
        bool b_Result = b_StopPackage;
        b_StopPackage = false;
        return b_Result;
    }
}
