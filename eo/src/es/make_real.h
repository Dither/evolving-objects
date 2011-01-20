// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// real.h
// (c) Maarten Keijzer, Marc Schoenauer and GeNeura Team, 2001
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
             mkeijzer@dhi.dk
 */
//-----------------------------------------------------------------------------

/** This file contains all ***INSTANCIATED*** declarations of all components 
 * of the library for ***std::vector<RealValues>*** evolution inside EO.
 * It should be included in the file that calls any of the corresponding fns
 *
 * The corresponding ***INSTANCIATED*** definitions are contained in 
 * the different .cpp files in the src/es dir, 
 * while the TEMPLATIZED code is define in the different make_XXX.h files
 * either in hte src/do dir for representation independant functions, 
 * or in the src/es dir for representation dependent stuff.
 *
 * See also es.h for the similar declarations of ES-like genotypes 
 *   i.e. ***with*** mutation parameters attached to individuals
 *
 * Unlike most EO .h files, it does not (and should not) contain any code, 
 * just declarations
 */

#ifndef real_h
#define real_h

#include <Algo.h>
#include <ScalarFitness.h>
#include <utils/Parser.h>
#include <EvalFuncPtr.h>
#include <EvalFuncCounter.h>
#include <utils/CheckPoint.h>
#include <GenOp.h>
#include <Pop.h>
#include <utils/Distance.h>

#include <es/RealInitBounded.h>
#include <es/Real.h>

namespace eo
{

    //Representation dependent - rewrite everything anew for each representation
    //////////////////////////


    /** @addtogroup Builders
     * @{
     */
    // the genotypes 
    RealInitBounded<Real<double> > & make_genotype(Parser& _parser, State& _state, Real<double> _eo);
    RealInitBounded<Real<MinimizingFitness> > & make_genotype(Parser& _parser, State& _state, Real<MinimizingFitness> _eo);

    // the operators
    GenOp<Real<double> >&  make_op(Parser& _parser, State& _state, RealInitBounded<Real<double> >& _init);
    GenOp<Real<MinimizingFitness> >&  make_op(Parser& _parser, State& _state, RealInitBounded<Real<MinimizingFitness> >& _init);

    //Representation INdependent
    ////////////////////////////
    // you don't need to modify that part even if you use your own representation

    // init pop
    Pop<Real<double> >&  make_pop(Parser& _parser, State& _state, Init<Real<double> >&);
    Pop<Real<MinimizingFitness> >&  make_pop(Parser& _parser, State& _state, Init<Real<MinimizingFitness> >&);

    // the continue's
    Continue<Real<double> >& make_continue(Parser& _parser, State& _state, EvalFuncCounter<Real<double> > & _eval);
    Continue<Real<MinimizingFitness> >& make_continue(Parser& _parser, State& _state, EvalFuncCounter<Real<MinimizingFitness> > & _eval);

    // the checkpoint
    CheckPoint<Real<double> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<Real<double> >& _eval, Continue<Real<double> >& _continue);
    CheckPoint<Real<MinimizingFitness> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<Real<MinimizingFitness> >& _eval, Continue<Real<MinimizingFitness> >& _continue);


    // the algo
    Algo<Real<double> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<Real<double> >& _eval, Continue<Real<double> >& _ccontinue, GenOp<Real<double> >& _op, Distance<Real<double> >* _dist = NULL);

    Algo<Real<MinimizingFitness> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<Real<MinimizingFitness> >& _eval, Continue<Real<MinimizingFitness> >& _ccontinue, GenOp<Real<MinimizingFitness> >& _op, Distance<Real<MinimizingFitness> >* _dist = NULL);

    // run
    void run_ea(Algo<Real<double> >& _ga, Pop<Real<double> >& _pop);
    void run_ea(Algo<Real<MinimizingFitness> >& _ga, Pop<Real<MinimizingFitness> >& _pop);

    // end of parameter input (+ .status + help)
    // that one is not templatized
    // Because of that, the source is in src/utils dir
    void make_help(Parser & _parser);

    /** @} */

}

#endif
