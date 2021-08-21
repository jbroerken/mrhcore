/**
 *  TransmissionSource.cpp
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
