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
#include <libmrhbf.h>

// Project
#include "./UserEventRoute.h"
#include "../Logger/Logger.h"
#include "../FilePaths.h"

// Pre-defined
namespace
{
    enum Identifier
    {
        // Block Name
        BLOCK_USER_EVENT_ROUTE = 0,
        
        // User Event Route Key
        KEY_ROUTE_ID = 1,
        // @NOTE: Event name keys are not known
        
        // Bounds
        IDENTIFIER_MAX = KEY_ROUTE_ID,
        
        IDENTIFIER_COUNT = IDENTIFIER_MAX + 1
    };

    const char* p_Identifier[IDENTIFIER_COUNT] =
    {
        // Block Name
        "UserEventRoute",
        
        // User Event Route Key
        "RouteID"
    };
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

UserEventRoute::UserEventRoute()
{
    Logger& c_Logger = Logger::Singleton();
    size_t us_Route = 0;
    
    c_Logger.Log(Logger::INFO, "Reading " MRH_USER_EVENT_ROUTE_FILE_PATH " user event route config...",
                 "UserEventRoute.cpp", __LINE__);
    
    try
    {
        MRH_BlockFile c_File(MRH_USER_EVENT_ROUTE_FILE_PATH);
        
        for (auto& Block : c_File.l_Block)
        {
            if (Block.GetName().compare(p_Identifier[BLOCK_USER_EVENT_ROUTE]) != 0)
            {
                continue;
            }
            
            try
            {
                // Provoke exception, we need this to exist
                std::string s_RouteID(Block.GetValue(p_Identifier[KEY_ROUTE_ID]));
                MRH_Uint32 u32_RouteID = static_cast<MRH_Uint32>(std::stoull(s_RouteID));
                
                auto Route = m_EventRoute.find(u32_RouteID);
                if (Route == m_EventRoute.end())
                {
                    if (m_EventRoute.insert(std::make_pair(u32_RouteID, std::vector<MRH_Uint32>())).second == false)
                    {
                        throw ConfigurationException("Failed to add new route!", ""); // caught as std::exception, fp replaced
                    }
                    
                    Route = m_EventRoute.find(u32_RouteID);
                }
                
                // Now, read all event ids
                MRH_ValueBlock::ValueList l_Value(Block.GetValues());
                size_t us_Event = 0;
                
                for (auto& Pair : l_Value)
                {
                    // Downside to exception check above, we now need to filter the field
                    if (Pair.first.compare(p_Identifier[KEY_ROUTE_ID]) == 0)
                    {
                        continue;
                    }
                    
                    try
                    {
                        Route->second.emplace_back(static_cast<MRH_Uint32>(std::stoull(Pair.second)));
                        
                        c_Logger.Log(Logger::INFO, "Added event " +
                                                   Pair.first +
                                                   " (" +
                                                   Pair.second +
                                                   ") to route " +
                                                   s_RouteID +
                                                   ".",
                                     "UserEventRoute.cpp", __LINE__);
                    }
                    catch (std::exception& e)
                    {
                        c_Logger.Log(Logger::WARNING, "Failed to add event " +
                                                      std::to_string(us_Event) +
                                                      " for user event route " +
                                                      std::to_string(us_Route) +
                                                      " from file " +
                                                      MRH_USER_EVENT_ROUTE_FILE_PATH +
                                                      ": " +
                                                      e.what(),
                                     "UserEventRoute.cpp", __LINE__);
                    }
                    
                    ++us_Event;
                }
            }
            catch (std::exception& e) // + MRH_BFException
            {
                c_Logger.Log(Logger::WARNING, "Failed to read user event route " +
                                              std::to_string(us_Route) +
                                              " from file " +
                                              MRH_USER_EVENT_ROUTE_FILE_PATH +
                                              ": " +
                                              e.what(),
                             "UserEventRoute.cpp", __LINE__);
            }
            
            ++us_Route;
        }
    }
    catch (std::exception& e)
    {
        throw ConfigurationException(e.what(), MRH_USER_EVENT_ROUTE_FILE_PATH);
    }
    
    c_Logger.Log(Logger::INFO, "Read " + std::to_string(us_Route) + " user event routes.",
                 "UserEventRoute.cpp", __LINE__);
}

UserEventRoute::~UserEventRoute() noexcept
{}

//*************************************************************************************
// Getters
//*************************************************************************************

size_t UserEventRoute::GetRouteCount() const noexcept
{
    return m_EventRoute.size();
}

std::vector<MRH_Uint32> const& UserEventRoute::GetRoute(MRH_Uint32 u32_Route) const
{
    auto Route = m_EventRoute.find(u32_Route);
    if (Route == m_EventRoute.end())
    {
        throw ConfigurationException("Invalid route " + std::to_string(u32_Route) + " specified!", MRH_USER_EVENT_ROUTE_FILE_PATH);
    }
    
    return Route->second;
}
