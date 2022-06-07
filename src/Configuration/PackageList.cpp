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
#include <MRH_Typedefs.h>

// Project
#include "./PackageList.h"
#include "../Logger/Logger.h"
#include "../FilePaths.h"

// Pre-defined
namespace
{
    const char* p_BlockIdentifier = "Package";
    const char* p_CountKey = "Count";
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PackageList::PackageList()
{
    Logger& c_Logger = Logger::Singleton();
    c_Logger.Log(Logger::INFO, "Reading " MRH_PACKAGE_LIST_FILE_PATH " package location config...",
                 "PackageLocationList.cpp", __LINE__);
    
    try
    {
        MRH_BlockFile c_File(MRH_PACKAGE_LIST_FILE_PATH);
        
        for (auto& Block : c_File.l_Block)
        {
            if (Block.GetName().compare(p_BlockIdentifier) != 0)
            {
                continue;
            }
            
            // Grab the count first
            MRH_Uint32 u32_Count = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_CountKey)));
            
            // Now read the packages
            MRH_ValueBlock::ValueList l_Value = Block.GetValues();
            
            for (auto& Value : l_Value)
            {
                try
                {
                    if (std::stoull(Value.first) < u32_Count && Value.second.size() > 0)
                    {
                        v_Package.emplace_back(Value.second);
                    }
                }
                catch (...)
                {}
            }
        }
    }
    catch (std::exception& e)
    {
        throw ConfigurationException(e.what(), MRH_PACKAGE_LIST_FILE_PATH);
    }
    
    c_Logger.Log(Logger::INFO, "Read " + std::to_string(v_Package.size()) + " package directories.",
                 "PackageLocationList.cpp", __LINE__);
}

PackageList::~PackageList() noexcept
{}

//*************************************************************************************
// Getters
//*************************************************************************************

std::vector<std::string> PackageList::GetPackages() const noexcept
{
    return v_Package;
}
