// Program to test several EO-ES features

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <algorithm>
#include <string>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <ctime>

#ifdef _MSC_VER
  #include <crtdbg.h>
#endif

using namespace std;

#include <eo>

// representation specific
#include <es/make_es.h>

#include "real_value.h"		// the sphere fitness

// Now the main
///////////////
typedef MinimizingFitness  FitT;

template <class EOT>
void runAlgorithm(EOT, Parser& _parser, State& _state);

int main_function(int argc, char *argv[])
{
    // Create the command-line parser
    Parser parser(argc, argv);  // for user-parameter reading
    State state;    // keeps all things allocated
    ValueParam<bool>& simpleParam = parser.getORcreateParam(true, "Isotropic",
                                                              "Isotropic self-adaptive mutation",
                                                              'i', "ES mutation");
    ValueParam<bool>& stdevsParam = parser.getORcreateParam(false, "Stdev",
                                                              "One self-adaptive stDev per variable",
                                                              's', "ES mutation");
    ValueParam<bool>& corrParam = parser.getORcreateParam(false, "Correl",
                                                            "Use correlated mutations",
                                                            'c', "ES mutation");
    // Run the appropriate algorithm
    if (simpleParam.value() == false)
    {
        std::cout << "Using Real" << std::endl;
        runAlgorithm(Real<FitT>(), parser, state);
    }
    else if (stdevsParam.value() == false)
    {
        std::cout << "Using EsSimple" << std::endl;
        runAlgorithm(EsSimple<FitT>(), parser, state);
    }
    else if (corrParam.value() == false)
    {
        std::cout << "Using EsStdev" << std::endl;
        runAlgorithm(EsStdev<FitT>(), parser, state);
    }
    else
    {
        std::cout << "Using EsFull" << std::endl;
        runAlgorithm(EsFull<FitT>(), parser, state);
    }
    return 0;
}



// A main that catches the exceptions
int main(int argc, char **argv)
{
#ifdef _MSC_VER
    // rng.reseed(42);
    int flag = _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
    flag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flag);
    // _CrtSetBreakAlloc(100);
#endif
    try
    {
        main_function(argc, argv);
    }
    catch(std::exception& e)
    {
        std::cout << "Exception: " << e.what() << '\n';
    }
}



/** The templatized main (sort of)

quite similar to the main of other genotypes (e.g. t-Real and t-eoGA
in test dir)
*/
template <class EOT>
void runAlgorithm(EOT, Parser& _parser, State& _state)
{
    typedef typename EOT::Fitness FitT;

    ///// FIRST, problem or representation dependent stuff
    //////////////////////////////////////////////////////

    // The evaluation fn - encapsulated into an eval counter for output
    EvalFuncPtr<EOT, double, const std::vector<double>&> mainEval( real_value );
    EvalFuncCounter<EOT> eval(mainEval);

    // the genotype - through a genotype initializer
    RealInitBounded<EOT>& init = make_genotype(_parser, _state, EOT());

    // Build the variation operator (any seq/prop construct)
    GenOp<EOT>& op = make_op(_parser, _state, init);

    //// Now the representation-independent things
    //////////////////////////////////////////////

    // initialize the population - and evaluate
    // yes, this is representation indepedent once you have an Init
    Pop<EOT>& pop = make_pop(_parser, _state, init);
    apply<EOT>(eval, pop);

    // stopping criteria
    Continue<EOT> & term = make_continue(_parser, _state, eval);
    // output
    CheckPoint<EOT> & checkpoint = make_checkpoint(_parser, _state, eval, term);
    // algorithm (need the operator!)
    Algo<EOT>& ga = make_algo_scalar(_parser, _state, eval, checkpoint, op);

    ///// End of construction of the algorith
    /////////////////////////////////////////
    // to be called AFTER all parameters have been read!!!
    make_help(_parser);

    //// GO
    ///////
    std::cout << "Initial Population\n";
    pop.sortedPrintOn(std::cout);
    std::cout << std::endl;

    run_ea(ga, pop); // run the ga

    std::cout << "Final Population\n";
    pop.sortedPrintOn(std::cout);
    std::cout << std::endl;
}
