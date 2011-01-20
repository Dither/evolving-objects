//-----------------------------------------------------------------------------
// Gnuplot1DMonitor.h
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
             mkeijzer@dhi.dk
 */
//-----------------------------------------------------------------------------
#ifndef Gnuplot1DMonitor_H
#define Gnuplot1DMonitor_H

#include <fstream>
#include <string>

#include "Object.h"
#include "utils/FileMonitor.h"
#include "utils/Gnuplot.h"
#include "utils/pipecom.h"

namespace eo
{

/** Plot Stat

    @author Marc Schoenauer
    @version 0.0 (2000)

    This class plots through gnuplot the Stat given as argument

    Gnuplot1DMonitor plots stats through gnuplot

    Assumes that the same file is appened every so and so, and replots it
    everytime

    @ingroup Monitors
*/
    class Gnuplot1DMonitor : public FileMonitor, public Gnuplot
    {
    public:

	// this "using" directive generates a compiler internal error in GCC 4.0.0 ...
	// it's been removed, and the only call to vec was replaced by this->vec in Gnuplot1DMonitor.cpp
	//    using Monitor::vec;

	/** Constructor */
	Gnuplot1DMonitor(std::string _filename, bool _top=false) :
	    FileMonitor(_filename, " "),
	    Gnuplot(_filename,(_top?"":"set key bottom"))
	    {}

	/** Destructor */
	virtual ~Gnuplot1DMonitor(){}

	virtual Monitor& operator()();

	virtual void FirstPlot();

	/** Class name */
	virtual std::string className() const
	    { return "Gnuplot1DMonitor"; }
    };

}

#endif // Gnuplot1DMonitor_H

// Local Variables:
// c-file-style: "Stroustrup"
// comment-column: 35
// fill-column: 80
// mode: C++
// End:
