#include <eo>
#include <es.h>
#include <utils/Stat.h>

#include "real_value.h"

using namespace eo;

typedef Real<MinimizingFitness> realVec;

double test( Pop<realVec>& pop, double target_value )
{
    EvalFuncPtr<realVec, double, const std::vector<double>&> eval( real_value );

    PopLoopEval<realVec> pop_eval(eval);

    pop_eval(pop,pop);

    InterquartileRangeStat<realVec> iqr_stat(0.0, "IQR");

    iqr_stat( pop );

    std::cout << iqr_stat.longName() << "=" << iqr_stat.value() << " should be " << target_value << std::endl;

    return iqr_stat.value();
}

int main()
{
    Pop<realVec> pop;

    // fixed test
    realVec sol1(2,-1);
    realVec sol2(2,-1);
    realVec sol3(2,1);
    realVec sol4(2,1);
    pop.push_back( sol1 );
    pop.push_back( sol2 );
    pop.push_back( sol3 );
    pop.push_back( sol4 );
    // on the sphere function everyone has the same fitness of 1
    if( test(pop, 0) != 0 ) {
	exit(1);
    }

    pop.erase(pop.begin(),pop.end());

    // fixed test
    sol1 = realVec(2,0);
    sol2 = realVec(2,0);
    sol3 = realVec(2,1);
    sol4 = realVec(2,1);
    pop.push_back( sol1 );
    pop.push_back( sol2 );
    pop.push_back( sol3 );
    pop.push_back( sol4 );
    if( test(pop, 1) != 1 ) {
	exit(1);
    }

    // test on a random normal distribution
    NormalGenerator<double> normal(1,rng);
    InitFixedLength<realVec> init_N(2, normal);
    pop = Pop<realVec>( 1000000, init_N );
    double iqr = test(pop, 1.09);
    if( iqr < 1.08 || iqr > 1.11 ) {
	exit(1);
    }
}
