/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

   -----------------------------------------------------------------------------
   SurviveAndDie.h
   (c) Maarten Keijzer, Marc Schoenauer, GeNeura Team, 2000

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

#ifndef _SurviveAndDie_h
#define _SurviveAndDie_h


//-----------------------------------------------------------------------------
#include <Pop.h>
#include <Functor.h>
#include <Merge.h>
#include <Reduce.h>
#include <utils/HowMany.h>
//-----------------------------------------------------------------------------

namespace eo
{

    /**
       SurviveAndDie: takes a population (first argument),
       kills the ones that are to die,
       puts the ones that are to survive into the second argument
       removes them from the first pop argument

       @class SurviveAndDie
       @class DeterministicSurviveAndDie,
       @class DeterministicSaDReplacement
    */

    /** @addtogroup Replacors
     * @{
     */

    /** SurviveAndDie
	A pure abstract class, to store the howmany's
    */
    template <class EOT>
    class SurviveAndDie : public BF<Pop<EOT> &, Pop<EOT> &, void>
    {
    public:
	SurviveAndDie(double _survive, double _die, bool _interpret_as_rate = true):
	    howmanySurvive(_survive, _interpret_as_rate),
	    howmanyDie(_die, _interpret_as_rate)
	{}

    protected:
	HowMany howmanySurvive;
	HowMany howmanyDie;

    };

    /** An instance (theonly one as of today, Dec. 20, 2000) of an
	SurviveAndDie, that does everything deterministically

	Used in DeterministicSaDReplacement.
    */
    template <class EOT>
    class DeterministicSurviveAndDie : public SurviveAndDie<EOT>
    {
    public:

	using SurviveAndDie< EOT >::howmanyDie;
	using SurviveAndDie< EOT >::howmanySurvive;

	/** constructor */
	DeterministicSurviveAndDie(double _survive, double _die, bool _interpret_as_rate = true)
	    : SurviveAndDie< EOT >(_survive, _die, _interpret_as_rate)
	{}


	void operator()(Pop<EOT> & _pop, Pop<EOT> & _luckyGuys)
	{
	    unsigned pSize = _pop.size();
	    unsigned nbSurvive = howmanySurvive(pSize);
	    // first, save the best into _luckyGuys
	    if (nbSurvive)
		{
		    _pop.nth_element(nbSurvive);
		    // copy best
		    _luckyGuys.resize(nbSurvive);
		    std::copy(_pop.begin(), _pop.begin()+nbSurvive, _luckyGuys.begin());
		    // erase them from pop
		    _pop.erase(_pop.begin(), _pop.begin()+nbSurvive);
		}
	    unsigned nbRemaining = _pop.size();

	    // carefull, we can have a rate of 1 if we want to kill all remaining
	    unsigned nbDie = std::min(howmanyDie(pSize), pSize-nbSurvive);
	    if (nbDie > nbRemaining)
		throw std::logic_error("DeterministicSurviveAndDie: Too many to kill!\n");

	    if (!nbDie)
		{
		    return;
		}
	    // else
	    // kill the worse nbDie
	    _pop.nth_element(nbRemaining-nbDie);
	    _pop.resize(nbRemaining-nbDie);
	}

    };

    /**
       DeterministicSaDReplacement: replacement strategy that is just, in sequence
       saves best and kill worse from parents
       + saves best and kill worse from offspring
       + merge remaining (neither save nor killed) parents and offspring
       + reduce that merged population
       = returns reduced pop + best parents + best offspring

       An obvious use is as strong elitist strategy,
       i.e. preserving best parents, and reducing
       (either offspring or parents+offspring)
    */
    template <class EOT>
    class DeterministicSaDReplacement : public Replacement<EOT>
    {
    public:
	/**  Constructor with reduce */
	DeterministicSaDReplacement(Reduce<EOT>& _reduceGlobal,
				      double _surviveParents, double _dieParents=0,
				      double _surviveOffspring=0, double _dieOffspring=0,
				      bool _interpret_as_rate = true ) :
	    reduceGlobal(_reduceGlobal),
	    sAdParents(_surviveParents, _dieParents, _interpret_as_rate),
	    sAdOffspring(_surviveOffspring, _dieOffspring, _interpret_as_rate)
	{}

	/**  Constructor with default truncate used as reduce */
	DeterministicSaDReplacement(
				      double _surviveParents, double _dieParents=0,
				      double _surviveOffspring=0, double _dieOffspring=0,
				      bool _interpret_as_rate = true ) :
	    reduceGlobal(truncate),
	    sAdParents(_surviveParents, _dieParents, _interpret_as_rate),
	    sAdOffspring(_surviveOffspring, _dieOffspring, _interpret_as_rate)
	{}

	void operator()(Pop<EOT>& _parents, Pop<EOT>& _offspring)
	{
	    unsigned pSize = _parents.size(); // target number of individuals

	    Pop<EOT> luckyParents;       // to hold the absolute survivors
	    sAdParents(_parents, luckyParents);

	    Pop<EOT> luckyOffspring;       // to hold the absolute survivors
	    sAdOffspring(_offspring, luckyOffspring);

	    unsigned survivorSize = luckyOffspring.size() + luckyParents.size();
	    if (survivorSize > pSize)
		throw std::logic_error("GeneralReplacement: More survivors than parents!\n");

	    plus(_parents, _offspring); // all that remain in _offspring

	    reduceGlobal(_offspring, pSize - survivorSize);
	    plus(luckyParents, _offspring);
	    plus(luckyOffspring, _offspring);

	    _parents.swap(_offspring);

	}

    private :
	Reduce<EOT>& reduceGlobal;
	DeterministicSurviveAndDie<EOT> sAdParents;
	DeterministicSurviveAndDie<EOT> sAdOffspring;
	// plus helper (could be replaced by operator+= ???)
	Plus<EOT> plus;
	// the default reduce: deterministic truncation
	Truncate<EOT> truncate;
    };


    /** @} */

}

#endif
