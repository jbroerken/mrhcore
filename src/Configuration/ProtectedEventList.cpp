/**
 *  ProtectedEventList.cpp
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
#include <MRH_Event.h>
#include <libmrhbf.h>

// Project
#include "./ProtectedEventList.h"
#include "../Logger/Logger.h"
#include "../FilePaths.h"

// Pre-defined
namespace
{
    const char* p_ProtectedEventBlock = "ProtectedEvent";
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

ProtectedEventList::ProtectedEventList() noexcept
{}

ProtectedEventList::~ProtectedEventList() noexcept
{}

//*************************************************************************************
// Singleton
//*************************************************************************************

ProtectedEventList& ProtectedEventList::Singleton() noexcept
{
    static ProtectedEventList c_ProtectedEventList;
    return c_ProtectedEventList;
}

//*************************************************************************************
// Update
//*************************************************************************************

void ProtectedEventList::Update()
{
    Logger& c_Logger = Logger::Singleton();
    
    try
    {
        c_Logger.Log(Logger::INFO, "Reading " MRH_PROTECTED_EVENT_LIST_FILE_PATH " protected event config...",
                     "ProtectedEventList.cpp", __LINE__);
        
        std::lock_guard<std::mutex> c_Guard(c_Mutex);
        
        MRH_BlockFile c_File(MRH_PROTECTED_EVENT_LIST_FILE_PATH);
        
        for (auto& Block : c_File.l_Block)
        {
            if (Block.GetName().compare(p_ProtectedEventBlock) != 0)
            {
                continue;
            }
            
            MRH_ValueBlock::ValueList l_Value(Block.GetValues());
            
            for (auto& Pair : l_Value)
            {
                v_Protected.emplace_back(static_cast<MRH_Uint32>(std::stoull(Pair.second)));
                
                c_Logger.Log(Logger::INFO, "Event " +
                                           Pair.first +
                                           " (" +
                                           Pair.second +
                                           ") is flagged and requires a password.",
                             "ProtectedEventList.cpp", __LINE__);
            }
        }
        
        c_Logger.Log(Logger::INFO, "Read " MRH_PROTECTED_EVENT_LIST_FILE_PATH " protected event config.",
                     "ProtectedEventList.cpp", __LINE__);
    }
    catch (std::exception& e)
    {
        throw ConfigurationException(e.what(), MRH_PROTECTED_EVENT_LIST_FILE_PATH);
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

size_t ProtectedEventList::GetEventCount() noexcept
{
    std::lock_guard<std::mutex> c_Guard(c_Mutex);
    return v_Protected.size();
}

MRH_Uint32 ProtectedEventList::GetEvent(size_t us_Event)
{
    try
    {
        std::lock_guard<std::mutex> s_Guard(c_Mutex);
        return v_Protected[us_Event];
    }
    catch (std::exception& e)
    {
        throw ConfigurationException(e.what(), MRH_PROTECTED_EVENT_LIST_FILE_PATH);
    }
}

bool ProtectedEventList::GetEventProtected(MRH_Uint32 u32_Type) noexcept
{
    if (u32_Type > MRH_EVENT_TYPE_MAX)
    {
        return false;
    }
    
    try
    {
        std::lock_guard<std::mutex> s_Guard(c_Mutex);
        
        for (auto& Protected : v_Protected)
        {
            if (Protected == u32_Type)
            {
                return true;
            }
        }
    }
    catch (std::exception& e)
    {
        Logger::Singleton().Log(Logger::WARNING, MRH_PROTECTED_EVENT_LIST_FILE_PATH ": " + std::string(e.what()),
                                "ProtectedEventList.cpp", __LINE__);
    }
    
    return false;
}
