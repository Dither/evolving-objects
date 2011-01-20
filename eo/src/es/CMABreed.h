// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; fill-column: 80; -*-

//-----------------------------------------------------------------------------
// CMABreed
// (c) Maarten Keijzer 2005
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

 */
//-----------------------------------------------------------------------------

#ifndef _CMABREED_H
#define _CMABREED_H

#include <Breed.h>
#include <Vector.h>
#include <es/CMAState.h>

#include <algorithm>

namespace eo
{

    /// @todo handle bounds
    template <class FitT>
    class CMABreed : public Breed< Vector<FitT, double> > {
    
	eo::CMAState& state;
	unsigned lambda;
    
	typedef Vector<FitT, double> EOT;
    
    public:
	CMABreed(eo::CMAState& state_, unsigned lambda_) : state(state_), lambda(lambda_) {}
    
	void operator()(const Pop<EOT>& parents, Pop<EOT>& offspring) {
	
	    // two temporary arrays of pointers to store the sorted population
	    std::vector<const EOT*> sorted(parents.size());
	
	    // mu stores population as vector (instead of Pop)
	    std::vector<const std::vector<double>* > mu(parents.size());
	
	    parents.sort(sorted);
	    for (unsigned i = 0; i < sorted.size(); ++i) {
		mu[i] = static_cast< const std::vector<double>* >( sorted[i] );
	    }
	
	    // learn
	    state.reestimate(mu, sorted[0]->fitness(), sorted.back()->fitness());
	
	    if (!state.updateEigenSystem(10)) {
		std::cerr << "No good eigensystem found" << std::endl;
	    }
	
	    // generate
	    offspring.resize(lambda);

	    for (unsigned i = 0; i < lambda; ++i) {
		state.sample( static_cast< std::vector<double>& >( offspring[i] ));
		offspring[i].invalidate();
	    }
	
	}
    };

}

#endif
