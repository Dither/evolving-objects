/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

   -----------------------------------------------------------------------------
   MGGReplacement.h 
   (c) Maarten Keijzer, Marc Schoenauer, 2002
 
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

    Contact: Marc.Schoenauer@inria.fr
             mkeijzer@dhi.dk
 */
//-----------------------------------------------------------------------------

#ifndef _MGGReplacement_h
#define _MGGReplacement_h


//-----------------------------------------------------------------------------
#include <Pop.h>
#include <Functor.h>
#include <Merge.h>
#include <Reduce.h>
#include <utils/HowMany.h>
#include <ReduceSplit.h>
//-----------------------------------------------------------------------------

namespace eo
{

    /**
       MGGReplacement is an Replacement:
       - choose N (2) parents RANDOMLY - remove them from the parent population
       - select best offspring, add to parents
       - merge (other?) offspring and the N removed parents
       - select best N-1 of this merged population (detTournament only at the moment)
       - put them back into parent population

       @ingroup Replacors
    */

    template <class EOT>
    class MGGReplacement : public Replacement<EOT>
    {
    public:
	MGGReplacement(HowMany _howManyEliminatedParents = HowMany(2, false),
			 unsigned _tSize=2) :
	    // split truncates the parents and returns eliminated parents
	    split(_howManyEliminatedParents, true), 
	    tSize(_tSize)
	{
	    if (tSize < 2)
		{ 
		    eo::log << eo::warnings << "Warning, Size for DetTournamentTruncateSplit adjusted to 2" << std::endl;
		    tSize = 2;
		}
	}
    
	void operator()(Pop<EOT> & _parents, Pop<EOT> & _offspring)
	{
	    Pop<EOT> temp;
	    split(_parents, temp);
	    unsigned toKeep = temp.size(); // how many to keep from merged populations
	    // minimal check
	    if (toKeep < 2)
		throw std::runtime_error("Not enough parents killed in MGGReplacement");

	    // select best offspring
	    typename Pop<EOT>::iterator it = _offspring.it_best_element();
	    // add to parents
	    _parents.push_back(*it);
	    // remove from offspring
	    _offspring.erase(it);

	    // merge temp into offspring
	    plus(temp, _offspring);
      
	    // repeatedly add selected offspring to parents
	    for (unsigned i=0; i<toKeep-1; i++)
		{
		    // select
		    it = deterministic_tournament(_offspring.begin(), _offspring.end(), tSize);
		    // add to parents
		    _parents.push_back(*it);
		    // remove from offspring
		    _offspring.erase(it);
		}
	}

    private:
	LinearTruncateSplit<EOT> split; // few parents to truncate -> linear 
	Plus<EOT> plus;
	unsigned int tSize;
    };

}

#endif
