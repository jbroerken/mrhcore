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
#include "./UserServiceList.h"
#include "../Logger/Logger.h"
#include "../FilePaths.h"

// Pre-defined
namespace
{
    const char* p_BlockServiceIdentifier = "UserService";
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

UserServiceList::UserServiceList()
{
    Logger& c_Logger = Logger::Singleton();
    size_t us_Service = 0;
    
    c_Logger.Log(Logger::INFO, "Reading " MRH_USER_SERVICE_LIST_FILE_PATH " user service config...",
                 "UserServiceList.cpp", __LINE__);
    
    try
    {
        MRH_BlockFile c_File(MRH_USER_SERVICE_LIST_FILE_PATH);
        
        for (auto& Block : c_File.l_Block)
        {
            if (Block.GetName().compare(p_BlockServiceIdentifier) != 0)
            {
                continue;
            }
            
            MRH_ValueBlock::ValueList l_Value(Block.GetValues());
            
            for (auto& Pair : l_Value)
            {
                v_Package.emplace_back(Pair.second);
                
                c_Logger.Log(Logger::INFO, "Read user service " +
                                           std::to_string(us_Service) +
                                           ": [ " +
                                           Pair.second +
                                           " ]",
                             "UserServiceList.cpp", __LINE__);
            }
            
            ++us_Service;
        }
    }
    catch (std::exception& e)
    {
        throw ConfigurationException(e.what(), MRH_USER_SERVICE_LIST_FILE_PATH);
    }
    
    c_Logger.Log(Logger::INFO, "Read " + std::to_string(us_Service) + " user services.",
                 "UserServiceList.cpp", __LINE__);
}

UserServiceList::~UserServiceList() noexcept
{}

//*************************************************************************************
// Getters
//*************************************************************************************

size_t UserServiceList::GetPackageCount() const noexcept
{
    return v_Package.size();
}

std::string UserServiceList::GetPackage(size_t us_Package) const
{
    if (v_Package.size() <= us_Package)
    {
        throw ConfigurationException("Invalid package position!", MRH_USER_SERVICE_LIST_FILE_PATH);
    }
    
    return v_Package[us_Package];
}

bool UserServiceList::GetUserService(std::string s_Package) const noexcept
{
    if (s_Package.size() == 0)
    {
        return false;
    }
    
    for (auto& Package : v_Package)
    {
        if (s_Package.compare(Package) == 0)
        {
            return true;
        }
    }
    
    return false;
}
