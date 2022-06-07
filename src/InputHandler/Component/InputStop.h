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

#ifndef InputStop_h
#define InputStop_h

// C / C++
#include <string>

// External
#include <libmrhvt/Input/MRH_InputTrigger.h>

// Project
#include "../../Event/Event.h"
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

    MRH_InputTrigger c_StopTrigger;
    
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
     *  \param c_Event The input event to check.
     *
     *  \return true a stop command was found, false if not.
     */
    
    bool UpdateStopCommand(Event const& c_Event) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

    bool b_StopPackage;
};

#endif /* InputStop_h */
