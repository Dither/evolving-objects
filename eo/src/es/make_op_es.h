/* (c) Maarten Keijzer, Marc Schoenauer and GeNeura Team, 2001

This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your option) any
later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this library; if not, write to the Free Software Foundation, Inc., 59
Temple Place, Suite 330, Boston, MA 02111-1307 USA

Contact: http://eodev.sourceforge.net
         todos@geneura.ugr.es, http://geneura.ugr.es
         Marc.Schoenauer@polytechnique.fr
         mkeijzer@dhi.dk
 */


#ifndef EO_make_op_h
#define EO_make_op_h

// the operators
#include <Op.h>
#include <GenOp.h>
#include <CloneOps.h>
#include <OpContainer.h>
// combinations of simple Ops (MonOp and QuadOp)
#include <ProportionalCombinedOp.h>

// the specialized Real stuff
#include <es/Real.h>
#include <es/RealAtomXover.h>
#include <es/EsChromInit.h>
#include <es/EsMutationInit.h>
#include <es/EsMutate.h>
#include <es/EsGlobalXover.h>
#include <es/EsStandardXover.h>
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
    GenOp<EOT> & do_make_op(Parser& _parser, State& _state, RealInitBounded<EOT>& _init)
    {
	// get std::vector size
	unsigned vecSize = _init.size();

	// First, decide whether the objective variables are bounded
	ValueParam<RealVectorBounds>& boundsParam
	    = _parser.getORcreateParam(RealVectorBounds(vecSize,DummyRealNoBounds),
				       "objectBounds", "Bounds for variables",
				       'B', "Variation Operators");

	std::cerr << boundsParam.value() << std::endl;

	// now we read Pcross and Pmut,
	ValueParam<std::string>& operatorParam
	    = _parser.getORcreateParam(std::string("SGA"), "operator",
				       "Description of the operator (SGA only now)",
				       'o', "Variation Operators");

	if (operatorParam.value() != std::string("SGA"))
	    throw std::runtime_error("Sorry, only SGA-like operator available right now\n");

	// now we read Pcross and Pmut,
	// and create the GenOp that is exactly
	// crossover with pcross + mutation with pmut

	ValueParam<double>& pCrossParam
	    = _parser.getORcreateParam(1.0, "pCross", "Probability of Crossover",
				       'C', "Variation Operators" );
	// minimum check
	if ( (pCrossParam.value() < 0) || (pCrossParam.value() > 1) )
	    throw std::runtime_error("Invalid pCross");

	ValueParam<double>& pMutParam
	    = _parser.getORcreateParam(1.0, "pMut", "Probability of Mutation",
				       'M', "Variation Operators" );
	// minimum check
	if ( (pMutParam.value() < 0) || (pMutParam.value() > 1) )
	    throw std::runtime_error("Invalid pMut");


	// crossover
	/////////////
	// ES crossover
	ValueParam<std::string>& crossTypeParam
	    = _parser.getORcreateParam(std::string("global"), "crossType",
				       "Type of ES recombination (global or standard)",
				       'C', "Variation Operators");

	ValueParam<std::string>& crossObjParam
	    = _parser.getORcreateParam(std::string("discrete"), "crossObj",
				       "Recombination of object variables (discrete, intermediate or none)",
				       'O', "Variation Operators");
	ValueParam<std::string>& crossStdevParam
	    = _parser.getORcreateParam(std::string("intermediate"), "crossStdev",
				       "Recombination of mutation strategy parameters "
				       "(intermediate, discrete or none)",
				       'S', "Variation Operators");

	// The pointers: first the atom Xover
	BinOp<double> *ptObjAtomCross = NULL;
	BinOp<double> *ptStdevAtomCross = NULL;
	// then the EOT-level one (need to be an GenOp as global Xover is
	GenOp<EOT> *ptCross;

	// check for the atom Xovers
	if (crossObjParam.value() == std::string("discrete"))
	    ptObjAtomCross = new DoubleExchange;
	else if (crossObjParam.value() == std::string("intermediate"))
	    ptObjAtomCross = new DoubleIntermediate;
	else if (crossObjParam.value() == std::string("none"))
	    ptObjAtomCross = new BinCloneOp<double>;
	else throw std::runtime_error("Invalid Object variable crossover type");

	if (crossStdevParam.value() == std::string("discrete"))
	    ptStdevAtomCross = new DoubleExchange;
	else if (crossStdevParam.value() == std::string("intermediate"))
	    ptStdevAtomCross = new DoubleIntermediate;
	else if (crossStdevParam.value() == std::string("none"))
	    ptStdevAtomCross = new BinCloneOp<double>;
	else throw std::runtime_error("Invalid mutation strategy parameter crossover type");

	// and build the indi Xover
	if (crossTypeParam.value() == std::string("global"))
	    ptCross = new EsGlobalXover<EOT>(*ptObjAtomCross, *ptStdevAtomCross);
	else if (crossTypeParam.value() == std::string("standard"))
	{	   // using a standard BinOp, but wrap it into an GenOp
	    BinOp<EOT> & crossTmp = _state.storeFunctor(
		new EsStandardXover<EOT>(*ptObjAtomCross, *ptStdevAtomCross)
		);
	    ptCross = new BinGenOp<EOT>(crossTmp);
	}
	else throw std::runtime_error("Invalide Object variable crossover type");

	// now that everything is OK, DON'T FORGET TO STORE MEMORY
	_state.storeFunctor(ptObjAtomCross);
	_state.storeFunctor(ptStdevAtomCross);
	_state.storeFunctor(ptCross);

	//  mutation
	/////////////

	// Ok, time to set up the self-adaptive mutation
	// Proxy for the mutation parameters
	EsMutationInit mutateInit(_parser, "Variation Operators");

	EsMutate<EOT> & mut =  _state.storeFunctor(
	    new EsMutate<EOT>(mutateInit, boundsParam.value()));

	// now the general op - a sequential application of crossover and mutatation
	// no need to first have crossover combined with a clone as it is an
	// BinOp and not an QuadOp as in SGA paradigm

	SequentialOp<EOT> & op = _state.storeFunctor(new SequentialOp<EOT>);
	op.add(*ptCross, pCrossParam.value());
	op.add(mut, pMutParam.value());

	// that's it!
	return op;
    }

}

#endif // EO_make_op_h

/** @} */

// Local Variables:
// coding: iso-8859-1
// mode:C++
// c-file-style: "Stroustrup"
// comment-column: 35
// fill-column: 80
// End:
