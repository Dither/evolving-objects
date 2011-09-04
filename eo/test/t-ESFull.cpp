// Program to test several EO-ES features

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <algorithm>
#include <string>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <time.h>

using namespace std;

#include <eo>

// representation specific
#include <es.h>

#include "real_value.h"		// the sphere fitness

// Now the main
///////////////
typedef MinimizingFitness  FitT;

template <class EOT>
void runAlgorithm(EOT, Parser& _parser, State& _state, RealVectorBounds& _bounds, ValueParam<string> _load_name);

int main_function(int argc, char *argv[])
{
  // Create the command-line parser
  Parser parser( argc, argv, "Basic EA for vector<float> with adaptive mutations");

  // Define Parameters and load them
  ValueParam<uint32_t>& seed        = parser.createParam(static_cast<uint32_t>(time(0)),
                                                           "seed", "Random number seed");
  ValueParam<string>& load_name   = parser.createParam(string(), "Load","Load a state file",'L');
  ValueParam<string>& save_name   = parser.createParam(string(), "Save","Saves a state file",'S');
  ValueParam<bool>&   stdevs      = parser.createParam(false, "Stdev", "Use adaptive mutation rates", 's');
  ValueParam<bool>&   corr        = parser.createParam(false, "Correl", "Use correlated mutations", 'c');
  ValueParam<unsigned>& chromSize = parser.createParam(unsigned(50), "ChromSize", "Number of chromosomes", 'n');
  ValueParam<double>& minimum     = parser.createParam(-1.0, "Min", "Minimum for Objective Variables", 'l');
  ValueParam<double>& maximum     = parser.createParam(1.0, "Max", "Maximum for Objective Variables", 'h');

  State state;
    state.registerObject(parser);
    rng.reseed(seed.value());

   if (!load_name.value().empty())
   { // load the parser. This is only neccessary when the user wants to
     // be able to change the parameters in the state file by hand
     // Note that only parameters inserted in the parser at this point
     // will be loaded!.
       state.load(load_name.value()); // load the parser
   }

    state.registerObject(rng);

    RealVectorBounds bounds(chromSize.value(), minimum.value(), maximum.value());

    // Run the appropriate algorithm
    if (stdevs.value() == false && corr.value() == false)
    {
        runAlgorithm(EsSimple<FitT>() ,parser, state, bounds, load_name);
    }
    else if (corr.value() == true)
    {
        runAlgorithm(EsFull<FitT>(),parser, state, bounds, load_name);
    }
    else
    {
        runAlgorithm(EsStdev<FitT>(), parser, state, bounds, load_name);
    }

    // and save
    if (!save_name.value().empty())
    {
	string file_name = save_name.value();
	save_name.value() = ""; // so that it does not appear in the parser section of the state file
	state.save(file_name);
    }

	return 0;
}

// A main that catches the exceptions

int main(int argc, char **argv)
{
#ifdef _MSC_VER
  //  rng.reseed(42);
    int flag = _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
     flag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flag);
//   _CrtSetBreakAlloc(100);
#endif

    try
    {
	main_function(argc, argv);
    }
    catch(std::exception& e)
    {
	std::cout << "Exception: " << e.what() << '\n';
    }

    return 1;
}

template <class EOT>
void runAlgorithm(EOT, Parser& _parser, State& _state, RealVectorBounds& _bounds, ValueParam<string> _load_name)
{
    // evaluation
    EvalFuncPtr<EOT, double, const vector<double>&> eval(  real_value );

    // population parameters, unfortunately these can not be altered in the state file
    ValueParam<unsigned> mu = _parser.createParam(unsigned(7), "mu","Size of the population");
    ValueParam<double>lambda_rate = _parser.createParam(double(7.0), "lambda_rate", "Factor of children to produce");

    if (lambda_rate.value() < 1.0f)
    {
	throw logic_error("lambda_rate must be larger than 1 in a comma strategy");
    }

    // Initialization
    EsChromInit<EOT> init(_bounds);

    // State takes ownership of pop because it needs to save it in caller
    Pop<EOT>& pop = _state.takeOwnership(Pop<EOT>(mu.value(), init));

    _state.registerObject(pop);

    if (!_load_name.value().empty())
    { // The real loading happens here when all objects are registered
       _state.load(_load_name.value()); // load all and everything
    }
    else
    {
	// evaluate initial population
	apply<EOT>(eval, pop);
    }

    // Ok, time to set up the algorithm
    // Proxy for the mutation parameters
    EsMutationInit mutateInit(_parser);

    EsMutate<EOT> mutate(mutateInit, _bounds);

    // monitoring, statistics etc.
    AverageStat<EOT> average;
    StdoutMonitor monitor;

    monitor.add(average);

    GenContinue<EOT> cnt(100);
    CheckPoint<EOT> checkpoint(cnt);
    checkpoint.add(monitor);
    checkpoint.add(average);

    // only mutation (== with rate 1.0)
    MonGenOp<EOT> op(mutate);

    // the selection: sequential selection
    SequentialSelect<EOT> select;
    // the general breeder (lambda is a rate -> true)
    GeneralBreeder<EOT> breed(select, op, lambda_rate.value(), true);

    // the replacement - hard-coded Comma replacement
    CommaReplacement<EOT> replace;

    // now the EasyEA
    EasyEA<EOT> es(checkpoint, eval, breed, replace);

    es(pop);

    pop.sort();
    std::cout << "Final population\n" << pop << std::endl;

}
