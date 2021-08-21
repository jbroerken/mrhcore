/**
 *  ProtectedEventList.h
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
