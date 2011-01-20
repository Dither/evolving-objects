/*
(c) Marc Schoenauer, Maarten Keijzer and GeNeura Team, 2000
(c) Thales group, 2010

    This library is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free
    Software Foundation; version 2 of the license.

    This library is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
    PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along
    with this library; if not, write to the Free Software Foundation, Inc., 59
    Temple Place, Suite 330, Boston, MA 02111-1307 USA

Contact: http://eodev.sourceforge.net

Authors:
	todos@geneura.ugr.es
	Marc.Schoenauer@polytechnique.fr
	mkeijzer@dhi.dk
	Johann Dréo <johann.dreo@thalesgroup.com>
*/

#ifndef _OStreamMonitor_h_
#define _OStreamMonitor_h_

#include <string>
#include <iostream>

#include <utils/Monitor.h>
#include <utils/Logger.h>
#include <Object.h>

namespace eo
{

    /**
       Prints statistics to a given ostream.

       You can pass any instance of an ostream to the constructor, like, for example, std::clog.

       @ingroup Monitors
    */
    class OStreamMonitor : public Monitor
    {
    public :
	OStreamMonitor( std::ostream & _out, bool _verbose=true, std::string _delim = "\t", unsigned int _width=20, char _fill=' ' ) : 
	    out(_out), delim(_delim), width(_width), fill(_fill), firsttime(true) 
	{
	    log << warnings << "WARNING: the use of the verbose parameter in OStreamMonitor constructor is deprecated and will be removed in the next release" << std::endl;
	}

	OStreamMonitor( std::ostream & _out, std::string _delim = "\t", unsigned int _width=20, char _fill=' ' ) : 
	    out(_out), delim(_delim), width(_width), fill(_fill), firsttime(true) 
	{}

	Monitor& operator()(void);

	virtual std::string className(void) const { return "OStreamMonitor"; }

    private :
	std::ostream & out;
	std::string delim;
	unsigned int width;
	char fill;
	bool firsttime;
    };

}

#endif // _OStreamMonitor_h_
