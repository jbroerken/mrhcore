/**
 *  PackageLocationList.cpp
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
