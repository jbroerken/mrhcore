/**
 *  SourcePipe.cpp
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
#include <unistd.h>
#include <fcntl.h>
#include <climits>
#include <cerrno>
#include <cstring>

// External

// Project
#include "./SourcePipe.h"
#include "../../Logger/Logger.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

SourcePipe::SourcePipe() noexcept : TransmissionSource(TransmissionSource::PIPE)
{
    for (size_t i = 0; i < PipeEnd::PIPE_END_COUNT; ++i)
    {
        p_FD[i] = -1;
    }
}

SourcePipe::~SourcePipe() noexcept
{
    Close();
}

//*************************************************************************************
// Reset
//*************************************************************************************

void SourcePipe::Reset()
{
    try
    {
        Close();
        Open();
    }
    catch (EventException& e)
    {
        throw EventException(e.what());
    }
}

//*************************************************************************************
// Open
//*************************************************************************************

void SourcePipe::Open()
{
    // Create pipe
    if (pipe(p_FD) < 0)
    {
        throw EventException("Failed to create pipe: " + std::string(std::strerror(errno)) + " (" + std::to_string(errno) + ")!");
    }

    // Set non-blocking
    if (fcntl(p_FD[PipeEnd::PIPE_END_READ], F_SETFL, fcntl(p_FD[PipeEnd::PIPE_END_READ], F_GETFL) | O_NONBLOCK) < 0 ||
        fcntl(p_FD[PipeEnd::PIPE_END_WRITE], F_SETFL, fcntl(p_FD[PipeEnd::PIPE_END_WRITE], F_GETFL) | O_NONBLOCK) < 0)
    {
        throw EventException("Failed to set pipe non-blocking: " + std::string(std::strerror(errno)) + " (" + std::to_string(errno) + ")!");
    }
}

//*************************************************************************************
// Close
//*************************************************************************************

void SourcePipe::Close()
{
    // Close pipe read end
    if (p_FD[PipeEnd::PIPE_END_READ] > -1)
    {
        close(p_FD[PipeEnd::PIPE_END_READ]);
        p_FD[PipeEnd::PIPE_END_READ] = -1;
    }

    // Close pipe write end
    if (p_FD[PipeEnd::PIPE_END_WRITE] > -1)
    {
        close(p_FD[PipeEnd::PIPE_END_WRITE]);
        p_FD[PipeEnd::PIPE_END_WRITE] = -1;
    }
}

//*************************************************************************************
// Poll
//*************************************************************************************

bool SourcePipe::Poll(PipeEnd e_End, int i_Event, MRH_Sint32 s32_TimeoutMS) noexcept
{
    // Setup poll
    switch (e_End)
    {
        case PIPE_END_READ:
            c_PollFD.fd = p_FD[PIPE_END_READ];
            break;
        case PIPE_END_WRITE:
            c_PollFD.fd = p_FD[PIPE_END_WRITE];
            break;
            
        default:
            Logger::Singleton().Log(Logger::WARNING, "Invalid pipe end for polling!", "SourcePipe.cpp", __LINE__);
            return false;
    }
    
    c_PollFD.events = i_Event;
    
    // Poll
    if (s32_TimeoutMS < 0)
    {
        s32_TimeoutMS = 0;
    }
    
    switch (poll(&(c_PollFD), 1, s32_TimeoutMS))
    {
        // Failed
        case -1:
            Logger::Singleton().Log(Logger::WARNING, "Could not poll pipe: " +
                                                     std::string(std::strerror(errno)) +
                                                     " (" +
                                                     std::to_string(errno) +
                                                     ")!",
                                    "SourcePipe.cpp", __LINE__);
        case 0:
            return false;
            
        // Result
        default:
            return (c_PollFD.revents & i_Event) ? true : false;
    }
}

//*************************************************************************************
// Read
//*************************************************************************************

bool SourcePipe::CanRead(MRH_Sint32 s32_TimeoutMS) noexcept
{
    return Poll(PIPE_END_READ, POLLIN, s32_TimeoutMS);
}

ssize_t SourcePipe::Read(std::vector<MRH_Uint8>& v_Data, MRH_Uint32 u32_Length) noexcept
{
    return Read(v_Data.data(), u32_Length);
}

ssize_t SourcePipe::Read(MRH_Uint8* p_Data, MRH_Uint32 u32_Length) noexcept
{
    if (p_Data == NULL || u32_Length == 0)
    {
        return 0;
    }
    
    ssize_t ss_Read;
    
    if ((ss_Read = read(p_FD[PipeEnd::PIPE_END_READ], p_Data, u32_Length)) < 0 && errno != EAGAIN)
    {
        Logger::Singleton().Log(Logger::WARNING, "Could not read pipe: " +
                                                 std::string(std::strerror(errno)) +
                                                 " (" +
                                                 std::to_string(errno) +
                                                 ")!",
                                "SourcePipe.cpp", __LINE__);
    }
    
    return ss_Read;
}

//*************************************************************************************
// Write
//*************************************************************************************

ssize_t SourcePipe::Write(std::vector<MRH_Uint8>& v_Data, MRH_Uint32 u32_Length) noexcept
{
    return Write((const MRH_Uint8*)&(v_Data[0]), u32_Length);
}

ssize_t SourcePipe::Write(const MRH_Uint8* p_Data, MRH_Uint32 u32_Length) noexcept
{
    if (p_Data == NULL || u32_Length == 0)
    {
        return 0;
    }
    
    ssize_t ss_Write;
    
    if ((ss_Write = write(p_FD[PipeEnd::PIPE_END_WRITE], p_Data, u32_Length)) < 0 && errno != EAGAIN)
    {
        Logger::Singleton().Log(Logger::WARNING, "Could not write pipe: " +
                                                 std::string(std::strerror(errno)) +
                                                 " (" +
                                                 std::to_string(errno) +
                                                 ")!",
                                "SourcePipe.cpp", __LINE__);
    }
    
    return ss_Write;
}

//*************************************************************************************
// Getters
//*************************************************************************************

int SourcePipe::GetFD(PipeEnd e_End) const
{
    if (e_End < PipeEnd::PIPE_END_COUNT)
    {
        return p_FD[e_End];
    }
    
    throw EventException("Invalid pipe end specified!");
}
