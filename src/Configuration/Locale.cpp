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
#include "./Locale.h"
#include "../Logger/Logger.h"
#include "../FilePaths.h"

// Pre-defined
namespace
{
    enum Identifier
    {
        // Block Names
        BLOCK_LOCALE = 0,
        
        // Locale Key
        KEY_ACTIVE = 1,
        
        // Bounds
        IDENTIFIER_MAX = KEY_ACTIVE,
        
        IDENTIFIER_COUNT = IDENTIFIER_MAX + 1
    };

    const char* p_Identifier[IDENTIFIER_COUNT] =
    {
        // Block Names
        "Locale",
        
        // Locale Key
        "Active"
    };
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

Locale::Locale() : s_Active("")
{
    Logger& c_Logger = Logger::Singleton();
    
    c_Logger.Log(Logger::INFO, "Reading " MRH_LOCALE_FILE_PATH " locale config...",
                 "Locale.cpp", __LINE__);
    
    try
    {
        MRH_BlockFile c_File(MRH_LOCALE_FILE_PATH);
        
        for (auto& Block : c_File.l_Block)
        {
            if (Block.GetName().compare(p_Identifier[BLOCK_LOCALE]) != 0)
            {
                continue;
            }
            
            s_Active = Block.GetValue(p_Identifier[KEY_ACTIVE]);
            break;
        }
    }
    catch (std::exception& e)
    {
        throw ConfigurationException(e.what(), MRH_LOCALE_FILE_PATH);
    }
    
    c_Logger.Log(Logger::INFO, "Read locale config.",
                 "Locale.cpp", __LINE__);
}

Locale::~Locale() noexcept
{}

//*************************************************************************************
// Getters
//*************************************************************************************

std::string Locale::GetLocale() const noexcept
{
    return s_Active;
}
