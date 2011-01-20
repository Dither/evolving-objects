// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// RealParticle.h
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
//-----------------------------------------------------------------------------

#ifndef _REALPARTICLE_H
#define _REALPARTICLE_H

#include <VectorParticle.h>

namespace eo
{

    /** RealParticle: Implementation of a real-coded particle for
     *  particle swarm optimization. Positions, velocities and best positions are real-coded.
     *
     *  @ingroup Real
     */
    template < class FitT> class RealParticle: public VectorParticle<FitT,double,double>

    {
    public:

	RealParticle(unsigned size = 0, double positions = 0.0,double velocities = 0.0,double bestPositions = 0.0): VectorParticle<FitT, double,double> (size, positions,velocities,bestPositions) {}

	virtual std::string className() const
	{
	    return "RealParticle";
	}
    };

}

#endif /*_REALPARTICLE_H*/
