/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

   -----------------------------------------------------------------------------
   MergeReduce.h 
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

#ifndef _MergeReduce_h
#define _MergeReduce_h


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

    /** 
	Replacement strategies that combine en Merge and an Reduce.

	@class MergeReduce, the base (pure abstract) class
	@class PlusReplacement the ES plus strategy
	@class CommaReplacement the ES comma strategy
    */

    /**
       MergeReduce: abstract replacement strategy that is just an application of 
       an embedded merge, followed by an embedded reduce
       @ingroup Replacors
    */
    template <class EOT>
    class MergeReduce : public Replacement<EOT>
    {
    public:
        MergeReduce(Merge<EOT>& _merge, Reduce<EOT>& _reduce) :
	    merge(_merge), reduce(_reduce)
        {}

        void operator()(Pop<EOT>& _parents, Pop<EOT>& _offspring)
        {
            merge(_parents, _offspring); // parents untouched, result in offspring
            reduce(_offspring, _parents.size());
	    _parents.swap(_offspring);
        }

    private :
        Merge<EOT>& merge;
        Reduce<EOT>& reduce;
    };

    /**
       ES type of replacement strategy: first add parents to population, then truncate
       @ingroup Replacors
    */
    template <class EOT>
    class PlusReplacement : public MergeReduce<EOT>
    {
    public :
        PlusReplacement() : MergeReduce<EOT>(plus, truncate) {}

    private :
        Plus<EOT> plus;
        Truncate<EOT> truncate;
    };

    /**
       ES type of replacement strategy: ignore parents, truncate offspring
       @ingroup Replacors
    */
    template <class EOT>
    class CommaReplacement : public MergeReduce<EOT>
    {
    public :
        CommaReplacement() : MergeReduce<EOT>(no_elite, truncate) {}

    private :
        NoElitism<EOT> no_elite;
        Truncate<EOT> truncate;
    };

    /**
       EP type of replacement strategy: first add parents to population, 
       then truncate using EP tournament
       @ingroup Replacors
    */
    template <class EOT>
    class EPReplacement : public MergeReduce<EOT>
    {
    public :
	EPReplacement(int _tSize) : MergeReduce<EOT>(plus, truncate), truncate(_tSize)
				      //  {truncate.setSize(_tSize);}
	{}
    private :
	Plus<EOT> plus;
	EPReduce<EOT> truncate;
    };

}

#endif
