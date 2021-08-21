/**
 *  InputStop.h
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

#ifndef InputStop_h
#define InputStop_h

// C / C++
#include <string>

// External
#include <libmrhvt/Input/MRH_InputTrigger.h>

// Project
#include "../InputException.h"


class InputStop
{
public:

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Check if a stop command was issued.
     *
     *  \param b_Reset Wether to reset the stop command to false or not.
     *
     *  \return true if the current package should be stopped, false if not.
     */
    
    bool GetStopPackage(bool b_Reset) noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

    MRH_InputTrigger c_StopTriggerList;
    
protected:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    InputStop();

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_InputStop InputStop class source.
     */

    InputStop(InputStop const& c_InputStop) = delete;

    /**
     *  Default destructor.
     */

    ~InputStop() noexcept;
    
    //*************************************************************************************
    // Update
    //*************************************************************************************
    
    /**
     *  Check a input string for a stop command.
     *
     *  \param s_String The UTF-8 input string to check.
     *
     *  \return true if the stop command was given, false if not.
     */
    
    bool UpdateStopCommand(std::string s_String) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

    bool b_StopPackage;
};

#endif /* InputStop_h */
