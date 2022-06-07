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
