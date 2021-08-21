/**
 *  UserServiceList.cpp
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
