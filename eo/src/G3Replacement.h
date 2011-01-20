/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

   -----------------------------------------------------------------------------
   G3Replacement.h 
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

#ifndef _G3Replacement_h
#define _G3Replacement_h


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
       G3Replacement is an Replacement:
       - no strong elitism (is suppposed to be within a steady-state engine)
       - choose N (2) parents RANDOMLY - remove them from the parent population
       - merge offspring and the N removed parents
       - select best N of this merged population
       - put them back into parent population

       @ingroup Replacors
    */
    template <class EOT>
    class G3Replacement : public Replacement<EOT>
    {
    public:
	G3Replacement(HowMany _howManyEliminatedParents = HowMany(2, false)) :
	    // split truncates the parents and returns eliminated parents
	    split(_howManyEliminatedParents, true), 
	    // reduce truncates the offpsring and does not return eliminated guys
	    reduce(-_howManyEliminatedParents, false) 
	{}
    
	void operator()(Pop<EOT> & _parents, Pop<EOT> & _offspring)
	{
	    Pop<EOT> temp;
	    split(_parents, temp);
	    unsigned toKeep = temp.size(); // how many to keep from merged populations
	    // merge temp into offspring
	    plus(temp, _offspring);	   // add temp to _offspring (a little inconsistent!)
      
	    // reduce merged
	    reduce(_offspring, temp);	   // temp dummy arg. will not be modified
	    // minimla check:
	    if (_offspring.size() != toKeep)
		{
		    std::cerr << "Les tailles " << _offspring.size() << " " << toKeep << std::endl;
		    throw std::runtime_error("G3Replacement: wrong number of remaining offspring");
		}
	    // and put back into _parents
	    plus(_offspring, _parents);      
	}

    private:
	LinearTruncateSplit<EOT> split; // few parents to truncate -> linear 
	TruncateSplit<EOT> reduce;	   // supposedly many offspring to truncate
	Plus<EOT> plus;
    };

}

#endif
