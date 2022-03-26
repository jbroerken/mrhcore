/**
 *  InputStop.cpp
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
