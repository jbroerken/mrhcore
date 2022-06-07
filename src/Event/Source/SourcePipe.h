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
