/**
 *  ServicePool.cpp
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
#include <sys/stat.h>
#include <unistd.h>
#include <csignal>

// External

// Project
#include "./ServicePool.h"
#include "../../Logger/Logger.h"
#include "../../FilePaths.h"

// Pre-defined
#ifndef MRH_CORE_CREATE_PID_DIR
    #define MRH_CORE_CREATE_PID_DIR 0
#endif


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

ServicePool::ServicePool() : b_Run(false)
{
    // Create pid dir
#if MRH_CORE_CREATE_PID_DIR > 0
    struct stat c_Stat;
    size_t us_Pos = 0;
    std::string s_Full(MRH_CORE_PID_FILE_DIR);
    std::string s_Current = "";
    
    while ((us_Pos = s_Full.find_first_of('/', us_Pos + 1)) != std::string::npos)
    {
        s_Current = s_Full.substr(0, us_Pos);
        
        if (stat(s_Current.c_str(), &c_Stat) == 0 && S_ISDIR(c_Stat.st_mode))
        {
            continue;
        }
        
        if (mkdir(s_Current.c_str(), 0777) < 0) // @TODO: Restricting might be better
        {
            Logger::Singleton().Log(Logger::WARNING, "Failed to create pid path: " + s_Current,
                                    "ServicePool.cpp", __LINE__);
            break;
        }
    }
#endif
    
    // Thread stuff
    try
    {
        p_Condition = std::make_shared<PoolCondition>();
    }
    catch (std::exception& e)
    {
        throw ProcessException("Failed to create pool condition: " + std::string(e.what()));
    }
}

ServicePool::~ServicePool() noexcept
{
    StopUpdate();
    v_Service.clear();
}

//*************************************************************************************
// Update
//*************************************************************************************

void ServicePool::StartUpdate(MRH_Sint32 s32_TimeoutMS)
{
    if (b_Run == true)
    {
        throw ProcessException("Service pool is already running!");
    }
    
    try
    {
        b_Run = true;
        c_Thread = std::thread(Update, this, s32_TimeoutMS);
    }
    catch (std::exception& e)
    {
        b_Run = false;
        throw ProcessException("Failed to start service pool thread: " + std::string(e.what()));
    }
}

void ServicePool::StopUpdate() noexcept
{
    // Do not clear services, might be restarted later
    if (b_Run == true)
    {
        b_Run = false;
    
        if (c_Thread.joinable() == true)
        {
            c_Thread.join();
        }
    }
}

void ServicePool::Update(ServicePool* p_ServicePool, MRH_Sint32 s32_TimeoutMS) noexcept
{
    // Get values directly, a bit more readable
    std::shared_ptr<PoolCondition>& p_Condition = p_ServicePool->p_Condition;
    
    // Constantly update
    while (p_ServicePool->b_Run == true)
    {
        // Wait for services to notify us
        p_Condition->Wait(s32_TimeoutMS);
        
        // Check service health
        p_ServicePool->CheckServiceStatus();
        
        // Call virtual to exchange service events defined by the inheriting class
        p_ServicePool->RetrieveRecievedEvents();
        p_ServicePool->DistributeSendEvents();
    }
}

void ServicePool::CheckServiceStatus() noexcept
{
    // Simple logging for user processes, all condsidered non essential
    bool b_Running;
    int i_Result;
    
    for (auto Service = v_Service.begin(); Service != v_Service.end();)
    {
        (*Service)->GetProcess()->GetProcessState(b_Running, i_Result);
        
        if (b_Running == false)
        {
            Logger::Singleton().Log(Logger::INFO, "Service " +
                                                 (*Service)->GetProcess()->GetRunPath() +
                                                 " stopped! Exit code: " +
                                                 std::to_string(i_Result),
                                    "ServicePool.cpp", __LINE__);
            
            // Crash core on essential service lost
            if ((*Service)->GetEssential() == true)
            {
                //std::raise(SIGTERM);
                kill(getpid(), SIGTERM);
            }
            
            // Remove stopped, dead weight
            Service = v_Service.erase(Service);
        }
        else
        {
            ++Service;
        }
    }
}

void ServicePool::WritePidList(std::string s_ListName, std::vector<pid_t> const& v_Pid) noexcept
{
    if (v_Pid.size() == 0 || s_ListName.size() == 0)
    {
        return;
    }
    
    s_ListName = MRH_CORE_PID_FILE_DIR + s_ListName;
    std::ofstream f_File(s_ListName, std::ios::trunc);
    
    if (f_File.is_open() == false)
    {
        Logger::Singleton().Log(Logger::WARNING, "Failed to open pid file: " + s_ListName,
                                "ServicePool.cpp", __LINE__);
        return;
    }
    
    for (size_t i = 0; i < v_Pid.size(); ++i)
    {
        f_File << std::to_string(v_Pid[i]);
        
        if (i < (v_Pid.size() - 1))
        {
            f_File << std::endl;
        }
    }
    
    f_File.close();
    
    Logger::Singleton().Log(Logger::INFO, "Wrote pid file: " + s_ListName,
                            "ServicePool.cpp", __LINE__);
}

//*************************************************************************************
// Recieve
//*************************************************************************************

void ServicePool::RetrieveRecievedEvents() noexcept
{
    for (auto& Service : v_Service)
    {
        std::shared_ptr<ServiceProcess> const& p_Process = Service->GetProcess();
        
        if (p_Process->GetRunning() == false || p_Process->GetCanSend() == false)
        {
            continue;
        }
        
        // Lock Mutexes
        p_Mutex[RECIEVED].lock();
        Service->LockRecievedEvents();
        
        // Event movement
        std::vector<Event>& v_Event = Service->RetrieveEvents();
        std::move(v_Event.begin(), v_Event.end(), std::back_inserter(p_Queue[RECIEVED]));
        
        // Unlock pool mutex, now available
        p_Mutex[RECIEVED].unlock();
        
        // Clear and unlock service
        v_Event.clear();
        Service->UnlockRecievedEvents();
    }
}

//*************************************************************************************
// Send
//*************************************************************************************

void ServicePool::DistributeSendEvents() noexcept
{
    // Lock send queue
    // NOTE: We lock for the whole distribution duration so that
    //       all current events are sent to all services, unlocking
    //       after each service might cause us to miss events
    p_Mutex[SEND].lock();
    
    for (auto& Service : v_Service)
    {
        std::shared_ptr<ServiceProcess> const& p_Process = Service->GetProcess();
        
        if (p_Process->GetRunning() == true && p_Process->GetCanRecieve() == true)
        {
            Service->SendEvents(p_Queue[SEND]);
        }
    }
    
    // All sent, clear original
    p_Queue[SEND].clear();
    p_Mutex[SEND].unlock();
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool ServicePool::GetRunning() const noexcept
{
    return b_Run;
}
