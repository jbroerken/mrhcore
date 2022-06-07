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
#include "./Timer.h"

// Pre-defined
namespace
{
    // Formatting typedefs
    typedef std::chrono::high_resolution_clock HighResClock;
    typedef std::chrono::seconds TimeSeconds;
    typedef std::chrono::milliseconds TimeMilliseconds;
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

Timer::Timer() noexcept
{
    Reset();
}

Timer::Timer(Timer const& c_Timer) noexcept : c_StartTime(c_Timer.c_StartTime)
{}

Timer::~Timer() noexcept
{}

//*************************************************************************************
// Reset
//*************************************************************************************

void Timer::Reset() noexcept
{
    c_StartTime = HighResClock::now();
}

//*************************************************************************************
// Getters
//*************************************************************************************

Timer::TimePoint Timer::GetStartTimePoint() noexcept
{
    return c_StartTime;
}

double Timer::GetTimePassedSeconds() noexcept
{
    return std::chrono::duration_cast<TimeSeconds>(HighResClock::now() - c_StartTime).count();
}

double Timer::GetTimePassedMilliseconds() noexcept
{
    return std::chrono::duration_cast<TimeMilliseconds>(HighResClock::now() - c_StartTime).count();
}
