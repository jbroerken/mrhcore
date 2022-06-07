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

#ifndef ServicePool_h
#define ServicePool_h

// C / C++
#include <atomic>
#include <vector>

// External

// Project
#include "./PoolService.h"


class ServicePool : public PoolEvents
{
public:

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Check if the service pool update is currently running.
     *
     *  \return true if the service pool update is running, false if not.
     */
    
    bool GetRunning() const noexcept;
    
private:

    //*************************************************************************************
    // Update
    //*************************************************************************************
    
    /**
     *  Update the service pool. This function is thread safe.
     *
     *  \param p_ServicePool The service pool instance to update with.
     *  \param s32_TimeoutMS The condition timeout in milliseconds.
     */
    
    static void Update(ServicePool* p_ServicePool, MRH_Sint32 s32_TimeoutMS) noexcept;
    
    /**
     *  Check the status of the pool services in use.
     */
    
    void CheckServiceStatus() noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    // Threaded update info
    std::thread c_Thread;
    std::atomic<bool> b_Run;
    
protected:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     */
    
    ServicePool();
    
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_ServicePool ServicePool class source.
     */
    
    ServicePool(ServicePool const& c_ServicePool) = delete;
    
    /**
     *  Default destructor.
     */
    
    ~ServicePool() noexcept;
    
    //*************************************************************************************
    // Update
    //*************************************************************************************
    
    /**
     *  Start the service pool update thread.
     *
     *  \param s32_TimeoutMS The condition timeout in milliseconds.
     */
    
    void StartUpdate(MRH_Sint32 s32_TimeoutMS);
    
    /**
     *  Stop the service pool update thread.
     */
    
    void StopUpdate() noexcept;
    
    /**
     *  Write the service pool proccess id list.
     *
     *  \param s_ListName The name of the pid list to write.
     *  \param v_Pid The process ids to write.
     */
    
    void WritePidList(std::string s_ListName, std::vector<pid_t> const& v_Pid) noexcept;
    
    //*************************************************************************************
    // Recieve
    //*************************************************************************************

    /**
     *  Retrieve all recieved service events.
     */
    
    virtual void RetrieveRecievedEvents() noexcept;

    //*************************************************************************************
    // Send
    //*************************************************************************************
    
    /**
     *  Add sendable events to the servies.
     */

    virtual void DistributeSendEvents() noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    // Service list
    std::vector<std::shared_ptr<PoolService>> v_Service;
    
    // Threaded update info
    std::shared_ptr<PoolCondition> p_Condition;
};

#endif /* ServicePool_h */
