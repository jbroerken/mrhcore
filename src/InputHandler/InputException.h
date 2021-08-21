/**
 *  InputException.h
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
