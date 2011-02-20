/*
The Evolving Distribution Objects framework (EDO) is a template-based, 
ANSI-C++ evolutionary computation library which helps you to write your 
own estimation of distribution algorithms.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Copyright (C) 2010 Thales group
*/
/*
Authors:
    Johann Dréo <johann.dreo@thalesgroup.com>
    Caner Candan <caner.candan@thalesgroup.com>
*/

#ifndef _edo_Continue_h
#define _edo_Continue_h

#include <eo/Functor.h>
#include <eo/Persistent.h>

namespace edo
{

    //! Continue< EOT > classe fitted to Distribution Object library

    template < typename D >
    class Continue : public eo::UF< const D&, bool >, public eo::Persistent
    {
    public:
	virtual std::string className(void) const { return "Continue"; }

	void readFrom(std::istream&)
	{
	    /* It should be implemented by subclasses ! */
	}

	void printOn(std::ostream&) const
	{
	    /* It should be implemented by subclasses ! */
	}
    };

    template < typename D >
    class DummyContinue : public Continue< D >
    {
	bool operator()(const D&){ return true; }

	virtual std::string className() const { return "DummyContinue"; }
    };

}

#endif // !_edo_Continue_h
