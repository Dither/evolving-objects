// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// make_op.h - the real-valued version
// (c) Maarten Keijzer, Marc Schoenauer and GeNeura Team, 2001
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

    Contact: todos@geneura.ugr.es, http://geneura.ugr.es
             Marc.Schoenauer@polytechnique.fr
             mkeijzer@dhi.dk
 */
//-----------------------------------------------------------------------------

#ifndef _make_op_h
#define _make_op_h

// the operators
#include <Op.h>
#include <GenOp.h>
#include <CloneOps.h>
#include <OpContainer.h>
// combinations of simple Ops (MonOp and QuadOp)
#include <ProportionalCombinedOp.h>

// the specialized Real stuff
#include <es/Real.h>
#include <es/RealInitBounded.h>
#include <es/RealOp.h>
#include <es/NormalMutation.h>
  // also need the parser and param includes
#include <utils/Parser.h>
#include <utils/State.h>

namespace eo
{

    /** @addtogroup Builders
     * @{
     */

    /*
     * This function builds the operators that will be applied to the Real
     *
     * It uses a parser (to get user parameters) and a state (to store the memory)
     * the last argument is an individual, needed for 2 reasons
     *     it disambiguates the call after instanciations
     *     some operator might need some private information about the indis
     *
     * This is why the template is the complete EOT even though only the fitness
     * is actually templatized here: the following only applies to bitstrings
     *
     * Note : the last parameter is an Init: if some operator needs some info
     *        about the gneotypes, the init has it all (e.g. bounds, ...)
     *        Simply do
     *        EOT myEO;
     *        _init(myEO);
     *        and myEO is then an ACTUAL object
     */
    template <class EOT>
    GenOp<EOT> & do_make_op(ParameterLoader& _parser, State& _state, Init<EOT>& _init)
    {
	// First, decide whether the objective variables are bounded
	ValueParam<ParamParamType>& boundsParam
	    = _parser.getORcreateParam(ParamParamType("(0,1)"), "objectBounds",
				       "Bounds for variables (unbounded if absent)",
				       'B', "Genetic Operators");

	// get initisalizer size == std::vector size
	//  RealInitBounded<EOT> * realInit = (RealInitBounded<EOT>*)(&_init);
	//  unsigned vecSize = realInit->theBounds().size();

	// get std::vector size: safer???
	EOT eoTmp;
	_init(eoTmp);
	unsigned vecSize = eoTmp.size();

	// the bounds pointer
	RealVectorBounds * ptBounds;
	if (_parser.isItThere(boundsParam))	// otherwise, no bounds
	    {
		/////Warning: this code should probably be replaced by creating
		/////    some ValueParam<RealVectorBounds> with specific implementation
		////     in Parser.cpp. At the moemnt, it is there (cf also make_genotype
		ParamParamType & ppBounds = boundsParam.value(); // std::pair<std::string,std::vector<std::string> >
		// transform into a std::vector<double>
		std::vector<double> v;
		std::vector<std::string>::iterator it;
		for (it=ppBounds.second.begin(); it<ppBounds.second.end(); it++)
		    {
			istrstream is(it->c_str());
			double r;
			is >> r;
			v.push_back(r);
		    }
		// now create the RealVectorBounds object
		if (v.size() == 2) // a min and a max for all variables
		    ptBounds = new RealVectorBounds(vecSize, v[0], v[1]);
		else				   // no time now
		    throw std::runtime_error("Sorry, only unique bounds for all variables implemented at the moment. Come back later");
		// we need to give ownership of this pointer to somebody
		/////////// end of temporary code
	    }
	else			   // no param for bounds was given
	    ptBounds = new RealVectorNoBounds(vecSize); // DON'T USE DummyVectorNoBounds
	// as it does not have any dimension

	// this is a temporary version(!),
	// while Maarten codes the full tree-structured general operator input
	// BTW we must leave that simple version available somehow, as it is the one
	// that 90% people use!
	ValueParam<std::string>& operatorParam
	    =  _parser.getORcreateParam(std::string("SGA"), "operator",
					"Description of the operator (SGA only now)",
					'o', "Genetic Operators");

	if (operatorParam.value() != std::string("SGA"))
	    throw std::runtime_error("Sorry, only SGA-like operator available right now\n");

	// now we read Pcross and Pmut,
	// the relative weights for all crossovers -> proportional choice
	// the relative weights for all mutations -> proportional choice
	// and create the GenOp that is exactly
	// crossover with pcross + mutation with pmut

	ValueParam<double>& pCrossParam
	    = _parser.getORcreateParam(0.6, "pCross", "Probability of Crossover",
				       'C', "Genetic Operators" );
	// minimum check
	if ( (pCrossParam.value() < 0) || (pCrossParam.value() > 1) )
	    throw std::runtime_error("Invalid pCross");

	ValueParam<double>& pMutParam
	    = _parser.getORcreateParam(0.1, "pMut", "Probability of Mutation",
				       'M', "Genetic Operators" );
	// minimum check
	if ( (pMutParam.value() < 0) || (pMutParam.value() > 1) )
	    throw std::runtime_error("Invalid pMut");

	// the crossovers
	/////////////////
	// the parameters
	ValueParam<double>& segmentRateParam
	    = _parser.getORcreateParam(double(1.0), "segmentRate",
				       "Relative rate for segment crossover",
				       's', "Genetic Operators" );
	// minimum check
	if ( (segmentRateParam.value() < 0) )
	    throw std::runtime_error("Invalid segmentRate");

	ValueParam<double>& arithmeticRateParam
	    = _parser.getORcreateParam(double(2.0), "arithmeticRate",
				       "Relative rate for arithmetic crossover",
				       'A', "Genetic Operators" );
	// minimum check
	if ( (arithmeticRateParam.value() < 0) )
	    throw std::runtime_error("Invalid arithmeticRate");

	// minimum check
	bool bCross = true;
	if (segmentRateParam.value()+arithmeticRateParam.value()==0)
	    {
		std::cerr << "Warning: no crossover" << std::endl;
		bCross = false;
	    }

	// Create the CombinedQuadOp
	PropCombinedQuadOp<EOT> *ptCombinedQuadOp = NULL;
	QuadOp<EOT> *ptQuad = NULL;

	if (bCross)
	    {
		// segment crossover for bitstring - pass it the bounds
		ptQuad = new SegmentCrossover<EOT>(*ptBounds);
		_state.storeFunctor(ptQuad);
		ptCombinedQuadOp = new PropCombinedQuadOp<EOT>(*ptQuad, segmentRateParam.value());

		// arithmetic crossover
		ptQuad = new ArithmeticCrossover<EOT>(*ptBounds);
		_state.storeFunctor(ptQuad);
		ptCombinedQuadOp->add(*ptQuad, arithmeticRateParam.value());

		// don't forget to store the CombinedQuadOp
		_state.storeFunctor(ptCombinedQuadOp);
	    }

	// the mutations
	/////////////////
	// the parameters
	ValueParam<double> & epsilonParam
	    = _parser.getORcreateParam(0.01, "epsilon", "Half-size of interval for Uniform Mutation",
				       'e', "Genetic Operators" );
	// minimum check
	if ( (epsilonParam.value() < 0) )
	    throw std::runtime_error("Invalid epsilon");

	ValueParam<double> & uniformMutRateParam
	    = _parser.getORcreateParam(1.0, "uniformMutRate",
				       "Relative rate for uniform mutation", 'u', "Genetic Operators" );
	// minimum check
	if ( (uniformMutRateParam.value() < 0) )
	    throw std::runtime_error("Invalid uniformMutRate");

	ValueParam<double> & detMutRateParam
	    = _parser.getORcreateParam(1.0, "detMutRate",
				       "Relative rate for deterministic uniform mutation",
				       'd', "Genetic Operators" );
	// minimum check
	if ( (detMutRateParam.value() < 0) )
	    throw std::runtime_error("Invalid detMutRate");

	ValueParam<double> & normalMutRateParam
	    = _parser.getORcreateParam(1.0, "normalMutRate",
				       "Relative rate for Gaussian mutation",
				       'd', "Genetic Operators" );
	// minimum check
	if ( (normalMutRateParam.value() < 0) )
	    throw std::runtime_error("Invalid normalMutRate");
	// and the sigma
	ValueParam<double> & sigmaParam
	    = _parser.getORcreateParam(1.0, "sigma",
				       "Sigma (fixed) for Gaussian mutation",
				       'S', "Genetic Operators" );
	// minimum check
	if ( (sigmaParam.value() < 0) )
	    throw std::runtime_error("Invalid sigma");

	// minimum check
	bool bMut = true;
	if (uniformMutRateParam.value()+detMutRateParam.value()+normalMutRateParam.value()==0)
	    {
		std::cerr << "Warning: no mutation" << std::endl;
		bMut = false;
	    }
	if (!bCross && !bMut)
	    throw std::runtime_error("No operator called in SGA operator definition!!!");

	// Create the CombinedMonOp
	PropCombinedMonOp<EOT> *ptCombinedMonOp = NULL;
	MonOp<EOT> *ptMon = NULL;

	if (bMut)
	    {
		// uniform mutation on all components:
		// offspring(i) uniformly chosen in [parent(i)-epsilon, parent(i)+epsilon]
		ptMon = new UniformMutation<EOT>(*ptBounds, epsilonParam.value());
		_state.storeFunctor(ptMon);
		// create the CombinedMonOp
		ptCombinedMonOp = new PropCombinedMonOp<EOT>(*ptMon, uniformMutRateParam.value());

		// mutate exactly 1 component (uniformly) per individual
		ptMon = new DetUniformMutation<EOT>(*ptBounds, epsilonParam.value());
		_state.storeFunctor(ptMon);
		ptCombinedMonOp->add(*ptMon, detMutRateParam.value());

		// mutate all component using Gaussian mutation
		ptMon = new NormalMutation<EOT>(*ptBounds, sigmaParam.value());
		_state.storeFunctor(ptMon);
		ptCombinedMonOp->add(*ptMon, normalMutRateParam.value());
		_state.storeFunctor(ptCombinedMonOp);
	    }

	// now build the GenOp:
	// to simulate SGA (crossover with proba pCross + mutation with proba pMut
	// we must construct
	//     a sequential combination of
	//          with proba 1, a proportional combination of
	//                        a QuadCopy and our crossover
	//          with proba pMut, our mutation

	// the crossover - with probability pCross
	ProportionalOp<EOT> * cross = new ProportionalOp<EOT> ;
	_state.storeFunctor(cross);
	ptQuad = new QuadCloneOp<EOT>;
	_state.storeFunctor(ptQuad);
	cross->add(*ptCombinedQuadOp, pCrossParam.value()); // user crossover
	cross->add(*ptQuad, 1-pCrossParam.value()); // clone operator

	// now the sequential
	SequentialOp<EOT> *op = new SequentialOp<EOT>;
	_state.storeFunctor(op);
	op->add(*cross, 1.0);	 // always crossover (but clone with prob 1-pCross
	op->add(*ptCombinedMonOp, pMutParam.value());

	// that's it!
	return *op;
    }

    /** @} */

}

#endif
