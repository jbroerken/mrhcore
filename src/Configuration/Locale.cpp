/**
 *  Locale.cpp
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
