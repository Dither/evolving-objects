// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// EasyEA.h
// (c) GeNeura Team, 1998
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
 */
//-----------------------------------------------------------------------------

#ifndef _EasyEA_h
#define _EasyEA_h

//-----------------------------------------------------------------------------

#include <apply.h>
#include <Algo.h>
#include <PopEvalFunc.h>
#include <Continue.h>
#include <Select.h>
#include <Transform.h>
#include <Breed.h>
#include <MergeReduce.h>
#include <Replacement.h>

namespace eo
{

    template <class EOT> class IslandsEasyEA ;

    template <class EOT> class DistEvalEasyEA ;

    /** An easy-to-use evolutionary algorithm; you can use any chromosome,
	and any selection transformation, merging and evaluation
	algorithms; you can even change in runtime parameters of those
	sub-algorithms

	Change (MS, July 3. 2001):
	Replaced the EvalFunc by an PopEvalFunc: this immediately
	allows many useful constructs, such as co-evolution (e.g. game players),
	parisian approach (the solution to the problem is the whole population)
	or simple distribution of evaluations on a cluster.
	In case an EvalFunc is passed, it is embedded on an PopLoopEval
	This makes things a little uglier (required an additional "dummy" member

	Note: it looks ugly only because we wanted to authorize many different
	constructors. Please only look at the operator() and there shall be light

	@ingroup Algorithms
    */
    template<class EOT> class EasyEA: public Algo<EOT>
    {
    public:

	/** Ctor taking a breed and merge */
	EasyEA(
		 Continue<EOT>& _continuator,
		 EvalFunc<EOT>& _eval,
		 Breed<EOT>& _breed,
		 Replacement<EOT>& _replace
		 ) : continuator(_continuator),
		     eval (_eval),
		     loopEval(_eval),
		     popEval(loopEval),
		     selectTransform(dummySelect, dummyTransform),
		     breed(_breed),
		     mergeReduce(dummyMerge, dummyReduce),
		     replace(_replace)
	{}

	/*
	  EasyEA(Continue <EOT> & _continuator,
	  PopEvalFunc <EOT> & _pop_eval,
	  Breed <EOT> & _breed,
	  Replacement <EOT> & _replace
	  ) :
	  continuator (_continuator),
	  eval (dummyEval),
	  loopEval(dummyEval),
	  popEval (_pop_eval),
	  selectTransform (dummySelect, dummyTransform),
	  breed (_breed),
	  mergeReduce (dummyMerge, dummyReduce),
	  replace (_replace) {

	  }
	*/

	/** NEW Ctor taking a breed and merge and an PopEval */
	EasyEA(
		 Continue<EOT>& _continuator,
		 PopEvalFunc<EOT>& _eval,
		 Breed<EOT>& _breed,
		 Replacement<EOT>& _replace
		 ) : continuator(_continuator),
		     eval (dummyEval),
		     loopEval(dummyEval),
		     popEval(_eval),
		     selectTransform(dummySelect, dummyTransform),
		     breed(_breed),
		     mergeReduce(dummyMerge, dummyReduce),
		     replace(_replace)
	{}


	/// Ctor Select, Transform, Replacement and an PopEval
	EasyEA(
		 Continue<EOT>& _continuator,
		 PopEvalFunc<EOT>& _eval,
		 Select<EOT>& _select,
		 Transform<EOT>& _transform,
		 Replacement<EOT>& _replace
		 ) : continuator(_continuator),
		     eval (dummyEval),
		     loopEval(dummyEval),
		     popEval(_eval),
		     selectTransform(_select, _transform),
		     breed(selectTransform),
		     mergeReduce(dummyMerge, dummyReduce),
		     replace(_replace)
	{}

	/// Ctor Breed, Merge and Reduce.
	EasyEA(
		 Continue<EOT>& _continuator,
		 EvalFunc<EOT>& _eval,
		 Breed<EOT>& _breed,
		 Merge<EOT>& _merge,
		 Reduce<EOT>& _reduce
		 ) : continuator(_continuator),
		     eval (_eval),
		     loopEval(_eval),
		     popEval(loopEval),
		     selectTransform(dummySelect, dummyTransform),
		     breed(_breed),
		     mergeReduce(_merge, _reduce),
		     replace(mergeReduce)
	{}

	/// Ctor Select, Transform, and Replacement
	EasyEA(
		 Continue<EOT>& _continuator,
		 EvalFunc<EOT>& _eval,
		 Select<EOT>& _select,
		 Transform<EOT>& _transform,
		 Replacement<EOT>& _replace
		 ) : continuator(_continuator),
		     eval (_eval),
		     loopEval(_eval),
		     popEval(loopEval),
		     selectTransform(_select, _transform),
		     breed(selectTransform),
		     mergeReduce(dummyMerge, dummyReduce),
		     replace(_replace)
	{}
    
	/// Ctor Select, Transform, Merge and Reduce.
	EasyEA(
		 Continue<EOT>& _continuator,
		 EvalFunc<EOT>& _eval,
		 Select<EOT>& _select,
		 Transform<EOT>& _transform,
		 Merge<EOT>&     _merge,
		 Reduce<EOT>&    _reduce
		 ) : continuator(_continuator),
		     eval (_eval),
		     loopEval(_eval),
		     popEval(loopEval),
		     selectTransform(_select, _transform),
		     breed(selectTransform),
		     mergeReduce(_merge, _reduce),
		     replace(mergeReduce)
	{}




	/// Apply a few generation of evolution to the population.
	virtual void operator()(Pop<EOT>& _pop)
	{
	    Pop<EOT> offspring, empty_pop;

	    popEval(empty_pop, _pop); // A first eval of pop.

	    do
		{
		    try
			{
			    unsigned pSize = _pop.size();
			    offspring.clear(); // new offspring

			    breed(_pop, offspring);

			    popEval(_pop, offspring); // eval of parents + offspring if necessary

			    replace(_pop, offspring); // after replace, the new pop. is in _pop

			    if (pSize > _pop.size())
				throw std::runtime_error("Population shrinking!");
			    else if (pSize < _pop.size())
				throw std::runtime_error("Population growing!");

			}
		    catch (std::exception& e)
			{
			    std::string s = e.what();
			    s.append( " in EasyEA");
			    throw std::runtime_error( s );
			}
		}
	    while ( continuator( _pop ) );
	}

    protected :

	// If selectTransform needs not be used, dummySelect and dummyTransform are used
	// to instantiate it.
	class DummySelect : public Select<EOT>
	{
	public :
	    void operator()(const Pop<EOT>&, Pop<EOT>&)
	    {}
	}
	dummySelect;

	class DummyTransform : public Transform<EOT>
	{
	public :
	    void operator()(Pop<EOT>&)
	    {}
	}
	    dummyTransform;

	class DummyEval : public EvalFunc<EOT>
	{
	public:
	    void operator()(EOT &)
	    {}
	}
	    dummyEval;

	Continue<EOT>&          continuator;

	EvalFunc <EOT> &        eval ;
	PopLoopEval<EOT>        loopEval;

	PopEvalFunc<EOT>&       popEval;

	SelectTransform<EOT>    selectTransform;
	Breed<EOT>&             breed;

	// If mergeReduce needs not be used, dummyMerge and dummyReduce are used
	// to instantiate it.
	NoElitism<EOT>          dummyMerge;
	Truncate<EOT>           dummyReduce;

	MergeReduce<EOT>        mergeReduce;
	Replacement<EOT>&       replace;

	// Friend classes
	friend class IslandsEasyEA <EOT> ;
	friend class DistEvalEasyEA <EOT> ;
    };

    /**
       @example t-EasyEA.cpp
       Example of a test program building an EA algorithm.
    */

}

#endif

