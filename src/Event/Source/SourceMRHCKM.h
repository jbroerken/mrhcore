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

#ifndef SourceMRHCKM_h
#define SourceMRHCKM_h

// C / C++

// External
#include <MRH_Typedefs.h>

// Project
#include "./TransmissionSource.h"


class SourceMRHCKM : public TransmissionSource
{
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    SourceMRHCKM() noexcept;
    
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param s_SourceMRHCKM SourceMRHCKM class source.
     */

    SourceMRHCKM(SourceMRHCKM const& s_SourceMRHCKM) = delete;
    
    /**
     *  Default destructor.
     */

    ~SourceMRHCKM() noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
protected:
    
};

#endif /* SourceMRHCKM_h */
