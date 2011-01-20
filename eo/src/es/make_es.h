// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// es.h
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
 * of the library for ***ES-like gnptype*** evolution inside EO.
 * It should be included in the file that calls any of the corresponding fns
 *
 * The corresponding ***INSTANCIATED*** definitions are contained in 
 * the different .cpp files in the src/es dir, 
 * while the TEMPLATIZED code is define in the different make_XXX.h files
 * either in hte src/do dir for representation independant functions, 
 * or in the src/es dir for representation dependent stuff.
 *
 * See also real.h for the similar declarations of Real genotypes 
 *   i.e. ***without*** mutation parameters attached to individuals
 *
 * Unlike most EO .h files, it does not (and should not) contain any code, 
 * just declarations
 */

#ifndef es_h
#define es_h

#include <Algo.h>
#include <ScalarFitness.h>
#include <utils/Parser.h>
#include <EvalFuncPtr.h>
#include <EvalFuncCounter.h>
#include <utils/CheckPoint.h>
#include <GenOp.h>
#include <Pop.h>
#include <utils/Distance.h>

#include <es/EsSimple.h>	   // one Sigma per individual
#include <es/EsStdev.h>	   // one sigmal per object variable
#include <es/EsFull.h>	   // full correlation matrix per indi

// include all similar declaration for Real - i.e. real-valued genotyes
// without self-adaptation
#include <es/make_real.h>

namespace eo
{

    /** @addtogroup Builders
     * @{
     */

    //Representation dependent - rewrite everything anew for each representation
    //////////////////////////
    // the genotypes 
    RealInitBounded<EsSimple<double> > & make_genotype(Parser& _parser, State& _state, EsSimple<double> _eo);
    RealInitBounded<EsSimple<MinimizingFitness> > & make_genotype(Parser& _parser, State& _state, EsSimple<MinimizingFitness> _eo);

    RealInitBounded<EsStdev<double> > & make_genotype(Parser& _parser, State& _state, EsStdev<double> _eo);
    RealInitBounded<EsStdev<MinimizingFitness> > & make_genotype(Parser& _parser, State& _state, EsStdev<MinimizingFitness> _eo);

    RealInitBounded<EsFull<double> > & make_genotype(Parser& _parser, State& _state, EsFull<double> _eo);
    RealInitBounded<EsFull<MinimizingFitness> > & make_genotype(Parser& _parser, State& _state, EsFull<MinimizingFitness> _eo);



    // the operators
    GenOp<EsSimple<double> >&  make_op(Parser& _parser, State& _state, RealInitBounded<EsSimple<double> >& _init);
    GenOp<EsSimple<MinimizingFitness> >&  make_op(Parser& _parser, State& _state, RealInitBounded<EsSimple<MinimizingFitness> >& _init);
    GenOp<EsStdev<double> >&  make_op(Parser& _parser, State& _state, RealInitBounded<EsStdev<double> >& _init);
    GenOp<EsStdev<MinimizingFitness> >&  make_op(Parser& _parser, State& _state, RealInitBounded<EsStdev<MinimizingFitness> >& _init);
    GenOp<EsFull<double> >&  make_op(Parser& _parser, State& _state, RealInitBounded<EsFull<double> >& _init);
    GenOp<EsFull<MinimizingFitness> >&  make_op(Parser& _parser, State& _state, RealInitBounded<EsFull<MinimizingFitness> >& _init);

    //Representation INdependent
    ////////////////////////////
    // you don't need to modify that part even if you use your own representation

    // init pop
    Pop<EsSimple<double> >&  make_pop(Parser& _parser, State& _state, Init<EsSimple<double> >&);
    Pop<EsSimple<MinimizingFitness> >&  make_pop(Parser& _parser, State& _state, Init<EsSimple<MinimizingFitness> >&);

    Pop<EsStdev<double> >&  make_pop(Parser& _parser, State& _state, Init<EsStdev<double> >&);
    Pop<EsStdev<MinimizingFitness> >&  make_pop(Parser& _parser, State& _state, Init<EsStdev<MinimizingFitness> >&);

    Pop<EsFull<double> >&  make_pop(Parser& _parser, State& _state, Init<EsFull<double> >&);
    Pop<EsFull<MinimizingFitness> >&  make_pop(Parser& _parser, State& _state, Init<EsFull<MinimizingFitness> >&);

    // the continue's
    Continue<EsSimple<double> >& make_continue(Parser& _parser, State& _state, EvalFuncCounter<EsSimple<double> > & _eval);
    Continue<EsSimple<MinimizingFitness> >& make_continue(Parser& _parser, State& _state, EvalFuncCounter<EsSimple<MinimizingFitness> > & _eval);

    Continue<EsStdev<double> >& make_continue(Parser& _parser, State& _state, EvalFuncCounter<EsStdev<double> > & _eval);
    Continue<EsStdev<MinimizingFitness> >& make_continue(Parser& _parser, State& _state, EvalFuncCounter<EsStdev<MinimizingFitness> > & _eval);

    Continue<EsFull<double> >& make_continue(Parser& _parser, State& _state, EvalFuncCounter<EsFull<double> > & _eval);
    Continue<EsFull<MinimizingFitness> >& make_continue(Parser& _parser, State& _state, EvalFuncCounter<EsFull<MinimizingFitness> > & _eval);

    // the checkpoint
    CheckPoint<EsSimple<double> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<EsSimple<double> >& _eval, Continue<EsSimple<double> >& _continue);
    CheckPoint<EsSimple<MinimizingFitness> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<EsSimple<MinimizingFitness> >& _eval, Continue<EsSimple<MinimizingFitness> >& _continue);

    CheckPoint<EsStdev<double> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<EsStdev<double> >& _eval, Continue<EsStdev<double> >& _continue);
    CheckPoint<EsStdev<MinimizingFitness> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<EsStdev<MinimizingFitness> >& _eval, Continue<EsStdev<MinimizingFitness> >& _continue);

    CheckPoint<EsFull<double> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<EsFull<double> >& _eval, Continue<EsFull<double> >& _continue);
    CheckPoint<EsFull<MinimizingFitness> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<EsFull<MinimizingFitness> >& _eval, Continue<EsFull<MinimizingFitness> >& _continue);


    // the algo
    Algo<EsSimple<double> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<EsSimple<double> >& _eval, Continue<EsSimple<double> >& _ccontinue, GenOp<EsSimple<double> >& _op, Distance<EsSimple<double> >* _dist = NULL);
    Algo<EsSimple<MinimizingFitness> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<EsSimple<MinimizingFitness> >& _eval, Continue<EsSimple<MinimizingFitness> >& _ccontinue, GenOp<EsSimple<MinimizingFitness> >& _op, Distance<EsSimple<MinimizingFitness> >* _dist = NULL);

    Algo<EsStdev<double> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<EsStdev<double> >& _eval, Continue<EsStdev<double> >& _ccontinue, GenOp<EsStdev<double> >& _op, Distance<EsStdev<double> >* _dist = NULL);
    Algo<EsStdev<MinimizingFitness> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<EsStdev<MinimizingFitness> >& _eval, Continue<EsStdev<MinimizingFitness> >& _ccontinue, GenOp<EsStdev<MinimizingFitness> >& _op, Distance<EsStdev<MinimizingFitness> >* _dist = NULL);

    Algo<EsFull<double> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<EsFull<double> >& _eval, Continue<EsFull<double> >& _ccontinue, GenOp<EsFull<double> >& _op, Distance<EsFull<double> >* _dist = NULL);
    Algo<EsFull<MinimizingFitness> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<EsFull<MinimizingFitness> >& _eval, Continue<EsFull<MinimizingFitness> >& _ccontinue, GenOp<EsFull<MinimizingFitness> >& _op, Distance<EsFull<MinimizingFitness> >* _dist = NULL);

    // run
    void run_ea(Algo<EsSimple<double> >& _ga, Pop<EsSimple<double> >& _pop);
    void run_ea(Algo<EsSimple<MinimizingFitness> >& _ga, Pop<EsSimple<MinimizingFitness> >& _pop);

    void run_ea(Algo<EsStdev<double> >& _ga, Pop<EsStdev<double> >& _pop);
    void run_ea(Algo<EsStdev<MinimizingFitness> >& _ga, Pop<EsStdev<MinimizingFitness> >& _pop);

    void run_ea(Algo<EsFull<double> >& _ga, Pop<EsFull<double> >& _pop);
    void run_ea(Algo<EsFull<MinimizingFitness> >& _ga, Pop<EsFull<MinimizingFitness> >& _pop);

    // end of parameter input (+ .status + help)
    // that one is not templatized, but is here for completeness
    void make_help(Parser & _parser);

}

/** @} */
/** @} */
#endif
