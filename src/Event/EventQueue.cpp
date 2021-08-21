/**
 *  EventQueue.cpp
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
#include <cstring>
#include <new>

// External

// Project
#include "./EventQueue.h"
#include "../Logger/EventLogger.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

EventQueue::EventQueue(TransmissionSource::SourceType e_Type) : p_Queue { [0 ... QueueType::QUEUE_MAX] = e_Type } // All queues should use param e_Type
{}

EventQueue::~EventQueue() noexcept
{}

EventQueue::Queue::Queue(TransmissionSource::SourceType e_Type) : p_Source(NULL),
                                                                  e_Data(Queue::TransmissionData::FINISHED),
                                                                  u32_BytesProcessed(0),
                                                                  u32_Type(MRH_EVENT_UNK),
                                                                  u32_DataSize(0)
{
    // Create source
    try
    {
        switch (e_Type)
        {
#ifdef __MRH_MRHCKM_SUPPORTED__
            case TransmissionSource::SourceType::MRHCKM:
                p_Source = new SourceMRHCKM();
                break;
#endif
            
            default:
                p_Source = new SourcePipe();
                break;
        }
    }
    catch (std::exception& e)
    {
        throw EventException("Failed to construct source: " + std::string(e.what()));
    }
}

EventQueue::Queue::~Queue() noexcept
{
    if (p_Source != NULL)
    {
        delete p_Source;
    }
}

//*************************************************************************************
// Reset
//*************************************************************************************

void EventQueue::Queue::Reset(QueueType e_Queue)
{
    // Reset current transmission
    u32_BytesProcessed = 0;
    u32_Type = MRH_EVENT_UNK;
    u32_DataSize = 0;
    v_Data.clear();
    
    // Set initial state
    switch (e_Queue)
    {
        // Read starts waiting for the first element
        case C_W_P_R:
            e_Data = Queue::TransmissionData::GROUP_ID;
            break;
            
        default:
            e_Data = Queue::TransmissionData::FINISHED;
            break;
    }
    
    // Reset source
    try
    {
        p_Source->Reset();
    }
    catch (EventException& e)
    {
        throw;
    }
}

void EventQueue::Reset()
{
    size_t i = 0;
    
    try
    {
        // Reset all queues completely
        for (; i < QUEUE_COUNT; ++i)
        {
            p_Queue[i].Reset(static_cast<EventQueue::QueueType>(i));
        }
    }
    catch (EventException& e)
    {
        throw EventException("Failed to reset event queue " + std::to_string(i) + ": " + e.what2());
    }
}

//*************************************************************************************
// Update
//*************************************************************************************

bool EventQueue::Queue::CheckNextData(TransmissionData e_NextData, MRH_Uint32 u32_Required) noexcept
{
    if (u32_BytesProcessed >= u32_Required)
    {
        u32_BytesProcessed = 0;
        e_Data = e_NextData;
        
        return true;
    }
    
    return false;
}

//*************************************************************************************
// Recieve
//*************************************************************************************

void EventQueue::Queue::Read(MRH_Uint8* p_Data, MRH_Uint32 u32_Length) noexcept
{
    ssize_t ss_Read;
    
    if ((ss_Read = p_Source->Read(&(p_Data[u32_BytesProcessed]), u32_Length - u32_BytesProcessed)) > 0)
    {
        u32_BytesProcessed += ss_Read;
    }
}

EventQueue::Queue::TransmissionState EventQueue::Queue::RecieveEvent(size_t const& us_ReserveStep) noexcept
{
    bool b_NextData;
    
    switch (e_Data)
    {
        case TransmissionData::GROUP_ID:
            Read((MRH_Uint8*)(&u32_GroupID), sizeof(u32_GroupID));
            b_NextData = CheckNextData(TransmissionData::TYPE, sizeof(u32_GroupID));
            break;
            
        case TransmissionData::TYPE:
            Read((MRH_Uint8*)(&u32_Type), sizeof(u32_Type));
            b_NextData = CheckNextData(TransmissionData::DATA_SIZE, sizeof(u32_Type));
            break;
            
        case TransmissionData::DATA_SIZE:
            Read((MRH_Uint8*)(&u32_DataSize), sizeof(u32_DataSize));
            
            // Check result manually since we need to know the datasize value for the next state
            if (u32_BytesProcessed < sizeof(u32_DataSize))
            {
                b_NextData = false;
                break;
            }
            
            // Add required data buffer space and set state
            if (v_Data.size() < u32_DataSize)
            {
                v_Data.insert(v_Data.end(), u32_DataSize - v_Data.size(), 0);
            }
            b_NextData = CheckNextData((u32_DataSize == 0 ? TransmissionData::FINISHED : TransmissionData::DATA), sizeof(u32_DataSize));
            break;
            
        case TransmissionData::DATA:
            Read(v_Data.data(), u32_DataSize);
            b_NextData = CheckNextData(TransmissionData::FINISHED, u32_DataSize);
            break;
            
        case TransmissionData::FINISHED:
            // Grow if needed
            if (v_Queue.size() == v_Queue.capacity())
            {
                v_Queue.reserve(v_Queue.capacity() + us_ReserveStep);
            }
            
            // Event constructor only uses the buffer up to u32_DataSize,
            // allowing us to skip removing old garbage data
            v_Queue.emplace_back(u32_GroupID,
                                 u32_Type,
                                 v_Data.data(),
                                 u32_DataSize);
            e_Data = TransmissionData::GROUP_ID;
            b_NextData = true;
            break;
            
        default:
            b_NextData = false;
            break;
    }
    
    // State change failed or other issues
    if (b_NextData == false)
    {
        return TransmissionState::FAILED;
    }
    
    // Did we just reach finished? We recieved a complete event if so
    return (e_Data == TransmissionData::FINISHED ? TransmissionState::COMPLETED : TransmissionState::CONTINUE);
}

#if MRH_CORE_EVENT_LOGGING > 0
void EventQueue::LogRecievedEvents(Event const& c_Event) noexcept
{
    EventLogger::Singleton().Log(c_Event, "Default event queue recieved logger.");
}
#endif

void EventQueue::RecieveEvents(MRH_Uint32 u32_EventLimit, MRH_Sint32 s32_TimeoutMS) noexcept
{
    // Clear last recieved
    p_Queue[C_W_P_R].v_Queue.clear();
    
    // Recieve until read fails or limit reached
    if (p_Queue[C_W_P_R].p_Source->CanRead(s32_TimeoutMS) == true)
    {
        MRH_Uint32 u32_Recieved = 0;
        size_t us_ReserveStep = u32_EventLimit; // EventLimit 0 -> No loop -> not required to check step = 0
        
        while (u32_Recieved < u32_EventLimit)
        {
            switch (p_Queue[C_W_P_R].RecieveEvent(us_ReserveStep))
            {
                case Queue::TransmissionState::COMPLETED: // Add recieved count for limit
#if MRH_CORE_EVENT_LOGGING > 0
                    LogRecievedEvents(p_Queue[C_W_P_R].GetLastProccessedEvent());
#endif
                    ++u32_Recieved;
                case Queue::TransmissionState::CONTINUE: // Keep loop alive
                    break;
                    
                default: // Failure, stop loop
                    return;
            }
        }
    }
}

std::vector<Event>& EventQueue::RetrieveEvents() noexcept
{
    // Return the current queue containing all read events
    return p_Queue[C_W_P_R].v_Queue;
}

//*************************************************************************************
// Send
//*************************************************************************************

void EventQueue::Queue::Write(const MRH_Uint8* p_Data, MRH_Uint32 u32_Length) noexcept
{
    ssize_t ss_Write;
    
    if ((ss_Write = p_Source->Write(&(p_Data[u32_BytesProcessed]), u32_Length - u32_BytesProcessed)) > 0)
    {
        u32_BytesProcessed += ss_Write;
    }
}

EventQueue::Queue::TransmissionState EventQueue::Queue::SendEvent() noexcept
{
    bool b_NextData;
    
    switch (e_Data)
    {
        case TransmissionData::GROUP_ID:
            Write((const MRH_Uint8*)(&u32_GroupID), sizeof(u32_GroupID));
            b_NextData = CheckNextData(TransmissionData::TYPE, sizeof(u32_GroupID));
            break;
            
        case TransmissionData::TYPE:
            Write((const MRH_Uint8*)(&u32_Type), sizeof(u32_Type));
            b_NextData = CheckNextData(TransmissionData::DATA_SIZE, sizeof(u32_Type));
            break;
                
        case TransmissionData::DATA_SIZE:
            Write((const MRH_Uint8*)(&u32_DataSize), sizeof(u32_DataSize));
            b_NextData = CheckNextData((u32_DataSize == 0 ? TransmissionData::FINISHED : TransmissionData::DATA), sizeof(u32_DataSize));
            break;
                    
        case TransmissionData::DATA:
            Write(v_Data.data(), u32_DataSize);
            b_NextData = CheckNextData(TransmissionData::FINISHED, u32_DataSize);
            break;
            
        case TransmissionData::FINISHED:
            if (v_Queue.size() > 0)
            {
                u32_GroupID = v_Queue[0].GetGroupID();
                u32_Type = v_Queue[0].GetType();
                
                // Skip data copy on empty
                if ((u32_DataSize = v_Queue[0].GetDataSize()) > 0)
                {
                    // Only add missing space, all old stuff is overwritten anyway
                    // The buffer is only written up to DataSize, skipping old garbage data
                    if (v_Data.size() < u32_DataSize)
                    {
                        v_Data.insert(v_Data.end(), u32_DataSize - v_Data.size(), 0);
                    }
                    
                    std::memcpy(v_Data.data(), v_Queue[0].GetData(), u32_DataSize);
                }
                
                v_Queue.erase(v_Queue.begin());
            
                e_Data = TransmissionData::GROUP_ID;
                b_NextData = true;
            }
            else
            {
                b_NextData = false;
            }
            break;
            
        default:
            b_NextData = false;
            break;
    }
    
    // State change failed or other issues
    if (b_NextData == false)
    {
        return TransmissionState::FAILED;
    }
    
    // Did we just reach finished? We recieved a complete event if so
    return (e_Data == TransmissionData::FINISHED ? TransmissionState::COMPLETED : TransmissionState::CONTINUE);
}

#if MRH_CORE_EVENT_LOGGING > 0
void EventQueue::LogSentEvents(Event const& c_Event) noexcept
{
    EventLogger::Singleton().Log(c_Event, "Default event queue sent logger.");
}
#endif

void EventQueue::AddSendEvents(std::vector<Event>& v_Event) noexcept
{
    if (v_Event.size() == 0)
    {
        return;
    }
    
    // Move given events to our send queue
    std::move(v_Event.begin(), v_Event.end(), std::back_inserter(p_Queue[P_W_C_R].v_Queue));
    v_Event.clear();
}

void EventQueue::SendEvents(MRH_Uint32 u32_EventLimit) noexcept
{
    // Send until write fails or limit reached
    MRH_Uint32 u32_Sent = 0;
    
    while (u32_Sent < u32_EventLimit)
    {
        switch (p_Queue[P_W_C_R].SendEvent())
        {
            case Queue::TransmissionState::COMPLETED: // Add send count for limit
#if MRH_CORE_EVENT_LOGGING > 0
                LogSentEvents(Event(p_Queue[P_W_C_R].GetLastProccessedEvent()));
#endif
                ++u32_Sent;
            case Queue::TransmissionState::CONTINUE: // Keep loop alive
                break;
                
            default: // Failure, stop loop
                return;
        }
    }
}

void EventQueue::SendEvents(std::vector<Event>& v_Event, MRH_Uint32 u32_EventLimit) noexcept
{
    AddSendEvents(v_Event);
    SendEvents(u32_EventLimit);
}

//*************************************************************************************
// Getters
//*************************************************************************************

#if MRH_CORE_EVENT_LOGGING > 0
Event EventQueue::Queue::GetLastProccessedEvent() noexcept
{
    return Event(u32_GroupID,
                 u32_Type,
                 v_Data.data(),
                 u32_DataSize);
}
#endif

int EventQueue::GetPipeFD(QueueType e_Queue, SourcePipe::PipeEnd e_End) const
{
    if (e_Queue < QueueType::QUEUE_COUNT)
    {
        if (p_Queue[e_Queue].p_Source->GetType() != TransmissionSource::PIPE)
        {
            throw EventException("Event queue source is not a pipe!");
        }
        
        try
        {
            return p_Queue[e_Queue].GetSource<SourcePipe*>()->GetFD(e_End);
        }
        catch (EventException& e)
        {
            throw;
        }
    }
    
    throw EventException("Invalid event queue pipe end requested!");
}
