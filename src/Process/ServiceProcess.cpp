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
#include "./ServiceProcess.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

ServiceProcess::ServiceProcess(TransmissionSource::SourceType e_Type,
                               bool b_CanRecieve,
                               bool b_CanSend) : EventQueue(e_Type),
                                                 b_CanRecieve(b_CanRecieve),
                                                 b_CanSend(b_CanSend),
                                                 s_RunPath("")
{}

ServiceProcess::~ServiceProcess() noexcept
{
    // @NOTE: Process class terminates process on destruction!
}

//*************************************************************************************
// Recieve
//*************************************************************************************

void ServiceProcess::RecieveEvents(MRH_Uint32 u32_EventLimit, MRH_Sint32 s32_TimeoutMS) noexcept
{
    EventQueue::RecieveEvents(u32_EventLimit, s32_TimeoutMS);
}

std::vector<Event>& ServiceProcess::RetrieveEvents() noexcept
{
    return EventQueue::RetrieveEvents();
}

//*************************************************************************************
// Send
//*************************************************************************************

void ServiceProcess::AddSendEvents(std::vector<Event>& v_Event) noexcept
{
    EventQueue::AddSendEvents(v_Event);
}

void ServiceProcess::SendEvents(MRH_Uint32 u32_EventLimit) noexcept
{
    EventQueue::SendEvents(u32_EventLimit);
}

void ServiceProcess::SendEvents(std::vector<Event>& v_Event, MRH_Uint32 u32_EventLimit) noexcept
{
    EventQueue::AddSendEvents(v_Event);
    EventQueue::SendEvents(u32_EventLimit);
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool ServiceProcess::GetCanRecieve() const noexcept
{
    return b_CanRecieve;
}

bool ServiceProcess::GetCanSend() const noexcept
{
    return b_CanSend;
}

std::string ServiceProcess::GetRunPath() const noexcept
{
    return s_RunPath;
}
