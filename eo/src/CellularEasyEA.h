// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

// "CellularEasyEA.h"

// (c) OPAC Team, LIFL, 2002

/* This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
   
   Contact: cahon@lifl.fr
*/

#ifndef CellularEasyEA_h
#define CellularEasyEA_h

#include <Continue.h>
#include <EvalFunc.h>
#include <SelectOne.h>
#include <PopEvalFunc.h>
#include <Algo.h>
#include <Op.h>

namespace eo
{

    /**
       The abstract cellular easy algorithm.

       @ingroup Algorithms
    */
    template <class EOT> class CellularEasyEA : public Algo <EOT> {

    public :
	/**
	   Two constructors
	*/

	CellularEasyEA (Continue <EOT> & _cont, // Stop. criterion
			  EvalFunc <EOT> & _eval, // Evaluation function
			  SelectOne <EOT> & _sel_neigh, // To choose a partner
			  BinOp <EOT> & _cross, // Cross-over operator
			  MonOp <EOT> & _mut, // Mutation operator
			  SelectOne <EOT> & _sel_repl /* Which to keep between the new
							   child and the old individual ? */
			  ) :
	    cont (_cont),
	    eval (_eval),
	    popEval (_eval),
	    sel_neigh (_sel_neigh),
	    cross (_cross),
	    mut (_mut),
	    sel_child (SelectFirstOne ()),
	    sel_repl (_sel_repl) {

	}

	CellularEasyEA (Continue <EOT> & _cont,
			  EvalFunc <EOT> & _eval,
			  SelectOne <EOT> & _sel_neigh,
			  QuadOp <EOT> & _cross,
			  MonOp <EOT> & _mut,
			  SelectOne <EOT> & _sel_child, /* To choose one from
							     the both children */
			  SelectOne <EOT> & _sel_repl
			  ) :
	    cont (_cont),
	    eval (_eval),
	    popEval (_eval),
	    sel_neigh (_sel_neigh),
	    cross (_cross),
	    mut (_mut),
	    sel_child (_sel_child),
	    sel_repl (_sel_repl) {

	}

	/**
	   For a given population.
	*/

	void operator () (Pop <EOT> & pop) {

	    do {

		for (unsigned i = 0 ; i < pop.size () ; i ++) {

		    // Who are neighbouring to the current individual ?
		    Pop <EOT> neigh = neighbours (pop, i) ;

		    // To select a partner
		    EOT part, old_sol = pop [i] ;
		    part = sel_neigh (neigh) ;

		    // To perform cross-over
		    cross (pop [i], part) ;

		    // To perform mutation
		    mut (pop [i]) ;
		    mut (part) ;

		    pop [i].invalidate () ;
		    part.invalidate () ;
		    eval (pop [i]) ;
		    eval (part) ;

		    // To choose one of the two children ...
		    Pop <EOT> pop_loc ;
		    pop_loc.push_back (pop [i]) ;
		    pop_loc.push_back (part) ;

		    pop [i] = sel_child (pop_loc) ;

		    // To choose only one between the new made child and the old individual
		    pop_loc.clear () ;
		    pop_loc.push_back (pop [i]) ;
		    pop_loc.push_back (old_sol) ;
		    pop [i] = sel_repl (pop_loc) ;
		}
	    } while (cont (pop)) ;
	}

    protected :

	virtual Pop <EOT> neighbours (const Pop <EOT> & pop, int rank) = 0 ;

    private :
	Continue <EOT> & cont ;
	EvalFunc <EOT> & eval ;
	PopLoopEval <EOT> popEval ;
	SelectOne <EOT> & sel_neigh ;
	BF <EOT &, EOT &, bool> & cross ;
	MonOp <EOT> & mut ;
	SelectOne <EOT> & sel_child ;
	SelectOne <EOT> & sel_repl ;

	class SelectFirstOne : public SelectOne <EOT> {
	public :
	    const EOT & operator () (const Pop <EOT> & pop) {
		return pop [0] ;
	    }

	};

    };

}

#endif
