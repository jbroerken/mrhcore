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
