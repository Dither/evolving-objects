// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// make_checkpoint_es.cpp
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

/** This file contains ***INSTANCIATED DEFINITIONS*** of checkpoint fns
 * of the library for evolution of ***ES genotypes*** inside EO.
 * It should be included in the file that calls any of the corresponding fns
 * Compiling this file allows one to generate part of the library (i.e. object
 * files that you just need to link with your own main and fitness code).
 *
 * The corresponding ***INSTANCIATED DECLARATIONS*** are contained
 *       in src/es/es.h
 * while the TEMPLATIZED code is define in make_checkpoint.h in the src/do dir
 */

// The templatized code
#include <do/make_checkpoint.h>
// the instanciating EOType(s)
#include <es/EsSimple.h>	   // one Sigma per individual
#include <es/EsStdev.h>	   // one sigmal per object variable
#include <es/EsFull.h>	   // full correlation matrix per indi

namespace eo
{

    /// The following function merely call the templatized do_* functions 

    // checkpoint
    /////////////
    CheckPoint<EsSimple<double> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<EsSimple<double> >& _eval, Continue<EsSimple<double> >& _continue)
    {
	return do_make_checkpoint(_parser, _state, _eval, _continue);
    }
    CheckPoint<EsSimple<MinimizingFitness> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<EsSimple<MinimizingFitness> >& _eval, Continue<EsSimple<MinimizingFitness> >& _continue) 
    {
	return do_make_checkpoint(_parser, _state, _eval, _continue);
    }

    /////////////
    CheckPoint<EsStdev<double> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<EsStdev<double> >& _eval, Continue<EsStdev<double> >& _continue)
    {
	return do_make_checkpoint(_parser, _state, _eval, _continue);
    }
    CheckPoint<EsStdev<MinimizingFitness> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<EsStdev<MinimizingFitness> >& _eval, Continue<EsStdev<MinimizingFitness> >& _continue) 
    {
	return do_make_checkpoint(_parser, _state, _eval, _continue);
    }

    /////////////
    CheckPoint<EsFull<double> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<EsFull<double> >& _eval, Continue<EsFull<double> >& _continue)
    {
	return do_make_checkpoint(_parser, _state, _eval, _continue);
    }
    CheckPoint<EsFull<MinimizingFitness> >& make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<EsFull<MinimizingFitness> >& _eval, Continue<EsFull<MinimizingFitness> >& _continue) 
    {
	return do_make_checkpoint(_parser, _state, _eval, _continue);
    }

}
