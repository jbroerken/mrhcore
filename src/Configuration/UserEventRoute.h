/**
 *  UserEventRoute.h
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
