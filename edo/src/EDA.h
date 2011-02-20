/*
The Evolving Distribution Objects framework (EDO) is a template-based, 
ANSI-C++ evolutionary computation library which helps you to write your 
own estimation of distribution algorithms.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Copyright (C) 2010 Thales group
*/
/*
Authors:
    Johann Dréo <johann.dreo@thalesgroup.com>
    Caner Candan <caner.candan@thalesgroup.com>
*/

#ifndef _edo_EDA_h
#define _edo_EDA_h

#include <eo>
#include <mo>

#include <eo/utils/RNG.h>

#include "Algo.h"
#include "Estimator.h"
#include "ModifierMass.h"
#include "Sampler.h"
#include "Continue.h"

namespace edo
{
    template < typename D >
    class EDA : public Algo< D >
    {
    public:
	//! Alias for the type EOT
	typedef typename D::EOType EOT;

	//! Alias for the atom type
	typedef typename EOT::AtomType AtomType;

	//! Alias for the fitness
	typedef typename EOT::Fitness Fitness;

    public:

	//! EDA constructor
	/*!
	  All the boxes used by a EDASA need to be given.

	  \param selector Population Selector
	  \param estimator Distribution Estimator
	  \param selectone SelectOne
	  \param modifier Distribution Modifier
	  \param sampler Distribution Sampler
	  \param pop_continue Population Continuator
	  \param distribution_continue Distribution Continuator
	  \param evaluation Evaluation function.
	  \param sa_continue Stopping criterion.
	  \param cooling_schedule Cooling schedule, describes how the temperature is modified.
	  \param initial_temperature The initial temperature.
	  \param replacor Population replacor
	*/
	EDA (eo::Select< EOT > & selector,
	     Estimator< D > & estimator,
	     eo::SelectOne< EOT > & selectone,
	     ModifierMass< D > & modifier,
	     Sampler< D > & sampler,
	     eo::Continue< EOT > & pop_continue,
	     Continue< D > & distribution_continue,
	     eo::EvalFunc < EOT > & evaluation,
	     //moContinuator< moDummyNeighbor<EOT> > & sa_continue,
	     //moCoolingSchedule<EOT> & cooling_schedule,
	     //double initial_temperature,
	     eo::Replacement< EOT > & replacor
	     )
	    : _selector(selector),
	      _estimator(estimator),
	      _selectone(selectone),
	      _modifier(modifier),
	      _sampler(sampler),
	      _pop_continue(pop_continue),
	      _distribution_continue(distribution_continue),
	      _evaluation(evaluation),
	      //_sa_continue(sa_continue),
	      //_cooling_schedule(cooling_schedule),
	      //_initial_temperature(initial_temperature),
	      _replacor(replacor)

	{}

	//! function that launches the EDASA algorithm.
	/*!
	  As a moTS or a moHC, the EDASA can be used for HYBRIDATION in an evolutionary algorithm.

	  \param pop A population to improve.
	  \return TRUE.
	*/
	void operator ()(eo::Pop< EOT > & pop)
	{
	    assert(pop.size() > 0);

	    //double temperature = _initial_temperature;

	    eo::Pop< EOT > current_pop;

	    eo::Pop< EOT > selected_pop;


	    //-------------------------------------------------------------
	    // Estimating a first time the distribution parameter thanks
	    // to population.
	    //-------------------------------------------------------------

	    D distrib = _estimator(pop);

	    double size = distrib.size();
	    assert(size > 0);

	    //-------------------------------------------------------------


	    do
		{
		    //-------------------------------------------------------------
		    // (3) Selection of the best points in the population
		    //-------------------------------------------------------------

		    selected_pop.clear();

		    _selector(pop, selected_pop);

		    assert( selected_pop.size() > 0 );

		    //-------------------------------------------------------------


		    //-------------------------------------------------------------
		    // (4) Estimation of the distribution parameters
		    //-------------------------------------------------------------

		    distrib = _estimator(selected_pop);

		    //-------------------------------------------------------------


		    // TODO: utiliser selected_pop ou pop ???

		    assert(selected_pop.size() > 0);


		    //-------------------------------------------------------------
		    // Init of a variable contening a point with the bestest fitnesses
		    //-------------------------------------------------------------

		    EOT current_solution = _selectone(selected_pop);

		    //-------------------------------------------------------------


		    //-------------------------------------------------------------
		    // Fit the current solution with the distribution parameters (bounds)
		    //-------------------------------------------------------------

		    // FIXME: si besoin de modifier la dispersion de la distribution
		    // _modifier_dispersion(distribution, selected_pop);
		    _modifier(distrib, current_solution);

		    //-------------------------------------------------------------


		    //-------------------------------------------------------------
		    // Evaluating a first time the current solution
		    //-------------------------------------------------------------

		    _evaluation( current_solution );

		    //-------------------------------------------------------------


		    //-------------------------------------------------------------
		    // Building of the sampler in current_pop
		    //-------------------------------------------------------------

		    //_sa_continue.init( current_solution );

		    current_pop.clear();

		    for ( unsigned int i = 0; i < pop.size(); ++i )
			//do
			{
			    EOT candidate_solution = _sampler(distrib);
			    _evaluation( candidate_solution );

			    // TODO: verifier le critere d'acceptation
			    if ( candidate_solution.fitness() < current_solution.fitness()
				 // || rng.uniform() < exp( ::fabs(candidate_solution.fitness() - current_solution.fitness()) / temperature )
				 )
				{
				    current_pop.push_back(candidate_solution);
				    current_solution = candidate_solution;
				}
			}
		    //while ( _sa_continue( current_solution) );

		    //-------------------------------------------------------------


		    _replacor(pop, current_pop); // copy current_pop in pop

		    pop.sort();

		    //if ( ! _cooling_schedule( temperature ) ){ eo::log << eo::debug << "_cooling_schedule" << std::endl; break; }

		    if ( ! _distribution_continue( distrib ) ){ eo::log << eo::debug << "_distribution_continue" << std::endl; break; }

		    if ( ! _pop_continue( pop ) ){ eo::log << eo::debug << "_pop_continue" << std::endl; break; }

		}
	    while ( 1 );
	}

    private:

	//! A EOT selector
	eo::Select < EOT > & _selector;

	//! A EOT estimator. It is going to estimate distribution parameters.
	Estimator< D > & _estimator;

	//! SelectOne
	eo::SelectOne< EOT > & _selectone;

	//! A D modifier
	ModifierMass< D > & _modifier;

	//! A D sampler
	Sampler< D > & _sampler;

	//! A EOT population continuator
	eo::Continue < EOT > & _pop_continue;

	//! A D continuator
	Continue < D > & _distribution_continue;

	//! A full evaluation function.
	eo::EvalFunc < EOT > & _evaluation;

	//! Stopping criterion before temperature update
	//moContinuator< moDummyNeighbor<EOT> > & _sa_continue;

	//! The cooling schedule
	//moCoolingSchedule<EOT> & _cooling_schedule;

	//! Initial temperature
	//double  _initial_temperature;

	//! A EOT replacor
	eo::Replacement < EOT > & _replacor;
    };
}

#endif // !_EDA_h
