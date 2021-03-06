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

#ifndef PoolCondition_h
#define PoolCondition_h

// C / C++
#include <condition_variable>
#include <mutex>

// External
#include <MRH_Typedefs.h>

// Project


class PoolCondition
{
    friend class ServicePool;
    
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     */
    
    PoolCondition() noexcept;
    
    /**
     *  Default destructor.
     */
    
    ~PoolCondition() noexcept;
    
    //*************************************************************************************
    // Notify
    //*************************************************************************************
    
    /**
     *  Notify a waiting thread. This function is thread safe.
     */
    
    void Notify() noexcept;
    
private:
    
    //*************************************************************************************
    // Wait
    //*************************************************************************************
    
    /**
     *  Wait for a notification. This function is thread safe.
     */
    
    void Wait() noexcept;
    
    /**
     *  Wait for a notification until time expires. This function is thread safe.
     *
     *  \param s32_TimeoutMS The timeout in milliseconds.
     */
    
    void Wait(MRH_Sint32 s32_TimeoutMS) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    std::condition_variable c_Condition;
    std::mutex c_Mutex;
    
protected:
    
};

#endif /* PoolCondition_h */
