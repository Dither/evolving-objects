/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

   -----------------------------------------------------------------------------
   ReduceMerge.h 
   (c) Maarten Keijzer, Marc Schoenauer, GeNeura Team, 2000
 
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

#ifndef _ReduceMerge_h
#define _ReduceMerge_h


//-----------------------------------------------------------------------------
#include <Pop.h>
#include <Functor.h>
#include <Merge.h>
#include <Reduce.h>
#include <Replacement.h>
#include <utils/HowMany.h>
//-----------------------------------------------------------------------------

namespace eo
{

    /** @addtogroup Replacors
     * @{
     */

    /** 
	ReduceMerge: Replacement strategies that start by reducing the parents, 
	then merge with the offspring

	This is the way to do SSGA: the offspring gets inserted in the population
	even if it is worse than anybody else.

	@see ReduceMerge
	@see SSGAWorseReplacement
	@see SSGADetTournamentReplacement
	@see SSGAStochTournamentReplacement
    */
    template <class EOT>
    class ReduceMerge : public Replacement<EOT>
    {
    public:
        ReduceMerge(Reduce<EOT>& _reduce, Merge<EOT>& _merge) :
	    reduce(_reduce), merge(_merge)
        {}

        void operator()(Pop<EOT>& _parents, Pop<EOT>& _offspring)
        {
	    if (_parents.size() < _offspring.size())
		throw std::logic_error("ReduceMerge: More offspring than parents!\n");
	    reduce(_parents, _parents.size() - _offspring.size()); 
	    merge(_offspring, _parents);
        }

    private :
        Reduce<EOT>& reduce;
        Merge<EOT>& merge;
    };

    /** 
	SSGA replace worst. Is an ReduceMerge.
    */
    template <class EOT> 
    class SSGAWorseReplacement : public ReduceMerge<EOT>
    {
    public :
        SSGAWorseReplacement() : ReduceMerge<EOT>(truncate, plus) {}

    private :
        LinearTruncate<EOT> truncate;
        Plus<EOT> plus;
    };

    /** 
	SSGA deterministic tournament replacement. Is an ReduceMerge.
    */
    template <class EOT> 
    class SSGADetTournamentReplacement : public ReduceMerge<EOT>
    {
    public :
        SSGADetTournamentReplacement(unsigned _t_size) : 
	    ReduceMerge<EOT>(truncate, plus), truncate(_t_size) {}

    private :
        DetTournamentTruncate<EOT> truncate;
        Plus<EOT> plus;
    };

    /** SSGA stochastic tournament replacement. Is an ReduceMerge.
	It much cleaner to insert directly the offspring in the parent population, 
	but it is NOT equivalent in case of more than 1 offspring as already 
	replaced could be removed , which is not possible in the ReduceMerge 
	So what the heck ! */
    template <class EOT> 
    class SSGAStochTournamentReplacement : public ReduceMerge<EOT>
    {
    public :
        SSGAStochTournamentReplacement(double _t_rate) : 
	    ReduceMerge<EOT>(truncate, plus), truncate(_t_rate) {}

    private :
        StochTournamentTruncate<EOT> truncate;
        Plus<EOT> plus;
    };

}

/** @} */
#endif
