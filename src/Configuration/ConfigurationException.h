/**
 *  ConfigurationException.h
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
