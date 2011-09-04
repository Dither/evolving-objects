//-----------------------------------------------------------------------------
// mastermind
//-----------------------------------------------------------------------------

#include <stdlib.h>                // EXIT_SUCCESS EXIT_FAILURE
#include <stdexcept>               // exception
#include <iostream>                // cerr cout
#include <fstream>                 // ifstream
#include <string>                  // string
#include <eo>                      // all usefull eo stuff

#include "mastermind.h"            // Chrom eoChromInit eoChromMutation eoChromXover eoChromEvaluator

using namespace std;

//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------

unsigned in, out, hidden;

//-----------------------------------------------------------------------------
// parameters
//-----------------------------------------------------------------------------

eo::ValueParam<unsigned> pop_size(16, "pop_size", "population size", 'p');
eo::ValueParam<unsigned> generations(100, "generations", "number of generation", 'g');
eo::ValueParam<double> mut_rate(0.1, "mut_rate", "mutation rate", 'm');
eo::ValueParam<double> xover_rate(0.5, "xover_rate", "default crossover rate", 'x');
eo::ValueParam<unsigned> col_p(default_colors, "colors", "number of colors", 'c');
eo::ValueParam<unsigned> len_p(default_length, "legth", "solution legth", 'l');
eo::ValueParam<string> sol_p(default_solution, "solution", "problem solution", 's');

//-----------------------------------------------------------------------------
// auxiliar functions
//-----------------------------------------------------------------------------

void arg(int argc, char** argv);
void ga();

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main(int argc, char** argv)
{
  try
    {
      arg(argc, argv);
      ga();
    }
  catch (exception& e)
    {
	cerr << argv[0] << ": " << e.what() << endl;
	exit(EXIT_FAILURE);
    }

  return 0;
}

//-----------------------------------------------------------------------------
// implementation
//-----------------------------------------------------------------------------

void arg(int argc, char** argv)
{
    eo::Parser parser(argc, argv);

    parser.processParam(pop_size,    "genetic operators");
    parser.processParam(generations, "genetic operators");
    parser.processParam(mut_rate,    "genetic operators");
    parser.processParam(xover_rate,  "genetic operators");
    parser.processParam(col_p,       "problem");
    parser.processParam(len_p,       "problem");
    parser.processParam(sol_p,       "problem");

    if (parser.userNeedsHelp())
	{
	    parser.printHelp(cout);
	    exit(EXIT_SUCCESS);
	}

    init_eoChromEvaluator(col_p.value(), len_p.value(), sol_p.value());
}

//-----------------------------------------------------------------------------

void ga()
{
    // create population
    eo::InitChrom init;
    eo::Pop<Chrom> pop(pop_size.value(), init);

    // evaluate population
    eo::EvalFuncPtr<Chrom> evaluator(eoChromEvaluator);
    eo::apply<Chrom>(evaluator, pop);

    // selector
    eo::ProportionalSelect<Chrom> select(pop);

    // genetic operators
    eo::ChromMutation mutation;
    eo::ChromXover xover;

    // stop condition
    eo::GenContinue<Chrom> continuator1(generations.value());
    eo::FitContinue<Chrom> continuator2(solution.fitness());
    eo::CombinedContinue<Chrom> continuator(continuator1, continuator2);

    // checkpoint
    eo::CheckPoint<Chrom> checkpoint(continuator);

    // monitor
    eo::StdoutMonitor monitor;
    checkpoint.add(monitor);

    // statistics
    eo::BestFitnessStat<Chrom> stats;
    checkpoint.add(stats);
    monitor.add(stats);

    // genetic algorithm
    eo::SGA<Chrom> sga(select,
		   xover, xover_rate.value(),
		   mutation, mut_rate.value(),
		   evaluator,
		   checkpoint);
    sga(pop);

    cout << "solution = " << solution << endl
	 << "best     = " << *max_element(pop.begin(), pop.end()) << endl;
}

//-----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// End:
