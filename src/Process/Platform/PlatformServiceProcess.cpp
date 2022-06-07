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
    // @NOTE: ServiceProcess class terminates process on destruction!
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
