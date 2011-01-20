/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

   -----------------------------------------------------------------------------
   Replacement.h 
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

#ifndef _Replacement_h
#define _Replacement_h


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
	---
	The MergeReduce, combination of Merge and Reduce, can be found 
	in file MergeReduce.h

	The ReduceMergeReduce that reduces the parents and the offspring,
	merges the 2 reduced populations, and eventually reduces that final
	population, can be found in ReduceMergeReduce.h

	LOG
	---
	Removed the const before first argument: though it makes too many classes 
	with the same interface, it allows to minimize the number of actual copies 
	by choosing the right destination
	I also removed the enforced "swap" in the eoEasyAlgo and hence the generational
	replacement gets a class of its own that only does the swap (instead of the 
	eoNoReplacement that did nothing, relying on the algo to swap popualtions).
	MS 12/12/2000

	@see Merge, Reduce, MergeReduce, ReduceMerge 

	@class Replacement,                    base (pure abstract) class
	@class GenerationalReplacement,        as it says ...
	@class WeakElitistReplacement          a wrapper to add elitism

    */

    /** The base class for all replacement functors.

	NOTE: two Pop as arguments
	the resulting population should be in the first argument (replace
	parents by offspring)! The second argument can contain any rubbish 

	@ingroup Replacors
    */
    template<class EOT>
    class Replacement : public BF<Pop<EOT>&, Pop<EOT>&, void>
    {};

    /**
       generational replacement == swap populations

       @ingroup Replacors
    */
    template <class EOT>
    class GenerationalReplacement : public Replacement<EOT>
    {
    public :
	/// swap
	void operator()(Pop<EOT>& _parents, Pop<EOT>& _offspring)
	{
	    _parents.swap(_offspring);
	}
    };

    /** 
	WeakElitistReplacement: a wrapper for other replacement procedures. 
	Copies in the new pop the best individual from the old pop, 
	AFTER normal replacement, if the best of the new pop is worse than the best 
	of the old pop. Removes the worse individual from the new pop.
	This could be changed by adding a selector there...

	@ingroup Replacors
    */
    template <class EOT>
    class WeakElitistReplacement : public Replacement<EOT>
    {
    public :
	typedef typename EOT::Fitness Fitness;

	// Ctor, takes an Replacement
	WeakElitistReplacement(Replacement<EOT> & _replace) :
	    replace(_replace) {}

	/// do replacement
	void operator()(Pop<EOT>& _pop, Pop<EOT>& _offspring)
	{
	    const EOT oldChamp = _pop.best_element();
	    replace(_pop, _offspring);	   // "normal" replacement, parents are the new
	    if (_pop.best_element() < oldChamp) // need to do something
		{
		    typename Pop<EOT>::iterator itPoorGuy = _pop.it_worse_element();
		    (*itPoorGuy) = oldChamp;
		}
	}
    private:
	Replacement<EOT> & replace;
    };

}

#endif
