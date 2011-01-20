// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// Initializer.h
// (c) OPAC Team, INRIA, 2007
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

    Contact: clive.canape@inria.fr


 */
//-----------------------------------------------------------------------------

#ifndef _Initializer_H
#define _Initializer_H

#include <utils/RealVectorBounds.h>
#include <VelocityInit.h>
#include <Pop.h>
#include <ParticleBestInit.h>
#include <Topology.h>

namespace eo
{

    /**
       @addtogroup Initializators
       @{
    */


    /**
     * Abstract class for initialization of algorithm PSO
     */
    template <class POT> class InitializerBase : public FunctorBase
    {
    public :

	virtual ~InitializerBase()
	{}

	virtual void operator()()
	{};
    };

    /**
       Base (name) class for Initialization of algorithm PSO

       @see InitializerBase UF apply
    */
    template <class POT> class Initializer : public InitializerBase <POT>
    {
    public:

	//!	Constructor
	//! @param _proc Evaluation function
	//! @param _initVelo Initialization of the velocity
	//! @param _initBest Initialization of the best
	//! @param _topology the topology to use
	//! @param _pop Population
	Initializer(
		      UF<POT&, void>& _proc,
		      VelocityInit < POT > &_initVelo,
		      ParticleBestInit <POT> &_initBest,
		      Topology <POT> &_topology,
		      Pop < POT > &_pop
		      ) : proc(_proc), initVelo(_initVelo), procPara(dummyEval), initBest(_initBest), topology(_topology), pop(_pop)
	{}

	//!	Constructor for parallel evaluation
	//! @param _proc Evaluation function
	//! @param _initVelo Initialization of the velocity
	//! @param _initBest Initialization of the best
	//! @param _topology the topology to use
	//! @param _pop Population
	Initializer(
		      PopEvalFunc <POT>& _proc,
		      VelocityInit < POT > &_initVelo,
		      ParticleBestInit <POT> &_initBest,
		      Topology <POT> &_topology,
		      Pop < POT > &_pop
		      ) : proc(dummy), initVelo(_initVelo), procPara(_proc), initBest(_initBest), topology(_topology), pop(_pop)
	{}


	//! Give the name of the class
	//! @return The name of the class
	virtual std::string className (void) const
	{
	    return "Initializer";
	}



	virtual void operator() ()
	{
	    Pop<POT> empty_pop;
	    apply(proc, pop);
	    procPara(empty_pop, pop);
	    apply < POT > (initVelo, pop);
	    apply < POT > (initBest, pop);
	    topology.setup(pop);
	}

    private :

	/*
	  @param proc First evaluation
	  @param initVelo Initialization of the velocity
	  @param initBest Initialization of the best
	*/
	UF<POT&, void>& proc;
	VelocityInit < POT > & initVelo;
	PopEvalFunc <POT>& procPara;
	ParticleBestInit <POT> & initBest;
	Topology <POT> & topology;
	Pop < POT > & pop;

	class DummyEval : public PopEvalFunc<POT>
	{
	public:
	    void operator()(Pop<POT> &,Pop<POT> &_pop)
	    {}
	} dummyEval;

	class Dummy : public UF<POT&, void>
	{
	public:
	    void operator()(POT &)
	    {}

	} dummy;

    };

}

#endif

/** @} */
