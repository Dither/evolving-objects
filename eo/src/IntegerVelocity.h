// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// IntegerVelocity.h
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
             clive.canape@inria.fr
 */
//-----------------------------------------------------------------------------

#ifndef INTEGERVELOCITY_H
#define INTEGERVELOCITY_H

//-----------------------------------------------------------------------------
#include <Functor.h>
#include <utils/RNG.h>
#include <Pop.h>
#include <utils/RealVectorBounds.h>
#include <RealBoundModifier.h>
#include <Topology.h>
//-----------------------------------------------------------------------------

namespace eo
{

    /** Integer velocity performer for particle swarm optimization. Derivated from abstract Velocity,
     *   At step t: v(t+1)= c1 * v(t) + c2 * r2 * ( xbest(t)-x(t) ) + c3 * r3 * ( gbest(t) - x(t) )
     *   v(t) is an INT for any time step
     *   (ci given and Ri chosen at random in [0;1]).
     *
     *   @ingroup Variators
     */
    template < class POT > class IntegerVelocity:public Velocity < POT >
    {

    public:

	/*
	 * Each element for the velocity evaluation is expected to be of type VelocityType.
	 */
	typedef typename POT::ParticleVelocityType VelocityType;

	/** Full constructor: Bounds and bound modifier required
	 * @param _topology - The topology to get the global/local/other best
	 * @param _c1 - The first learning factor quantify how much the particle trusts itself. Type must be POT::ParticleVelocityType 
	 * @param _c2 - The second learning factor used for the particle's best. Type must be POT::ParticleVelocityType 
	 * @param _c3 - The third learning factor used for the local/global best(s). Type must be POT::ParticleVelocityType 
	 * @param _bounds - An RealBaseVectorBounds: real bounds for real velocities. 
	 * If the velocities are not real, they won't be bounded by default. Should have a Bounds ?
	 * @param _bndsModifier - An RealBoundModifier used to modify the bounds (for real bounds only).
	 * @param _gen - The eo random generator, default=rng
	 */
	IntegerVelocity (Topology < POT > & _topology,
			   const VelocityType & _c1,
			   const VelocityType & _c2,
			   const VelocityType & _c3,
			   RealVectorBounds & _bounds,
			   RealBoundModifier & _bndsModifier,
			   Rng & _gen = rng):
            topology(_topology),
            c1 (_c1),
            c2 (_c2),
            c3 (_c3),
            bounds(_bounds),
            bndsModifier(_bndsModifier),
            gen(_gen){}


	/** Constructor: No bound updater required <-> fixed bounds
	 * @param _topology - The topology to get the global/local/other best
	 * @param _c1 - The first learning factor quantify how much the particle trusts itself. Type must be POT::ParticleVelocityType 
	 * @param _c2 - The second learning factor used for the particle's best. Type must be POT::ParticleVelocityType 
	 * @param _c3 - The third learning factor used for the local/global best(s). Type must be POT::ParticleVelocityType 
	 * @param _bounds - An RealBaseVectorBounds: real bounds for real velocities. 
	 * If the velocities are not real, they won't be bounded by default. Should have a Bounds ?
	 * @param _gen - The eo random generator, default=rng
	 */
	IntegerVelocity (Topology < POT > & _topology,
			   const VelocityType & _c1,
			   const VelocityType & _c2,
			   const VelocityType & _c3,
			   RealVectorBounds & _bounds,
			   Rng & _gen = rng):
            topology(_topology),
            c1 (_c1),
            c2 (_c2),
            c3 (_c3),
            bounds(_bounds),
            bndsModifier(dummyModifier),
            gen(_gen){}


	/** Constructor: Neither bounds nor bound updater required <-> free velocity
	 * @param _topology the topology to use
	 * @param _c1 - The first learning factor quantify how much the particle trusts itself. Type must be POT::ParticleVelocityType 
	 * @param _c2 - The second learning factor used for the particle's best. Type must be POT::ParticleVelocityType 
	 * @param _c3 - The third learning factor used for the local/global best(s). Type must be POT::ParticleVelocityType 
	 * @param _gen - The eo random generator, default=rng
	 */
	IntegerVelocity (Topology < POT > & _topology,
			   const VelocityType & _c1,
			   const VelocityType & _c2,
			   const VelocityType & _c3,
			   Rng & _gen = rng):
            topology(_topology),
	    c1 (_c1),
            c2 (_c2),
            c3 (_c3),
            bounds(*(new RealVectorNoBounds(0))),
            bndsModifier(dummyModifier),
            gen(_gen)
	{}


	/**
	 *  Evaluate the new velocities of the given particle. Need an indice to identify the particle
	 * into the topology.
	 * @param _po - A particle
	 * @param _indice - The indice (into the topology) of the given particle
	 */
	void operator  () (POT & _po,unsigned _indice)
	{
	    VelocityType r2;
	    VelocityType r3;

	    VelocityType newVelocity;

	    // cast the learning factors to VelocityType
	    r2 =  (VelocityType) rng.uniform (1) * c2;
	    r3 =  (VelocityType) rng.uniform (1) * c3;

	    // need to resize the bounds even if there are dummy because of "isBounded" call
	    bounds.adjust_size(_po.size());

	    // assign the new velocities
	    for (unsigned j = 0; j < _po.size (); j++)
		{
		    newVelocity= round (c1 *  _po.velocities[j] + r2 * (_po.bestPositions[j] - _po[j]) +  r3 * (topology.best (_indice)[j] - _po[j]));

		    /* check bounds */
		    if (bounds.isMinBounded(j))
			newVelocity=std::max(newVelocity,bounds.minimum(j));
		    if (bounds.isMaxBounded(j))
			newVelocity=std::min(newVelocity,bounds.maximum(j));

		    _po.velocities[j]=newVelocity;
		}
	}

	/**
	 * Update the neighborhood.
	 */
	void updateNeighborhood(POT & _po,unsigned _indice)
	{
	    topology.updateNeighborhood(_po,_indice);
	}
    
	//! Topology<POT> getTopology
	//! @return topology

	Topology<POT> & getTopology ()
	{
	    return topology;
	}

    protected:
	Topology < POT > & topology;
	const VelocityType & c1;  // social/cognitive coefficient 
	const VelocityType & c2;  // social/cognitive coefficient 
	const VelocityType & c3;  // social/cognitive coefficient 
  
	RealVectorBounds bounds; // REAL bounds even if the velocity could be of another type.
	RealBoundModifier & bndsModifier;

	Rng & gen; // the random generator
	
	// If the bound modifier doesn't need to be used, use the dummy instance
	DummyRealBoundModifier dummyModifier;
    };

}

#endif /*INTEGERVELOCITY_H */
