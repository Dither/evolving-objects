// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// FDCStat.h
// (c) Marc Schoenauer, Maarten Keijzer and GeNeura Team, 2000, 2001
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

#ifndef _FDCStat_h
#define _FDCStat_h

#include <utils/Stat.h>
#include <utils/Distance.h>
#include <utils/FileSnapshot.h>

namespace eo
{

    /**
       The Fitness Distance Correlation computation.
    
       Stores the values into ValueParam<EOT,double>
       so they can be snapshot by some GnuplotSnapshot ...

       @ingroup Stats
    */
    template <class EOT>
    class FDCStat : public Stat<EOT, double>
    {
    public:

	using Stat<EOT, double>::value;

	/** Ctor without the optimum */
	FDCStat(Distance<EOT> & _dist, std::string _description = "FDC") :
	    Stat<EOT,double>(0, _description), dist(_dist), boolOpt(false) {}

	/** Ctor with the optimum
	 */
	FDCStat(Distance<EOT> & _dist, EOT & _theBest,
		  std::string _description = "FDC") :
	    Stat<EOT,double>(0, _description), dist(_dist),
	    theBest(_theBest), boolOpt(true) {}

	/** Compute the FDC - either from best in pop, or from absolute best
	 *  if it was passed in the constructor
	 */
	virtual void operator()(const Pop<EOT>& _pop)
	{
	    unsigned i;
	    if (!boolOpt)		   // take the local best
		theBest = _pop.best_element();
	    unsigned int pSize = _pop.size();
	    distToBest.value().resize(pSize);
	    fitnesses.value().resize(pSize);
	    double sumFit = 0.0, sumDist = 0.0;
	    for (i=0; i<pSize; i++)
		{
		    sumDist += (distToBest.value()[i] = dist(_pop[i], theBest));
		    sumFit += (fitnesses.value()[i] = _pop[i].fitness());
		}
	    // now the FDC coefficient
	    double avgDist = sumDist/pSize;
	    double avgFit = sumFit/pSize;
	    sumDist = sumFit = 0.0;
	    double num = 0.0;
	    for (i=0; i<pSize; i++)
		{
		    double devDist = distToBest.value()[i] - avgDist ;
		    double devFit = fitnesses.value()[i] - avgFit ;
		    sumDist += devDist*devDist;
		    sumFit += devFit * devFit;
		    num += devDist * devFit ;
		}
	    value() = num/(sqrt(sumDist)*sqrt(sumFit));
	}

	/** accessors to the private ValueParam<std::vector<double> >
	 */
	const ValueParam<std::vector<double> > & theDist()
	{ return distToBest; }
	const ValueParam<std::vector<double> > & theFit()
	{ return fitnesses; }


    private:
	Distance<EOT> & dist;
	EOT theBest;
	bool boolOpt;			   // whether the best is known or not
	ValueParam<std::vector<double> > distToBest;
	ValueParam<std::vector<double> > fitnesses;
    };

    /** Specific class for FDCStat monitoring:
     *  As I failed to have FDC stat as an Stat, this is the trick
     *  to put the 2 Param<std::vector<double> > into a monitor
     *  This class does nothing else.

     @ingroup Stats
    */
    template <class EOT>
    class FDCFileSnapshot : public FileSnapshot	// is an Monitor
    {
    public:
	/** Ctor: in addition to the parameters of the ctor of an FileSnapshot
            we need here an FDCStat. The 2 std::vectors (distances to optimum
	    and fitnesses) are added to the monitor so they can be processed
            later to a file - and eventually by gnuplot
	*/
	FDCFileSnapshot(FDCStat<EOT> & _FDCstat,
			  std::string _dirname = "tmpFDC", unsigned _frequency = 1,
			  std::string _filename = "FDC", std::string _delim = " "):
	    FileSnapshot(_dirname, _frequency, _filename, _delim),
	    FDCstat(_FDCstat)
	{
	    FileSnapshot::add(FDCstat.theDist());
	    FileSnapshot::add(FDCstat.theFit());
	}

	/** just to be sure the add method is not called further
	 */
	virtual void add(const Param& _param)
	{ throw std::runtime_error("FDCFileSnapshot::add(). Trying to add stats to an FDCFileSnapshot"); }

    private:
	FDCStat<EOT> & FDCstat;
    };

}

#endif
