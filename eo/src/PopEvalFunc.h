/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

-----------------------------------------------------------------------------
    PopEvalFunc.h
    Abstract class for global evaluation of the population

    (c) GeNeura Team, 2000
 
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

#ifndef PopEvalFunc_H
#define PopEvalFunc_H

#include <EvalFunc.h>
#include <apply.h>

namespace eo
{

    /** PopEvalFunc: This abstract class is for GLOBAL evaluators 
     *    of a population after variation.
     *    It takes 2 populations (typically the parents and the offspring)
     *    and is suppposed to evaluate them alltogether
     *
     *  Basic use: apply an embedded EvalFunc to the offspring 
     *
     *  Time-varying fitness: apply the embedded EvalFunc to both 
     *     offspring and parents
     *
     *  Advanced uses: Co-evolution or "parisian" approach, or ...
     *
     *  Basic parallelization (synchronous standard evolution engine): 
     *    call the slaves and wait for the results
     *
     *    @ingroup Evaluation
     */
    template<class EOT>
    class PopEvalFunc : public BF<Pop<EOT> & , Pop<EOT> &, void> 
    {};

    /////////////////////////////////////////////////////////////
    //           PopLoopEval
    /////////////////////////////////////////////////////////////

    /** PopLoopEval: an instance of PopEvalFunc that simply applies 
     *     a private EvalFunc to all offspring
     *
     *    @ingroup Evaluation
     */
    template<class EOT>
    class PopLoopEval : public PopEvalFunc<EOT> {
    public:
	/** Ctor: set value of embedded EvalFunc */
	PopLoopEval(EvalFunc<EOT> & _eval) : eval(_eval) {}

	/** Do the job: simple loop over the offspring */
	void operator()(Pop<EOT> & _parents, Pop<EOT> & _offspring)
	{
	    apply<EOT>(eval, _offspring);
	}

    private:
	EvalFunc<EOT> & eval;
    };

    /////////////////////////////////////////////////////////////
    //           TimeVaryingLoopEval
    /////////////////////////////////////////////////////////////

    /** PopLoopEval: an instance of PopEvalFunc that simply applies 
     *     a private EvalFunc to all offspring AND ALL PARENTS
     *     as the fitness is supposed here to vary 
     *
     *    @ingroup Evaluation
     */
    template<class EOT>
    class TimeVaryingLoopEval : public PopEvalFunc<EOT> {
    public:
	/** Ctor: set value of embedded EvalFunc */
	TimeVaryingLoopEval(EvalFunc<EOT> & _eval) : eval(_eval) {}

	/** Do the job: simple loop over the offspring */
	void operator()(Pop<EOT> & _parents, Pop<EOT> & _offspring)
	{
	    apply<EOT>(eval, _parents);
	    apply<EOT>(eval, _offspring);
	}

    private:
	EvalFunc<EOT> & eval;
    };

}

#endif

