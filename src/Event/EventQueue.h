/**
 *  EventQueue.h
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

#ifndef EventQueue_h
#define EventQueue_h

// C / C++

// External

// Project
#ifdef __MRH_MRHCKM_SUPPORTED__
#include "./Source/SourceMRHCKM.h"
#endif
#include "./Source/SourcePipe.h"
#include "./Event.h"

// Pre-defined
#ifndef MRH_CORE_EVENT_LOGGING
    #define MRH_CORE_EVENT_LOGGING 0
#endif


class EventQueue
{
public:

    //*************************************************************************************
    // Types
    //*************************************************************************************

    typedef enum
    {
        P_W_C_R = 0, // Written by parent, read by child: Events written by mrhcore
        C_W_P_R = 1, // Written by child, read by parent: Events read by mrhcore

        QUEUE_MAX = 1,

        QUEUE_COUNT = 2

    }QueueType;

private:
    
    //*************************************************************************************
    // Queue
    //*************************************************************************************
    
    class Queue
    {
    public:
    
        //*************************************************************************************
        // Types
        //*************************************************************************************
    
        typedef enum
        {
            FAILED = 0,
            COMPLETED = 1,
            CONTINUE = 2,
            
            TRANSMISSION_STATE_MAX = CONTINUE,
        
            TRANSMISSION_STATE_COUNT = TRANSMISSION_STATE_MAX + 1
        
        }TransmissionState;
        
        //*************************************************************************************
        // Constructor / Destructor
        //*************************************************************************************

        /**
         *  Default constructor.
         *
         *  \param e_Type The transmission source type.
         */

        Queue(TransmissionSource::SourceType e_Type);

        /**
         *  Default destructor.
         */

        ~Queue() noexcept;
        
        //*************************************************************************************
        // Reset
        //*************************************************************************************
        
        /**
         *  Reset queue.
         *
         *  \param e_Queue Queue type.
         */
        
        void Reset(QueueType e_Queue);
        
        //*************************************************************************************
        // Recieve
        //*************************************************************************************
        
        /**
         *  Recieve the current event.
         *
         *  \param us_ReserveStep The reserve step size for growing the event vector.
         *
         *  \return The current queue update state.
         */
        
        TransmissionState RecieveEvent(size_t const& us_ReserveStep) noexcept;
        
        //*************************************************************************************
        // Send
        //**************************************************************************************
        
        /**
         *  Send the current event.
         *
         *  \return The current queue update state.
         */
        
        TransmissionState SendEvent() noexcept;
        
        //*************************************************************************************
        // Getters
        //*************************************************************************************
        
        /**
         *  Return the last worked on event.
         *
         *  \return The last worked on event.
         */
        
#if MRH_CORE_EVENT_LOGGING > 0
        Event GetLastProccessedEvent() noexcept;
#endif
        
        /**
         *  Get the queue source.
         *
         *  \return The queue source.
         */
        
        template <typename T> T GetSource() const noexcept
        {
            return static_cast<T>(p_Source);
        }
        
        //*************************************************************************************
        // Data
        //*************************************************************************************
        
        // Source
        TransmissionSource* p_Source;
        
        // Event
        std::vector<Event> v_Queue;
        
    private:
        
        //*************************************************************************************
        // Types
        //*************************************************************************************
        
        typedef enum
        {
            GROUP_ID = 0,
            TYPE = 1,
            DATA_SIZE = 2,
            DATA = 3,
            FINISHED = 4,
            
            TRANSMISSION_DATA_MAX = FINISHED,
            
            TRANSMISSION_DATA_COUNT = TRANSMISSION_DATA_MAX + 1
            
        }TransmissionData;
        
        //*************************************************************************************
        // Update
        //*************************************************************************************
        
        /**
         *  Check and update the transmission state.
         *
         *  \param e_NextData The next transmission data.
         *  \param u32_Required The required processed byte size.
         *
         *  \return true if the state was updated, false if not.
         */
        
        bool CheckNextData(TransmissionData e_NextData, MRH_Uint32 u32_Required) noexcept;
        
        //*************************************************************************************
        // Recieve
        //*************************************************************************************
        
        /**
         *  Read data from source.
         *
         *  \param p_Data The buffer to read to.
         *  \param u32_Length The length to read.
         */
        
        inline void Read(MRH_Uint8* p_Data, MRH_Uint32 u32_Length) noexcept;
        
        //*************************************************************************************
        // Send
        //*************************************************************************************
        
        /**
         *  Write data to source.
         *
         *  \param p_Data The buffer to write from.
         *  \param u32_Length The length to write.
         */
        
        inline void Write(const MRH_Uint8* p_Data, MRH_Uint32 u32_Length) noexcept;
        
        //*************************************************************************************
        // Data
        //*************************************************************************************
        
        TransmissionData e_Data;
        MRH_Uint32 u32_BytesProcessed;
        
        MRH_Uint32 u32_GroupID;
        MRH_Uint32 u32_Type;
        MRH_Uint32 u32_DataSize;
        std::vector<MRH_Uint8> v_Data;
        
    protected:
        
    };
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

    Queue p_Queue[QUEUE_COUNT];

protected:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param e_Type The transmission source type.
     */

    EventQueue(TransmissionSource::SourceType e_Type);
    
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_EventQueue EventQueue class source.
     */

    EventQueue(EventQueue const& c_EventQueue) = delete;

    /**
     *  Default destructor.
     */

    ~EventQueue() noexcept;

    //*************************************************************************************
    // Reset
    //*************************************************************************************

    /**
     *  Reset event queue.
     */

    void Reset();
    
    //*************************************************************************************
    // Recieve
    //*************************************************************************************

    /**
     *  Log recieved events. This function is called for each C_W_P_R event.
     *
     *  \param c_Event The event to log.
     */
    
#if MRH_CORE_EVENT_LOGGING > 0
    virtual void LogRecievedEvents(Event const& c_Event) noexcept;
#endif
    
    /**
     *  Recieve events. The events are read from C_W_P_R.
     *
     *  \param u32_EventLimit The max amount of event to be sent / recieved in a update.
     *  \param s32_TimeoutMS The recieve timeout in milliseconds.
     */

    void RecieveEvents(MRH_Uint32 u32_EventLimit, MRH_Sint32 s32_TimeoutMS) noexcept;
    
    /**
     *  Retrieve recieved events.
     *
     *  \return A vector containing all read events.
     */
    
    std::vector<Event>& RetrieveEvents() noexcept;

    //*************************************************************************************
    // Send
    //*************************************************************************************
    
    /**
     *  Log sent events. This function is called for each P_W_C_R event.
     *
     *  \param c_Event The event to log.
     */
    
#if MRH_CORE_EVENT_LOGGING > 0
    virtual void LogSentEvents(Event const& c_Event) noexcept;
#endif
    
    /**
     *  Add new events to send.
     *
     *  \param v_Event The events to send. The events will be moved an the vector cleared.
     */
    
    void AddSendEvents(std::vector<Event>& v_Event) noexcept;
    
    /**
     *  Send remaining events. The events are written to P_W_C_R.
     *
     *  \param u32_EventLimit The max amount of event to be sent / recieved in a update.
     */
    
    void SendEvents(MRH_Uint32 u32_EventLimit) noexcept;
    
    /**
     *  Send new and remaining events. The events are written to P_W_C_R.
     *
     *  \param v_Event The events to send. The events will be moved an the vector cleared.
     *  \param u32_EventLimit The max amount of event to be sent / recieved in a update.
     */

    void SendEvents(std::vector<Event>& v_Event, MRH_Uint32 u32_EventLimit) noexcept;

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get an event queue pipe end file descriptor.
     *
     *  \param e_Queue The queue content request.
     *  \param e_End The pipe end requested.
     *
     *  \return The event queue pipe end file descriptor.
     */
    
    int GetPipeFD(QueueType e_Queue, SourcePipe::PipeEnd e_End) const;
};

#endif /* EventQueue_h */
