// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-
//-----------------------------------------------------------------------------
// Op.h
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
    CVS Info: $Date: 2004-08-10 17:19:46 $ $Header: /home/nojhan/dev/eodev/eodev_cvs/eo/src/Op.h,v 1.29 2004-08-10 17:19:46 jmerelo Exp $ $Author: jmerelo $ 
 */
//-----------------------------------------------------------------------------

#ifndef _Op_H
#define _Op_H

#include <Object.h>
#include <Printable.h>
#include <Functor.h>
#include <utils/RNG.h>

namespace eo
{

    /**
       @defgroup Operators Evolutionary Operators

       in EO, an operator is any functors that modifies objects and inherits from an Op.

       Typically, a mutation is an operator that modifies an individual, and an algorithm is an operator that modifies a population.

       In EO, there is a genetic operator hierarchy, with Op as father and
       MonOp (monary or unary operator), BinOp and QuadOp (binary operators)
       and GenOp (any number of inputs and outputs, see GenOp.h)
       as subclasses.
       Nobody should subclass Op, you should subclass GenOp, BinOp, QuadOp
       or MonOp, those are the ones actually used here.

       #Op#s are only printable objects, so if you want to build them
       from a file, it has to be done in another class, namely factories.
       Each hierarchy of #Op#s should have its own factory, which know
       how to build them from a description in a file.

       @author GeNeura Team, Marten Keijzer and Marc Schoenauer
       @version 0.9
       @see GenOp.h OpFactory
    */
    //@{

    /**
       @defgroup Variators Variation operators
       Variators are operators that modify individuals.

       @defgroup Selectors Selection operators

       Selectors are operators that select a subset of a population.

       Example:
       @include t-Select.cpp


       @defgroup Replacors Replacement operators

       Replacors are operators that replace a subset of a population by another set of individuals.

       Here is an example with several replacement operators:
       @include t-Replacement.cpp
    */

    /** Abstract data types for EO operators.
	Genetic operators act on chromosomes, changing them.
	The type to use them on is problem specific. If your genotype
	is a std::vector<bool>, there are operators that work specifically
	on std::vector<bool>, but you might also find that generic operators
	working on std::vector<T> are what you need.

    */
    template<class EOType>
    class Op
    {
    public:
	//@{
	enum OpType { unary = 0, binary = 1, quadratic = 2, general = 3};
	///

	/// Ctor
	Op(OpType _type)
	    :opType( _type ) {};

	/// Copy Ctor
	Op( const Op& _eop )
	    :opType( _eop.opType ) {};

	/// Needed virtual destructor
	virtual ~Op(){};

	/// getType: number of operands it takes and individuals it produces
	OpType getType() const {return opType;};

    private:

	/// OpType is the type of the operator: how many operands it takes and how many it produces
	OpType opType;
    };

    /**
       MonOp is the monary operator: genetic operator that takes only one EO.
       When defining your own, make sure that you return a boolean value
       indicating that you have changed the content.
    */
    template <class EOType>
    class MonOp: public Op<EOType>, public UF<EOType&, bool>
    {
    public:
	/// Ctor
	MonOp()
	    : Op<EOType>( Op<EOType>::unary ) {};
	virtual std::string className() const {return "MonOp";};
    };


    /** Binary genetic operator: subclasses Op, and defines basically the
     *  operator() with two operands, only the first one can be modified
     When defining your own, make sure that you return a boolean value
     indicating that you have changed the content.
    */
    template<class EOType>
    class BinOp: public Op<EOType>, public BF<EOType&, const EOType&, bool>
    {
    public:
	/// Ctor
	BinOp()
	    :Op<EOType>( Op<EOType>::binary ) {};
	virtual std::string className() const {return "BinOp";};
    };

    /** Quad genetic operator: subclasses Op, and defines basically the
	operator() with two operands, both can be modified.
	When defining your own, make sure that you return a boolean value
	indicating that you have changed the content.
    */
    template<class EOType>
    class QuadOp: public Op<EOType>, public BF<EOType&, EOType&, bool> {
    public:
	/// Ctor
	QuadOp()
	    :Op<EOType>( Op<EOType>::quadratic ) {};
	virtual std::string className() const {return "QuadOp";};
    };

    /** Turning an QuadOp into an BinOp: simply don't touch the second arg!
     */
    template <class EOT>
    class Quad2BinOp: public BinOp<EOT>
    {
    public:
	/** Ctor
	 * @param _quadOp the QuadOp to be transformed
	 */
	Quad2BinOp(QuadOp<EOT> & _quadOp) : quadOp(_quadOp) {}

	/** Operator() simply calls embedded quadOp operator() with dummy second arg
	 */
	bool operator()(EOT & _eo1, const EOT & _eo2)
	{
	    EOT eoTmp = _eo2;		   // a copy that can be modified
	    // if the embedded QuadOp is not symmetrical,
	    // the result might be biased - hence the flip ...
	    if (rng.flip(0.5))
		return quadOp(_eo1, eoTmp);	   // both are modified - that's all
	    else
		return quadOp(eoTmp, _eo1);	   // both are modified - that's all
	}

    private:
	QuadOp<EOT> & quadOp;
    };

}

#endif

//@}
