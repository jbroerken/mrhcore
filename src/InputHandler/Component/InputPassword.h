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

#ifndef InputPassword_h
#define InputPassword_h

// C / C++

// External

// Project
#include "../../Event/Event.h"
#include "../InputException.h"


class InputPassword
{
public:

    //*************************************************************************************
    // Clear
    //*************************************************************************************

    /**
     *  Clear password verification.
     */

    void ClearPasswordVerification() noexcept;

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get the password verification state.
     *
     *  \return true if the user password was verified, false if not.
     */
    
    bool GetVerified() noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

protected:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    InputPassword() noexcept;

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_InputPassword InputPassword class source.
     */

    InputPassword(InputPassword const& c_InputPassword) = delete;

    /**
     *  Default destructor.
     */

    ~InputPassword() noexcept;
    
    //*************************************************************************************
    // Update
    //*************************************************************************************
    
    /**
     *  Update the password verification state.
     *
     *  \param c_Event The password verification event to check.
     *
     *  \return true if the password was successfully verified, false if not.
     */
    
    bool UpdateVerification(Event const& c_Event);

    //*************************************************************************************
    // Setters
    //*************************************************************************************
    
    /**
     *  Set the password verification state.
     *
     *  \param b_Verified The verification state.
     */
    
    void SetVerified(bool b_Verified) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

    bool b_Verified;
};

#endif /* InputPassword_h */
