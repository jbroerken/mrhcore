/**
 *  InputPassword.cpp
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

// Project
#include "./InputPassword.h"

// Pre-defined
namespace
{
    typedef enum
    {
        CHECK_PASSWORD_S_RESULT_POS = 0 // size = MRH_Uint8
        
    }Event_CheckPassword_S;

    typedef enum
    {
        PASSWORD_CHECK_FAIL = 0,
        PASSWORD_CHECK_SUCCESS = 1,
        
        PASSWORD_CHECK_RESULT_MAX = PASSWORD_CHECK_SUCCESS,
        
        PASSWORD_CHECK_RESULT_COUNT = PASSWORD_CHECK_RESULT_MAX + 1
        
    }Event_CheckPassword_Result;

    constexpr MRH_Uint32 u32_MinDataSize = CHECK_PASSWORD_S_RESULT_POS + sizeof(MRH_Uint8);
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

InputPassword::InputPassword() noexcept : b_Verified(false)
{}

InputPassword::~InputPassword() noexcept
{}

//*************************************************************************************
// Clear
//*************************************************************************************

void InputPassword::ClearPasswordVerification() noexcept
{
    b_Verified = false;
}

//*************************************************************************************
// Update
//*************************************************************************************

bool InputPassword::UpdateVerification(Event const& c_Event)
{
    if (c_Event.GetData() == NULL || c_Event.GetDataSize() < u32_MinDataSize)
    {
        // No result, event damaged?
        throw InputException("Invalid password event data!", "Password");
    }
    
    if (c_Event.CastValue<MRH_Uint8>(CHECK_PASSWORD_S_RESULT_POS) == PASSWORD_CHECK_SUCCESS)
    {
        b_Verified = true;
    }
    
    return b_Verified;
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool InputPassword::GetVerified() noexcept
{
    return b_Verified;
}

//*************************************************************************************
// Setters
//*************************************************************************************

void InputPassword::SetVerified(bool b_Verified) noexcept
{
    this->b_Verified = b_Verified;
}
