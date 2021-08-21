/**
 *  PlatformServiceProcess.cpp
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
#include "./PlatformServiceProcess.h"
#include "../../Logger/EventLogger.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

#ifdef __MRH_MRHCKM_SUPPORTED__
PlatformServiceProcess::PlatformServiceProcess() : ServiceProcess(TransmissionSource::SourceType::MRHCKM,
                                                                  true,
                                                                  true)
#else
PlatformServiceProcess::PlatformServiceProcess() : ServiceProcess(TransmissionSource::SourceType::PIPE,
                                                                  true,
                                                                  true)
#endif
{}

PlatformServiceProcess::~PlatformServiceProcess() noexcept
{
    // NOTE: ServiceProcess class terminates process on destruction!
}

//*************************************************************************************
// Run
//*************************************************************************************

void PlatformServiceProcess::Run(std::string s_BinaryPath, MRH_Uint32 u32_EventLimit, MRH_Sint32 s32_RecieveTimeoutMS)
{
    // Set run path
    this->s_RunPath = s_BinaryPath;
    
    // Re-create event queue
    try
    {
        EventQueue::Reset();
    }
    catch (EventException& e)
    {
        throw ProcessException("Failed to reset event queue: " + e.what2()); // Convert to process exception
    }
    
    // Create arguments
    std::vector<std::vector<char>> v_Arg;
    
    try
    {
        v_Arg.emplace_back(GetArgumentBytes(std::to_string(GetPipeFD(QueueType::P_W_C_R, SourcePipe::PipeEnd::PIPE_END_READ))));
        v_Arg.emplace_back(GetArgumentBytes(std::to_string(GetPipeFD(QueueType::C_W_P_R, SourcePipe::PipeEnd::PIPE_END_WRITE))));
        v_Arg.emplace_back(GetArgumentBytes(std::to_string(u32_EventLimit)));
        v_Arg.emplace_back(GetArgumentBytes(std::to_string(s32_RecieveTimeoutMS)));
    }
    catch (ProcessException& e)
    {
        throw ProcessException("Failed to setup platform service process launch arguments: " + e.what2()); // More error info
    }
    catch (std::exception& e)
    {
        throw ProcessException("Failed to setup platform service process launch arguments: " + std::string(e.what()));
    }
    
    // Run
    try
    {
        Process::Run(s_BinaryPath, v_Arg);
    }
    catch (ProcessException& e)
    {
        throw;
    }
}

//*************************************************************************************
// Recieve
//*************************************************************************************
    
#if MRH_CORE_EVENT_LOGGING > 0
void PlatformServiceProcess::LogRecievedEvents(Event const& c_Event) noexcept
{
    EventLogger::Singleton().Log(c_Event, "Recieved event from platform service process: " + GetRunPath());
}
#endif

//*************************************************************************************
// Send
//*************************************************************************************

#if MRH_CORE_EVENT_LOGGING > 0
void PlatformServiceProcess::LogSentEvents(Event const& c_Event) noexcept
{
    EventLogger::Singleton().Log(c_Event, "Sending event to platform service process: " + GetRunPath());
}
#endif
