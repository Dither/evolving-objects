// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// FlOrQuadOp.h
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

#ifndef _FlOrQuadOp_h
#define _FlOrQuadOp_h

#include <Functor.h>
#include <Op.h>

namespace eo
{

    /** @addtogroup Variators
     * @{
     */

    /** Generic QuadOps on fixed length genotypes.
     *  Contains exchange crossovers (1pt and uniform)
     *      and crossovers that applies an Atom crossover 
     *          to all components with given rate, or
     *          to a fixed prescribed nb of components
     */

    //////////////////////////////////////////////////////////////////////
    //                        FlOrAllAtomQuadOp
    //////////////////////////////////////////////////////////////////////

    /** Quad Crossover using an Atom Crossover
     */
    template <class EOT>
    class FlOrAllAtomQuadOp : public QuadOp<EOT>
    {
    public :

	typedef typename EOT::AtomType AtomType;

	/** default ctor: requires an Atom QuadOp */
	FlOrAllAtomQuadOp( QuadOp<AtomType>& _op, double _rate = 1):
	    op(_op), rate( _rate ) {}

	/** applies Atom crossover to ALL components with given rate */
	bool operator()(EOT & _eo1, EOT & _eo2)
	{
	    bool changed = false;
	    for ( unsigned i = 0; i < _eo1.size(); i++ ) {
		if ( rng.flip( rate ) ) {
		    bool changedHere = op( _eo1[i], _eo2[i] );
		    changed |= changedHere;
		}
	    }
	    return changed;
	}

	/** inherited className()*/
	virtual string className() const { return "FlOrAllAtomQuadOp"; }

    private:
	double rate;
	QuadOp<AtomType> & op;
    };

    //////////////////////////////////////////////////////////////////////
    //                        FlOrKAtomQuadOp
    //////////////////////////////////////////////////////////////////////
    /** Quad Crossover using an Atom Crossover
     *  that is applied to a FIXED NB of components
     */
    template <class EOT>
    class FlOrKAtomQuadOp : public QuadOp<EOT>
    {
    public :

	typedef typename EOT::AtomType AtomType;

	/** default ctor: requires an Atom QuadOp and an unsigned */
	FlOrAtomQuadOp( QuadOp<AtomType>& _op, unsigned _k = 1):
	    op(_op), k( _k ) {}

	/** applies the Atom QuadOp to some components */
	bool operator()(EOT & _eo1, const EOT & _eo2)
	{
	    if (_eo1.size() != _eo2.size())
		{
		    string s = "Operand size don't match in " + className();
		    throw runtime_error(s);
		}

	    bool changed = false;
	    for ( unsigned i = 0; i < k; i++ ) //! @todo check that we don't do twice the same
		{
		    unsigned where = eo::rng.random(_eo1.size());
		    bool changedHere = op( _eo1[where], _eo2[where] );
		    changed |= changedHere;
		}
	    return changed;
	}

	/** inherited className()*/
	virtual string className() const { return "FlOrKAtomQuadOp"; }

    private:
	unsigned k;
	QuadOp<AtomType> & op;
    };


    //////////////////////////////////////////////////////////////////////
    //                        FlOrUniformQuadOp
    //////////////////////////////////////////////////////////////////////
    /** The uniform crossover - exchanges atoms uniformly ! */
    template <class EOT>
    class FlOrUniformQuadOp : public QuadOp<EOT>
    {
    public :

	typedef typename EOT::AtomType AtomType;

	/** default ctor: requires a rate - 0.5 by default */
	VlUniformQuadOp(double _rate=0.5) : QuadOp<EOT>(_size),
					      rate(_rate) {}

	/** excahnges atoms at given rate */
	bool operator()(EOT & _eo1, EOT & _eo2)
	{
	    unsigned i;
	    Atom tmp;
	    if (_eo1.size() != _eo2.size())
		{
		    string s = "Operand size don't match in " + className();
		    throw runtime_error(s);
		}
	    bool hasChanged = false;
	    for (unsigned i=0; i<_eo1.size(); i++)
		{
		    if ( (_eo1[i]!=_eo2[i]) && (eo::rng.filp(rate)) )
			{
			    tmp = _eo1[i];
			    _eo1[i] = _eo2[i];
			    _eo2[i] = tmp;
			    hasChanged = true;
			}
		}
	    return hasChanged;
	}

	/** inherited className()*/
	virtual string className() const { return "FlOrUniformQuadOp"; }

    private:
	double rate;
    };

    //////////////////////////////////////////////////////////////////////
    //                        FlOr1ptQuadOp
    //////////////////////////////////////////////////////////////////////
    /** The 1pt  crossover (just in case someone wants it some day!) */
    template <class EOT>
    class FlOr1ptQuadOp : public QuadOp<EOT>
    {
    public :

	typedef typename EOT::AtomType AtomType;

	/** default ctor: no argument */
	VlUniformQuadOp() {}

	/** exchanges first and second parts of the vectors of Atoms */
	bool operator()(EOT & _eo1, EOT & _eo2)
	{
	    unsigned i;
	    Atom tmp;
	    if (_eo1.size() != _eo2.size())
		{
		    string s = "Operand size don't match in " + className();
		    throw runtime_error(s);
		}
	    bool hasChanged = false;
	    unsigned where = eo::rng.random(_eo1.size()-1);
	    for (unsigned i=where+1; i<_eo1.size(); i++)
		{
		    if ( (_eo1[i]!=_eo2[i]) )
			{
			    tmp = _eo1[i];
			    _eo1[i] = _eo2[i];
			    _eo2[i] = tmp;
			    hasChanged = true;
			}
		}
	    return hasChanged;
	}

	/** inherited className()*/
	virtual string className() const { return "FlOr1ptQuadOp"; }

    };

}

/** @} */

#endif
