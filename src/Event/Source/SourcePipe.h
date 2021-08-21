/**
 *  SourcePipe.h
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

#ifndef SourcePipe_h
#define SourcePipe_h

// C / C++
#include <poll.h>

// External

// Project
#include "TransmissionSource.h"
#include "../EventException.h"


class SourcePipe : public TransmissionSource
{
public:
    
    //*************************************************************************************
    // Types
    //*************************************************************************************
    
    typedef enum
    {
        PIPE_END_READ = 0,
        PIPE_END_WRITE = 1,
        
        PIPE_END_MAX = 1,
        
        PIPE_END_COUNT = 2
        
    }PipeEnd;
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    SourcePipe() noexcept;
    
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_SourcePipe SourcePipe class source.
     */

    SourcePipe(SourcePipe const& c_SourcePipe) = delete;
    
    /**
     *  Default destructor.
     */

    ~SourcePipe() noexcept;
    
    //*************************************************************************************
    // Reset
    //*************************************************************************************
    
    /**
     *  Reset pipe.
     */
    
    void Reset() override;
    
    //*************************************************************************************
    // Open
    //*************************************************************************************
    
    /**
     *  Open pipe.
     */
    
    void Open() override;
    
    //*************************************************************************************
    // Close
    //*************************************************************************************
    
    /**
     *  Close pipe.
     */
    
    void Close() override;
    
    //*************************************************************************************
    // Read
    //*************************************************************************************
    
    /**
     *  Check if a pipe contains data to read.
     *
     *  \param s32_TimeoutMS The data check timeout (blocking) in milliseconds.
     *
     *  \return true if data can be read, false if not.
     */
    
    bool CanRead(MRH_Sint32 s32_TimeoutMS) noexcept override;
    
    /**
     *  Read data from pipe to a vector.
     *
     *  \param v_Data The byte data vector to read to.
     *  \param u32_Length The length in bytes to read.
     *
     *  \return The amount of bytes read on success, -1 on failure.
     */
    
    ssize_t Read(std::vector<MRH_Uint8>& v_Data, MRH_Uint32 u32_Length) noexcept override;
    
    /**
     *  Read data from pipe to a buffer.
     *
     *  \param p_Data The byte data buffer to read to. This buffer has to be allocated.
     *  \param u32_Length The length in bytes to read.
     *
     *  \return The amount of bytes read on success, -1 on failure.
     */
    
    ssize_t Read(MRH_Uint8* p_Data, MRH_Uint32 u32_Length) noexcept override;
    
    //*************************************************************************************
    // Write
    //*************************************************************************************
     
     /**
      *  Write data to pipe from a vector.
      *
      *  \param v_Data The byte data vector to write from.
      *  \param u32_Length The length in bytes to write.
      *
      *  \return The amount of bytes read on success, -1 on failure.
      */
     
     ssize_t Write(std::vector<MRH_Uint8>& v_Data, MRH_Uint32 u32_Length) noexcept override;
     
     /**
      *  Write data from pipe from a buffer.
      *
      *  \param p_Data The byte data buffer to write from.
      *  \param u32_Length The length in bytes to write.
      *
      *  \return The amount of bytes read on success, -1 on failure.
      */
     
     ssize_t Write(const MRH_Uint8* p_Data, MRH_Uint32 u32_Length) noexcept override;
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Get pipe end file descriptor.
     *
     *  \param e_End The pipe end file destriptor to get.
     *
     *  \return The requested pipe end file descriptor.
     */
    
    int GetFD(PipeEnd e_End) const;
    
private:
    
    //*************************************************************************************
    // Poll
    //*************************************************************************************

    /**
     *  Poll event pipe.
     *
     *  \param e_End The pipe end to poll.
     *  \param i_Event The poll event to check.
     *  \param s32_TimeoutMS The poll timeout (blocking) in milliseconds.
     *
     *  \return true if the event was found, false if not.
     */
    
    bool Poll(PipeEnd e_End, int i_Event, MRH_Sint32 s32_TimeoutMS) noexcept;
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    // Pipe ends
    int p_FD[PIPE_END_COUNT];
    
    // Polling
    struct pollfd c_PollFD;
    
protected:
    
};

#endif /* SourcePipe_h */
