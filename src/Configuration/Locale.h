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
