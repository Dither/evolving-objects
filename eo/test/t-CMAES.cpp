
#include <iostream>

#include <ScalarFitness.h>
#include <Vector.h>
#include <Pop.h>
#include <utils/Parser.h>
#include <utils/CheckPoint.h>
#include <EvalFuncPtr.h>

#include <GenContinue.h>
#include <FitContinue.h>
#include <utils/StdoutMonitor.h>
#include <utils/Stat.h>
#include <utils/TimedMonitor.h>

#include <MergeReduce.h>
#include <EasyEA.h>

#include <es/CMAState.h>
#include <es/CMAParams.h>
#include <es/CMAInit.h>
#include <es/CMABreed.h>

using namespace eo;
using namespace std;

typedef MinimizingFitness FitT;
typedef Vector<FitT, double> EoType;

double sqr(double x) { return x*x; }

ValueParam<int> evals(0,"Function Evals","Number of Evaluations");

double f_sphere(const vector<double>& values) {
    double sum = 0.0;
    for (unsigned i = 0; i < values.size(); ++i) {
	sum += values[i] * values[i];
    }
    ++evals.value();
    return sum;
}

double f_rosen(const vector<double>& x) {
    double sum =0.0;

    for (unsigned i = 0; i < x.size()-1; ++i) {
	sum += 100 * sqr(sqr(x[i])-x[i+1]) + sqr(1.-x[i]);
    }
    ++evals.value();
    return sum;
}



int main(int argc, char* argv[]) {

    // make sure we have a dimensionality parameter (for testing)
    char** rargv = new char*[argc+1];
    rargv[0] = argv[0];
    rargv[1] = (char*)"-N10";
    for (int i = 2; i < argc; ++i) {
	rargv[i] = argv[i-1];
    }

    Parser parser(argc+1, rargv);

    CMAParams params(parser);

    vector<double> initial_point(params.n, 0.0);

    CMAState state(params, initial_point);

    if (parser.userNeedsHelp())
    {
	parser.printHelp(std::cout);
	return 1;
    }

    CMAInit<FitT> init(state);

    Pop<EoType> pop(params.mu, init);

    EvalFuncPtr<EoType, double, const vector<double>&> eval(  f_rosen );

    CMABreed<FitT> breed(state, params.lambda);

    for (unsigned i = 0; i < pop.size(); ++i) {
	eval(pop[i]);
    }

    CommaReplacement<EoType> comma;

    GenContinue<EoType> gen(params.maxgen);
    FitContinue<EoType> fit(1e-10);

    CheckPoint<EoType> checkpoint(gen);
    checkpoint.add(fit);

    BestFitnessStat<EoType> stat;

    StdoutMonitor mon;
    mon.add(stat);
    mon.add(evals);

    TimedMonitor timed(1);// 1 seconds
    timed.add(mon); // wrap it

    checkpoint.add(timed);
    checkpoint.add(stat);

    EasyEA<EoType> algo(
	    checkpoint,
	    eval,
	    breed,
	    comma);


    algo(pop);
    pop.sort();

    cout << pop[0] << endl;
    cout << "Fitness achieved = " << pop[0].fitness() << endl;
    cout << "Function evaluations = " << evals.value() << endl;
}



