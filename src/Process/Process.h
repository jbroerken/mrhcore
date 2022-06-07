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

#ifndef Process_h
#define Process_h

// C / C++
#include <unistd.h>
#include <vector>

// External

// Project
#include "./ProcessException.h"


class Process
{
public:
    
    //*************************************************************************************
    // Signal
    //*************************************************************************************
    
    /**
     *  Signal handling.
     *
     *  \param i_Signal The signal to send.
     */
    
    void Signal(int i_Signal) noexcept;
    
    //*************************************************************************************
    // Stop
    //*************************************************************************************
    
    /**
     *  Stop the currently running process.
     */
    
    void Stop(bool b_Force) noexcept;
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Get the current process state.
     *
     *  \param b_Running Wether the process is running or not.
     *  \param i_Result The process exit code.
     */
    
    void GetProcessState(bool& b_Running, int& i_Result) noexcept;
    
    /**
     *  Check if a process is currently running.
     *
     *  \return true if running, false if not.
     */
    
    bool GetRunning() noexcept;
    
    /**
     *  Check the process return result.
     *
     *  \return The process result on success, -1 on failure.
     */
    
    int GetResult() noexcept;
    
    /**
     *  Get the current process id.
     *
     *  \return The process id in use.
     */
    
    pid_t GetProcessID() const noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    pid_t s32_ProcessID;
    
protected:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     */
    
    Process() noexcept;
    
    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_Process Process class source.
     */
    
    Process(Process const& c_Process) = delete;
    
    /**
     *  Default destructor.
     */
    
    virtual ~Process() noexcept;
    
    //*************************************************************************************
    // Run
    //*************************************************************************************
    
    /**
     *  Run a process.
     *
     *  \param s_BinaryPath The binary to start.
     *  \param v_Arg Process argument list. The first argument (binary path) will be added by this function.
     */
    
    void Run(std::string s_BinaryPath, std::vector<std::vector<char>> v_Arg);
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Get a string converted to vector.
     *
     *  \param s_String The string to convert.
     *
     *  \return A vector containing the argument bytes.
     */
    
    std::vector<char> GetArgumentBytes(std::string s_String) noexcept;
};

#endif /* Process_h */
