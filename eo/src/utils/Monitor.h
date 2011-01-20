// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

// (c) Marc Schoenauer, Maarten Keijzer and GeNeura Team, 2000
/*
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact: todos@geneura.ugr.es, http://geneura.ugr.es
             Marc.Schoenauer@polytechnique.fr
             mak@dhi.dk
 */

#ifndef _Monitor_h
#define _Monitor_h

#include <vector>
#include <iostream>
#include <Functor.h>

namespace eo
{

    /** @defgroup Monitors Monitoring
     *
     * Monitors take a set of value parameters (ValueParam) objects and
     * output them on a given stream, file, pipe, etc.
     *
     * They can be called from within an CheckPoint, to print different values
     * of parameters or of Stat at each generation.
     *
     * @see CheckPoint
     * @see Stat
     * @ingroup Utilities
     */


    class Param;
    template <class EOT> class CheckPoint;

    /**
       The abstract monitor class is a std::vector of parameter pointers. Use
       either push_back a pointer or add a reference to a parameter.
       Derived classes will then implement the operator()(void) which
       will stream or pipe the current values of the parameters to wherever you
       want it streamed or piped to.

       @ingroup Monitors
    */
    class Monitor : public F<Monitor&>
    {
    public :

	virtual void lastCall() {}

	/**
	   Adds a parameter to the monitor. It is virtual so you can do some type checking
	   in derived classes if you must.
	*/
	virtual void add(const Param& _param) { vec.push_back(&_param); }

	virtual std::string className(void) const { return "Monitor"; }

	template <class EOT>
	Monitor& addTo(CheckPoint<EOT>& cp) { cp.add(*this); return *this; }

    protected :
	typedef std::vector<const Param*>::iterator iterator;
	std::vector<const Param*> vec;
    };

}

#endif
