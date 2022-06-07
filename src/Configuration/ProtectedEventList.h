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

#ifndef ProtectedEventList_h
#define ProtectedEventList_h

// C / C++
#include <mutex>
#include <vector>

// External
#include <MRH_Typedefs.h>

// Project
#include "./ConfigurationException.h"


class ProtectedEventList
{
public:

    //*************************************************************************************
    // Singleton
    //*************************************************************************************

    /**
     *  Get the class instance. This function is thread safe.
     *
     *  \return The class instance.
     */

    static ProtectedEventList& Singleton() noexcept;
    
    //*************************************************************************************
    // Constructor
    //*************************************************************************************

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_ProtectedEventList ProtectedEventList class source.
     */

    ProtectedEventList(ProtectedEventList const& c_ProtectedEventList) = delete;

    //*************************************************************************************
    // Update
    //*************************************************************************************

    /**
     *  Update the configuration. This function is thread safe.
     */
    
    void Update();

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get the amount of user services packages in this list. This function
     *  is thread safe.
     *
     *  \return The user service package count.
     */

    size_t GetEventCount() noexcept;
    
    /**
     *  Get a protected event. This function is thread safe.
     *
     *  \param us_Event The event requested.
     *
     *  \return The requested event.
     */
    
    MRH_Uint32 GetEvent(size_t us_Event);
    
    /**
     *  Check if a given event is protected. This function is thread safe.
     *
     *  \param u32_Type The event type to check.
     *
     *  \return true if the event is protected, false if not.
     */
    
    bool GetEventProtected(MRH_Uint32 u32_Type) noexcept;
    
private:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    ProtectedEventList() noexcept;

    /**
     *  Default destructor.
     */

    ~ProtectedEventList() noexcept;

    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::mutex c_Mutex;
    std::vector<MRH_Uint32> v_Protected;
    
protected:

};

#endif /* ProtectedEventList_h */
