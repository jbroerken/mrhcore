/**
 *  InputHandler.cpp
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
