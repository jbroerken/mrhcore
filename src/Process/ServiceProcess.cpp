/**
 *  ServiceProcess.cpp
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
