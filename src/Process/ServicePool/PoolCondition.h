/**
 *  PoolCondition.h
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
