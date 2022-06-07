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

#ifndef TransmissionSource_h
#define TransmissionSource_h

// C / C++
#include <sys/types.h> // ssize_t
#include <vector>

// External
#include <MRH_Typedefs.h>

// Project


class TransmissionSource
{
public:
    
    //*************************************************************************************
    // Types
    //*************************************************************************************
    
    typedef enum
    {
        MRHCKM = 0,
        PIPE = 1,
        
        SOURCE_TYPE_MAX = 1,
        
        SOURCE_TYPE_COUNT = 2
        
    }SourceType;
    
    //*************************************************************************************
    // Destructor
    //*************************************************************************************

    /**
     *  Default destructor.
     */

    virtual ~TransmissionSource() noexcept; // Virtual is needed, we expect work to happen with the base class
    
    //*************************************************************************************
    // Reset
    //*************************************************************************************
    
    /**
     *  Reset transmission source.
     */
    
    virtual void Reset();
    
    //*************************************************************************************
    // Open
    //*************************************************************************************
    
    /**
     *  Open transmission source.
     */
    
    virtual void Open();
    
    //*************************************************************************************
    // Close
    //*************************************************************************************
    
    /**
     *  Close transmission source.
     */
    
    virtual void Close();
    
    //*************************************************************************************
    // Read
    //*************************************************************************************

    /**
     *  Check if a transmission source contains data to read.
     *
     *  \param s32_TimeoutMS The data check timeout (blocking) in milliseconds.
     *
     *  \return true if data can be read, false if not.
     */
    
    virtual bool CanRead(MRH_Sint32 s32_TimeoutMS) noexcept;
    
    /**
     *  Read data from transmission source to a vector.
     *
     *  \param v_Data The byte data vector to read to.
     *  \param u32_Length The length in bytes to read.
     *
     *  \return The amount of bytes read on success, -1 on failure.
     */
    
    virtual ssize_t Read(std::vector<MRH_Uint8>& v_Data, MRH_Uint32 u32_Length) noexcept;
    
    /**
     *  Read data from transmission source to a buffer.
     *
     *  \param p_Data The byte data buffer to read to. This buffer has to be allocated.
     *  \param u32_Length The length in bytes to read.
     *
     *  \return The amount of bytes read on success, -1 on failure.
     */
    
    virtual ssize_t Read(MRH_Uint8* p_Data, MRH_Uint32 u32_Length) noexcept;
    
    //*************************************************************************************
    // Write
    //*************************************************************************************
     
     /**
      *  Write data to transmission source from a vector.
      *
      *  \param v_Data The byte data vector to write from.
      *  \param u32_Length The length in bytes to write.
      *
      *  \return The amount of bytes read on success, -1 on failure.
      */
     
     virtual ssize_t Write(std::vector<MRH_Uint8>& v_Data, MRH_Uint32 u32_Length) noexcept;
     
     /**
      *  Write data to transmission from a buffer.
      *
      *  \param p_Data The byte data buffer to write from.
      *  \param u32_Length The length in bytes to write.
      *
      *  \return The amount of bytes read on success, -1 on failure.
      */
     
     virtual ssize_t Write(const MRH_Uint8* p_Data, MRH_Uint32 u32_Length) noexcept;
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get transmission source type.
     *
     *  \return The transmission source type.
     */

    SourceType GetType() const noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    SourceType e_Type;
    
protected:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param e_Type The transmission source type.
     */

    TransmissionSource(SourceType e_Type) noexcept;
    
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_TransmissionSource TransmissionSource class source.
     */

    TransmissionSource(TransmissionSource const& c_TransmissionSource) = delete;
};

#endif /* TransmissionSource_h */
