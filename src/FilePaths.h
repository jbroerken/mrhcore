/**
 *  FilePaths.h
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

#ifndef FilePaths_h
#define FilePaths_h

// C / C++

// External

// Project


//*************************************************************************************
// Configuration Paths
//*************************************************************************************

#ifndef MRH_CORE_CONFIGURATION_FILE_PATH
    #define MRH_CORE_CONFIGURATION_FILE_PATH "/usr/local/etc/mrh/MRH_Core.conf"
#endif

#ifndef MRH_LOCALE_FILE_PATH
    #define MRH_LOCALE_FILE_PATH "/usr/local/etc/mrh/MRH_Locale.conf"
#endif

#ifndef MRH_PLATFORM_SERVICE_LIST_FILE_PATH
    #define MRH_PLATFORM_SERVICE_LIST_FILE_PATH "/usr/local/etc/mrh/MRH_PlatformServiceList.conf"
#endif

#ifndef MRH_USER_SERVICE_LIST_FILE_PATH
    #define MRH_USER_SERVICE_LIST_FILE_PATH "/usr/local/etc/mrh/MRH_UserServiceList.conf"
#endif

#ifndef MRH_PACKAGE_LIST_FILE_PATH
    #define MRH_PACKAGE_LIST_FILE_PATH "/usr/local/etc/mrh/MRH_PackageList.conf"
#endif

#ifndef MRH_PROTECTED_EVENT_LIST_FILE_PATH
    #define MRH_PROTECTED_EVENT_LIST_FILE_PATH "/usr/local/etc/mrh/MRH_ProtectedEventList.conf"
#endif

#ifndef MRH_USER_EVENT_ROUTE_FILE_PATH
    #define MRH_USER_EVENT_ROUTE_FILE_PATH "/usr/local/etc/mrh/MRH_UserEventRoute.conf"
#endif

//*************************************************************************************
// Package Application Parent
//*************************************************************************************

#ifndef MRH_UAPP_LAUNCH_INPUT_FILE_PATH
    #define MRH_UAPP_LAUNCH_INPUT_FILE_PATH "/var/mrh/mrhuapp/LaunchInput.txt"
#endif

//*************************************************************************************
// Trigger Paths
//*************************************************************************************

#ifndef MRH_CORE_INPUT_STOP_TRIGGER_PATH
    #define MRH_CORE_INPUT_STOP_TRIGGER_PATH "/var/mrh/mrhcore"
#endif

#ifndef MRH_CORE_INPUT_STOP_TRIGGER_FILE
    #define MRH_CORE_INPUT_STOP_TRIGGER_FILE "Stop.mrhit"
#endif

//*************************************************************************************
// PID Paths
//*************************************************************************************

#ifndef MRH_CORE_PID_FILE_DIR
    #define MRH_CORE_PID_FILE_DIR "/tmp/mrhpid/"
#endif

#ifndef MRH_CORE_USER_SERVICE_PID_FILE
    #define MRH_CORE_USER_SERVICE_PID_FILE "mrhuservice_pid"
#endif

#ifndef MRH_CORE_PLATFORM_SERVICE_PID_FILE
    #define MRH_CORE_PLATFORM_SERVICE_PID_FILE "mrhpservice_pid"
#endif


#endif /* FilePaths_h */
