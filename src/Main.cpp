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
#include <sys/stat.h>
#include <csignal>
#include <cstdlib>
#include <new>
#include <clocale>

// External

// Project
#include "./Process/ServicePool/Platform/PlatformServicePool.h"
#include "./Process/ServicePool/User/UserServicePool.h"
#include "./Process/User/UserProcess.h"
#include "./InputHandler/InputHandler.h"
#include "./Package/PackageContainer.h"
#include "./Configuration/ConfigurationFiles.h"
#include "./Logger/Logger.h"
#include "./Timer.h"
#include "./FilePaths.h"
#include "./Revision.h"

// Pre-defined
#ifndef MRH_CORE_DAEMON_MODE
    #define MRH_CORE_DAEMON_MODE 0
#endif

namespace
{
    // Signal
    int i_LastSignal = -1;
    
    // Locale
    std::string s_DefaultLocale = "en_US.UTF-8";

    // Platform service startup timeout
    constexpr MRH_Uint32 u32_ServiceStartupTimeoutS = 60;
}


//*************************************************************************************
// Signal Handler
//*************************************************************************************

// Prevent name wrangling
extern "C"
{
    void SignalHandler(int i_Signal)
    {
        switch (i_Signal)
        {
            case SIGILL:
            case SIGTRAP:
            case SIGFPE:
            case SIGABRT:
            case SIGSEGV:
                Logger::Singleton().Backtrace(25, "Caught Signal: " + std::to_string(i_Signal));
                exit(EXIT_FAILURE);
                break;
                
            case SIGTERM:
            case SIGHUP:
            case SIGINT:
                i_LastSignal = i_Signal;
                break;
                
            default:
                i_LastSignal = -1;
                break;
        }
    }
}

//*************************************************************************************
// Daemon
//*************************************************************************************

#if MRH_CORE_DAEMON_MODE > 0
static int Daemonize() noexcept
{
    pid_t s32_Pid = 0;
    
    // First fork to leave parent
    if ((s32_Pid = fork()) < 0)
    {
        return -1;
    }
    else if (s32_Pid > 0)
    {
        // Parent exit
        exit(EXIT_SUCCESS);
    }
    
    // Set session id
    if (setsid() < 0)
    {
        return -1;
    }
    
    // Ignore child signal
    signal(SIGCHLD, SIG_IGN);
    
    // Second fork to attach to proccess 1
    if ((s32_Pid = fork()) < 0)
    {
        return -1;
    }
    else if (s32_Pid > 0)
    {
        // Parent exit
        exit(EXIT_SUCCESS);
    }
    
    // Set new file permissions
    umask(0);
    
    // Return to root
    chdir("/");
    
    // in, out and error to NULL
    stdin = fopen("/dev/null", "r");
    stdout = fopen("/dev/null", "w+");
    stderr = fopen("/dev/null", "w+");
    
    // Child success
    return 0;
}
#endif

//*************************************************************************************
// Directories
//*************************************************************************************

static void CreateDirectory(std::string s_Path)
{
    struct stat c_Stat;
    size_t us_Pos = 0;
    std::string s_Current = "";
    
    while ((us_Pos = s_Path.find_first_of('/', us_Pos + 1)) != std::string::npos)
    {
        s_Current = s_Path.substr(0, us_Pos);
        
        if (stat(s_Current.c_str(), &c_Stat) == 0 && S_ISDIR(c_Stat.st_mode))
        {
            continue;
        }
        
        if (mkdir(s_Current.c_str(), 0777) < 0) // @TODO: Restricting might be better
        {
            Logger::Singleton().Log(Logger::WARNING, "Failed to create dir path: " + s_Current,
                                    "Main.cpp", __LINE__);
            break;
        }
    }
}

//*************************************************************************************
// Process
//*************************************************************************************

static void StopProcess(Process* p_Process, MRH_Uint32 u32_StopTimerS, MRH_Uint32 u32_SleepTimerMS)
{
    // Check
    if (p_Process->GetRunning() == false)
    {
        return;
    }
    
    // Initial stop signal
    Timer c_Timer;
    p_Process->Stop(false);
    
    // Check status
    while (p_Process->GetRunning() == true)
    {
        // Force quit if timer exceeded
        if (c_Timer.GetTimePassedSeconds() >= u32_StopTimerS)
        {
            p_Process->Stop(true);
            break;
        }
        
        // Wait
        std::this_thread::sleep_for(std::chrono::milliseconds(u32_SleepTimerMS));
    }
}

//*************************************************************************************
// Locale
//*************************************************************************************

static void SetLocale() noexcept
{
    Logger& s_Logger = Logger::Singleton();
    s_Logger.Log(Logger::INFO, "Updating locale...",
                 "Main.cpp", __LINE__);
    
    std::string s_Locale;
    
    try
    {
        Locale s_File;
        s_Locale = s_File.GetLocale();
    }
    catch (ConfigurationException& e)
    {
        s_Logger.Log(Logger::WARNING, "Failed to read locale file: " +
                                      e.what2() +
                                      " (" +
                                      e.filepath2() +
                                      ")",
                     "Main.cpp", __LINE__);
        s_Locale = s_DefaultLocale;
    }
    
    std::setlocale(LC_ALL, s_Locale.c_str());
    
    if (s_Locale.compare(std::setlocale(LC_ALL, NULL)) != 0)
    {
        s_Logger.Log(Logger::WARNING, "Failed to set locale to " + s_Locale + "!",
                     "Main.cpp", __LINE__);
        std::setlocale(LC_ALL, s_DefaultLocale.c_str());
    }
    else
    {
        s_Logger.Log(Logger::INFO, "Locale set to " + s_Locale + "!",
                     "Main.cpp", __LINE__);
    }
}

//*************************************************************************************
// Configuration
//*************************************************************************************

static void LoadProtectedEventList() noexcept
{
    try
    {
        ProtectedEventList::Singleton().Update();
    }
    catch (ConfigurationException& e)
    {
        Logger::Singleton().Log(Logger::WARNING, "Load static configuration: " +
                                                 e.what2() +
                                                 " (" +
                                                 e.filepath2() +
                                                 ")",
                                "Main.cpp", __LINE__);
    }
}

static void LoadPackageList() noexcept
{
    // No exception, simply reload
    PackageContainer::Singleton().Reload();
}

static void LoadVariableConfiguration() noexcept
{
    LoadProtectedEventList();
    LoadPackageList();
}

static void LoadStaticConfiguration() noexcept
{
    try
    {
        CoreConfiguration::Singleton().Load();
    }
    catch (ConfigurationException& e)
    {
        Logger::Singleton().Log(Logger::WARNING, "Load static configuration: " +
                                                 e.what2() +
                                                 " (" +
                                                 e.filepath2() +
                                                 ")",
                                "Main.cpp", __LINE__);
    }
}

//*************************************************************************************
// Main
//*************************************************************************************

int main(int argc, char* argv[])
{
    // Daemonize
#if MRH_CORE_DAEMON_MODE > 0
    if (Daemonize() < 0)
    {
        Logger::Singleton().Log(Logger::ERROR, "Failed to daemonize!", 
                                "Main.cpp", __LINE__);
        
        return EXIT_FAILURE;
    }
#endif
    
    // Log Setup
    Logger& c_Logger = Logger::Singleton();
    c_Logger.Log(Logger::INFO, "=============================================", "Main.cpp", __LINE__);
    c_Logger.Log(Logger::INFO, "= Started MRH Core (" + std::string(VERSION_NUMBER) + ")", "Main.cpp", __LINE__);
    c_Logger.Log(Logger::INFO, "=============================================", "Main.cpp", __LINE__);
    
    // Install signal handlers
    std::signal(SIGTERM, SignalHandler);
    std::signal(SIGILL, SignalHandler);
    std::signal(SIGTRAP, SignalHandler);
    std::signal(SIGFPE, SignalHandler);
    std::signal(SIGABRT, SignalHandler);
    std::signal(SIGSEGV, SignalHandler);
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGHUP, SignalHandler);
    
    // Create run directories
    CreateDirectory(MRH_CORE_PID_FILE_DIR);
    CreateDirectory(MRH_CORE_TMP_COMMON_DIR_PATH);
    CreateDirectory(MRH_CORE_LOG_FILE_DIR);
    CreateDirectory(MRH_CORE_LAUNCH_INPUT_DIR);
    
    // The configuration has to be loaded now, the service pools rely on it
    SetLocale();
    LoadStaticConfiguration();
    LoadVariableConfiguration();
    
    // Now create the required components
    UserProcess* p_UserProcess;
    UserServicePool* p_UserPool;
    PlatformServicePool* p_PlatformPool;
    InputHandler* p_Input;
    CoreConfiguration& c_CoreConfiguration = CoreConfiguration::Singleton();
    bool b_ComponentFailed = true;
    
    try
    {
        // @NOTE: Platform and user pool start on construction!
        p_PlatformPool = new PlatformServicePool();
        p_UserPool = new UserServicePool();
        p_UserProcess = new UserProcess();
        p_Input = new InputHandler();
        
        // Add here directly, home package is a required component!
        p_Input->UpdateLaunch(c_CoreConfiguration.GetHomePackagePath(),
                              "", /* No input, auto start */
                              c_CoreConfiguration.GetHomePackageStartupLaunchCommandID());
        
        // No failure
        b_ComponentFailed = false;
    }
    catch (ProcessException& e)
    {
        c_Logger.Log(Logger::ERROR, "Process:" + e.what2(),
                     "Main.cpp", __LINE__);
    }
    catch (InputException& e)
    {
        c_Logger.Log(Logger::ERROR, "Input (" + e.module2() + "): " + e.what2(),
                     "Main.cpp", __LINE__);
    }
    catch (std::exception& e)
    {
        c_Logger.Log(Logger::ERROR, "Unknown source: " + std::string(e.what()),
                     "Main.cpp", __LINE__);
    }
    
    if (b_ComponentFailed == true)
    {
        c_Logger.Log(Logger::INFO, "Component setup failed, stopping core.",
                     "Main.cpp", __LINE__);
        
        // Delete the two components which have child processes to kill the children
        if (p_PlatformPool != NULL)
        {
            delete p_PlatformPool;
        }
        
        if (p_UserPool != NULL)
        {
            delete p_UserPool;
        }
        
        return EXIT_FAILURE;
    }
    
    // Wait for platform pool service availability
    if (p_PlatformPool->GetAllRunning() == false)
    {
        c_Logger.Log(Logger::INFO, "Not all platform services available, waiting for startup",
                     "Main.cpp", __LINE__);
        Timer s_Timer;
        
        while (p_PlatformPool->GetAllRunning() == false && i_LastSignal != SIGTERM)
        {
            // Force quit if timer exceeded
            if (s_Timer.GetTimePassedSeconds() >= u32_ServiceStartupTimeoutS)
            {
                c_Logger.Log(Logger::ERROR, "Platform service startup timeout, stopping core!",
                             "Main.cpp", __LINE__);
                return EXIT_FAILURE;
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(c_CoreConfiguration.GetWaitSleepTimerMS()));
        }
    }
    
    // Continious loop until termination request
    std::vector<Event> v_PlatformEvent;
    PackageConfiguration::OSAppType e_UserProccessOSAppType = PackageConfiguration::OSAppType::NONE;
    bool b_UserProcessStopDisabled = false; // Silence warning, can't be accessed before a process starts
    bool b_UserProcessIsHome = true; // Home is always the first app
    bool b_UserProcessRunning = false;
    int i_UserProcessStatus = -1; // Start at -1 to skip exit check
    
    while (i_LastSignal != SIGTERM)
    {
        /**
         *  Step 1: Signal handling
         */
        
        if (i_LastSignal != -1)
        {
            switch (i_LastSignal)
            {
                case SIGHUP:
                    LoadVariableConfiguration();
                    p_UserPool->Reload();
                    break;
                
                default:
                    break;
            }
            
            i_LastSignal = -1;
        }
        
        /**
         *  Step 2: Store recieved platform service pool events
         */
        
        // Platform Service Pool -> User Processes, Input Handler
        p_PlatformPool->LockRecievedEvents();
        v_PlatformEvent = std::vector<Event>(p_PlatformPool->RetrieveEvents()); // Copy needed, can't lock all the way
        p_PlatformPool->RetrieveEvents().clear(); // Usually cleared by reciever, but we copy here!
        p_PlatformPool->UnlockRecievedEvents();

        /**
         *  Step 3: Update input handler
         */
        
        p_Input->Update(v_PlatformEvent);
        
        /**
         *  Step 4: Update user process
         */
        
        p_UserProcess->GetProcessState(b_UserProcessRunning, i_UserProcessStatus);
        
        if (b_UserProcessRunning == true)
        {
            /**
             *  Step 5.1: Running, Check user stop input
             */
            
            // Home cannot be stopped
            if (b_UserProcessStopDisabled == false && b_UserProcessIsHome == false && p_Input->GetStopPackage(true) == true)
            {
                StopProcess(p_UserProcess,
                            c_CoreConfiguration.GetForceStopTimerS(),
                            c_CoreConfiguration.GetWaitSleepTimerMS());
            }
            else
            {
                /**
                 *  Step 5.2: Verify the password if possible
                 */
                
                // Password verification
                if (p_UserProcess->GetPasswordVerified() == false)
                {
                    p_UserProcess->SetPasswordVerified(p_Input->GetVerified());
                }
                
                /**
                 *  Step 5.3: Exchange user events (process + pool) with platform service pool
                 */
                
                // User Service Pool -> Platform Service Pool
                p_UserPool->LockRecievedEvents();
                p_PlatformPool->SendEvents(p_UserPool->RetrieveEvents());
                p_UserPool->UnlockRecievedEvents();
                    
                // User Process -> Platform Service Pool
                p_UserProcess->RecieveEvents();
                p_PlatformPool->SendEvents(p_UserProcess->RetrieveEvents());
                
                // Platform Service Pool -> User Process
                p_UserProcess->SendEvents(v_PlatformEvent);
            }
        }
        else
        {
            /**
             *  Step 6.1: Check why a process is not running
             */
            
            // How did the process exit?
            if (i_UserProcessStatus == EXIT_SUCCESS)
            {
                // Check OS app type on success exit, maybe we need to reload something
                switch (e_UserProccessOSAppType)
                {
                    case PackageConfiguration::OSAppType::SETTINGS:
                        LoadProtectedEventList();
                        break;
                    case PackageConfiguration::OSAppType::PACKAGE_MANAGER:
                        LoadPackageList();
                        p_UserPool->Reload(); // Reload user services for changes
                        break;
                        
                    // Do nothing
                    default:
                        break;
                }
            }
            else if (i_UserProcessStatus > EXIT_SUCCESS)
            {
                // Is the crashed process home?
                if (b_UserProcessIsHome == true)
                {
                    // Default crashed, stop core
                    c_Logger.Log(Logger::ERROR, "Home package crashed, stopping core!",
                                 "Main.cpp", __LINE__);
                    break;
                }
            }
            
            /**
             *  Step 6.2: Check and enforce a launch request
             */
            
            if (p_Input->GetLaunchRequested() == false)
            {
                // Set home if nothing was requested
                p_Input->UpdateLaunch(c_CoreConfiguration.GetHomePackagePath(),
                                      "", /* Once again, no input for auto launch */
                                      c_CoreConfiguration.GetHomePackageDefaultLaunchCommandID());
            }
            
            /**
             *  Step 6.3: Launch the current request
             */
            
            try
            {
                InputHandler::LaunchRequest c_Request = p_Input->GetLaunchRequest(true);
                Package const& c_Package = PackageContainer::Singleton().GetPackage(c_Request.s_PackagePath);
                
                // Reset input handler for new package process
                p_Input->ClearPasswordVerification();
                
                // Set process info
                // @NOTE: This is handled here because launch requests can be sent from apps
                //        and that request might be a home launch (not automated)
                if (c_Request.s_PackagePath.compare(c_CoreConfiguration.GetHomePackagePath()) == 0)
                {
                    b_UserProcessIsHome = true;
                }
                else
                {
                    b_UserProcessIsHome = false;
                }
                
                e_UserProccessOSAppType = c_Package.PackageApp::GetOSAppType();
                b_UserProcessStopDisabled = c_Package.PackageApp::GetStopDisabled();
                
                // Reset user process status
                b_UserProcessRunning = false;
                i_UserProcessStatus = -1;
                
                // All set, start process
                p_UserProcess->Run(c_Package,
                                   c_Request.i_LaunchCommand,
                                   c_Request.s_LaunchInput,
                                   c_CoreConfiguration.GetAppParentBinaryPath(),
                                   c_CoreConfiguration.GetEventLimit(CoreConfiguration::USER_APP),
                                   c_CoreConfiguration.GetRecieveTimeoutMS(CoreConfiguration::USER_APP));
            }
            catch (InputException& e)
            {
                c_Logger.Log(Logger::ERROR, "Launch failed! Input (" + e.module2() + "): " + e.what2(),
                             "Main.cpp", __LINE__);
            }
            catch (PackageException& e)
            {
                c_Logger.Log(Logger::ERROR, "Launch failed! Package (" + e.package2() + "): " + e.what2(),
                             "Main.cpp", __LINE__);
            }
            catch (ProcessException& e)
            {
                c_Logger.Log(Logger::ERROR, "Launch failed! User Process: " + e.what2(),
                             "Main.cpp", __LINE__);
            }
            catch (std::exception& e)
            {
                c_Logger.Log(Logger::ERROR, "Launch failed! Unknown: " + std::string(e.what()),
                             "Main.cpp", __LINE__);
            }
        }
    }
    
    // All done, now terminate
    // @NOTE: We give the user process extra time to stop
    StopProcess(p_UserProcess,
                c_CoreConfiguration.GetForceStopTimerS(),
                c_CoreConfiguration.GetWaitSleepTimerMS());
    
    delete p_UserProcess;
    delete p_UserPool;
    delete p_PlatformPool;
    delete p_Input;
    
    c_Logger.Log(Logger::INFO, "Successfully closed core.", "Main.cpp", __LINE__);
    return EXIT_SUCCESS;
}
