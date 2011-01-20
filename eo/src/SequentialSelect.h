// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// SequentialSelect.h
// (c) GeNeura Team, 1998 - Maarten Keijzer 2000, Marc Schoenauer, 2002
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
//-----------------------------------------------------------------------------

#ifndef SequentialSelect_h
#define SequentialSelect_h

#include <limits>

#include "utils/Data.h"
#include "utils/RNG.h"
#include "SelectOne.h"

namespace eo
{

    /** Contains the following classes:
     *  - SequentialSelect, returns all individuals one by one,
     *    either sorted or shuffled
     *  - EliteSequentialSelect, returns all indivisuals one by one
     *    starting with best, continuing shuffled (see G3 engine)
     */


    /** All Individuals in order

	Looping back to the beginning when exhausted, can be from best to
	worse, or in random order.

	It is the SelectOne equivalent of DetSelect - though DetSelect
	always returns individuals from best to worst

	@ingroup Selectors
    */
    template <class EOT> class SequentialSelect: public SelectOne<EOT>
    {
    public:
	/** Ctor: sets the current pter to numeric_limits<unsigned>::max() so init will take place first time
	    not very elegant, maybe ...
	*/
	SequentialSelect(bool _ordered = true)
	    : ordered(_ordered), current(std::numeric_limits<unsigned>::max()) {}

	void setup(const Pop<EOT>& _pop)
	{
	    Pters.resize(_pop.size());
	    if (ordered)    // probably we could have a marker to avoid re-sorting
		_pop.sort(Pters);
	    else
		_pop.shuffle(Pters);
	    current=0;
	}

	virtual const EOT& operator()(const Pop<EOT>& _pop)
	{
	    if (current >= _pop.size())
		setup(_pop);

	    unsigned eoN = current;
	    current++;
	    return *Pters[eoN] ;
	}
    private:
	bool ordered;
	unsigned current;
	std::vector<const EOT*> Pters;
    };



    /** All Individuals in order

	The best individual first, then the others in sequence (random order).
	for G3 evolution engine, see Deb, Anad and Joshi, CEC 2002

	As SequentialSelect, it is an SelectOne to be used within the
	eoEaseyEA algo, but conceptually it should be a global Select, as it
	selects a bunch of guys in one go (done in the setup function now)

	@ingroup Selectors
    */
    template <class EOT> class EliteSequentialSelect: public SelectOne<EOT>
    {
    public:

	/** Ctor: sets the current pter to numeric_limits<unsigned>::max() so init will take place first time
	    not very elegant, maybe ...
	*/
	EliteSequentialSelect(): current(std::numeric_limits<unsigned>::max()) {}

	void setup(const Pop<EOT>& _pop)
	{
	    Pters.resize(_pop.size());
	    _pop.shuffle(Pters);
	    // now get the best
	    unsigned int ibest = 0;
	    const EOT * best = Pters[0];
	    if (_pop.size() == 1)
		throw std::runtime_error("Trying EliteSequentialSelect with only one individual!");
	    for (unsigned i=1; i<_pop.size(); i++)
		if (*Pters[i]>*best)
		    {
			ibest = i;
			best = Pters[ibest];
		    }
	    // and put it upfront
	    const EOT *ptmp = Pters[0];
	    Pters[0]=best;
	    Pters[ibest] = ptmp;
	    // exit after setting current
	    current=0;
	}

	virtual const EOT& operator()(const Pop<EOT>& _pop)
	{
	    if (current >= _pop.size())
		setup(_pop);

	    unsigned eoN = current;
	    current++;
	    return *Pters[eoN] ;
	}
    private:
	unsigned current;
	std::vector<const EOT*> Pters;
    };

}

#endif
