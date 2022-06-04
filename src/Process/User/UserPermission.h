/**
 *  UserPermission.h
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

#ifndef UserPermission_h
#define UserPermission_h

// C / C++
#include <vector>

// External

// Project
#include "../../Package/Package.h"
#include "../../Event/Event.h"
#include "../ProcessException.h"


class UserPermission
{
public:

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Check if the password verification has been performed successfully.
     *
     *  \return true if the password has been verified successfully, false if not.
     */

    bool GetPasswordVerified() const noexcept;
    
    //*************************************************************************************
    // Setters
    //*************************************************************************************

    /**
     *  Set if the password verification has been performed successfully.
     *
     *  \param b_Verified true if the password has been verified successfully, false if not.
     */

    void SetPasswordVerified(bool b_Verified) noexcept;
    
private:

    //*************************************************************************************
    // Event Permissions
    //*************************************************************************************

    /**
     *  Check if the permissions required for a event are available.
     *
     *  \param u32_Type The event type to check permissions for.
     *
     *  \return true if the permission set contains the required permissions,
     *          false if not.
     */

    bool PermissionGiven(MRH_Uint32 u32_Type) noexcept;

    //*************************************************************************************
    // Event Password Protection
    //*************************************************************************************
    
    /**
     *  Check if a given event is password protected.
     *
     *  \param u32_Type The event type to check.
     *
     *  \return true if the event is password protected, false if not.
     */

    bool PasswordProtected(MRH_Uint32 u32_Type) noexcept;
    
    //*************************************************************************************
    // Event Service Restriction
    //*************************************************************************************
    
    /**
     *  Check if a given event is intended for user services.
     *
     *  \param u32_Type The event type to check.
     *
     *  \return true if the event is available to user services, false if not.
     */
    
    bool IsServiceEvent(MRH_Uint32 u32_Type) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    bool b_IsService;
    
    Package::EventPermission p_Permission[Package::EVENT_PERMISSION_LIST_COUNT];
    bool b_PasswordVerified;
    
protected:
    
    //*************************************************************************************
    // Permissions
    //*************************************************************************************

    // @NOTE: Bit flags are used (1, 2, 4, 8, etc.) to allow for combining permissions!
    typedef enum
    {
        /* Event Version 1 */
        
        CUSTOM_NONE = 0,                         // No custom service permissions
        CUSTOM_USABLE = 1,                       // Custom service usage enabled
    
    }PermissionCustom;

    typedef enum
    {
        /* Event Version 1 */
        
        APPLICATION_NONE = 0,                    // No application permissions
        APPLICATION_LAUNCH = 1,                  // Launch application by path
        APPLICATION_CLEAR_LAUNCH = 2,            // Clear application launch request
        APPLICATION_LAUNCH_TIMER = 4,            // Timed application launch by path
        APPLICATION_CLEAR_LAUNCH_TIMER = 8,      // Clear all timed application launches request
        APPLICATION_CUSTOM_COMMAND = 16,         // Allow sending and recieving of custom commands
    
    }PermissionApplication;

    typedef enum
    {
        /* Event Version 1 */
        
        LISTEN_NONE = 0,                         // No listen permissions
        LISTEN_LISTEN = 1,                       // Speech listen events
        LISTEN_GET_METHOD = 2,                   // Get speech input method
        LISTEN_CUSTOM_COMMAND = 4,               // Allow sending and recieving of custom commands
    
    }PermissionListen;

    typedef enum
    {
        /* Event Version 1 */
        
        SAY_NONE = 0,                            // No say permissions
        SAY_SAY = 1,                             // Speech say events
        SAY_GET_METHOD = 2,                      // Get speech output method
        SAY_NOTIFICATION_APP = 4,                // Send notification from app 
        SAY_NOTIFICATION_SERVICE = 8,            // Send notification from service
        SAY_CUSTOM_COMMAND = 16,                  // Allow sending and recieving of custom commands
    
    }PermissionSay;

    typedef enum
    {
        /* Event Version 1 */
        
        PASSWORD_NONE = 0,                       // No password permissions
        PASSWORD_CHECK = 1,                      // Check user password
        PASSWORD_SET = 2,                        // Set user password
        PASSWORD_CUSTOM_COMMAND = 4,             // Allow sending and recieving of custom commands
    
    }PermissionPassword;

    typedef enum
    {
        /* Event Version 1 */
        
        USER_NONE = 0,                           // No user permissions
        USER_DOCUMENTS_ACCESS = 1,               // Access user documents
        USER_PICTURES_ACCESS = 2,                // Access user pictures
        USER_MUSIC_ACCESS = 4,                   // Access user music
        USER_VIDEOS_ACCESS = 8,                  // Access user videos
        USER_DOWNLOADS_ACCESS = 16,              // Access user downloads
        USER_CLIPBOARD_ACCESS = 32,              // Access user clipboard
        USER_INFO_PERSON_ACCESS = 64,            // Access user person info
        USER_INFO_RESIDENCE_ACCESS = 128,        // Access user residence info
        USER_GET_LOCATION = 256,                 // Request user location
        USER_CUSTOM_COMMAND = 512,               // Allow sending and recieving of custom commands
    
    }PermissionUser;

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param b_IsService Wether this package is used for user services or not.
     */

    UserPermission(bool b_IsService) noexcept;

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_UserPermission UserPermission class source.
     */

    UserPermission(UserPermission const& c_UserPermission) = delete;

    /**
     *  Default destructor.
     */

    ~UserPermission() noexcept;
    
    //*************************************************************************************
    // Update
    //*************************************************************************************
    
    /**
     *  Update user permission info.
     *
     *  \param c_Package The package containing user permission info.
     */
    
    void UpdatePermissions(Package const& c_Package);
    
    //*************************************************************************************
    // Event Filter
    //*************************************************************************************

    /**
     *  Filter user application events based on available permissions and password verification.
     *
     *  \param v_Event The event vector to check.
     *  \param b_AddResponseEvent Add response events for each event with missing permissions.
     */
     
    void FilterEventsPermission(std::vector<Event>& v_Event, bool b_AddResponseEvent) noexcept;
    
    /**
     *  Filter events based on the process event version supported.
     *
     *  \param v_Event The event vector to check.
     *  \param i_EventVer The given event version.
     */
    
    void FilterEventsVersion(std::vector<Event>& v_Event, int i_EventVer) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    std::vector<Event> v_PermissionDenied;
};

#endif /* UserPermission_h */
