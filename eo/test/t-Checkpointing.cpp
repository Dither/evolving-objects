// to avoid long name warnings
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <stdexcept>  // runtime_error

// general
#include <utils/RNG.h>		// Random number generators
#include <ga.h>
#include <utils/Parser.h>
#include <utils/State.h>
#include <GenContinue.h>

// include package checkpointing
#include <utils/checkpointing>

using namespace eo;

struct Dummy : public EO<double>
{
    typedef double Type;
};


struct DummyPop : public Pop<Dummy>
{
public :
    DummyPop(int s = 2) { resize(s); }
};


int the_main(int argc, char **argv)
{ // ok, we have a command line parser and a state

    typedef Bit<float> Chrom;

    Parser parser(argc, argv);

    // Define Parameters
    ValueParam<double> rate(0.01, "mutationRatePerBit", "Initial value for mutation rate per bit");
    ValueParam<double> factor(0.99, "mutationFactor", "Decrease factor for mutation rate");
    ValueParam<uint32_t> seed(time(0), "seed", "Random number seed");
    ValueParam<std::string> load_name("", "Load","Load",'L');
    ValueParam<std::string> save_name("", "Save","Save",'S');

    // Register them
    parser.processParam(rate,       "Genetic Operators");
    parser.processParam(factor,     "Genetic Operators");
    parser.processParam(load_name,  "Persistence");
    parser.processParam(save_name,  "Persistence");
    parser.processParam(seed,       "Rng seeding");

   State state;
   state.registerObject(parser);

   if (load_name.value() != "")
   { // load the parser. This is only neccessary when the user wants to
     // be able to change the parameters in the state file by hand.
       state.load(load_name.value()); // load the parser
   }

    // Create the algorithm here
    typedef Dummy EoType;

    DummyPop pop;

    GenContinue<EoType> genTerm(5); // run for 5 generations

    CheckPoint<EoType> checkpoint(genTerm);
    // The algorithm will now quit after five generations

    // Create a counter parameter
    ValueParam<unsigned> generationCounter(0, "Generation");

    // Create an incrementor (wich is an Updater). Note that the
    // Parameter's value is passed by reference, so every time the incrementer increments,
    // the data in generationCounter will change.
    Incrementor<unsigned> increment(generationCounter.value());

    // Add it to the checkpoint, this will result in the counter being incremented every generation
    checkpoint.add(increment);

    // The file monitor will print parameters to a comma seperated file
    FileMonitor monitor("monitor.csv");

    // the checkpoint mechanism can handle multiple monitors
    checkpoint.add(monitor);

    // the monitor can monitor parameters such as the generationCounter
    monitor.add(generationCounter);

    // Second moment stats: average and stdev
    SecondMomentStats<EoType> stats;

    // Add it to the checkpoint to get it called at the appropriate time
    checkpoint.add(stats);

    // Add it to the monitor to get it written to the file
    monitor.add(stats);

    // save state every third generation
    CountedStateSaver stateSaver1(3, state, "generation");
    // save state every 2 seconds
    TimedStateSaver   stateSaver2(2, state, "time");

    // And add the two savers to the checkpoint
    checkpoint.add(stateSaver1);
    checkpoint.add(stateSaver2);

    // Register the algorithm
    state.registerObject(rng);
    state.registerObject(pop);

    if (parser.userNeedsHelp())
    {
	parser.printHelp(std::cout);
	return 0;
    }

    // Either load or initialize
    if (load_name.value() != "")
    {
	state.load(load_name.value()); // load the rest
    }
    else
    {
	// else

	// initialize rng and population

	rng.reseed(seed.value());

	pop.resize(2);

	pop[0].fitness(1);
	pop[1].fitness(2);
    }

    while(checkpoint(pop))
    {
	pop[0].fitness(pop[0].fitness() + 1);

	time_t now = time(0);

	while (time(0) == now) {} // wait a second to test timed saver

	std::cout << "gen " << generationCounter.value() << std::endl;
    }

    // run the algorithm

    // Save when needed
    if (save_name.value() != "")
    {
	std::string file_name = save_name.value();
	save_name.value() = ""; // so that it does not appear in the parser section of the state file
	state.save(file_name);
    }

    return 1;
}

int main(int argc, char **argv)
{
    try
    {
	the_main(argc, argv);
    }
    catch(std::exception& e)
    {
	std::cout << "Exception: " << e.what() << std::endl;
    }

}
