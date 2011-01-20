// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// FlOrMonOp.h
// (c) Marc Schoenauer - Maarten Keijzer 2000-2003
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

    Contact: Marc.Schoenauer@polytechnique.fr
             mkeijzer@cs.vu.nl
 */
//-----------------------------------------------------------------------------

#ifndef _FlOrMonOp_h
#define _FlOrMonOp_h

#include <Functor.h>
#include <Op.h>
#include <Init.h>

namespace eo
{

    /** @addtogroup Variators
     * @{
     */

    /** Base classes for generic mutations on fixed length chromosomes.
     *  Contains 2 classes that both use an atomic mutation
     *      FlOrAllMutation applies the atom mutation to all components with given rate
     *      FlOrKMutation applies the atom mutation to a fixed nb of components
     *
     * Remark: the standard bit-flip mutation is an FlOrAllMutation 
     *                      with atom mutation == bitflipping
     */

    /** applies an atomic mutation to all the components with a given rate
     */
    template <class EOT>
    class FlOrAllMutation : public MonOp<EOT>
    {
    public :

	typedef typename EOT::AtomType AtomType;

	/** default ctor: requires an Atom mutation and a rate */
	FlOrAllMutation(MonOp<AtomType> & _atomMutation, double _rate=1.0) :
	    atomMutation(_atomMutation), rate(_rate) {}

	/** applies the atom mutation to all components with given rate */
	bool operator()(EOT & _eo)
	{
	    bool modified=false;
	    for (unsigned i=0; i<_eo.size(); i++)
		if (eo::rng.flip(rate))
		    if (atomMutation(_eo[i]))
			modified = true;

	    return modified;
	}

	/** inherited className() */
	virtual std::string className() const 
	{ 
	    return "FlOrAllMutation(" + atomMutation.className() + ")";
	}

    private:
	MonOp<AtomType> & atomMutation; // the atom mutation
	double rate;			   // the mutation rate PER ATOM
    };

    /** Applies an atomic mutation to a fixed
	number of components (1 by default)
    */
    template <class EOT>
    class FlOrKMutation : public MonOp<EOT>
    {
    public :

	typedef typename EOT::AtomType AtomType;

	/** default ctor: requires an Atom mutation */
	FlOrKMutation(MonOp<AtomType> & _atomMutation, unsigned _nb=1) :
	    nb(_nb), atomMutation(_atomMutation) {}


	/** applies the atom mutation to K randomly selected components */
	bool operator()(EOT & _eo)
	{
	    bool modified=false;
	    for (unsigned k=0; k<nb; k++)
		{
		    unsigned i = rng.random(_eo.size()); // we don't test for duplicates...
		    if (atomMutation(_eo[i]))
			modified = true;
		}
	    return modified;
	}

	/** inherited className() */
	virtual std::string className() const
	{ 
	    return "FlOrKMutation(" + atomMutation.className() + ")";
	}

    private:
	unsigned nb;			   // the number of atoms to mutate
	MonOp<AtomType> & atomMutation; // the atom mutation
    };

}

/** @} */

#endif
