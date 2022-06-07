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

// C / C++

// External

// Project
#include "./UserService.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

UserService::UserService(std::shared_ptr<ServiceProcess>& p_Process,
                         std::shared_ptr<PoolCondition>& p_Condition,
                         MRH_Uint32 u32_EventLimit,
                         MRH_Sint32 s32_TimeoutMS) : PoolService(p_Process,
                                                                 p_Condition,
                                                                 u32_EventLimit,
                                                                 s32_TimeoutMS,
                                                                 false) // User services are not essential
{}

UserService::~UserService() noexcept
{}
