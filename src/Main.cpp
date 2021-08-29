/**
 *  Main.cpp
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
namespace
{
    // Signal
    int i_LastSignal = -1;
    
    // Locale
    std::string s_DefaultLocale = "en_US";

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
                i_LastSignal = i_Signal;
                break;
                
            default:
                i_LastSignal = -1;
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
    s_Logger.Log(Logger::INFO, "Updating locale...", "Language.cpp", __LINE__);
    
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
                     "Language.cpp", __LINE__);
        s_Locale = s_DefaultLocale;
    }
    
    std::setlocale(LC_ALL, s_Locale.c_str());
    
    if (s_Locale.compare(std::setlocale(LC_ALL, NULL)) != 0)
    {
        s_Logger.Log(Logger::WARNING, "Failed to set locale to " + s_Locale + "!", "Language.cpp", __LINE__);
        std::setlocale(LC_ALL, s_DefaultLocale.c_str());
    }
    else
    {
        s_Logger.Log(Logger::INFO, "Locale set to " + s_Locale + "!", "Language.cpp", __LINE__);
    }
}

//*************************************************************************************
// Configuration
//*************************************************************************************

static void LoadVariableConfiguration() noexcept
{
    // Configuration first, followed by package list needed for user service pool
    Logger& c_Logger = Logger::Singleton();
    
    try
    {
        ProtectedEventList::Singleton().Update();
    }
    catch (ConfigurationException& e)
    {
        c_Logger.Log(Logger::WARNING, "Load static configuration: " +
                                      e.what2() +
                                      " (" +
                                      e.filepath2() +
                                      ")",
                     "Main.cpp", __LINE__);
    }
    
    // Configs loaded, now load the packages
    PackageContainer::Singleton().Reload();
}

static void LoadStaticConfiguration() noexcept
{
    // Configuration first, followed by package list needed for user service pool
    Logger& c_Logger = Logger::Singleton();
    
    try
    {
        CoreConfiguration::Singleton().Load();
    }
    catch (ConfigurationException& e)
    {
        c_Logger.Log(Logger::WARNING, "Load static configuration: " +
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
    std::signal(SIGHUP, SignalHandler);
    
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
    
    try
    {
        // NOTE: Platform and user pool start on construction!
        p_PlatformPool = new PlatformServicePool();
        p_UserPool = new UserServicePool();
        p_UserProcess = new UserProcess();
        p_Input = new InputHandler();
        
        // Add here directly, home package is a required component!
        p_Input->UpdateLaunch(c_CoreConfiguration.GetHomePackagePath(),
                              "", /* No input, auto start */
                              c_CoreConfiguration.GetHomePackageStartupLaunchCommandID());
    }
    catch (ProcessException& e)
    {
        c_Logger.Log(Logger::ERROR, "Process:" + e.what2(), "Main.cpp", __LINE__);
        return EXIT_FAILURE;
    }
    catch (InputException& e)
    {
        c_Logger.Log(Logger::ERROR, "Input (" + e.module2() + "): " + e.what2(), "Main.cpp", __LINE__);
        return EXIT_FAILURE;
    }
    catch (std::exception& e)
    {
        c_Logger.Log(Logger::ERROR, "Unknown source: " + std::string(e.what()), "Main.cpp", __LINE__);
        return EXIT_FAILURE;
    }
    
    // Wait for platform pool service availability
    if (p_PlatformPool->GetAllRunning() == false)
    {
        c_Logger.Log(Logger::INFO, "Not all platform services available, waiting for startup", "Main.cpp", __LINE__);
        Timer s_Timer;
        
        while (p_PlatformPool->GetAllRunning() == false && i_LastSignal != SIGTERM)
        {
            // Force quit if timer exceeded
            if (s_Timer.GetTimePassedSeconds() >= u32_ServiceStartupTimeoutS)
            {
                c_Logger.Log(Logger::ERROR, "Platform service startup timeout, stopping core!", "Main.cpp", __LINE__);
                return EXIT_FAILURE;
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(c_CoreConfiguration.GetWaitSleepTimerMS()));
        }
    }
    
    // Continious loop until termination request
    std::vector<Event> v_PlatformEvent;
    bool b_UserProcessStopDisabled = false; // Silence warning, can't be accessed before a process starts
    bool b_UserProcessIsHome = true; // Home is always the first app
    bool b_UserProcessRunning = false;
    int i_UserProcessStatus = -1;
    
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
             *  Step 6.1: Not running, why? Did home crash?
             */
            
            if (i_UserProcessStatus > EXIT_SUCCESS && b_UserProcessIsHome == true)
            {
                // Default crashed, stop core
                c_Logger.Log(Logger::ERROR, "Home package crashed, stopping core!", "Main.cpp", __LINE__);
                break;
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
                p_Input->ClearInput();
                
                // Set process info
                // NOTE: This is handled here because launch requests can be sent from apps
                //       and that request might be a home launch (not automated)
                if (c_Request.s_PackagePath.compare(c_CoreConfiguration.GetHomePackagePath()) == 0)
                {
                    b_UserProcessIsHome = true;
                }
                else
                {
                    b_UserProcessIsHome = false;
                }
                
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
                c_Logger.Log(Logger::ERROR, "Launch failed! Input (" + e.module2() + "): " + e.what2(), "Main.cpp", __LINE__);
            }
            catch (PackageException& e)
            {
                c_Logger.Log(Logger::ERROR, "Launch failed! Package (" + e.package2() + "): " + e.what2(), "Main.cpp", __LINE__);
            }
            catch (ProcessException& e)
            {
                c_Logger.Log(Logger::ERROR, "Launch failed! User Process: " + e.what2(), "Main.cpp", __LINE__);
            }
            catch (std::exception& e)
            {
                c_Logger.Log(Logger::ERROR, "Launch failed! Unknown: " + std::string(e.what()), "Main.cpp", __LINE__);
            }
        }
    }
    
    // All done, now terminate
    // NOTE: We give the user process extra time to stop
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
