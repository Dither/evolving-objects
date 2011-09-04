// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// DetTournament.h
// (c) GeNeura Team, 1998 - EEAAX 1999
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
 */
//-----------------------------------------------------------------------------

#ifndef DetTournament_h
#define DetTournament_h

//-----------------------------------------------------------------------------

#include <functional>  //
#include <numeric>     // accumulate

#include <Functor.h>
#include <Pop.h>
#include <utils/Logger.h>
#include <utils/selectors.h>

namespace eo
{

    /** DetTournamentSelect: a selection method that selects ONE individual by
	deterministic tournament 
	-MS- 24/10/99

	@ingroup Selectors
    */
    template <class EOT> class DetTournamentSelect: public SelectOne<EOT>
    {
    public:
	/* (Default) Constructor - 
	   @param _tSize tournament size
	*/
	DetTournamentSelect(unsigned _tSize = 2 ):SelectOne<EOT>(), tSize(_tSize) {
	    // consistency check
	    if (tSize < 2) {
		log << warnings << "Tournament size should be >= 2, adjusted to 2" << std::endl;
		tSize = 2;
	    }
	}
  
	/* Perform deterministic tournament calling the appropriate fn 
	   see selectors.h
	*/
	virtual const EOT& operator()(const Pop<EOT>& _pop) 
	{
	    return deterministic_tournament(_pop, tSize);
	}
  
    private:
	unsigned tSize;
    };

    //-----------------------------------------------------------------------------

}

#endif
