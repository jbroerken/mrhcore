/**
 *  UserService.h
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

#ifndef UserService_h
#define UserService_h

// C / C++

// External

// Project
#include "../PoolService.h"


class UserService : public PoolService
{
    friend class UserServicePool;
    
public:
    
    //*************************************************************************************
    // Destructor
    //*************************************************************************************
    
    /**
     *  Default destructor.
     */
    
    ~UserService() noexcept;
    
private:

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
     */
    
    UserService(std::shared_ptr<ServiceProcess>& p_Process,
                std::shared_ptr<PoolCondition>& p_Condition,
                MRH_Uint32 u32_EventLimit,
                MRH_Sint32 s32_TimeoutMS) noexcept;
                
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_UserService UserService class source.
     */
                
    UserService(UserService const& c_UserService) = delete;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
protected:
    
};

#endif /* UserService_h */
