/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

   -----------------------------------------------------------------------------
   SelectPerc.h 
   (c) Maarten Keijzer, GeNeura Team, 2000
 
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
 */
//-----------------------------------------------------------------------------

#ifndef _SelectPerc_h
#define _SelectPerc_h


//-----------------------------------------------------------------------------
#include <Select.h>
#include <SelectOne.h>
#include <math.h>
//-----------------------------------------------------------------------------

namespace eo
{

    /** SelectPerc selects many individuals using SelectOne as it's 
	mechanism. Therefore SelectPerc needs an SelectOne in its ctor

	It will select floor(rate*pop.size()) individuals and pushes them to
	the back of the destination population.

	@ingroup Selectors
    */
    template<class EOT>
    class SelectPerc : public Select<EOT>
    {
    public:
	/// init
	SelectPerc(SelectOne<EOT>& _select, float _rate = 1.0) 
	    : select(_select), rate(_rate) {}

	/**
	   The implementation selects a percentage 

	   @param _source the source population
	   @param _dest  the resulting population (size of this population is the number of times SelectOne is called. It empties the destination and adds the selection into it)
	*/
	virtual void operator()(const Pop<EOT>& _source, Pop<EOT>& _dest)
	{
	    size_t target = static_cast<size_t>(floor(rate * _source.size()));
    
	    _dest.resize(target);
    
	    select.setup(_source);
    
	    for (size_t i = 0; i < _dest.size(); ++i)
		_dest[i] = select(_source);
	}
  
    private :
	SelectOne<EOT>& select;
	float rate;
    };

}

#endif
