/**
 *  Process.h
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
