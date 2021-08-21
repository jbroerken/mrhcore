/**
 *  InputPassword.h
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
