/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

   -----------------------------------------------------------------------------
   TruncSelect.h
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
             Marc.Schoenauer@polytechnique.fr
             mkeijzer@dhi.dk
 */
//-----------------------------------------------------------------------------

#ifndef _TruncSelect_h
#define _TruncSelect_h


//-----------------------------------------------------------------------------
#include <Select.h>
#include <utils/HowMany.h>
#include <math.h>
//-----------------------------------------------------------------------------

namespace eo
{

    /** TruncSelect selects individuals after truncating the population
     * using SelectOne as it's mechanism. 
     * Therefore SelectMany needs an SelectOne in its ctor
     * It will use an eoHowMnay to determine the number of guys to keep,
     *
     * @ingroup Selectors
     */
    template<class EOT>
    class TruncSelect : public Select<EOT>
    {
    public:
	/** Ctor: from an Select (and an Many to tell how many are kept for selectino */
	TruncSelect(SelectOne<EOT>& _select, HowMany _howMany) 
	    : select(_select), howMany(_howMany) {}
  
	/**
	   The implementation repeatidly selects an individual

	   @param _source the source population
	   @param _dest  the resulting population (size of this population is the number of times SelectOne is called. It empties the destination and adds the selection into it)
	*/
	virtual void operator()(const Pop<EOT>& _source, Pop<EOT>& _dest)
	{
	    unsigned target = howMany(_source.size());
    
	    _dest.resize(target);
    
	    select.setup(_source);
    
	    for (size_t i = 0; i < _dest.size(); ++i)
		_dest[i] = select(_source);
	}
  
    private :
	SelectOne<EOT>& select;
	HowMany howMany;
    };

}

#endif
