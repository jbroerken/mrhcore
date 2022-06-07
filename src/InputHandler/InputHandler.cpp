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
#include "./InputHandler.h"
#include "../Logger/Logger.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************/

InputHandler::InputHandler()
{}

InputHandler::~InputHandler() noexcept
{}

//*************************************************************************************
// Update
//*************************************************************************************/

void InputHandler::Update(std::vector<Event> const& v_Event) noexcept
{
    Logger& c_Logger = Logger::Singleton();
    
    for (auto& Event : v_Event)
    {
        try
        {
            switch (Event.GetType())
            {
                /**
                 *  Launch 
                 */
                    
                // @NOTE: Timed launch events are not handled here, we expect
                //        the handling to be done by the service. All we want
                //        is a simple launch or clear launch event.
                //
                //        Also, no group id check here! Timed launches have no group id,
                //        they are created by the service alone without an App (the app
                //        requesting them might already be terminated).
                //
                case MRH_EVENT_APP_LAUNCH_SOA_S:
                {
                    UpdateLaunch(Event);
                    break;
                }
                case MRH_EVENT_APP_LAUNCH_SOA_CLEAR_S:
                {
                    ClearLaunchRequest();
                    break;
                }
                    
                /**
                 *  Listen
                 */
                    
                case MRH_EVENT_LISTEN_STRING_S:
                {
                    if (UpdateStopCommand(Event) == true)
                    {
                        // Force quit by user should also reset that apps launches!
                        ClearLaunchRequest();
                    }
                    break;
                }
                    
                /**
                 *  Password
                 */
                    
                case MRH_EVENT_PASSWORD_CHECK_S:
                {
                    UpdateVerification(Event);
                    break;
                }
                    
                /**
                 *  Unk
                 */
                    
                default:
                {
                    break;
                }
            }
        }
        catch (InputException& e)
        {
            c_Logger.Log(Logger::WARNING, "Failed to process input event " +
                                          std::to_string(Event.GetType()) +
                                          ": "
                                          + e.what2(),
                         "InputHandler.cpp", __LINE__);
        }
    }
}
