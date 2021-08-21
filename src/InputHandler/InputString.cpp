/**
 *  InputVoice.cpp
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
#include <cstring>

// External

// Project
#include "./InputString.h"
#include "../Logger/Logger.h"

// Pre-defined
namespace
{
    // Event
    typedef enum
    {
        LISTEN_STRING_U_TYPE_POS = 0,
        LISTEN_STRING_U_ID_POS = LISTEN_STRING_U_TYPE_POS + sizeof(MRH_Uint8),
        LISTEN_STRING_U_PART_POS = LISTEN_STRING_U_ID_POS + sizeof(MRH_Uint32),
        LISTEN_STRING_U_STRING_POS = LISTEN_STRING_U_PART_POS + sizeof(MRH_Uint32)
    
    }Event_ListenString_S;

    typedef enum
    {
        STRING_TYPE_UNFINISHED = 0,
        STRING_TYPE_END = 1,
    
        STRING_TYPE_MAX = STRING_TYPE_END,
    
        STRING_TYPE_COUNT = STRING_TYPE_MAX + 1
    
    }Event_ListenString_Type;

    constexpr MRH_Uint32 u32_MinDataSize = LISTEN_STRING_U_STRING_POS + 1; // terminator
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

InputString::InputString() noexcept
{}

InputString::~InputString() noexcept
{}

//*************************************************************************************
// Clear
//*************************************************************************************

void InputString::ClearInput() noexcept
{
    m_Unfinished.clear();
    l_Finished.clear();
}

void InputString::ClearUnfinished() noexcept
{
    m_Unfinished.clear();
}

void InputString::ClearFinished() noexcept
{
    l_Finished.clear();
}

//*************************************************************************************
// Add
//*************************************************************************************

void InputString::AddVoiceInput(std::string s_String) noexcept
{
    if (s_String.length() == 0)
    {
        return;
    }
    
    l_Finished.emplace_back(s_String);
}

void InputString::AddVoiceInput(Event const& c_Event) noexcept
{
    if (c_Event.GetData() == NULL || c_Event.GetDataSize() < u32_MinDataSize)
    {
        return;
    }
    
    // Grab all required info from the event (ID, String and Type) while making
    // sure that the chunk map is in order
    MRH_Uint32 u32_StringID = c_Event.CastValue<MRH_Uint32>(LISTEN_STRING_U_ID_POS);
    MRH_Uint32 u32_PartID = c_Event.CastValue<MRH_Uint32>(LISTEN_STRING_U_PART_POS);
    std::string s_String(c_Event.CastPointer<const char*>(LISTEN_STRING_U_STRING_POS),
                         c_Event.GetDataSize() - LISTEN_STRING_U_STRING_POS);
    bool b_End = c_Event.CastValue<MRH_Uint8>(LISTEN_STRING_U_TYPE_POS) == STRING_TYPE_END ? true : false;
    
    try
    {
        // Add if not existing
        auto Unfinished = m_Unfinished.find(u32_StringID);
        if (Unfinished == m_Unfinished.end())
        {
            if (b_End == true)
            {
                // This new string is complete already
                l_Finished.emplace_back(s_String);
            }
            else if (m_Unfinished.insert(std::make_pair(u32_StringID, MRH_SpeechString(s_String,
                                                                                       u32_PartID,
                                                                                       u32_StringID,
                                                                                       b_End))).second == false)
            {
                Logger::Singleton().Log(Logger::ERROR, "Failed to add new voice input!",
                                        "InputString.cpp", __LINE__);
            }
        }
        else
        {
            Unfinished->second.Add(s_String, u32_PartID, b_End);
            
            // Build if finished
            if (Unfinished->second.GetState() == MRH_SpeechString::COMPLETE)
            {
                l_Finished.emplace_back(Unfinished->second.GetString());
                m_Unfinished.erase(Unfinished);
            }
        }
    }
    catch (MRH_VTException& e)
    {
        Logger::Singleton().Log(Logger::ERROR, "Failed to add voice input: " + e.what2(),
                                "InputString.cpp", __LINE__);
    }
}
