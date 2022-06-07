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
#include "./TransmissionSource.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

TransmissionSource::TransmissionSource(SourceType e_Type) noexcept : e_Type(e_Type)
{}

TransmissionSource::~TransmissionSource() noexcept
{}

//*************************************************************************************
// Reset
//*************************************************************************************

void TransmissionSource::Reset()
{}

//*************************************************************************************
// Open
//*************************************************************************************

void TransmissionSource::Open()
{}

//*************************************************************************************
// Close
//*************************************************************************************

void TransmissionSource::Close()
{}

//*************************************************************************************
// Read
//*************************************************************************************

bool TransmissionSource::CanRead(MRH_Sint32 s32_TimeoutMS) noexcept
{
    return false;
}

ssize_t TransmissionSource::Read(std::vector<MRH_Uint8>& v_Data, MRH_Uint32 u32_Length) noexcept
{
    return 0;
}

ssize_t TransmissionSource::Read(MRH_Uint8* p_Data, MRH_Uint32 u32_Length) noexcept
{
    return 0;
}

//*************************************************************************************
// Write
//*************************************************************************************

ssize_t TransmissionSource::Write(std::vector<MRH_Uint8>& v_Data, MRH_Uint32 u32_Length) noexcept
{
    return 0;
}

ssize_t TransmissionSource::Write(const MRH_Uint8* p_Data, MRH_Uint32 u32_Length) noexcept
{
    return 0;
}

//*************************************************************************************
// Getters
//*************************************************************************************

TransmissionSource::SourceType TransmissionSource::GetType() const noexcept
{
    return e_Type;
}
