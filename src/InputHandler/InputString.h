/**
 *  InputString.h
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

#ifndef InputString_h
#define InputString_h

// C / C++
#include <map>
#include <list>
#include <string>

// External
#include <libmrhvt/String/MRH_SpeechString.h>

// Project
#include "../Event/Event.h"
#include "./InputException.h"


class InputString
{
public:

    //*************************************************************************************
    // Clear
    //*************************************************************************************

    /**
     *  Clear all finished and unfinished strings.
     */

    void ClearInput() noexcept;

    //*************************************************************************************
    // Add
    //*************************************************************************************

    /**
     *  Add a voice input string.
     *
     *  \param s_String The full UTF-8 input string to add.
     */
    
    void AddVoiceInput(std::string s_String) noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::map<MRH_Uint32, MRH_SpeechString> m_Unfinished; // <String ID, Unfinished>
    
protected:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    InputString() noexcept;

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_InputString InputString class source.
     */

    InputString(InputString const& c_InputString) = delete;

    /**
     *  Default destructor.
     */

    ~InputString() noexcept;
    
    //*************************************************************************************
    // Clear
    //*************************************************************************************
    
    /**
     *  Clear all unfinished input.
     */
    
    void ClearUnfinished() noexcept;
    
    /**
     *  Clear all finished input strings.
     */
    
    void ClearFinished() noexcept;

    //*************************************************************************************
    // Add
    //*************************************************************************************

     /**
      *  Add a voice input string.
      *
      *  \param c_Event The event containing the voice input.
      */
     
     void AddVoiceInput(Event const& c_Event) noexcept;

    //*************************************************************************************
    // Data
    //*************************************************************************************
    
     std::list<std::string> l_Finished;
};

#endif /* InputString_h */
