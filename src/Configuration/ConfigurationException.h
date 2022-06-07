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

#ifndef ConfigurationException_h
#define ConfigurationException_h

// C / C++
#include <string>
#include <exception>

// External

// Project


class ConfigurationException : public std::exception
{
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  std::string constructor.
     *
     *  \param s_Message The error message.
     *  \param s_FilePath The path to the configuration file.
     */
    
    ConfigurationException(std::string s_Message,
                           std::string s_FilePath)
    {
        this->s_Message = s_Message;
        this->s_FilePath = s_FilePath;
    }
    
    /**
     *  const char* constructor.
     *
     *  \param p_Message The error message.
     *  \param p_FilePath The path to the configuration file.
     */
    
    ConfigurationException(const char* p_Message,
                           const char* p_FilePath)
    {
        this->s_Message = std::string(p_Message);
        this->s_FilePath = std::string(p_FilePath);
    }
    
    /**
     *  Default destructor.
     */
    
    ~ConfigurationException()
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
     *  Get the file path for this exception.
     *
     *  \return A const char* string containing the file path.
     */
    
    const char* filepath() const throw()
    {
        return s_FilePath.c_str();
    }
    
    /**
     *  Get the file path for this exception.
     *
     *  \return A std::string string containing the file path.
     */
    
    std::string filepath2() const throw()
    {
        return s_FilePath;
    }
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    std::string s_Message;
    std::string s_FilePath;
    
protected:
    
};

#endif /* ConfigurationException_h */
