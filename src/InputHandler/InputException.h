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

#ifndef InputException_h
#define InputException_h

// C / C++
#include <string>
#include <exception>

// External

// Project


class InputException : public std::exception
{
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  std::string constructor.
     *
     *  \param s_Message The error message.
     *  \param s_Module The origin module.
     */
    
    InputException(std::string s_Message,
                   std::string s_Module)
    {
        this->s_Message = s_Message;
        this->s_Module = s_Module;
    }
    
    /**
     *  const char* constructor.
     *
     *  \param p_Message The error message.
     *  \param p_Module The origin module.
     */
    
    InputException(const char* p_Message,
                   const char* p_Module)
    {
        this->s_Message = std::string(p_Message);
        this->s_Module = std::string(p_Module);
    }
    
    /**
     *  Default destructor.
     */
    
    ~InputException()
    {}
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Get the exception string.
     *
     *  \return A const char* string with the error message.
     */
    
    const char* what() const throw()
    {
        return s_Message.c_str();
    }
    
    /**
     *  Get the exception string.
     *
     *  \return A std::string string with the error message.
     */
    
    std::string what2() const throw()
    {
        return s_Message;
    }
    
    /**
     *  Get the module string.
     *
     *  \return A const char* string with the module source.
     */
    
    const char* module() const throw()
    {
        return s_Module.c_str();
    }
    
    /**
     *  Get the module string.
     *
     *  \return A std::string string with the module source.
     */
    
    std::string module2() const throw()
    {
        return s_Module;
    }
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    std::string s_Message;
    std::string s_Module;
    
protected:
    
};

#endif /* InputException_h */
