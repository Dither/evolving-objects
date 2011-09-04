// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// make_op.h
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

// the specialized GA stuff
#include <ga/Bit.h>
#include <ga/BitOp.h>
  // also need the parser and param includes
#include <utils/Parser.h>
#include <utils/State.h>

namespace eo
{

    /////////////////// bitstring operators ///////////////
    // canonical (crossover + mutation) only at the moment //

    /*
     * This function builds the operators that will be applied to the bitstrings
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
     *
     * @ingroup bitstring
     * @ingroup Builders
     */

    template <class EOT>
    GenOp<EOT> & do_make_op(Parser& _parser, State& _state, Init<EOT>& _init)
    {
	// this is a temporary version, while Maarten codes the full tree-structured
	// general operator input
	// BTW we must leave that simple version available somehow, as it is the one
	// that 90% people use!
	ValueParam<std::string>& operatorParam =  _parser.createParam(std::string("SGA"), "operator", "Description of the operator (SGA only now)", 'o', "Variation Operators");

	if (operatorParam.value() != std::string("SGA"))
	    throw std::runtime_error("Only SGA-like operator available right now\n");

	// now we read Pcross and Pmut, 
	// the relative weights for all crossovers -> proportional choice
	// the relative weights for all mutations -> proportional choice
	// and create the GenOp that is exactly 
	// crossover with pcross + mutation with pmut

	ValueParam<double>& pCrossParam = _parser.createParam(0.6, "pCross", "Probability of Crossover", 'C', "Variation Operators" );
	// minimum check
	if ( (pCrossParam.value() < 0) || (pCrossParam.value() > 1) )
	    throw std::runtime_error("Invalid pCross");

	ValueParam<double>& pMutParam = _parser.createParam(0.1, "pMut", "Probability of Mutation", 'M', "Variation Operators" );
	// minimum check
	if ( (pMutParam.value() < 0) || (pMutParam.value() > 1) )
	    throw std::runtime_error("Invalid pMut");

	// the crossovers
	/////////////////
	// the parameters
	ValueParam<double>& onePointRateParam = _parser.createParam(double(1.0), "onePointRate", "Relative rate for one point crossover", '1', "Variation Operators" );
	// minimum check
	if ( (onePointRateParam.value() < 0) )
	    throw std::runtime_error("Invalid onePointRate");

	ValueParam<double>& twoPointsRateParam = _parser.createParam(double(1.0), "twoPointRate", "Relative rate for two point crossover", '2', "Variation Operators" );
	// minimum check
	if ( (twoPointsRateParam.value() < 0) )
	    throw std::runtime_error("Invalid twoPointsRate");

	ValueParam<double>& uRateParam = _parser.createParam(double(2.0), "uRate", "Relative rate for uniform crossover", 'U', "Variation Operators" );
	// minimum check
	if ( (uRateParam.value() < 0) )
	    throw std::runtime_error("Invalid uRate");

	// minimum check
	bool bCross = true;
	if (onePointRateParam.value()+twoPointsRateParam.value()+uRateParam.value()==0)
	    {
		std::cerr << "Warning: no crossover" << std::endl;
		bCross = false;
	    }

	// Create the CombinedQuadOp
	PropCombinedQuadOp<EOT> *ptCombinedQuadOp = NULL;
	QuadOp<EOT> *ptQuad = NULL;
	// 1-point crossover for bitstring
	ptQuad = new OnePtBitXover<EOT>;
	_state.storeFunctor(ptQuad);
	ptCombinedQuadOp = new PropCombinedQuadOp<EOT>(*ptQuad, onePointRateParam.value());

	// uniform crossover for bitstring
	ptQuad = new UBitXover<EOT>;
	_state.storeFunctor(ptQuad);
	ptCombinedQuadOp->add(*ptQuad, uRateParam.value());

	// 2-points xover
	ptQuad = new NPtsBitXover<EOT>;
	_state.storeFunctor(ptQuad);
	ptCombinedQuadOp->add(*ptQuad, twoPointsRateParam.value());

	// don't forget to store the CombinedQuadOp
	_state.storeFunctor(ptCombinedQuadOp);

	// the mutations
	/////////////////
	// the parameters
	ValueParam<double> & pMutPerBitParam = _parser.createParam(0.01, "pMutPerBit", "Probability of flipping 1 bit in bit-flip mutation", 'b', "Variation Operators" );
	// minimum check
	if ( (pMutPerBitParam.value() < 0) || (pMutPerBitParam.value() > 0.5) )
	    throw std::runtime_error("Invalid pMutPerBit");

	ValueParam<double> & bitFlipRateParam = _parser.createParam(0.01, "bitFlipRate", "Relative rate for bit-flip mutation", 's', "Variation Operators" );
	// minimum check
	if ( (bitFlipRateParam.value() < 0) )
	    throw std::runtime_error("Invalid bitFlipRate");
      
	ValueParam<double> & oneBitRateParam = _parser.createParam(0.01, "oneBitRate", "Relative rate for deterministic bit-flip mutation", 'd', "Variation Operators" );
	// minimum check
	if ( (oneBitRateParam.value() < 0) )
	    throw std::runtime_error("Invalid oneBitRate");

	// minimum check
	bool bMut = true;
	if (bitFlipRateParam.value()+oneBitRateParam.value()==0)
	    {
		std::cerr << "Warning: no mutation" << std::endl;
		bMut = false;
	    }
    
	// Create the CombinedMonOp
	PropCombinedMonOp<EOT> *ptCombinedMonOp = NULL;
	MonOp<EOT> *ptMon = NULL;

	// standard bit-flip mutation for bitstring
	ptMon = new BitMutation<EOT>(pMutPerBitParam.value());
	_state.storeFunctor(ptMon);
	// create the CombinedMonOp
	ptCombinedMonOp = new PropCombinedMonOp<EOT>(*ptMon, bitFlipRateParam.value());

	// mutate exactly 1 bit per individual
	ptMon = new DetBitFlip<EOT>; 
	_state.storeFunctor(ptMon);
	ptCombinedMonOp->add(*ptMon, oneBitRateParam.value());

	_state.storeFunctor(ptCombinedMonOp);

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

}

#endif
