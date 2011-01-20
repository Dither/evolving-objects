/*
(c) 2010 Thales group

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; version 2
    of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

Contact: http://eodev.sourceforge.net

Authors:
Johann Dréo <johann.dreo@thalesgroup.com>
*/

#ifndef _FeasibleRatioStat_h_
#define _FeasibleRatioStat_h_

#include <algorithm>

#include <DualFitness.h>
#include <utils/Logger.h>

#include "Stat.h"

namespace eo
{

    /** Ratio of the number of individuals with a feasible dual fitness in the population (@see DualFitness)
     *
     * @ingroup Stats
     */
    template<class EOT>
    class FeasibleRatioStat : public Stat< EOT, double >
    {
    public:
	using Stat<EOT, double>::value;

	FeasibleRatioStat( std::string description = "FeasibleRatio" ) : Stat<EOT,double>( 0.0, description ) {}

	virtual void operator()( const Pop<EOT> & pop )
	{
#ifndef NDEBUG
	    assert( pop.size() > 0 );

	    double count = static_cast<double>( std::count_if( pop.begin(), pop.end(), IsFeasible<EOT> ) );
	    double size = static_cast<double>( pop.size() );
	    double ratio = count/size;
	    eo::log << eo::xdebug << "FeasibleRatioStat: " << count << " / " << size << " = " << ratio << std::endl;
	    value() = ratio;
#else
	    value() = static_cast<double>( std::count_if( pop.begin(), pop.end(), IsFeasible<EOT> ) ) / static_cast<double>( pop.size() );
#endif
	}

	virtual std::string className(void) const { return "FeasibleRatioStat"; }
    };

}

#endif // _FeasibleRatioStat_h_
