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
