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
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

// External

// Project
#include "./PackageContainer.h"
#include "./PackagePaths.h"
#include "../Configuration/CoreConfiguration.h"
#include "../Configuration/PackageList.h"
#include "../Logger/Logger.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PackageContainer::PackageContainer() noexcept
{}

PackageContainer::~PackageContainer() noexcept
{}

//*************************************************************************************
// Singleton
//*************************************************************************************

PackageContainer& PackageContainer::Singleton() noexcept
{
    static PackageContainer c_PackageContainer;
    return c_PackageContainer;
}

//*************************************************************************************
// Reload
//*************************************************************************************

void PackageContainer::Reload() noexcept
{
    // Lock during whole operation
    std::lock_guard<std::mutex> c_Guard(c_Mutex);
    
    // Clear old first
    v_Package.clear();
    
    // Now read each given path and add if possible
    Logger& c_Logger = Logger::Singleton();
    
    try
    {
        std::vector<std::string> v_Path = PackageList().GetPackages();
        v_Path.emplace_back(CoreConfiguration::Singleton().GetHomePackagePath()); // Add extra
        
        struct stat c_Stat;
        size_t p_Length[2] =
        {
            0,
            std::strlen(PACKAGE_EXTENSION),
        };
        
        for (auto& Path : v_Path)
        {
            if (Path.size() == 0)
            {
                continue;
            }
            else if (*(--(Path.end())) == '/')
            {
                Path.erase(--(Path.end()));
            }
            
            try
            {
                if (stat(Path.c_str(), &c_Stat) != 0 || !S_ISDIR(c_Stat.st_mode))
                {
                    c_Logger.Log(Logger::INFO, Path +
                                               " not valid: Not a directory!",
                                 "PackageList.cpp", __LINE__);
                }
                else if ((p_Length[0] = Path.size()) <= p_Length[1])
                {
                    c_Logger.Log(Logger::INFO, Path +
                                               " not valid: Character count too low!",
                                 "PackageList.cpp", __LINE__);
                }
                else if (std::strncmp(&(Path[p_Length[0] - p_Length[1]]), PACKAGE_EXTENSION, p_Length[1]) != 0)
                {
                    c_Logger.Log(Logger::INFO, Path +
                                               " not valid: Extension missmatch!",
                                 "PackageList.cpp", __LINE__);
                }
                else
                {
                    std::string s_Location = Path.substr(0, Path.find_last_of('/') + 1);
                    std::string s_Package = Path.substr(Path.find_last_of('/') + 1);
                    
                    v_Package.emplace_back(s_Location,
                                           s_Package);
                    c_Logger.Log(Logger::INFO, "Loaded package: " + Path, "PackageList.cpp", __LINE__);
                }
            }
            catch (PackageException& e)
            {
                c_Logger.Log(Logger::WARNING, Path + " failed to load: " + e.what2(),
                             "PackageContainer.cpp", __LINE__);
            }
            catch (std::exception& e)
            {
                c_Logger.Log(Logger::WARNING, Path + " failed to load: " + e.what(),
                             "PackageContainer.cpp", __LINE__);
            }
        }
    }
    catch (ConfigurationException& e)
    {
        c_Logger.Log(Logger::WARNING, "Package list failed to load: " + e.what2(),
                     "PackageContainer.cpp", __LINE__);
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

size_t PackageContainer::GetPackageCount() noexcept
{
    std::lock_guard<std::mutex> c_Guard(c_Mutex);
    return v_Package.size();
}

Package const& PackageContainer::GetPackage(size_t us_Package)
{
    std::lock_guard<std::mutex> c_Guard(c_Mutex);
    
    if (v_Package.size() > us_Package)
    {
        return v_Package[us_Package];
    }
    
    throw PackageException("No package for index " + std::to_string(us_Package) + "!", "PackageContainer");
}

Package const& PackageContainer::GetPackage(std::string s_PackagePath)
{
    if (s_PackagePath.length() == 0 || s_PackagePath[s_PackagePath.size() - 1] != '/')
    {
        s_PackagePath += "/";
    }
    
    std::lock_guard<std::mutex> c_Guard(c_Mutex);
    
    for (size_t i = 0; i < v_Package.size(); ++i)
    {
        if (s_PackagePath.compare(v_Package[i].GetPackagePath()) == 0)
        {
            return v_Package[i];
        }
    }
    
    throw PackageException("No package for path " + s_PackagePath + "!", "PackageContainer");
}

bool PackageContainer::GetPackageExists(std::string s_PackagePath) noexcept
{
    if (s_PackagePath.length() == 0 || *(s_PackagePath.end() - 1) != '/')
    {
        s_PackagePath += "/";
    }
    
    std::lock_guard<std::mutex> c_Guard(c_Mutex);
    
    for (auto& Package : v_Package)
    {
        if (Package.GetPackagePath().compare(s_PackagePath) == 0)
        {
            return true;
        }
    }
    
    return false;
}
