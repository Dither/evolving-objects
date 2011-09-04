// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// ga.h
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
 * of the library for ***BISTRING*** evolution inside EO.
 * It should be included in the file that calls any of the corresponding fns
 *
 * The corresponding ***INSTANCIATED*** definitions are contained in ga.cpp
 * while the TEMPLATIZED code is define in the different makeXXX.h files
 *
 * Unlike most EO .h files, it does not (and should not) contain any code,
 * just declarations
 */

#ifndef ga_h
#define ga_h

#include <Algo.h>
#include <ScalarFitness.h>
#include <utils/Parser.h>
#include <EvalFuncCounter.h>
#include <utils/CheckPoint.h>
#include <GenOp.h>
#include <Pop.h>
#include <utils/Distance.h>

#include <ga/Bit.h>

namespace eo
{

    //Representation dependent - rewrite everything anew for each representation
    //////////////////////////

    /** @addtogroup Builders
     * @{
     */

    // the genotypes 
    Init<Bit<double> > & make_genotype(Parser& _parser, State& _state, Bit<double> _eo);
    Init<Bit<MinimizingFitness> > & make_genotype(Parser& _parser, State& _state, Bit<MinimizingFitness> _eo);

    // the operators
    GenOp<Bit<double> >&  make_op(Parser& _parser, State& _state, Init<Bit<double> >& _init);
    GenOp<Bit<MinimizingFitness> >&  make_op(Parser& _parser, State& _state, Init<Bit<MinimizingFitness> >& _init);

    //Representation INdependent
    ////////////////////////////
    // if you use your own representation, simply change the types of templates

    // init pop
    Pop<Bit<double> >&  make_pop(Parser& _parser, State& _state, Init<Bit<double> >&);
    Pop<Bit<MinimizingFitness> >&  make_pop(Parser& _parser, State& _state, Init<Bit<MinimizingFitness> >&);

    // the continue's
    Continue<Bit<double> >& make_continue(Parser& _parser, State& _state, EvalFuncCounter<Bit<double> > & _eval);
    Continue<Bit<MinimizingFitness> >& make_continue(Parser& _parser, State& _state, EvalFuncCounter<Bit<MinimizingFitness> > & _eval);

    // the checkpoint
    CheckPoint<Bit<double> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<Bit<double> >& _eval, Continue<Bit<double> >& _continue);
    CheckPoint<Bit<MinimizingFitness> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<Bit<MinimizingFitness> >& _eval, Continue<Bit<MinimizingFitness> >& _continue);


    // the algo
    Algo<Bit<double> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<Bit<double> >& _eval, Continue<Bit<double> >& _ccontinue, GenOp<Bit<double> >& _op, Distance<Bit<double> >* _dist = NULL);

    Algo<Bit<MinimizingFitness> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<Bit<MinimizingFitness> >& _eval, Continue<Bit<MinimizingFitness> >& _ccontinue, GenOp<Bit<MinimizingFitness> >& _op, Distance<Bit<MinimizingFitness> >* _dist = NULL);

    // run
    void run_ea(Algo<Bit<double> >& _ga, Pop<Bit<double> >& _pop);
    void run_ea(Algo<Bit<MinimizingFitness> >& _ga, Pop<Bit<MinimizingFitness> >& _pop);

    // end of parameter input (+ .status + help)
    // that one is not templatized
    // Because of that, the source is in src/utils dir
    void make_help(Parser & _parser);

}

/** @} */
#endif
