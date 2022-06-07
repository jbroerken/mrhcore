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

#ifndef MRH_CORE_LAUNCH_INPUT_DIR
    #define MRH_CORE_LAUNCH_INPUT_DIR "/var/mrh/mrhuapp/"
#endif

#ifndef MRH_CORE_LAUNCH_INPUT_FILE
    #define MRH_CORE_LAUNCH_INPUT_FILE "LaunchInput.txt"
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
// Log Paths
//*************************************************************************************

#ifndef MRH_CORE_LOG_FILE_DIR
    #define MRH_CORE_LOG_FILE_DIR "/var/log/mrh/"
#endif

//*************************************************************************************
// TMP Paths
//*************************************************************************************

#ifndef MRH_CORE_TMP_COMMON_DIR_PATH
    #define MRH_CORE_TMP_COMMON_DIR_PATH "/tmp/mrh/"
#endif

//*************************************************************************************
// PID Paths
//*************************************************************************************

#ifndef MRH_CORE_PID_FILE_DIR
    #define MRH_CORE_PID_FILE_DIR "/tmp/mrh/"
#endif

#ifndef MRH_CORE_USER_SERVICE_PID_FILE
    #define MRH_CORE_USER_SERVICE_PID_FILE "mrhuservice_pid"
#endif

#ifndef MRH_CORE_PLATFORM_SERVICE_PID_FILE
    #define MRH_CORE_PLATFORM_SERVICE_PID_FILE "mrhpservice_pid"
#endif

#ifndef MRH_CORE_USER_PID_FILE
    #define MRH_CORE_USER_PID_FILE "mrhuapp_pid"
#endif


#endif /* FilePaths_h */
