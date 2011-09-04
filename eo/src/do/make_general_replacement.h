// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// make_general_replacement.h
// (c) Marc Schoenauer and Pierre Collet, 2002
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

#ifndef _make_general_replacement_h
#define _make_general_replacement_h

#include <utils/Data.h>     // for eo_is_a_rate

// Replacement
#include <ReduceMergeReduce.h>

// also need the parser and param includes
#include <utils/Parser.h>
#include <utils/State.h>

namespace eo
{

    /** a helper function that decodes a parameter read by the parser into an
     * Reduce<EOT> & (allocates the pointer and stores it into an State)
     *
     * @ingroup Builders
     */
    template <class EOT>
    Reduce<EOT> & decode_reduce(ParamParamType & _ppReduce, State & _state)
    {
	unsigned int detSize;
	Reduce<EOT> * ptReduce;

	// ---------- Deterministic
	if ( (_ppReduce.first == std::string("Deterministic")) || 
	     (_ppReduce.first == std::string("Sequential")) 
	     )
	    {
		ptReduce = new Truncate<EOT>;
	    }
	// ---------- EP
	else if (_ppReduce.first == std::string("EP"))
	    {
		if (!_ppReduce.second.size())   // no parameter added
		    {
			std::cerr << "WARNING, no parameter passed to EP, using 6" << std::endl;
			detSize = 6;
			// put back 6 in parameter for consistency (and status file)
			_ppReduce.second.push_back(std::string("6"));
		    }
		else	  // parameter passed by user as EP(T)
		    detSize = atoi(_ppReduce.second[0].c_str());
		ptReduce = new EPReduce<EOT>(detSize);
	    }
	// ---------- DetTour
	else if (_ppReduce.first == std::string("DetTour"))
	    {
		if (!_ppReduce.second.size())   // no parameter added
		    {
			std::cerr << "WARNING, no parameter passed to DetTour, using 2" << std::endl;
			detSize = 2;
			// put back 2 in parameter for consistency (and status file)
			_ppReduce.second.push_back(std::string("2"));
		    }
		else	  // parameter passed by user as DetTour(T)
		    detSize = atoi(_ppReduce.second[0].c_str());
		ptReduce = new DetTournamentTruncate<EOT>(detSize);
	    }
	else if (_ppReduce.first == std::string("StochTour"))
	    {
		double p;
		if (!_ppReduce.second.size())   // no parameter added
		    {
			std::cerr << "WARNING, no parameter passed to StochTour, using 1" << std::endl;
			p = 1;
			// put back p in parameter for consistency (and status file)
			_ppReduce.second.push_back(std::string("1"));
		    }
		else	  // parameter passed by user as DetTour(T)
		    {
			p = atof(_ppReduce.second[0].c_str());
			if ( (p<=0.5) || (p>1) )
			    throw std::runtime_error("Stochastic tournament size should be in [0.5,1]");
		    }
      
		ptReduce = new StochTournamentTruncate<EOT>(p);
	    }
	else if ( (_ppReduce.first == std::string("Uniform")) || 
		  (_ppReduce.first == std::string("Random"))
		  )
	    {
		ptReduce = new RandomReduce<EOT>;
	    }
	else // no known reduction entered
	    {
		throw std::runtime_error("Unknown reducer: " + _ppReduce.first);
	    }
	// all done, stores and return a reference
	_state.storeFunctor(ptReduce);
	return (*ptReduce);
    }

    /** Helper function that creates a replacement from the class 
     * ReduceMergeReduce using 6 parameters 
     *      (after the usual State and Parser)
     * 
     *  HowMany _elite              the number of elite parents (0 = no elitism)
     *       see below
     *  bool _strongElitism           if elite > 0, std::string elitism or weak elitism
     *       strong = elite parents survive, whatever the offspring
     *       weak - elite patents compete AFTER replacement with best offspring
     *  HowMany _surviveParents     number of parents after parents recuction
     *  ParamParamType & _reduceParentType      how the parents are reduced
     *  HowMany _surviveOffspring   number of offspring after offspring recuction
     *  ParamParamType & _reduceOffspringType   how the offspring are reduced
     *  ParamParamType & _reduceFinalType       how the final population is reduced to initial population size
     *
     * @ingroup Builders
     */
    template <class EOT>
    Replacement<EOT> & make_general_replacement(
						  Parser& _parser, State& _state, 
						  HowMany _elite = HowMany(0), 
						  bool _strongElitism = false,
						  HowMany _surviveParents = HowMany(0.0),
						  ParamParamType & _reduceParentType = ParamParamType("Deterministic"),
						  HowMany _surviveOffspring = HowMany(1.0),
						  ParamParamType & _reduceOffspringType = ParamParamType("Deterministic"),
						  ParamParamType & _reduceFinalType  = ParamParamType("Deterministic")
						  )
    {
	/////////////////////////////////////////////////////
	// the replacement
	/////////////////////////////////////////////////////

	// Elitism
	HowMany elite =  _parser.createParam(_elite, "elite", "Nb of elite parents (percentage or absolute)", '\0', "Evolution Engine / Replacement").value();

	bool strongElitism = _parser.createParam(_strongElitism,"eliteType", "Strong (true) or weak (false) elitism (set elite to 0 for none)", '\0', "Evolution Engine / Replacement").value();

	// reduce the parents
	HowMany surviveParents =  _parser.createParam(_surviveParents, "surviveParents", "Nb of surviving parents (percentage or absolute)", '\0', "Evolution Engine / Replacement").value();
  
	ParamParamType & reduceParentType = _parser.createParam(_reduceParentType, "reduceParents", "Parents reducer: Deterministic, EP(T), DetTour(T), StochTour(t), Uniform", '\0', "Evolution Engine / Replacement").value();

	Reduce<EOT> & reduceParent = decode_reduce<EOT>(reduceParentType, _state);

	// reduce the offspring
	HowMany surviveOffspring =  _parser.createParam(_surviveOffspring, "surviveOffspring", "Nb of surviving offspring (percentage or absolute)", '\0', "Evolution Engine / Replacement").value();
  
	ParamParamType & reduceOffspringType = _parser.createParam(_reduceOffspringType, "reduceOffspring", "Offspring reducer: Deterministic, EP(T), DetTour(T), StochTour(t), Uniform", '\0', "Evolution Engine / Replacement").value();

	Reduce<EOT> & reduceOffspring = decode_reduce<EOT>(reduceOffspringType, _state);

	ParamParamType & reduceFinalType = _parser.createParam(_reduceFinalType, "reduceFinal", "Final reducer: Deterministic, EP(T), DetTour(T), StochTour(t), Uniform", '\0', "Evolution Engine / Replacement").value();

	Reduce<EOT> & reduceFinal = decode_reduce<EOT>(reduceFinalType, _state);

	// now the replacement itself
	ReduceMergeReduce<EOT> *ptReplace = new ReduceMergeReduce<EOT>(elite, strongElitism, surviveParents, reduceParent, surviveOffspring, reduceOffspring, reduceFinal);
	_state.storeFunctor(ptReplace);

	// that's it!
	return *ptReplace;
    }

}

#endif
