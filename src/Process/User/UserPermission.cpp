/**
 *  UserPermission.cpp
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
#include "./UserPermission.h"
#include "../../Configuration/ProtectedEventList.h"
#include "../../Logger/Logger.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

UserPermission::UserPermission(bool b_IsService) noexcept : b_IsService(b_IsService), // Filtering
                                                            b_PasswordVerified(false)
{
    // Set intial permission info (all blocked)
    for (size_t i = 0; i < Package::EVENT_PERMISSION_LIST_COUNT; ++i)
    {
        p_Permission[i] = Package::u32_NoPermission;
    }
}

UserPermission::~UserPermission() noexcept
{}

//*************************************************************************************
// Update
//*************************************************************************************

void UserPermission::UpdatePermissions(Package const& c_Package)
{
    try
    {
        for (size_t i = 0; i < Package::EVENT_PERMISSION_LIST_COUNT; ++i)
        {
            p_Permission[i] = c_Package.PackageApp::GetPermission(static_cast<Package::EventPermissionList>(i));
        }
    }
    catch (PackageException& e)
    {
        throw ProcessException(e.what2()); // Convert to keep in line with expected exceptions
    }
    
    // OS Apps don't require password authentification, considered safe
    if (c_Package.PackageApp::GetOSAppType() > PackageConfiguration::OSAppType::NONE)
    {
        b_PasswordVerified = true;
    }
    else
    {
        b_PasswordVerified = false;
    }
}

//*************************************************************************************
// Permission Event Filter
//*************************************************************************************

void UserPermission::FilterEventsPermission(std::vector<Event>& v_Event, bool b_AddResponseEvent) noexcept
{
    Logger& c_Logger = Logger::Singleton();
    
    MRH_Uint32 u32_CurrentEvent;
    MRH_Uint32 u32_ResponseEvent;
    bool b_EventDenied = false;
    
    // Services cannot recieve events
    if (b_IsService == true)
    {
        b_AddResponseEvent = false;
    }
    
    for (auto Event = v_Event.begin(); Event != v_Event.end();)
    {
        u32_CurrentEvent = Event->GetType();
        
        // Event permissions not met?
        // @NOTE: Service events see the user process type as a permission
        //        So, service events require a service, user application events a user application
        if (IsServiceEvent(u32_CurrentEvent) != b_IsService || PermissionGiven(u32_CurrentEvent) == false)
        {
            c_Logger.Log(Logger::WARNING, "Cannot send user process event: Missing permissions for event " +
                                          std::to_string(u32_CurrentEvent) +
                                          "!",
                         "UserPermission.cpp", __LINE__);
            
            u32_ResponseEvent = MRH_EVENT_PERMISSION_DENIED;
            b_EventDenied = true;
        }
        else if (b_PasswordVerified == false && PasswordProtected(u32_CurrentEvent) == true)
        {
            c_Logger.Log(Logger::WARNING, "Cannot send user process event: Password verification required for event " +
                                          std::to_string(u32_CurrentEvent) +
                                          "!",
                         "UserPermission.cpp", __LINE__);
            
            u32_ResponseEvent = MRH_EVENT_PASSWORD_REQUIRED;
            b_EventDenied = true;
        }
        
        // Handle event
        if (b_EventDenied == true)
        {
            if (b_AddResponseEvent == true)
            {
                v_PermissionDenied.emplace_back(Event->GetGroupID(), /* Keep group id, same process (0 for service) */
                                                u32_ResponseEvent,
                                                (MRH_Uint8*)&(u32_CurrentEvent), /* Match event buffer, contains event id */
                                                sizeof(u32_CurrentEvent));
            }
            
            Event = v_Event.erase(Event);
            b_EventDenied = false;
        }
        else
        {
            ++Event;
        }
    }
}

void UserPermission::FilterEventsVersion(std::vector<Event>& v_Event, int i_EventVer) noexcept
{
    for (auto Event = v_Event.begin(); Event != v_Event.end();)
    {
        switch (i_EventVer)
        {
            /**
             *  Event Version 1
             */
                
            case 1:
                if (Event->GetType() > MRH_EVENT_TYPE_MAX)
                {
                    Event = v_Event.erase(Event);
                }
                else
                {
                    ++Event;
                }
                break;
                
            /**
             *  Default
             */
                
            default:
                Event = v_Event.erase(Event);
                break;
        }
    }
}

//*************************************************************************************
// Event Permissions
//*************************************************************************************

bool UserPermission::PermissionGiven(MRH_Uint32 u32_Type) noexcept
{
    switch (u32_Type)
    {
        /**
         *  Event Version 1
         */
            
        // Unknown, error or default value
        case MRH_EVENT_UNK:
            return false;
            
        // Missing permissions
        case MRH_EVENT_PERMISSION_DENIED:
            return true;
            
        // Not implemented by service
        case MRH_EVENT_NOT_IMPLEMENTED_S:
            return true;
            
        // Service reset
        case MRH_EVENT_PS_RESET_REQUEST_U:
        case MRH_EVENT_PS_RESET_ACKNOLEDGED_U:
            return true;
            
        // Custom service
        case MRH_EVENT_CUSTOM_AVAIL_U:
        case MRH_EVENT_CUSTOM_AVAIL_S:
        case MRH_EVENT_CUSTOM_CUSTOM_COMMAND_U:
        case MRH_EVENT_CUSTOM_CUSTOM_COMMAND_S:
            return p_Permission[Package::CUSTOM] > PermissionCustom::CUSTOM_NONE;
            
        // Voice - Listen
        case MRH_EVENT_LISTEN_AVAIL_U:
        case MRH_EVENT_LISTEN_AVAIL_S:
            return p_Permission[Package::LISTEN] > PermissionListen::LISTEN_NONE;
        case MRH_EVENT_LISTEN_STRING_S:
            return p_Permission[Package::LISTEN] & PermissionListen::LISTEN_LISTEN;
        case MRH_EVENT_LISTEN_GET_METHOD_U:
        case MRH_EVENT_LISTEN_GET_METHOD_S:
            return p_Permission[Package::LISTEN] & PermissionListen::LISTEN_GET_METHOD;
        case MRH_EVENT_LISTEN_CUSTOM_COMMAND_U:
        case MRH_EVENT_LISTEN_CUSTOM_COMMAND_S:
            return p_Permission[Package::LISTEN] & PermissionListen::LISTEN_CUSTOM_COMMAND;
            
        // Voice - Say
        case MRH_EVENT_SAY_AVAIL_U:
        case MRH_EVENT_SAY_AVAIL_S:
            return p_Permission[Package::SAY] > PermissionSay::SAY_NONE;
        case MRH_EVENT_SAY_STRING_U:
        case MRH_EVENT_SAY_STRING_S:
            return p_Permission[Package::SAY] & PermissionSay::SAY_SAY;
        case MRH_EVENT_SAY_GET_METHOD_U:
        case MRH_EVENT_SAY_GET_METHOD_S:
            return p_Permission[Package::SAY] & PermissionSay::SAY_GET_METHOD;
        case MRH_EVENT_SAY_NOTIFICATION_APP_U:
        case MRH_EVENT_SAY_NOTIFICATION_APP_S:
            return p_Permission[Package::SAY] & PermissionSay::SAY_NOTIFICATION_APP;
        case MRH_EVENT_SAY_NOTIFICATION_SERVICE_U:
            return p_Permission[Package::SAY] & PermissionSay::SAY_NOTIFICATION_SERVICE;
        case MRH_EVENT_SAY_CUSTOM_COMMAND_U:
        case MRH_EVENT_SAY_CUSTOM_COMMAND_S:
            return p_Permission[Package::SAY] & PermissionSay::SAY_CUSTOM_COMMAND;
            
        // Password
        case MRH_EVENT_PASSWORD_AVAIL_U:
        case MRH_EVENT_PASSWORD_AVAIL_S:
            return p_Permission[Package::PASSWORD] > PermissionPassword::PASSWORD_NONE;
        case MRH_EVENT_PASSWORD_CHECK_U:
        case MRH_EVENT_PASSWORD_CHECK_S:
            return p_Permission[Package::PASSWORD] & PermissionPassword::PASSWORD_CHECK;
        case MRH_EVENT_PASSWORD_SET_U:
        case MRH_EVENT_PASSWORD_SET_S:
            return p_Permission[Package::PASSWORD] & PermissionPassword::PASSWORD_SET;
        case MRH_EVENT_PASSWORD_CUSTOM_COMMAND_U:
        case MRH_EVENT_PASSWORD_CUSTOM_COMMAND_S:
            return p_Permission[Package::PASSWORD] & PermissionPassword::PASSWORD_CUSTOM_COMMAND;
            
        // User
        case MRH_EVENT_USER_AVAIL_U:
        case MRH_EVENT_USER_AVAIL_S:
            return p_Permission[Package::USER] > PermissionUser::USER_NONE;
        case MRH_EVENT_USER_ACCESS_DOCUMENTS_U:
        case MRH_EVENT_USER_ACCESS_DOCUMENTS_S:
            return p_Permission[Package::USER] & PermissionUser::USER_DOCUMENTS_ACCESS;
        case MRH_EVENT_USER_ACCESS_PICTURES_U:
        case MRH_EVENT_USER_ACCESS_PICTURES_S:
            return p_Permission[Package::USER] & PermissionUser::USER_PICTURES_ACCESS;
        case MRH_EVENT_USER_ACCESS_MUSIC_U:
        case MRH_EVENT_USER_ACCESS_MUSIC_S:
            return p_Permission[Package::USER] & PermissionUser::USER_MUSIC_ACCESS;
        case MRH_EVENT_USER_ACCESS_VIDEOS_U:
        case MRH_EVENT_USER_ACCESS_VIDEOS_S:
            return p_Permission[Package::USER] & PermissionUser::USER_VIDEOS_ACCESS;
        case MRH_EVENT_USER_ACCESS_DOWNLOADS_U:
        case MRH_EVENT_USER_ACCESS_DOWNLOADS_S:
            return p_Permission[Package::USER] & PermissionUser::USER_DOWNLOADS_ACCESS;
        case MRH_EVENT_USER_ACCESS_CLIPBOARD_U:
        case MRH_EVENT_USER_ACCESS_CLIPBOARD_S:
            return p_Permission[Package::USER] & PermissionUser::USER_CLIPBOARD_ACCESS;
        case MRH_EVENT_USER_ACCESS_INFO_PERSON_U:
        case MRH_EVENT_USER_ACCESS_INFO_PERSON_S:
            return p_Permission[Package::USER] & PermissionUser::USER_INFO_PERSON_ACCESS;
        case MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_U:
        case MRH_EVENT_USER_ACCESS_INFO_RESIDENCE_S:
            return p_Permission[Package::USER] & PermissionUser::USER_INFO_RESIDENCE_ACCESS;
        case MRH_EVENT_USER_ACCESS_CLEAR_U:
        case MRH_EVENT_USER_ACCESS_CLEAR_S:
            if (p_Permission[Package::USER] & PermissionUser::USER_DOCUMENTS_ACCESS ||
                p_Permission[Package::USER] & PermissionUser::USER_PICTURES_ACCESS ||
                p_Permission[Package::USER] & PermissionUser::USER_MUSIC_ACCESS ||
                p_Permission[Package::USER] & PermissionUser::USER_VIDEOS_ACCESS ||
                p_Permission[Package::USER] & PermissionUser::USER_DOWNLOADS_ACCESS ||
                p_Permission[Package::USER] & PermissionUser::USER_CLIPBOARD_ACCESS ||
                p_Permission[Package::USER] & PermissionUser::USER_INFO_PERSON_ACCESS ||
                p_Permission[Package::USER] & PermissionUser::USER_INFO_RESIDENCE_ACCESS)
            {
                return true;
            }
            return false;
        case MRH_EVENT_USER_GET_LOCATION_U:
        case MRH_EVENT_USER_GET_LOCATION_S:
            return p_Permission[Package::USER] & PermissionUser::USER_GET_LOCATION;
        case MRH_EVENT_USER_CUSTOM_COMMAND_U:
        case MRH_EVENT_USER_CUSTOM_COMMAND_S:
            return p_Permission[Package::USER] & PermissionUser::USER_CUSTOM_COMMAND;
            
        // Application
        case MRH_EVENT_APP_AVAIL_U:
        case MRH_EVENT_APP_AVAIL_S:
            return p_Permission[Package::APP] > PermissionApplication::APPLICATION_NONE;
        case MRH_EVENT_APP_LAUNCH_SOA_U:
        case MRH_EVENT_APP_LAUNCH_SOA_S:
            return p_Permission[Package::APP] & PermissionApplication::APPLICATION_LAUNCH;
        case MRH_EVENT_APP_LAUNCH_SOA_TIMER_U:
        case MRH_EVENT_APP_LAUNCH_SOA_TIMER_S:
            return p_Permission[Package::APP] & PermissionApplication::APPLICATION_LAUNCH_TIMER;
        case MRH_EVENT_APP_LAUNCH_SOA_CLEAR_U:
        case MRH_EVENT_APP_LAUNCH_SOA_CLEAR_S:
            return p_Permission[Package::APP] & PermissionApplication::APPLICATION_CLEAR_LAUNCH;
        case MRH_EVENT_APP_LAUNCH_SOA_CLEAR_TIMER_U:
        case MRH_EVENT_APP_LAUNCH_SOA_CLEAR_TIMER_S:
            return p_Permission[Package::APP] & PermissionApplication::APPLICATION_CLEAR_LAUNCH_TIMER;
        case MRH_EVENT_APP_LAUNCH_SOA_TIMER_REMINDER_S:
            return true; // Reminder to close for other app should always be recievable!
        case MRH_EVENT_APP_CUSTOM_COMMAND_U:
        case MRH_EVENT_APP_CUSTOM_COMMAND_S:
            return p_Permission[Package::APP] & PermissionApplication::APPLICATION_CUSTOM_COMMAND;
            
        /**
         *  Unk
         */
            
        // Default
        default:
            return false;
    }
}

//*************************************************************************************
// Event Password Protection
//*************************************************************************************

bool UserPermission::PasswordProtected(MRH_Uint32 u32_Type) noexcept
{
    switch (u32_Type)
    {
        // Some events are never protected, even on list (system events, etc)
     
        /**
         *  Event Version 1
         */
            
        // System
        case MRH_EVENT_UNK:
        case MRH_EVENT_PERMISSION_DENIED:
        case MRH_EVENT_PASSWORD_REQUIRED:
        case MRH_EVENT_NOT_IMPLEMENTED_S:
        case MRH_EVENT_PS_RESET_REQUEST_U:
        case MRH_EVENT_PS_RESET_ACKNOLEDGED_U:
            
        // Say
        case MRH_EVENT_SAY_STRING_S: // Only recieved if SAY_U was used, and that can be password protected (also: No info on what was said)
            
        // Password
        case MRH_EVENT_PASSWORD_CHECK_U: // Blocking password checking by password blocks all password protection and makes no sense
        case MRH_EVENT_PASSWORD_CHECK_S:
            
        // Applications
        case MRH_EVENT_APP_LAUNCH_SOA_TIMER_REMINDER_S: // Reminder sent from service
            
        // Service
        case MRH_EVENT_SAY_NOTIFICATION_SERVICE_U: // Can't react to password event
            return false;
            
        /**
         *  Unk
         */
            
        default:
            return ProtectedEventList::Singleton().GetEventProtected(u32_Type);
    }
}

//*************************************************************************************
// Event Service Restriction
//*************************************************************************************

bool UserPermission::IsServiceEvent(MRH_Uint32 u32_Type) noexcept
{
    switch (u32_Type)
    {
        // Services have little in terms of events, just list allowed ones
        case MRH_EVENT_SAY_NOTIFICATION_SERVICE_U:
            return true;
            
        default:
            return false;
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool UserPermission::GetPasswordVerified() const noexcept
{
    return b_PasswordVerified;
}

//*************************************************************************************
// Setters
//*************************************************************************************

void UserPermission::SetPasswordVerified(bool b_Verified) noexcept
{
    b_PasswordVerified = b_Verified;
}
