// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// make_checkpoint.h
// (c) Maarten Keijzer, Marc Schoenauer and GeNeura Team, 2000
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

#ifndef _make_checkpoint_h
#define _make_checkpoint_h

#include <climits>

#include <ScalarFitness.h>
#include <utils/selectors.h> // for minimizing_fitness()
#include <EO.h>
#include <EvalFuncCounter.h>
#include <utils/checkpointing>

namespace eo
{

    // at the moment, in utils/make_help.cpp
    // this should become some eoUtils.cpp with corresponding eoUtils.h
    bool testDirRes(std::string _dirName, bool _erase);
    /////////////////// The checkpoint and other I/O //////////////


    /**
     * @ingroup Builders
     */
    template <class EOT>
    CheckPoint<EOT>& do_make_checkpoint(Parser& _parser, State& _state, EvalFuncCounter<EOT>& _eval, Continue<EOT>& _continue)
    {
	// first, create a checkpoint from the Continue
	CheckPoint<EOT> *checkpoint = new CheckPoint<EOT>(_continue);
	_state.storeFunctor(checkpoint);

	///////////////////
	// Counters
	//////////////////
	// is nb Eval to be used as counter?
	ValueParam<bool>& useEvalParam = _parser.createParam(true, "useEval", "Use nb of eval. as counter (vs nb of gen.)", '\0', "Output");

	// Create anyway a generation-counter parameter
	ValueParam<unsigned> *generationCounter = new ValueParam<unsigned>(0, "Gen.");
	// Create an incrementor (sub-class of Updater).
	Incrementor<unsigned>* increment = new Incrementor<unsigned>(generationCounter->value());
	// Add it to the checkpoint,
	checkpoint->add(*increment);
	// and store it in the state
	_state.storeFunctor(increment);

	// dir for DISK output
	ValueParam<std::string>& dirNameParam =  _parser.createParam(std::string("Res"), "resDir", "Directory to store DISK outputs", '\0', "Output - Disk");
	// shoudl we empty it if exists
	ValueParam<bool>& eraseParam = _parser.createParam(false, "eraseDir", "erase files in dirName if any", '\0', "Output - Disk");
	bool dirOK = false;		   // not tested yet

	/////////////////////////////////////////
	// now some statistics on the population:
	/////////////////////////////////////////
	/**
	 * existing stats as of today, April 10. 2001
	 *
	 * BestFitnessStat : best value in pop - type EOT::Fitness
	 * AverageStat     : average value in pop - type EOT::Fitness
	 * SecondMomentStat: average + stdev - type std::pair<double, double>
	 * SortedPopStat   : whole population - type std::string (!!)
	 * ScalarFitnessStat: the fitnesses - type std::vector<double>
	 * eoDFCSTat         : FDC wrt best in pop or absolute best - type double
	 *                     requires an Distance. See FDCStat.h
	 *                     also computes all elements for the FDC scatter plot
	 */

	// Best fitness in population
	//---------------------------
	ValueParam<bool>& printBestParam = _parser.createParam(true, "printBestStat", "Print Best/avg/stdev every gen.", '\0', "Output");
	ValueParam<bool>& plotBestParam = _parser.createParam(false, "plotBestStat", "Plot Best/avg Stat", '\0', "Output - Graphical");
	ValueParam<bool>& fileBestParam = _parser.createParam(false, "fileBestStat", "Output bes/avg/std to file", '\0', "Output - Disk");

	BestFitnessStat<EOT> *bestStat = NULL;
	if ( printBestParam.value() || plotBestParam.value() || fileBestParam.value() )
	    // we need the bestStat for at least one of the 3 above
	    {
		bestStat = new BestFitnessStat<EOT>;
		// store it
		_state.storeFunctor(bestStat);
		// add it to the checkpoint
		checkpoint->add(*bestStat);
	    }

	// Average fitness alone
	//----------------------
	AverageStat<EOT> *averageStat = NULL; // do we need averageStat?
	if ( plotBestParam.value() ) // we need it for gnuplot output
	    {
		averageStat = new AverageStat<EOT>;
		// store it
		_state.storeFunctor(averageStat);
		// add it to the checkpoint
		checkpoint->add(*averageStat);
	    }

	// Second moment stats: average and stdev
	//---------------------------------------
	SecondMomentStats<EOT> *secondStat = NULL;
	if ( printBestParam.value() ) // we need it for sreen output
	    {
		secondStat = new SecondMomentStats<EOT>;
		// store it
		_state.storeFunctor(secondStat);
		// add it to the checkpoint
		checkpoint->add(*secondStat);
	    }


	// Dump of the whole population
	//-----------------------------
	SortedPopStat<EOT> *popStat = NULL;
	ValueParam<bool>& printPopParam = _parser.createParam(false, "printPop", "Print sorted pop. every gen.", '\0', "Output");
	if ( printPopParam.value() ) // we do want pop dump
	    {
		popStat = new SortedPopStat<EOT>("Dump of whole population");
		// store it
		_state.storeFunctor(popStat);
		// add it to the checkpoint
		checkpoint->add(*popStat);
	    }


	// the Fitness Distance Correlation
	//---------------------------------
	ValueParam<bool>& printFDCParam = _parser.createParam(true, "printFDC", "Print FDC coeff. every gen.", '\0', "Output");
	ValueParam<bool>& plotFDCParam = _parser.createParam(false, "plotFDCStat", "Plot FDC scatter plot", '\0', "Output - Graphical");

	FDCStat<EOT> *fdcStat = NULL;
	if ( printFDCParam.value() || plotFDCParam.value() ) // we need FDCStat
	    {
		// need first an object to compute the distances - here Hamming dist.
		QuadDistance<EOT> *dist = new QuadDistance<EOT>;
		_state.storeFunctor(dist);
		fdcStat = new FDCStat<EOT>(*dist);
		// storeFunctor it
		_state.storeFunctor(fdcStat);
		// add it to the checkpoint
		checkpoint->add(*fdcStat);
	    }

	// do we wnat some histogram of fitnesses snpashots?
	ValueParam<bool> plotHistogramParam = _parser.createParam(false, "plotHisto", "Plot histogram of fitnesses", '\0', "Output - Graphical");

	///////////////
	// The monitors
	///////////////
	// do we want an StdoutMonitor?
	bool needStdoutMonitor = printBestParam.value() || printFDCParam.value()
	    || printPopParam.value() ;

	// The Stdout monitor will print parameters to the screen ...
	if ( needStdoutMonitor )
	    {
		StdoutMonitor *monitor = new StdoutMonitor(false);
		_state.storeFunctor(monitor);

		// when called by the checkpoint (i.e. at every generation)
		checkpoint->add(*monitor);

		// the monitor will output a series of parameters: add them
		monitor->add(*generationCounter);
		if (useEvalParam.value()) // we want nb of evaluations
		    monitor->add(_eval);
		if (printBestParam.value())
		    {
			monitor->add(*bestStat);
			monitor->add(*secondStat);
		    }
		if (printFDCParam.value())
		    monitor->add(*fdcStat);
		if ( printPopParam.value())
		    monitor->add(*popStat);
	    }

	// first handle the dir test - if we need at least one file
	if ( ( fileBestParam.value() || plotBestParam.value() ||
	       plotFDCParam.value() || plotHistogramParam.value() )
	     && !dirOK )		   // just in case we add something before
	    dirOK = testDirRes(dirNameParam.value(), eraseParam.value()); // TRUE

	if (fileBestParam.value())    // A file monitor for best & secondMoment
	    {
		std::string stmp = dirNameParam.value() + "/best.xg";
		FileMonitor *fileMonitor = new FileMonitor(stmp);
		// save and give to checkpoint
		_state.storeFunctor(fileMonitor);
		checkpoint->add(*fileMonitor);
		// and feed with some statistics
		fileMonitor->add(*generationCounter);
		fileMonitor->add(_eval);
		fileMonitor->add(*bestStat);
		fileMonitor->add(*secondStat);
	    }

	if (plotBestParam.value())    // an Gnuplot1DMonitor for best & average
	    {
		std::string stmp = dirNameParam.value() + "_gnu_best.xg";
		Gnuplot1DMonitor *gnuMonitor = new Gnuplot1DMonitor(stmp,minimizing_fitness<EOT>());
		// save and give to checkpoint
		_state.storeFunctor(gnuMonitor);
		checkpoint->add(*gnuMonitor);
		// and feed with some statistics
		if (useEvalParam.value())
		    gnuMonitor->add(_eval);
		else
		    gnuMonitor->add(*generationCounter);
		gnuMonitor->add(*bestStat);
		gnuMonitor->add(*averageStat);
	    }

	if (plotFDCParam.value())    // a specific plot monitor for FDC
	    {
		// first into a file (it adds everything ti itself
		FDCFileSnapshot<EOT> *fdcFileSnapshot = new FDCFileSnapshot<EOT>(*fdcStat, dirNameParam.value());
		_state.storeFunctor(fdcFileSnapshot);
		// then to a Gnuplot monitor
		Gnuplot1DSnapshot *fdcGnuplot = new Gnuplot1DSnapshot(*fdcFileSnapshot);
		_state.storeFunctor(fdcGnuplot);

		// and of course add them to the checkPoint
		checkpoint->add(*fdcFileSnapshot);
		checkpoint->add(*fdcGnuplot);
	    }

	// historgram?
	if (plotHistogramParam.value()) // want to see how the fitness is spread?
	    {
		ScalarFitnessStat<EOT> *fitStat = new ScalarFitnessStat<EOT>;
		_state.storeFunctor(fitStat);
		checkpoint->add(*fitStat);
		// a gnuplot-based monitor for snapshots: needs a dir name
		Gnuplot1DSnapshot *fitSnapshot = new Gnuplot1DSnapshot(dirNameParam.value());
		_state.storeFunctor(fitSnapshot);
		// add any stat that is a std::vector<double> to it
		fitSnapshot->add(*fitStat);
		// and of course add it to the checkpoint
		checkpoint->add(*fitSnapshot);
	    }

	//////////////////////////////////
	// State savers
	//////////////////////////////

	// feed the state to state savers
	// save state every N  generation
	ValueParam<unsigned>& saveFrequencyParam = _parser.createParam(unsigned(0), "saveFrequency", "Save every F generation (0 = only final state, absent = never)", '\0', "Persistence" );

	if (_parser.isItThere(saveFrequencyParam))
	    {
		// first make sure dirName is OK
		if (! dirOK )
		    dirOK = testDirRes(dirNameParam.value(), eraseParam.value()); // TRUE

		unsigned freq = (saveFrequencyParam.value()>0 ? saveFrequencyParam.value() : UINT_MAX );
		std::string stmp = dirNameParam.value() + "/generations";
		CountedStateSaver *stateSaver1 = new CountedStateSaver(freq, _state, stmp);
		_state.storeFunctor(stateSaver1);
		checkpoint->add(*stateSaver1);
	    }

	// save state every T seconds
	ValueParam<unsigned>& saveTimeIntervalParam = _parser.createParam(unsigned(0), "saveTimeInterval", "Save every T seconds (0 or absent = never)", '\0',"Persistence" );
	if (_parser.isItThere(saveTimeIntervalParam) && saveTimeIntervalParam.value()>0)
	    {
		// first make sure dirName is OK
		if (! dirOK )
		    dirOK = testDirRes(dirNameParam.value(), eraseParam.value()); // TRUE

		std::string stmp = dirNameParam.value() + "/time";
		TimedStateSaver *stateSaver2 = new TimedStateSaver(saveTimeIntervalParam.value(), _state, stmp);
		_state.storeFunctor(stateSaver2);
		checkpoint->add(*stateSaver2);
	    }

	// and that's it for the (control and) output
	return *checkpoint;
    }

}

#endif
