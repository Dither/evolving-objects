// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// SGA.h
// (c) GeNeura Team, 2000 - EEAAX 1999 - Maarten Keijzer 2000
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
             mak@dhi.dk
 */
//-----------------------------------------------------------------------------

#ifndef _SGA_h
#define _SGA_h

#include <InvalidateOps.h>
#include <Continue.h>
#include <Pop.h>
#include <SelectOne.h>
#include <SelectPerc.h>
#include <EvalFunc.h>
#include <Algo.h>
#include <apply.h>

namespace eo
{

    /** The Simple Genetic Algorithm, following Holland and Goldberg
     *
     * Needs a selector (class SelectOne) a crossover (Quad, i.e. a
     * 2->2 operator) and a mutation with their respective rates, of
     * course an evaluation function (EvalFunc) and a continuator
     * (Continue) which gives the stopping criterion. Performs full
     * generational replacement.
     *
     * @ingroup Algorithms
     */
    template <class EOT>
    class SGA : public Algo<EOT>
    {
    public :

	// added this second ctor as I didn't like the ordering of the parameters
	// in the one above. Any objection :-) MS
	SGA(
	      SelectOne<EOT>& _select,
	      QuadOp<EOT>& _cross, float _crate,
	      MonOp<EOT>& _mutate, float _mrate,
	      EvalFunc<EOT>& _eval,
	      Continue<EOT>& _cont)
	    : cont(_cont),
	      mutate(_mutate),
	      mutationRate(_mrate),
	      cross(_cross),
	      crossoverRate(_crate),
	      select(_select),
	      eval(_eval) {}

	void operator()(Pop<EOT>& _pop)
	{
	    Pop<EOT> offspring;

	    do
		{
		    select(_pop, offspring);

		    unsigned i;

		    for (i=0; i<_pop.size()/2; i++)
			{
			    if ( rng.flip(crossoverRate) )
				{
				    // this crossover generates 2 offspring from two parents
				    if (cross(offspring[2*i], offspring[2*i+1]))
					{
					    offspring[2*i].invalidate();
					    offspring[2*i+1].invalidate();
					}
				}
			}

		    for (i=0; i < offspring.size(); i++)
			{
			    if (rng.flip(mutationRate) )
				{
				    if (mutate(offspring[i]))
					offspring[i].invalidate();
				}
			}

		    _pop.swap(offspring);
		    apply<EOT>(eval, _pop);

		} while (cont(_pop));
	}

    private :

	Continue<EOT>& cont;
	/// InvalidateMonOp invalidates the embedded operator
	InvalidateMonOp<EOT> mutate;
	float mutationRate;
	// InvalidateQuadOp invalidates the embedded operator
	InvalidateQuadOp<EOT> cross;
	float crossoverRate;
	SelectPerc<EOT> select;
	EvalFunc<EOT>& eval;
    };

}

#endif
