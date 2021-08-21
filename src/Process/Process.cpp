/**
 *  Process.cpp
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
#include <sys/types.h>
#include <sys/wait.h>
#include <csignal>
#include <cerrno>
#include <cstring>
#include <new>

// External

// Project
#include "./Process.h"
#include "../Logger/Logger.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

Process::Process() noexcept
{
    s32_ProcessID = -1;
}

Process::~Process() noexcept
{
    // Force kill, either caused by SIGTERM or when closing the launcher
    Stop(true);
}

//*************************************************************************************
// Run
//*************************************************************************************

void Process::Run(std::string s_BinaryPath, std::vector<std::vector<char>> v_Arg)
{
    // Check
    if (GetRunning() == true)
    {
        throw ProcessException("Tried to start a process with another process of this type already running!");
    }
    
    // Fork the process
    s32_ProcessID = fork();
    
    // Check results
    if (s32_ProcessID < 0)
    {
        // Failed
        throw ProcessException("Failed to fork process: " + std::string(std::strerror(errno)) + " (" + std::to_string(errno) + ")!");
    }
    else if (s32_ProcessID > 0)
    {
        // Parent, nothing left to do
        return;
    }
    else
    {
        // Log
        Logger::Singleton().Log(Logger::INFO, "Starting process " +
                                              s_BinaryPath +
                                              ":",
                                "Process.cpp", __LINE__);
        
        // Add binary path
        v_Arg.insert(v_Arg.begin(), GetArgumentBytes(s_BinaryPath));
        
        // Create argument list
        char* p_Arg[v_Arg.size() + 1];
        
        for (size_t i = 0; i < v_Arg.size(); ++i)
        {
            // Copy
            p_Arg[i] = new char[v_Arg[i].size() + 1];
            std::memcpy(p_Arg[i], &(v_Arg[i][0]), v_Arg[i].size());
            p_Arg[i][v_Arg[i].size()] = '\0';
            
            // Log
            Logger::Singleton().Log(Logger::INFO, "argv[" +
                                                  std::to_string(i) +
                                                  "]: " +
                                                  std::string(p_Arg[i]),
                                    "Process.cpp", __LINE__);
        }
        
        p_Arg[v_Arg.size()] = (char*)NULL;
        
        // Child, Start launcher
        if (execv(s_BinaryPath.c_str(), p_Arg) < 0)
        {
            Logger::Singleton().Log(Logger::ERROR, "Run failed: " +
                                                   std::string(std::strerror(errno)) +
                                                   " (" + std::to_string(errno) +
                                                   ")!",
                                    "Process.cpp", __LINE__);
            exit(EXIT_FAILURE); // Child exit
        }
    }
}

//*************************************************************************************
// Signal
//*************************************************************************************

void Process::Signal(int i_Signal) noexcept
{
    // Running?
    if (GetRunning() == false)
    {
        return;
    }
    
    // Send signal to child
    kill(s32_ProcessID, i_Signal);
    
    switch (i_Signal)
    {
        case SIGTERM:
            //wait(NULL);
            break;
            
        default:
            break;
    }
}

//*************************************************************************************
// Stop
//*************************************************************************************

void Process::Stop(bool b_Force) noexcept
{
    // Signal end to child
    if (GetRunning() == false)
    {
        return;
    }
    
    Logger::Singleton().Log(Logger::INFO, "Terminating currently running process" +
                                          std::string(b_Force ? " (forced)." : "."),
                            "Process.cpp", __LINE__);
    
    Signal(b_Force ? SIGKILL : SIGTERM);
}

//*************************************************************************************
// Getters
//*************************************************************************************

void Process::GetProcessState(bool& b_Running, int& i_Result) noexcept
{
    if (s32_ProcessID > -1)
    {
        // Get process status
        int i_Status;
        pid_t s32_Result = waitpid(s32_ProcessID, &i_Status, WNOHANG);
        
        // Check result of wait
        if (s32_Result < 0)
        {
            // Error
            s32_ProcessID = -1;
            Logger::Singleton().Log(Logger::WARNING, "Could not get child process status!", "Process.cpp", __LINE__);
            
            b_Running = false;
            i_Result = -1;
        }
        else if (s32_Result == s32_ProcessID)
        {
            // Normal termination
            s32_ProcessID = -1;
            
            b_Running = false;
        }
        else
        {
            // Still running
            b_Running = true;
            i_Result = -1;
        }
        
        // Get status of process
        if (WIFEXITED(i_Status))
        {
            i_Result = WEXITSTATUS(i_Status);
        }
    }
    else
    {
        b_Running = false;
        i_Result = -1;
    }
}

bool Process::GetRunning() noexcept
{
    bool b_Running;
    int i_Result;
    
    GetProcessState(b_Running, i_Result);
    
    return b_Running;
}

int Process::GetResult() noexcept
{
    bool b_Running = false; // Silence warning
    int i_Result = -1; // Same thing
    
    GetProcessState(b_Running, i_Result);
    
    return i_Result;
}

pid_t Process::GetProcessID() const noexcept
{
    return s32_ProcessID;
}

std::vector<char> Process::GetArgumentBytes(std::string s_String) noexcept
{
    if (s_String.length() == 0)
    {
        return std::vector<char>();
    }
    
    return std::vector<char>(s_String.c_str(), s_String.c_str() + s_String.length());
}
