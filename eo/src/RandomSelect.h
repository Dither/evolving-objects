// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// RandomSelect.h
// (c) GeNeura Team, 1998 - EEAAX 1999, Maarten Keijzer 2000
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

#ifndef RandomSelect_h
#define RandomSelect_h

/** This file contains straightforward selectors:
 * RandomSelect          returns an individual uniformly selected
 * BestSelect            always return the best individual
 * SequentialSelect      returns all individuals in turn
 */

#include <utils/RNG.h>
#include <SelectOne.h>

namespace eo
{

    /** RandomSelect: a selection method that selects ONE individual randomly 
     *
     * @ingroup Selectors
     */
    template <class EOT> class RandomSelect: public SelectOne<EOT>
    {
    public:

	/// not a big deal!!!
	virtual const EOT& operator()(const Pop<EOT>& _pop)
	{
	    return _pop[rng.random(_pop.size())] ;
	}
    };

    /** BestSelect: a selection method that always return the best
     *                (mainly for testing purposes)
     *
     * @ingroup Selectors
     */
    template <class EOT> class BestSelect: public SelectOne<EOT>
    {
    public:

	/// not a big deal!!!
	virtual const EOT& operator()(const Pop<EOT>& _pop)
	{
	    return _pop.best_element() ;
	}
    };

    /** NoSelect: returns all individual in order WITHOUT USING FITNESS!!!
     *       looping back to the beginning when exhasuted
     *
     * @ingroup Selectors
     */
    template <class EOT> class NoSelect: public SelectOne<EOT>
    {
    public:
	/** Ctor
	 */
	NoSelect(): current(0) {}

	virtual const EOT& operator()(const Pop<EOT>& _pop)
	{
	    if (current >= _pop.size())
		current=0;

	    current++;
	    return _pop[current-1] ;
	}
    private:
	unsigned current;
    };

}

#endif
