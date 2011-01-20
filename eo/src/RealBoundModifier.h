// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// RealBoundModifier.h
// (c) OPAC 2007
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

    Contact: thomas.legrand@lifl.fr
 */

#ifndef REALBOUNDMODIFIER_H
#define REALBOUNDMODIFIER_H

#include <Functor.h>
#include <utils/RealVectorBounds.h>

namespace eo
{

    /** @defgroup Bounds Bounds management
     *
     * Bounds are a set of utilities that permits to manage ranges of existence
     * for variables. For example to restrain vectors or parameters to a given domain.
     *
     * @ingroup Utilities
     */

    /**
     * Abstract class for RealVectorBounds modifier.
     * Used to modify the bounds included into the RealVectorBounds object.
     *
     * @ingroup Bounds
     */
    class RealBoundModifier: public BF < RealBaseVectorBounds &,unsigned,void > {};


    /**
     * An RealBoundModifier that modify nothing !
     * @ingroup Bounds
     */
    class DummyRealBoundModifier: public RealBoundModifier
    {
    public:

	DummyRealBoundModifier (){}

	void operator() (RealBaseVectorBounds & _bnds,unsigned _i)
	{
	    (void)_bnds;
	    (void)_i;
	}
    };


    /**
     * Modify an Real(Base)VectorBounds :
     * At iteration t, the interval I(t)=[min,max] is updated as:
     * I(t)=[min,(1-(t/Nt)^alpha)*max] where
     * - t, the current iteration, is given with an ValueParam<unsigned>
     * - Nt is the stopping criteria <=> the total number of iterations
     * - alpha a coefficient
     * 
     */
    class ExpDecayingBoundModifier: public RealBoundModifier
    {
    public:
	/**
	 * Constructor
	 * @param _stopCriteria - The total number of iterations
	 * @param _alpha 
	 * @param _genCounter - An ValueParam<unsigned> that gives the current iteration
	 */
	ExpDecayingBoundModifier (unsigned _stopCriteria,
				    double _alpha,
				    ValueParam<unsigned> & _genCounter):
	    stopCriteria(_stopCriteria),
	    alpha(_alpha),
	    genCounter(_genCounter){}

	void operator() (RealBaseVectorBounds & _bnds,unsigned _i)
	{
	    double newMaxBound=(1-pow((double)genCounter.value()/stopCriteria,alpha))*_bnds.maximum(_i);

	    // should delete the old RealBounds ?
	    _bnds[_i]=new RealInterval(_bnds.minimum(_i),std::max(_bnds.minimum(_i),newMaxBound));
	}


    protected:
	unsigned stopCriteria;
	double alpha;
	ValueParam<unsigned> & genCounter;

    };

}

#endif/*REALBOUNDMODIFIER_H*/
