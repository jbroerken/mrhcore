/**
 *  PlatformServiceList.cpp
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
#include "./PlatformServiceList.h"
#include "../Logger/Logger.h"
#include "../FilePaths.h"

// Pre-defined
namespace
{
    enum Identifier
    {
        // Block Name
        BLOCK_PLATFORM_SERVICE = 0,
        
        // Platform Service Key
        KEY_BINARY_PATH = 1,
        KEY_ROUTE_ID = 2,
        KEY_DISABLED = 3,
        KEY_IS_ESSENTIAL = 4,
        
        // Bounds
        IDENTIFIER_MAX = KEY_IS_ESSENTIAL,
        
        IDENTIFIER_COUNT = IDENTIFIER_MAX + 1
    };

    const char* p_Identifier[IDENTIFIER_COUNT] =
    {
        // Block Name
        "PlatformService",
        
        // Platform Service Key
        "BinaryPath",
        "RouteID",
        "Disabled",
        "IsEssential"
    };
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PlatformServiceList::PlatformServiceList()
{
    Logger& c_Logger = Logger::Singleton();
    size_t us_Service = 0;
    
    c_Logger.Log(Logger::INFO, "Reading " MRH_PLATFORM_SERVICE_LIST_FILE_PATH " platform service config...",
                 "PlatformServiceList.cpp", __LINE__);
    
    try
    {
        MRH_BlockFile c_File(MRH_PLATFORM_SERVICE_LIST_FILE_PATH);
        
        for (auto& Block : c_File.l_Block)
        {
            if (Block.GetName().compare(p_Identifier[BLOCK_PLATFORM_SERVICE]) != 0)
            {
                continue;
            }
            
            try
            {
                v_Service.push_back(Service(Block.GetValue(p_Identifier[KEY_BINARY_PATH]),
                                            static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[KEY_ROUTE_ID]))),
                                            Block.GetValue(p_Identifier[KEY_DISABLED]).compare("1") == 0 ? true : false,
                                            Block.GetValue(p_Identifier[KEY_IS_ESSENTIAL]).compare("1") == 0 ? true : false));
                
                c_Logger.Log(Logger::INFO, "Read platform service " +
                                           std::to_string(us_Service) +
                                           ": [ " +
                                           Block.GetValue(p_Identifier[KEY_BINARY_PATH]) +
                                           " | " +
                                           Block.GetValue(p_Identifier[KEY_ROUTE_ID]) +
                                           " | " +
                                           Block.GetValue(p_Identifier[KEY_DISABLED]) +
                                           " | " +
                                           Block.GetValue(p_Identifier[KEY_IS_ESSENTIAL]) +
                                           " ]",
                             "PlatformServiceList.cpp", __LINE__);
            }
            catch (std::exception& e) // + MRH_BFException
            {
                c_Logger.Log(Logger::WARNING, "Failed to read platform service " +
                                              std::to_string(us_Service) +
                                              " from file " +
                                              MRH_PLATFORM_SERVICE_LIST_FILE_PATH +
                                              ": " +
                                              e.what(),
                             "PlatformServiceList.cpp", __LINE__);
            }
            
            ++us_Service;
        }
    }
    catch (std::exception& e)
    {
        throw ConfigurationException(e.what(), MRH_PLATFORM_SERVICE_LIST_FILE_PATH);
    }
    
    c_Logger.Log(Logger::INFO, "Read " + std::to_string(us_Service) + " platform services.",
                 "PlatformServiceList.cpp", __LINE__);
}

PlatformServiceList::~PlatformServiceList() noexcept
{}

PlatformServiceList::Service::Service(std::string const& s_BinaryPath,
                                      MRH_Uint32 u32_RouteID,
                                      bool b_Disabled,
                                      bool b_Essential) noexcept
{
    this->s_BinaryPath = s_BinaryPath;
    this->u32_RouteID = u32_RouteID;
    this->b_Disabled = b_Disabled;
    this->b_Essential = b_Essential;
}

PlatformServiceList::Service::Service(Service const& c_Service) noexcept
{
    s_BinaryPath = c_Service.s_BinaryPath;
    u32_RouteID = c_Service.u32_RouteID;
    b_Disabled = c_Service.b_Disabled;
    b_Essential = c_Service.b_Essential;
}

PlatformServiceList::Service::~Service() noexcept
{}

//*************************************************************************************
// Getters
//*************************************************************************************

size_t PlatformServiceList::GetServiceCount() const noexcept
{
    return v_Service.size();
}

PlatformServiceList::Service const& PlatformServiceList::GetService(size_t us_Service) const
{
    if (us_Service >= v_Service.size())
    {
        throw ConfigurationException("Invalid service " + std::to_string(us_Service) + " specified!", MRH_PLATFORM_SERVICE_LIST_FILE_PATH);
    }
    
    return v_Service[us_Service];
}
