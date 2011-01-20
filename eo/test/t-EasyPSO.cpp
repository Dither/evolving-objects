//-----------------------------------------------------------------------------
// t-EasyPSO.cpp
//-----------------------------------------------------------------------------

#ifndef __GNUG__
// to avoid long name warnings
#pragma warning(disable:4786)
#endif // __GNUG__

#include <eo>

using namespace eo;

//-----------------------------------------------------------------------------
typedef MinimizingFitness FitT;
typedef RealParticle < FitT > Particle;
//-----------------------------------------------------------------------------

// the objective function
double real_value (const Particle & _particle)
{
    double sum = 0;
    for (unsigned i = 0; i < _particle.size ()-1; i++)
        sum += pow(_particle[i],2);
    return (sum);
}


int main()
{
    const unsigned int VEC_SIZE = 2;
    const unsigned int POP_SIZE = 20;
    const unsigned int NEIGHBORHOOD_SIZE= 5;
    unsigned i;

    // the population:
    Pop<Particle> pop;

    // Evaluation
    EvalFuncPtr<Particle, double, const Particle& > eval(  real_value );

    // position init
    UniformGenerator < double >uGen (-3, 3);
    InitFixedLength < Particle > random (VEC_SIZE, uGen);

    // velocity init
    UniformGenerator < double >sGen (-2, 2);
    VelocityInitFixedLength < Particle > veloRandom (VEC_SIZE, sGen);

    // local best init
    FirstIsBestInit < Particle > localInit;

    // perform position initialization
    pop.append (POP_SIZE, random);
  
    // topology
    LinearTopology<Particle> topology(NEIGHBORHOOD_SIZE);

	// the full initializer
    Initializer <Particle> init(eval,veloRandom,localInit,topology,pop);
    init();
    
   
    // bounds
    RealVectorBounds bnds(VEC_SIZE,-1.5,1.5);

    // velocity
    StandardVelocity <Particle> velocity (topology,1,1.6,2,bnds);

    // flight
    StandardFlight <Particle> flight;

    // Terminators
    GenContinue <Particle> genCont1 (50);
	GenContinue <Particle> genCont2 (50);
	
    // PS flight
    EasyPSO<Particle> pso1(genCont1, eval, velocity, flight);

	EasyPSO<Particle> pso2(init,genCont2, eval, velocity, flight);
    
    // flight
    try
    {
        pso1(pop);
        std::cout << "FINAL POPULATION AFTER PSO n°1:" << std::endl;
    	for (i = 0; i < pop.size(); ++i)
        	std::cout << "\t" <<  pop[i] << " " << pop[i].fitness() << std::endl;
        
        pso2(pop);
         std::cout << "FINAL POPULATION AFTER PSO n°2:" << std::endl;
    	for (i = 0; i < pop.size(); ++i)
        	std::cout << "\t" <<  pop[i] << " " << pop[i].fitness() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;;
        exit(EXIT_FAILURE);
    }

   

    return 0;
}

