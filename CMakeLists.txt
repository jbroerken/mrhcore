#########################################################################
#
#  CMAKE
#
#########################################################################

###
#  Minimum Version
#  ---------------
#  The CMake version required.
###
cmake_minimum_required(VERSION 3.1)

###
#  CMake Configuration
#  -------------------
#  Configuration settings for CMake.
#
#  NOTE:
#  These settings have to be applied before the project() setting!
###
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 14)

###
#  Project Info
#  ------------
#  General simple information about our project.
###
project(mrhcore VERSION 1.0
                DESCRIPTION "MRH core platform executable"
                LANGUAGES CXX)


#########################################################################
#
#  PATHS
#
#########################################################################

###
#  Binary Paths
#  ------------
#  The paths for our created binary file(s).
###
set(BIN_DIR_PATH "${CMAKE_SOURCE_DIR}/bin/")
file(MAKE_DIRECTORY ${BIN_DIR_PATH})

###
#  Build Paths
#  -----------
#  The paths for the cmake build.
###
set(BUILD_DIR_PATH "${CMAKE_SOURCE_DIR}/build/")
file(MAKE_DIRECTORY ${BUILD_DIR_PATH})

###
#  Source Paths
#  ------------
#  The paths to the source files to use.
#  Add OS specific source files in their own list.
###
set(SRC_DIR_PATH "${CMAKE_SOURCE_DIR}/src/")

set(SRC_LIST_PROCESS "${SRC_DIR_PATH}/Process/ServicePool/User/UserService.cpp"
                     "${SRC_DIR_PATH}/Process/ServicePool/User/UserService.h"
					 "${SRC_DIR_PATH}/Process/ServicePool/User/UserServicePool.cpp"
					 "${SRC_DIR_PATH}/Process/ServicePool/User/UserService.h"
					 "${SRC_DIR_PATH}/Process/ServicePool/Platform/PlatformService.cpp"
					 "${SRC_DIR_PATH}/Process/ServicePool/Platform/PlatformService.h"
					 "${SRC_DIR_PATH}/Process/ServicePool/Platform/PlatformServicePool.cpp"
					 "${SRC_DIR_PATH}/Process/ServicePool/Platform/PlatformServicePool.h"
					 "${SRC_DIR_PATH}/Process/ServicePool/PoolCondition.cpp"
					 "${SRC_DIR_PATH}/Process/ServicePool/PoolCondition.h"
					 "${SRC_DIR_PATH}/Process/ServicePool/PoolEvents.cpp"
					 "${SRC_DIR_PATH}/Process/ServicePool/PoolEvents.h"
					 "${SRC_DIR_PATH}/Process/ServicePool/PoolService.cpp"
					 "${SRC_DIR_PATH}/Process/ServicePool/PoolService.h"
					 "${SRC_DIR_PATH}/Process/ServicePool/ServicePool.cpp"
					 "${SRC_DIR_PATH}/Process/ServicePool/ServicePool.h"
					 "${SRC_DIR_PATH}/Process/User/UserProcess.cpp"
					 "${SRC_DIR_PATH}/Process/User/UserProcess.h"
					 "${SRC_DIR_PATH}/Process/User/UserServiceProcess.cpp"
					 "${SRC_DIR_PATH}/Process/User/UserServiceProcess.h"
					 "${SRC_DIR_PATH}/Process/User/UserPermission.cpp"
					 "${SRC_DIR_PATH}/Process/User/UserPermission.h"
					 "${SRC_DIR_PATH}/Process/Platform/PlatformServiceProcess.cpp"
					 "${SRC_DIR_PATH}/Process/Platform/PlatformServiceProcess.h"
					 "${SRC_DIR_PATH}/Process/ServiceProcess.cpp"
					 "${SRC_DIR_PATH}/Process/ServiceProcess.h"
					 "${SRC_DIR_PATH}/Process/Process.cpp"
					 "${SRC_DIR_PATH}/Process/Process.h"
					 "${SRC_DIR_PATH}/Process/ProcessException.h")

set(SRC_LIST_EVENT "${SRC_DIR_PATH}/Event/Source/SourceMRHCKM.cpp"
				   "${SRC_DIR_PATH}/Event/Source/SourceMRHCKM.h"
				   "${SRC_DIR_PATH}/Event/Source/SourcePipe.cpp"
				   "${SRC_DIR_PATH}/Event/Source/SourcePipe.h"
				   "${SRC_DIR_PATH}/Event/Source/TransmissionSource.cpp"
				   "${SRC_DIR_PATH}/Event/Source/TransmissionSource.h"
				   "${SRC_DIR_PATH}/Event/EventQueue.cpp"
				   "${SRC_DIR_PATH}/Event/EventQueue.h"
				   "${SRC_DIR_PATH}/Event/Event.cpp"
				   "${SRC_DIR_PATH}/Event/Event.h"
				   "${SRC_DIR_PATH}/Event/EventException.h")

set(SRC_LIST_PACKAGE "${SRC_DIR_PATH}/Package/Content/PackageConfiguration.cpp"
                     "${SRC_DIR_PATH}/Package/Content/PackageConfiguration.h"
                     "${SRC_DIR_PATH}/Package/Content/PackageService.cpp"
                     "${SRC_DIR_PATH}/Package/Content/PackageService.h"
                     "${SRC_DIR_PATH}/Package/Content/PackageApp.cpp"
                     "${SRC_DIR_PATH}/Package/Content/PackageApp.h"
                     "${SRC_DIR_PATH}/Package/Package.cpp"
                     "${SRC_DIR_PATH}/Package/Package.h"
                     "${SRC_DIR_PATH}/Package/PackageContainer.cpp"
                     "${SRC_DIR_PATH}/Package/PackageContainer.h"
					 "${SRC_DIR_PATH}/Package/PackageException.h")
					 
set(SRC_LIST_CONFIGURATION "${SRC_DIR_PATH}/Configuration/PlatformServiceList.cpp"
                           "${SRC_DIR_PATH}/Configuration/PlatformServiceList.h"
						   "${SRC_DIR_PATH}/Configuration/UserServiceList.cpp"
						   "${SRC_DIR_PATH}/Configuration/UserServiceList.h"
						   "${SRC_DIR_PATH}/Configuration/UserEventRoute.cpp"
						   "${SRC_DIR_PATH}/Configuration/UserEventRoute.h"
						   "${SRC_DIR_PATH}/Configuration/ProtectedEventList.cpp"
						   "${SRC_DIR_PATH}/Configuration/ProtectedEventList.h"
						   "${SRC_DIR_PATH}/Configuration/PackageList.cpp"
						   "${SRC_DIR_PATH}/Configuration/PackageList.h"
						   "${SRC_DIR_PATH}/Configuration/Locale.cpp"
						   "${SRC_DIR_PATH}/Configuration/Locale.h"
						   "${SRC_DIR_PATH}/Configuration/CoreConfiguration.cpp"
						   "${SRC_DIR_PATH}/Configuration/CoreConfiguration.h"
						   "${SRC_DIR_PATH}/Configuration/ConfigurationFiles.h"
						   "${SRC_DIR_PATH}/Configuration/ConfigurationException.h")
						   
set(SRC_LIST_INPUT_HANDLER "${SRC_DIR_PATH}/InputHandler/Component/InputPassword.cpp"
                           "${SRC_DIR_PATH}/InputHandler/Component/InputPassword.h"
						   "${SRC_DIR_PATH}/InputHandler/Component/InputStop.cpp"
						   "${SRC_DIR_PATH}/InputHandler/Component/InputStop.h"
						   "${SRC_DIR_PATH}/InputHandler/Component/InputLaunch.cpp"
						   "${SRC_DIR_PATH}/InputHandler/Component/InputLaunch.h"
						   "${SRC_DIR_PATH}/InputHandler/InputString.cpp"
						   "${SRC_DIR_PATH}/InputHandler/InputString.h"
						   "${SRC_DIR_PATH}/InputHandler/InputHandler.cpp"
						   "${SRC_DIR_PATH}/InputHandler/InputHandler.h"
						   "${SRC_DIR_PATH}/InputHandler/InputException.h")

set(SRC_LIST_LOGGER "${SRC_DIR_PATH}/Logger/EventLogger.cpp"
				    "${SRC_DIR_PATH}/Logger/EventLogger.h"
				    "${SRC_DIR_PATH}/Logger/Logger.cpp"
				    "${SRC_DIR_PATH}/Logger/Logger.h")

set(SRC_LIST_BASE "${SRC_DIR_PATH}/Timer.cpp"
				  "${SRC_DIR_PATH}/Timer.h"
				  "${SRC_DIR_PATH}/FilePaths.h"
				  "${SRC_DIR_PATH}/Main.cpp"
				  "${SRC_DIR_PATH}/Revision.h")

###
#  Library Paths
#  -------------
#  The paths to the external libraries to use. 
#  Both the binary and include paths should be set here.
#  Add OS specific libraries in their own lists.
###
set(LIB_DIR_PATH "${CMAKE_SOURCE_DIR}/lib/")
set(INCLUDE_DIR_PATH "${CMAKE_SOURCE_DIR}/include/")

set(LIB_BIN_LIST "${LIB_DIR_PATH}/libmrhbf.a"
                 "${LIB_DIR_PATH}/libmrhvt.a")
set(LIB_INCLUDE_LIST "${INCLUDE_DIR_PATH}/mrhshared"
					 "${INCLUDE_DIR_PATH}/libmrhbf"
					 "${INCLUDE_DIR_PATH}/libmrhvt")


#########################################################################
#
#  TARGET
#
#########################################################################

###
#  Target
#  ------
#  The target(s) to build.
###
add_executable(mrhcore ${SRC_LIST_PROCESS}
					   ${SRC_LIST_EVENT}
					   ${SRC_LIST_PACKAGE}
					   ${SRC_LIST_CONFIGURATION}
					   ${SRC_LIST_INPUT_HANDLER}
					   ${SRC_LIST_LOGGER}
					   ${SRC_LIST_BASE})
set_target_properties(mrhcore
    		          PROPERTIES
    		          ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
    		          LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
    		          RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR_PATH})

###
#  Required Libraries
#  ------------------
#  Libraries required by this application.
###
target_link_libraries(mrhcore PUBLIC ${LIB_BIN_LIST})
include_directories(${LIB_INCLUDE_LIST})

###
#  Source Definitions
#  ------------------
#  Preprocessor source definitions.
###
target_compile_definitions(mrhcore PRIVATE MRH_CORE_LOG_FILE_PATH="/var/log/mrh/mrhcore.log")
target_compile_definitions(mrhcore PRIVATE MRH_CORE_BACKTRACE_FILE_PATH="/var/log/mrh/bt_mrhcore.log")
target_compile_definitions(mrhcore PRIVATE MRH_LOGGER_PRINT_CLI=0)
target_compile_definitions(mrhcore PRIVATE MRH_CORE_EVENT_LOG_FILE_PATH="/var/log/mrh/ev_mrhcore.log")
target_compile_definitions(mrhcore PRIVATE MRH_EVENT_LOGGER_PRINT_CLI=0)
target_compile_definitions(mrhcore PRIVATE MRH_CORE_EVENT_LOGGING=0)
target_compile_definitions(mrhcore PRIVATE MRH_LOCALE_FILE_PATH="/etc/MRH_Locale.conf")
target_compile_definitions(mrhcore PRIVATE MRH_CORE_CONFIGURATION_FILE_PATH="/etc/MRH_Core.conf")
target_compile_definitions(mrhcore PRIVATE MRH_USER_SERVICE_LIST_FILE_PATH="/etc/MRH_UserServiceList.conf")
target_compile_definitions(mrhcore PRIVATE MRH_PACKAGE_LIST_FILE_PATH="/etc/MRH_PackageList.conf")
target_compile_definitions(mrhcore PRIVATE MRH_PLATFORM_SERVICE_LIST_FILE_PATH="/etc/MRH_PlatformServiceList.conf")
target_compile_definitions(mrhcore PRIVATE MRH_PROTECTED_EVENT_LIST_FILE_PATH="/etc/MRH_ProtectedEventList.conf")
target_compile_definitions(mrhcore PRIVATE MRH_USER_EVENT_ROUTE_FILE_PATH="/etc/MRH_UserEventRoute.conf")
target_compile_definitions(mrhcore PRIVATE MRH_UAPP_LAUNCH_INPUT_FILE_PATH="/var/mrhuapp/LaunchInput.txt")
target_compile_definitions(mrhcore PRIVATE MRH_CORE_INPUT_STOP_TRIGGER_PATH="/var/mrhcore")
target_compile_definitions(mrhcore PRIVATE MRH_CORE_INPUT_STOP_TRIGGER_FILE="Stop.mrhit")