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

#ifndef UserServicePool_h
#define UserServicePool_h

// C / C++

// External

// Project
#include "../ServicePool.h"


class UserServicePool : public ServicePool
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************/
    
    /**
     *  Default constructor.
     */
    
    UserServicePool();
    
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_UserServicePool UserServicePool class source.
     */
    
    UserServicePool(UserServicePool const& c_UserServicePool) = delete;
    
    /**
     *  Default destructor.
     */
    
    ~UserServicePool() noexcept;

    //*************************************************************************************
    // Update
    //*************************************************************************************
    
    /**
     *  Reload the available user services.
     */
    
    void Reload();
    
private:

    //*************************************************************************************
    // Update
    //*************************************************************************************

    /**
     *  Add a user service to the service pool.
     *
     *  \param s_PackageName The name of the package containing the user service.
     */
    
    pid_t AddService(std::string const& s_PackageName);
    
    //*************************************************************************************
    // Send
    //*************************************************************************************
    
    /**
     *  Add sendable events to the servies.
     */

    void DistributeSendEvents() noexcept override;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
protected:
    
};

#endif /* UserServicePool_h */
