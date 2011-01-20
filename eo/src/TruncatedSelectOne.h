/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

   -----------------------------------------------------------------------------
   TruncatedSelectOne.h
   (c) Maarten Keijzer, Marc Schoenauer, GeNeura Team, 2002

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
             Marc.Schoenauer@inria.fr
             mkeijzer@dhi.dk
 */
//-----------------------------------------------------------------------------

#ifndef _TruncatedSelectOne_h
#define _TruncatedSelectOne_h


//-----------------------------------------------------------------------------
#include <Select.h>
#include <SelectOne.h>
#include <utils/HowMany.h>
#include <math.h>
//-----------------------------------------------------------------------------

namespace eo
{

    /** TruncatedSelectOne selects one individual using SelectOne as it's 
	mechanism. Therefore TruncatedSelectOne needs an SelectOne in its ctor

	It will perform selection only from the top guys in the population.

	@ingroup Selectors
    */
    template<class EOT>
    class TruncatedSelectOne : public SelectOne<EOT>
    {
    public:
	/** Ctor from rate and bool */
	TruncatedSelectOne(SelectOne<EOT>& _select, 
			     double  _rateFertile, 
			     bool _interpret_as_rateF = true)
	    : select(_select), 
	      howManyFertile(_rateFertile, _interpret_as_rateF),
	      tmpPop(), actualPop(tmpPop)
	{}

	/** Ctor with HowMany */
	TruncatedSelectOne(SelectOne<EOT>& _select, 
			     HowMany _howManyFertile) 
	    : select(_select), howManyFertile(_howManyFertile),
	      tmpPop(), actualPop(tmpPop)
	{}


	/** setup procedures: fills the temporary population with the fertile guys */
	void setup(const Pop<EOT>& _source)
	{
	    unsigned fertile = howManyFertile(_source.size());
	    if (fertile == _source.size())  // noting to do, all are fertile
		{
		    actualPop = _source;
		}
	    else
		{
		    // copy only best ferile to actualPop
		    tmpPop.resize(fertile);
		    std::vector<const EOT *> result;
		    _source.nth_element(fertile, result);
		    for (unsigned i=0; i<fertile; i++)
			tmpPop[i] = *result[i];

		    // and just in case
		    actualPop = tmpPop;
		}

	    // AND DON'T FORGET to call the embedded select setup method on actualPop
	    select.setup(actualPop);

	    return;
	}
  
	/**
	   The implementation selects an individual from the fertile pop

	   @param _pop the source population
	   @return the selected guy
	*/
	const EOT& operator()(const Pop<EOT>& _pop) 
	{
	    return select(actualPop);
	}


    private :
	SelectOne<EOT>& select;	   // selector for one guy
	HowMany howManyFertile;	   // number of fertile guys
	Pop<EOT> tmpPop;		   // intermediate population - fertile guys
	Pop<EOT> & actualPop;	   // to avoid copies when 100% fertility
    };

}

#endif
