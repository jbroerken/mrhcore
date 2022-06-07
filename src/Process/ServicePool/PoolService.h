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

#ifndef PoolService_h
#define PoolService_h

// C / C++
#include <thread>
#include <mutex>
#include <vector>
#include <memory>

// External

// Project
#include "../ServiceProcess.h"
#include "./PoolCondition.h"
#include "./PoolEvents.h"


class PoolService : public PoolEvents
{
public:
    
    //*************************************************************************************
    // Destructor
    //*************************************************************************************
    
    /**
     *  Default destructor.
     */
    
    virtual ~PoolService() noexcept;
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Get the service process. This function is thread safe.
     *
     *  \return The service process.
     */
    
    std::shared_ptr<ServiceProcess> const& GetProcess() const noexcept;
    
    /**
     *  Check if the service is essential. This function is thread safe.
     *
     *  \return true if the service is essential, false if not.
     */
    
    bool GetEssential() const noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    static const MRH_Uint32 u32_StopTimerS = 5;
    
private:

    //*************************************************************************************
    // Update
    //*************************************************************************************
    
    /**
     *  Update the service.
     *
     *  \param p_Service The service instance to update with.
     *  \param u32_EventLimit The max amount of event to be sent / recieved in a update.  
     *  \param s32_TimeoutMS The event recieve timeout in milliseconds.
     */
    
    static void Update(PoolService* p_Service, MRH_Uint32 u32_EventLimit, MRH_Sint32 s32_TimeoutMS) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    std::thread c_Thread;
    std::shared_ptr<PoolCondition> p_Condition;
    bool b_Essential;
    
protected:
    
    //*************************************************************************************
    // Constructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     *
     *  \param p_Process The process for this service.
     *  \param p_Condition The service pool condition for notification.
     *  \param u32_EventLimit The max amount of event to be sent / recieved in a update.
     *  \param s32_TimeoutMS The event recieve timeout in milliseconds.
     *  \param b_Essential Wether the service is essential or not.
     */
    
    PoolService(std::shared_ptr<ServiceProcess>& p_Process,
                std::shared_ptr<PoolCondition>& p_Condition,
                MRH_Uint32 u32_EventLimit,
                MRH_Sint32 s32_TimeoutMS,
                bool b_Essential);
    
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_PoolService PoolService class source.
     */
    
    PoolService(PoolService const& c_PoolService) = delete;
                
    //*************************************************************************************
    // Data
    //*************************************************************************************
                
    std::shared_ptr<ServiceProcess> p_Process;
};

#endif /* PoolService_h */
