 /* -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*- */

//-----------------------------------------------------------------------------
// make_checkpoint_assembled.h
// Marc Wintermantel & Oliver Koenig
// IMES-ST@ETHZ.CH
// March 2003

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
             Marc.Schoenauer@inria.fr
             mak@dhi.dk
*/
//-----------------------------------------------------------------------------

#ifndef _make_checkpoint_assembled_h
#define _make_checkpoint_assembled_h

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <climits>
#include <vector>
#include <string>

#include <ScalarFitnessAssembled.h>
#include <utils/selectors.h>
#include <EO.h>
#include <EvalFuncCounter.h>
#include <utils/checkpointing>

namespace eo
{

    // at the moment, in utils/make_help.cpp
    // this should become some eoUtils.cpp with corresponding eoUtils.h
    bool testDirRes(std::string _dirName, bool _erase);
    /////////////////// The checkpoint and other I/O //////////////

    /** Of course, Fitness needs to be an ScalarFitnessAssembled!!! 
     *
     *
     * @ingroup Builders
     * */
    template <class EOT>
    CheckPoint<EOT>& do_make_checkpoint_assembled(Parser& _parser, State& _state, EvalFuncCounter<EOT>& _eval, Continue<EOT>& _continue)
    {

	// SOME PARSER PARAMETERS
	// ----------------------
	std::string dirName = _parser.getORcreateParam(std::string("Res"), "resDir",
						       "Directory to store DISK outputs",
						       '\0', "Output").value();
	bool erase = _parser.getORcreateParam(true, "eraseDir",
					      "Erase files in dirName if any",
					      '\0', "Output").value();
	bool gnuplots = _parser.getORcreateParam(true, "plots",
						 "Plot stuff using GnuPlot",
						 '\0', "Output").value();
	bool printFile = _parser.getORcreateParam(true, "printFile",
						  "Print statistics file",
						  '\0', "Output").value();

	ValueParam<unsigned>& saveFrequencyParam
	    = _parser.getORcreateParam(unsigned(0), "saveFrequency",
				       "Save every F generation (0 = only final state, absent = never)",
				       '\0', "Persistence" );

	testDirRes(dirName, erase); // TRUE

	// CREATE CHECKPOINT FROM Continue
	// ---------------------------------
	CheckPoint<EOT> *checkpoint = new CheckPoint<EOT>(_continue);
	_state.storeFunctor(checkpoint);

	// GENERATIONS
	// -----------
	IncrementorParam<unsigned> *generationCounter = new IncrementorParam<unsigned>("Gen.");
	_state.storeFunctor(generationCounter);
	checkpoint->add(*generationCounter);

	// TIME
	// ----
	TimeCounter * tCounter = NULL;
	tCounter = new TimeCounter;
	_state.storeFunctor(tCounter);
	checkpoint->add(*tCounter);

	// ACCESS DESCRIPTIONS OF TERMS OF FITNESS CLASS
	// ---------------------------------------------
	// define a temporary fitness instance
	typedef typename EOT::Fitness Fit;
	Fit fit;
	std::vector<std::string> fitness_descriptions = fit.getDescriptionVector();
	unsigned nTerms = fitness_descriptions.size();

	// STAT VALUES OF A POPULATION
	// ---------------------------

	// average vals
	std::vector<AssembledFitnessAverageStat<EOT>* > avgvals( nTerms );
	for (unsigned i=0; i < nTerms; ++i){
	    std::string descr = "Avg. of " + fitness_descriptions[i];
	    avgvals[i] = new AssembledFitnessAverageStat<EOT>(i, descr);
	    _state.storeFunctor( avgvals[i] );
	    checkpoint->add( *avgvals[i] );
	}

	// best vals
	std::vector<AssembledFitnessBestStat<EOT>* > bestvals( nTerms );
	for (unsigned j=0; j < nTerms; ++j){
	    std::string descr = fitness_descriptions[j] + " of best ind.";
	    bestvals[j] = new AssembledFitnessBestStat<EOT>(j, descr);
	    _state.storeFunctor( bestvals[j] );
	    checkpoint->add( *bestvals[j] );
	}

	// STDOUT
	// ------
	StdoutMonitor *monitor = new StdoutMonitor(false);
	_state.storeFunctor(monitor);
	checkpoint->add(*monitor);
	monitor->add(*generationCounter);
	monitor->add(_eval);
	monitor->add(*tCounter);

	// Add best fitness
	monitor->add( *bestvals[0] );

	// Add all average vals
	for (unsigned l=0; l < nTerms; ++l)
	    monitor->add( *avgvals[l] );

	// GNUPLOT
	// -------
	if (gnuplots ){
	    std::string stmp;

	    // Histogramm of the different fitness vals
	    ScalarFitnessStat<EOT> *fitStat = new ScalarFitnessStat<EOT>;
	    _state.storeFunctor(fitStat);
	    checkpoint->add(*fitStat);
#ifdef HAVE_GNUPLOT
	    // a gnuplot-based monitor for snapshots: needs a dir name
	    Gnuplot1DSnapshot *fitSnapshot = new Gnuplot1DSnapshot(dirName);
	    _state.storeFunctor(fitSnapshot);
	    // add any stat that is a vector<double> to it
	    fitSnapshot->add(*fitStat);
	    // and of course add it to the checkpoint
	    checkpoint->add(*fitSnapshot);

	    std::vector<Gnuplot1DMonitor*> gnumonitors(nTerms, NULL );
	    for (unsigned k=0; k < nTerms; ++k){
		stmp = dirName + "/gnuplot_" + fitness_descriptions[k] + ".xg";
		gnumonitors[k] = new Gnuplot1DMonitor(stmp,true);
		_state.storeFunctor(gnumonitors[k]);
		checkpoint->add(*gnumonitors[k]);
		gnumonitors[k]->add(*generationCounter);
		gnumonitors[k]->add(*bestvals[k]);
		gnumonitors[k]->add(*avgvals[k]);
	    }
#endif
	}

	// WRITE STUFF TO FILE
	// -------------------
	if( printFile ){
	    std::string stmp2 = dirName + "/Statistics.sav";
	    FileMonitor *fileMonitor = new FileMonitor(stmp2);
	    _state.storeFunctor(fileMonitor);
	    checkpoint->add(*fileMonitor);
	    fileMonitor->add(*generationCounter);
	    fileMonitor->add(_eval);
	    fileMonitor->add(*tCounter);

	    for (unsigned i=0; i < nTerms; ++i){
		fileMonitor->add(*bestvals[i]);
		fileMonitor->add(*avgvals[i]);
	    }

	}

	// STATE SAVER
	// -----------
	// feed the state to state savers

	if (_parser.isItThere(saveFrequencyParam)) {

	    unsigned freq = (saveFrequencyParam.value() > 0 ? saveFrequencyParam.value() : UINT_MAX );
	    std::string stmp = dirName + "/generations";
	    CountedStateSaver *stateSaver1 = new CountedStateSaver(freq, _state, stmp);
	    _state.storeFunctor(stateSaver1);
	    checkpoint->add(*stateSaver1);
	}

	// and that's it for the (control and) output
	return *checkpoint;
    }

}

#endif
