// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// SyncEasyPSO.h
// (c) OPAC 2007
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

    Contact: thomas.legrand@lifl.fr
 */
//-----------------------------------------------------------------------------

#ifndef _SYNCEASYPSO_H
#define _SYNCEASYPSO_H

//-----------------------------------------------------------------------------
#include <Continue.h>
#include <PopEvalFunc.h>
#include <PSO.h>
#include <Velocity.h>
#include <Flight.h>
//-----------------------------------------------------------------------------

namespace eo
{

    /** An easy-to-use synchronous particle swarm algorithm; you can use any particle,
     *   any flight, any topology... 
     *
     *   The main steps are :
     * 	 - perform a first evaluation of the population
     * 	 - for each generation
     * 	 - evaluate ALL the velocities
     * 	 	-- perform the fligth of ALL the particles
     *    	-- evaluate ALL the particles
     *    	-- update the neighborhoods
     *
     *   @ingroup Algorithms
     */
    template < class POT > class SyncEasyPSO:public PSO < POT >
    {
    public:

	/** Full constructor
	 * @param _init - An InitializerBase that initializes the topology, velocity, best particle(s)
	 * @param _continuator - An Continue that manages the stopping criterion and the checkpointing system
	 * @param _eval - An EvalFunc: the evaluation performer
	 * @param _velocity - An Velocity that defines how to compute the velocities
	 * @param _flight - An Flight that defines how to make the particle flying: that means how 
	 * to modify the positions according to the velocities
	 */
	SyncEasyPSO (
		       InitializerBase <POT> &_init,
		       Continue < POT > &_continuator,
		       EvalFunc < POT > &_eval,
		       Velocity < POT > &_velocity,
		       Flight < POT > &_flight):
            init(_init),
            continuator (_continuator),
            eval (_eval),
            loopEval(_eval),
            popEval(loopEval),
            velocity (_velocity),
            flight (_flight)
	{}


	/** Constructor without Flight. For special cases when the flight is performed withing the velocity.
	 * @param _init - An InitializerBase that initializes the topology, velocity, best particle(s)
	 * @param _continuator - An Continue that manages the stopping criterion and the checkpointing system
	 * @param _eval - An EvalFunc: the evaluation performer
	 * @param _velocity - An Velocity that defines how to compute the velocities
	 */
	SyncEasyPSO (
		       InitializerBase <POT> &_init,
		       Continue < POT > &_continuator,
		       EvalFunc < POT > &_eval,
		       Velocity < POT > &_velocity):
            init(_init),
            continuator (_continuator),
            eval (_eval),
            loopEval(_eval),
            popEval(loopEval),
            velocity (_velocity),
            flight (dummyFlight)
	{}

	/** Full constructor - Can be used in parallel
	 * @param _init - An InitializerBase that initializes the topology, velocity, best particle(s)
	 * @param _continuator - An Continue that manages the stopping criterion and the checkpointing system
	 * @param _eval - An PopEvalFunc
	 * @param _velocity - An Velocity that defines how to compute the velocities
	 * @param _flight - An Flight
	 */
	SyncEasyPSO (
		       InitializerBase <POT> &_init,
		       Continue < POT > &_continuator,
		       PopEvalFunc < POT > &_eval,
		       Velocity < POT > &_velocity,
		       Flight <POT> &_flight):
            init(_init),
            continuator (_continuator),
            eval (dummyEval),
            loopEval(dummyEval),
            popEval(_eval),
            velocity (_velocity),
            flight (_flight)
	{}


	/** Another constructor without initializer
	 * @param _continuator - An Continue that manages the stopping criterion and the checkpointing system
	 * @param _eval - An EvalFunc: the evaluation performer
	 * @param _velocity - An Velocity that defines how to compute the velocities
	 * @param _flight - An Flight that defines how to make the particle flying: that means how 
	 * to modify the positions according to the velocities
	 */
	SyncEasyPSO (
		       Continue < POT > &_continuator,
		       EvalFunc < POT > &_eval,
		       Velocity < POT > &_velocity,
		       Flight < POT > &_flight):
            init(dummyInit),
            continuator (_continuator),
            eval (_eval),
            loopEval(_eval),
            popEval(loopEval),
            velocity (_velocity),
            flight (_flight)
	{}


	/** Constructor without Flight nor Initializer. For special cases when the flight is performed withing the velocity.
	 * @param _continuator - An Continue that manages the stopping criterion and the checkpointing system
	 * @param _eval - An EvalFunc: the evaluation performer
	 * @param _velocity - An Velocity that defines how to compute the velocities
	 */
	SyncEasyPSO (
		       Continue < POT > &_continuator,
		       EvalFunc < POT > &_eval,
		       Velocity < POT > &_velocity):
            init(dummyInit),
            continuator (_continuator),
            eval (_eval),
            loopEval(_eval),
            popEval(loopEval),
            velocity (_velocity),
            flight (dummyFlight)
	{}

	/** Full constructor - Can be used in parallel
	 * @param _continuator - An Continue that manages the stopping criterion and the checkpointing system
	 * @param _eval - An PopEvalFunc
	 * @param _velocity - An Velocity that defines how to compute the velocities
	 * @param _flight - An Flight
	 */
	SyncEasyPSO (
		       Continue < POT > &_continuator,
		       PopEvalFunc < POT > &_eval,
		       Velocity < POT > &_velocity,
		       Flight <POT> &_flight):
            init(dummyInit),
            continuator (_continuator),
            eval (dummyEval),
            loopEval(dummyEval),
            popEval(_eval),
            velocity (_velocity),
            flight (_flight)
	{}
    
	/// Apply a few iteration of flight to the population (=swarm).
	virtual void operator  () (Pop < POT > &_pop)
	{

	    try
		{
		    // initializes the topology, velocity, best particle(s)
		    init();
            
		    // just to use a loop eval
		    Pop<POT> empty_pop;

		    do
			{
			    // perform velocity evaluation
			    velocity.apply (_pop);

			    // apply the flight
			    flight.apply (_pop);

			    // evaluate the position (with a loop eval, empty_swarm IS USELESS)
			    popEval(empty_pop,_pop);

			    // update the topology (particle and local/global best(s))
			    velocity.updateNeighborhood(_pop);

			}
		    while (continuator (_pop));

		}
	    catch (std::exception & e)
		{
		    std::string s = e.what ();
		    s.append (" in SyncEasyPSO");
		    throw std::runtime_error (s);
		}

	}

    private:

	InitializerBase <POT> &init;
	Continue < POT > &continuator;

	EvalFunc < POT > &eval;
	PopLoopEval<POT>        loopEval;
	PopEvalFunc<POT>&       popEval;

	Velocity < POT > &velocity;
	Flight < POT > &flight;

	// if the eval does not need to be used, use the dummy eval instance
	class DummyEval : public EvalFunc<POT>
	{
	public:
	    void operator()(POT &)
	    {}
	}
	    dummyEval;

	class DummyFlight:public Flight < POT >
	{
	public:
	    DummyFlight () {}
	    void operator  () (POT & _po) {}
	}dummyFlight;
	
	// if the initializer does not need to be used, use the dummy one instead
	class DummyInitializer:public InitializerBase < POT >
	{
	public:
	    DummyInitializer () {}
	    void operator  () (POT & _po) {}
	}dummyInit;
	
    };
    /** @example t-SyncEasyPSO.cpp
     */

}

#endif /*_SYNCEASYPSO_H*/
