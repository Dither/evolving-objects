/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

   -----------------------------------------------------------------------------
   Breed.h
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

#ifndef _Breed_h
#define _Breed_h


//-----------------------------------------------------------------------------
#include <Pop.h>
#include <Functor.h>
#include <Select.h>
#include <Transform.h>
//-----------------------------------------------------------------------------

namespace eo
{

    /** Breeding: combination of selecting and transforming a population

	Breeding is thought of a combination of selecting and transforming a
	population. For efficiency reasons you might want to build your own
	Breed derived class rather than relying on a seperate select and
	transform function.

	@see Select, Transform, SelectTransform

	@ingroup Combination
    */
    template<class EOT>
    class Breed : public BF<const Pop<EOT>&, Pop<EOT>&, void>
    {};



    /** Embedded select, followed by an embedded transform

	Special breeder that is just an application of an embedded select,
	followed by an embedded transform

	@ingroup Combination
    */
    template <class EOT>
    class SelectTransform : public Breed<EOT>
    {
    public:
        SelectTransform(Select<EOT>& _select, Transform<EOT>& _transform) :
	    select(_select), transform(_transform)
        {}

        void operator()(const Pop<EOT>& _parents, Pop<EOT>& _offspring)
        {
            select(_parents, _offspring);
            transform(_offspring);
        }

    private :
        Select<EOT>& select;
        Transform<EOT>& transform;
    };

}

#endif
