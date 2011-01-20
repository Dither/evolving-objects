// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// BitParticle.h
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

#ifndef _BITPARTICLE_H
#define _BITPARTICLE_H


#include <VectorParticle.h>

namespace eo
{

    /** BitParticle: Implementation of a bit-coded particle (swarm optimization).
     *  Positions and best positions are 0 or 1 but the velocity is a vector of double.
     *
     *  @ingroup Bitstring
     */
    template < class FitT> class BitParticle: public VectorParticle<FitT,bool,double>

    {
    public:

	BitParticle(unsigned size = 0, bool positions = 0,double velocities = 0.0,bool bestPositions = 0): VectorParticle<FitT, bool,double> (size, positions,velocities,bestPositions) {}

	virtual std::string className() const
	{
	    return "BitParticle";
	}
    };

}

#endif /*_BITPARTICLE_H*/
