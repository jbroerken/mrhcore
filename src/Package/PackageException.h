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

#ifndef PackageException_h
#define PackageException_h

// C / C++
#include <string>
#include <exception>

// External

// Project


class PackageException : public std::exception
{
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  std::string constructor.
     *
     *  \param s_Message The error message.
     *  \param s_PackageInfo The package info this exception is for.
     */
    
    PackageException(std::string s_Message,
                     std::string s_PackageInfo)
    {
        this->s_Message = s_Message;
        this->s_PackageInfo = s_PackageInfo;
    }
    
    /**
     *  const char* constructor.
     *
     *  \param p_Message The error message.
     *  \param p_PackageInfo The package info this exception is for.
     */
    
    PackageException(const char* p_Message,
                     const char* p_PackageInfo)
    {
        this->s_Message = std::string(p_Message);
        this->s_PackageInfo = std::string(p_PackageInfo);
    }
    
    /**
     *  Default destructor.
     */
    
    ~PackageException()
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
     *  Get the package info for this exception.
     *
     *  \return A const char* containing package info.
     */
    
    const char* package() const throw()
    {
        return s_PackageInfo.c_str();
    }
    
    /**
     *  Get the package info for this exception.
     *
     *  \return A std::string string containing package info.
     */
    
    std::string package2() const throw()
    {
        return s_PackageInfo;
    }
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    std::string s_Message;
    std::string s_PackageInfo;
    
protected:
    
};

#endif /* PackageException_h */
