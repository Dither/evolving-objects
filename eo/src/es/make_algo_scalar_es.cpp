// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// make_algo_scalar_es.cpp
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

#ifdef _MSC_VER
// to avoid long name warnings
#pragma warning(disable:4786)
#endif 

/** This file contains ***INSTANCIATED DEFINITIONS*** of select/replace fns
 * of the library for evolution of ***eoEs genotypes*** inside EO.
 * It should be included in the file that calls any of the corresponding fns
 * Compiling this file allows one to generate part of the library (i.e. object
 * files that you just need to link with your own main and fitness code).
 *
 * The corresponding ***INSTANCIATED DECLARATIONS*** are contained 
 *       in src/es/es.h
 * while the TEMPLATIZED code is define in make_algo_scalar.h in the src/do dir
 */

// The templatized code
#include <do/make_algo_scalar.h>
// the instanciating EOType(s)
#include <es/EsSimple.h>	   // one Sigma per individual
#include <es/EsStdev.h>	   // one sigmal per object variable
#include <es/EsFull.h>	   // full correlation matrix per indi

namespace eo
{

    /// The following function merely call the templatized do_* functions above

    // Algo
    ///////
    Algo<EsSimple<double> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<EsSimple<double> >& _eval, Continue<EsSimple<double> >& _continue, GenOp<EsSimple<double> >& _op, Distance<EsSimple<double> >* _dist)
    {
	return do_make_algo_scalar(_parser, _state, _eval, _continue, _op);
    }

    Algo<EsSimple<MinimizingFitness> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<EsSimple<MinimizingFitness> >& _eval, Continue<EsSimple<MinimizingFitness> >& _continue, GenOp<EsSimple<MinimizingFitness> >& _op, Distance<EsSimple<MinimizingFitness> >* _dist)
    {
	return do_make_algo_scalar(_parser, _state, _eval, _continue, _op);
    }

    //////////////
    Algo<EsStdev<double> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<EsStdev<double> >& _eval, Continue<EsStdev<double> >& _continue, GenOp<EsStdev<double> >& _op, Distance<EsStdev<double> >* _dist)
    {
	return do_make_algo_scalar(_parser, _state, _eval, _continue, _op);
    }

    Algo<EsStdev<MinimizingFitness> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<EsStdev<MinimizingFitness> >& _eval, Continue<EsStdev<MinimizingFitness> >& _continue, GenOp<EsStdev<MinimizingFitness> >& _op, Distance<EsStdev<MinimizingFitness> >* _dist)
    {
	return do_make_algo_scalar(_parser, _state, _eval, _continue, _op);
    }

    ///////////////
    Algo<EsFull<double> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<EsFull<double> >& _eval, Continue<EsFull<double> >& _continue, GenOp<EsFull<double> >& _op, Distance<EsFull<double> >* _dist)
    {
	return do_make_algo_scalar(_parser, _state, _eval, _continue, _op);
    }

    Algo<EsFull<MinimizingFitness> >&  make_algo_scalar(Parser& _parser, State& _state, EvalFunc<EsFull<MinimizingFitness> >& _eval, Continue<EsFull<MinimizingFitness> >& _continue, GenOp<EsFull<MinimizingFitness> >& _op, Distance<EsFull<MinimizingFitness> >* _dist)
    {
	return do_make_algo_scalar(_parser, _state, _eval, _continue, _op);
    }

}
