// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// make_run_es.cpp
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

/** This file contains ***INSTANCIATED DEFINITIONS*** of run funs
 * of the library for evolution of  ***ES genotypes***  inside EO.
 * It should be included in the file that calls any of the corresponding fns
 * Compiling this file allows one to generate part of the library (i.e. object
 * files that you just need to link with your own main and fitness code).
 *
 * The corresponding ***INSTANCIATED DECLARATIONS*** are contained 
 *       in src/es/es.h
 * while the TEMPLATIZED code is define in make_run.h in the src/do dir
 */

// The templatized code
#include <do/make_run.h>
// the instanciating EOType(s)
#include <es/EsSimple.h>	   // one Sigma per individual
#include <es/EsStdev.h>	   // one sigmal per object variable
#include <es/EsFull.h>	   // full correlation matrix per indi
// the instanciating fitnesses
#include <ScalarFitness.h>

namespace eo
{

    /// The following function merely call the templatized do_* functions above

    // run
    /////////
    void run_ea(Algo<EsSimple<double> >& _ga, Pop<EsSimple<double> >& _pop)
    {
	do_run(_ga, _pop);
    }

    void run_ea(Algo<EsSimple<MinimizingFitness> >& _ga, Pop<EsSimple<MinimizingFitness> >& _pop)
    {
	do_run(_ga, _pop);
    }

    /////////
    void run_ea(Algo<EsStdev<double> >& _ga, Pop<EsStdev<double> >& _pop)
    {
	do_run(_ga, _pop);
    }

    void run_ea(Algo<EsStdev<MinimizingFitness> >& _ga, Pop<EsStdev<MinimizingFitness> >& _pop)
    {
	do_run(_ga, _pop);
    }

    /////////
    void run_ea(Algo<EsFull<double> >& _ga, Pop<EsFull<double> >& _pop)
    {
	do_run(_ga, _pop);
    }

    void run_ea(Algo<EsFull<MinimizingFitness> >& _ga, Pop<EsFull<MinimizingFitness> >& _pop)
    {
	do_run(_ga, _pop);
    }

}
