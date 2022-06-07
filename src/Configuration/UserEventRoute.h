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

#ifndef UserEventRoute_h
#define UserEventRoute_h

// C / C++
#include <unordered_map>
#include <vector>

// External
#include <MRH_Typedefs.h>

// Project
#include "./ConfigurationException.h"


class UserEventRoute
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    UserEventRoute();

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_UserEventRoute UserEventRoute class source.
     */

    UserEventRoute(UserEventRoute const& c_UserEventRoute) = delete;

    /**
     *  Default destructor.
     */

    ~UserEventRoute() noexcept;

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get the amount of package locations in this list.
     *
     *  \return The package location amount.
     */

    size_t GetRouteCount() const noexcept;
    
    /**
     *  Get a user event route.
     *
     *  \param u32_Route The event route requested.
     *
     *  \return The requested event route.
     */
    
    std::vector<MRH_Uint32> const& GetRoute(MRH_Uint32 u32_Route) const;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::unordered_map<MRH_Uint32, std::vector<MRH_Uint32>> m_EventRoute;
    
protected:

};

#endif /* UserEventRoute_h */
