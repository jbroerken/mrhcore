/**
 *  Locale.h
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

#ifndef Locale_h
#define Locale_h

// C / C++

// External

// Project
#include "./ConfigurationException.h"


class Locale
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    Locale();

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_Locale Locale class source.
     */

    Locale(Locale const& c_Locale) = delete;

    /**
     *  Default destructor.
     */

    ~Locale() noexcept;

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get the current locale.
     *
     *  \return A string containing the locale.
     */

    std::string GetLocale() const noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::string s_Active;
    
protected:

};

#endif /* Locale_h */
