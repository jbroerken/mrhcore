/**
 *  PoolService.cpp
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
#include "./PoolService.h"
#include "../../Timer.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PoolService::PoolService(std::shared_ptr<ServiceProcess>& p_Process,
                         std::shared_ptr<PoolCondition>& p_Condition,
                         MRH_Uint32 u32_EventLimit,
                         MRH_Sint32 s32_TimeoutMS,
                         bool b_Essential) : p_Process(p_Process),
                                             p_Condition(p_Condition),
                                             b_Essential(b_Essential)
{
    try
    {
        c_Thread = std::thread(Update, this, u32_EventLimit, s32_TimeoutMS);
    }
    catch (std::exception& e)
    {
        throw ProcessException("Failed to start service pool thread: " + std::string(e.what()));
    }
}

PoolService::~PoolService() noexcept
{
    if (p_Process->GetRunning() == true)
    {
        // Initial stop signal
        Timer c_Timer;
        p_Process->Stop(false);
        MRH_Uint32 u32_SleepTimerMS = (u32_StopTimerS * 1000) / 5;
        
        // Check status
        while (p_Process->GetRunning() == true)
        {
            // Force quit if timer exceeded
            if (c_Timer.GetTimePassedSeconds() >= u32_StopTimerS)
            {
                p_Process->Stop(true);
                break;
            }
            
            // Give process some time to terminate
            std::this_thread::sleep_for(std::chrono::milliseconds(u32_SleepTimerMS));
        }
    }
    
    // Process stopped, join thread
    if (c_Thread.joinable() == true)
    {
        c_Thread.join();
    }
}

//*************************************************************************************
// Update
//*************************************************************************************

void PoolService::Update(PoolService* p_Service, MRH_Uint32 u32_EventLimit, MRH_Sint32 s32_TimeoutMS) noexcept
{
    // Get values directly, a bit more readable
    // @NOTE: This function can only be called from a instance itself,
    //        so we're able to use raw pointers more safely
    std::shared_ptr<PoolCondition>& p_Condition = p_Service->p_Condition;
    std::shared_ptr<ServiceProcess>& p_Process = p_Service->p_Process;
    std::vector<Event>* p_Queue = p_Service->p_Queue;
    std::mutex* p_Mutex = p_Service->p_Mutex;
    bool b_Recieve = p_Process->GetCanSend();
    bool b_Send = p_Process->GetCanRecieve();
    
    // Constantly update, stalled by recieving events
    // @NOTE: Nothing of the process needs to be locked, only PoolEvents data!
    while (p_Process->GetRunning() == true)
    {
        // Read incoming events
        if (b_Recieve == true)
        {
            // Recieve first
            p_Process->RecieveEvents(u32_EventLimit, s32_TimeoutMS);
            
            // Add to recieved queue so that the pool can retrieve the events
            p_Mutex[RECIEVED].lock();
            std::vector<Event>& v_Event = p_Process->RetrieveEvents();
            std::move(v_Event.begin(), v_Event.end(), std::back_inserter(p_Queue[RECIEVED]));
            p_Mutex[RECIEVED].unlock();
            
            // Signal condition, we have something to get for the service pool
            p_Condition->Notify();
        }
        else if (s32_TimeoutMS > 0)
        {
            // Wait for timeout, we dont want to spin this loop unless
            // explicitly desired by timeout value
            std::this_thread::sleep_for(std::chrono::milliseconds(s32_TimeoutMS));
        }
        
        // Send events
        if (b_Send == true)
        {
            p_Mutex[SEND].lock();
            p_Process->SendEvents(p_Queue[SEND], u32_EventLimit);
            p_Mutex[SEND].unlock();
        }
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

std::shared_ptr<ServiceProcess> const& PoolService::GetProcess() const noexcept
{
    return p_Process;
}

bool PoolService::GetEssential() const noexcept
{
    return b_Essential;
}
