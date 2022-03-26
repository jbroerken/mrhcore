/**
 *  InputHandler.h
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

#ifndef InputHandler_h
#define InputHandler_h

// C / C++
#include <vector>

// External

// Project
#include "./Component/InputLaunch.h"
#include "./Component/InputStop.h"
#include "./Component/InputPassword.h"


class InputHandler : public InputLaunch,
                     public InputStop,
                     public InputPassword
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    InputHandler();

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_InputHandler InputHandler class source.
     */

    InputHandler(InputHandler const& c_InputHandler) = delete;

    /**
     *  Default destructor.
     */

    ~InputHandler() noexcept;
    
    //*************************************************************************************
    // Update
    //*************************************************************************************
    
    /**
     *  Update the input handler with recieved events.
     *
     *  \param v_Event The events to process.
     */
    
    void Update(std::vector<Event> const& v_Event) noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

protected:

};

#endif /* InputHandler_h */
