/*

    This program is free software; you can redistribute it and/or modify

    it under the terms of the GNU General Public License as published by

    the Free Software Foundation; either version 2 of the License, or

    (at your option) any later version.



    This library is distributed in the hope that it will be useful,

    but WITHOUT ANY WARRANTY; without even the implied warranty of

    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU

    Lesser General Public License for more details.



    You should have received a copy of the GNU General Public License

    along with this library; if not, write to the Free Software

    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



    Contact: todos@geneura.ugr.es, http://geneura.ugr.es

             jeggermo@liacs.nl

*/



#ifdef _MSC_VER

#pragma warning(disable:4786)

#endif



#ifdef HAVE_CONFIG_H

#include <config.h>

#endif



#include <iostream>

#include "gp/ParseTree.h"

#include "eo"



using namespace gp_parse_tree;

using namespace std;



//-----------------------------------------------------------------------------



#include "node.h"

#include "parameters.h"

#include "fitness.h"





// TYPE DECLARATIONS FOR GP





typedef ParseTree<FitnessType, Node > EoType;

typedef Pop<EoType> Pop;



//-----------------------------------------------------------------------------



int main(int argc, char *argv[])

{



	// the vector containing the possible nodes

	vector<Node> initSequence;



	// initialise parameters

	Parameters parameter(argc, argv);



	// set the randomseed

	rng.reseed(parameter.randomseed);



	 // Create a generation counter

    	ValueParam<unsigned> generationCounter(0, "Gen.");



    	// Create an incrementor (sub-class of Updater). Note that the

    	// parameter's value is passed by reference,

    	// so every time the incrementer is updated (every generation),

    	// the data in generationCounter will change.

    	Incrementor<unsigned> increment(generationCounter.value());





	// create an instantiation of the fitness/evaluation function

	// it initializes the initSequence vector

	// the parameters are passed on as well

        RegFitness eval(generationCounter, initSequence, parameter);



	// Depth Initializor, set for Ramped Half and Half Initialization

        ParseTreeDepthInit<FitnessType, Node> initializer(parameter.InitMaxDepth, initSequence, true, true);



	// create the initial population

        Pop pop(parameter.population_size, initializer);



	// and evaluate the individuals

        apply<EoType>(eval, pop);



	generationCounter.value()++; // set the generationCounter to 1





    	// define X-OVER



	SubtreeXOver<FitnessType, Node>   xover(parameter.MaxSize);



	// define MUTATION

      eoBranchMutation<FitnessType, Node> mutation(initializer, parameter.MaxSize);

//      ExpansionMutation<FitnessType, Node> mutation(initializer, parameter.MaxSize);

//	CollapseSubtreeMutation<FitnessType, Node> mutation(initializer, parameter.MaxSize);

//	PointMutation<FitnessType, Node> mutation(initSequence);

//	HoistMutation<FitnessType, Node> mutation;



	// The operators are  encapsulated into an eoTRansform object,

    	// that performs sequentially crossover and mutation

	SGATransform<EoType> transform(xover, parameter.xover_rate, mutation, parameter.mutation_rate);



	// The robust tournament selection

	// in our case 5-tournament selection

    	DetTournamentSelect<EoType> selectOne(parameter.tournamentsize);

	// is now encapsulated in a SelectMany

	SelectMany<EoType> select(selectOne, parameter.offspring_size, eo_is_an_integer);



	// and the generational replacement

    	//GenerationalReplacement<EoType> replace;

	// or the SteadtState replacment

	//SSGAWorseReplacement<EoType> replace;

	// or comma selection

	CommaReplacement<EoType> replace;



    	// Terminators

    	GenContinue<EoType> term(parameter.nGenerations);



    	CheckPoint<EoType> checkPoint(term);



	// STATISTICS

    	AverageStat<EoType>     avg;

    	BestFitnessStat<EoType> best;





    	// Add it to the checkpoint,

    	// so the counter is updated (here, incremented) every generation

    	checkPoint.add(increment);

	checkPoint.add(avg);

	checkPoint.add(best);



#ifdef HAVE_GNUPLOT

	Gnuplot1DMonitor gnuplotmonitor("gnuplotBestStats");

  	gnuplotmonitor.add(generationCounter);

	gnuplotmonitor.add(best);

	// we need to add a empty string variable if we want to seed the second fitness value

	ValueParam<string> dummy1("", "Smallest Tree Size");

	gnuplotmonitor.add(dummy1);



	Gnuplot1DMonitor gnuplotAvgmonitor("gnuplotAvgStats");

	gnuplotAvgmonitor.add(generationCounter);

	gnuplotAvgmonitor.add(avg);

	// we need to add a empty string variable if we want to seed the second fitness value

	ValueParam<string> dummy2("", "Average Tree Size");

	gnuplotAvgmonitor.add(dummy2);



	checkPoint.add(gnuplotmonitor);

  	checkPoint.add(gnuplotAvgmonitor);

#endif

	// GP Generation

	EasyEA<EoType> gp(checkPoint, eval, select, transform, replace);



    	cout << "Initialization done" << endl;





    	try

    	{

      		gp(pop);

    	}

    	catch (exception& e)

    	{

	    cout << "exception: " << e.what() << endl;;

	    exit(EXIT_FAILURE);

    	}



    	return 1;



}







