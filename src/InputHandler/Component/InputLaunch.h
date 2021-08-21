/**
 *  InputLaunch.h
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

#ifndef InputLaunch_h
#define InputLaunch_h

// C / C++

// External

// Project
#include "../../Event/Event.h"
#include "../InputException.h"


class InputLaunch
{
public:

    //*************************************************************************************
    // Launch Request
    //*************************************************************************************

    class LaunchRequest
    {
        friend class InputLaunch;
        
    public:

        //*************************************************************************************
        // Destructor
        //*************************************************************************************

        /**
         *  Copy constructor.
         *
         *  \param c_LaunchRequest LaunchRequest class source.
         */

        LaunchRequest(LaunchRequest const& c_LaunchRequest) noexcept;

        /**
         *  Default destructor.
         */

        ~LaunchRequest() noexcept;
        
        //*************************************************************************************
        // Data
        //*************************************************************************************
        
        std::string s_PackagePath;
        std::string s_LaunchInput;
        int i_LaunchCommand;
        
    private:
        
        //*************************************************************************************
        // Constructor
        //*************************************************************************************
    
        /**
         *  Default constructor.
         *
         *  \param s_PackagePath The full path to the package.
         *  \param s_LaunchInput The full UTF-8 launch input string.
         *  \param i_LaunchCommand The given launch command.
         */
        
        LaunchRequest(std::string s_PackagePath,
                      std::string s_LaunchInput,
                      int i_LaunchCommand) noexcept;
        
    protected:
        
    };
    
    //*************************************************************************************
    // Clear
    //*************************************************************************************
    
    /**
     *  Clear launch request.
     */
    
    void ClearLaunchRequest() noexcept;
    
    //*************************************************************************************
    // Update
    //*************************************************************************************
    
    /**
     *  Set a new launch request.
     *
     *  \param s_PackagePath The full path to the package.
     *  \param s_LaunchInput The full UTF-8 launch input string.
     *  \param i_LaunchCommand The given launch command.
     */
    
    void UpdateLaunch(std::string s_PackagePath, std::string s_LaunchInput, int i_LaunchCommand);
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Check if a launch request is available.
     *
     *  \return true if a launch request exists, false if not.
     */
    
    bool GetLaunchRequested() noexcept;
    
    /**
     *  Get the currently set launch request.
     *
     *  \param b_Reset Wether to reset the launch request or not.
     *
     *  \return The currently set launch request.
     */
    
    LaunchRequest GetLaunchRequest(bool b_Reset);
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    LaunchRequest c_LaunchRequest;
    bool b_LaunchRequested;

protected:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    InputLaunch() noexcept;

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_InputLaunch InputLaunch class source.
     */

    InputLaunch(InputLaunch const& c_InputLaunch) = delete;

    /**
     *  Default destructor.
     */

    ~InputLaunch() noexcept;
    
    //*************************************************************************************
    // Update
    //*************************************************************************************
    
    /**
     *  Update the current package launch request.
     *
     *  \param c_Event The launch event to check.
     */
    
    void UpdateLaunch(Event const& c_Event);
};

#endif /* InputLaunch_h */
