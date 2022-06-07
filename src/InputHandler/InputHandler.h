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
