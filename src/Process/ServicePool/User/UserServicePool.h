/**
 *  UserServicePool.h
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
    
    void AddService(std::string const& s_PackageName);
    
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
