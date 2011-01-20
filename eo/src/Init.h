// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// Init.h
// (c) Maarten Keijzer 2000, GeNeura Team, 2000
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
             mak@dhi.dk
 */
//-----------------------------------------------------------------------------

#ifndef _Init_H
#define _Init_H

#include <algorithm>

#include <Op.h>
#include <STLFunctor.h>
#include <utils/RndGenerators.h>
#include <utils/rnd_generators.h>  // for shuffle method

namespace eo
{

    /**
       @defgroup Initializators Initialization operators
       @ingroup Operators

       Initializators are operators that creates initial individuals and populations.
    */
    /** @{*/
    /**
       Base (name) class for Initialization of chromosomes, used in a population
       contructor. It is derived from MonOp, so it can be used
       inside the algorithm as well.

       @see Pop
    */
    template <class EOT>
    class Init : public UF<EOT&, void>
    {
    public:

	/** className: Mandatory because of CombinedInit. 
	    SHould be pure virtual, but then we should go over the whole
	    * code to write the method for all derived classes ... MS 16/7/04 */
	virtual std::string className(void) const { return "Init"; }
    };

    /** turning an Init into a generator 
     * probably we should only use genrators - and suppress Init ???
     * MS - July 2001
     */
    template <class EOT>
    class InitGenerator :  public F<EOT>
    {
    public:

	/** Ctor from a plain Init */
	InitGenerator(Init<EOT> & _init):init(_init) {}

	virtual EOT operator()()
	{
	    EOT p;
	    init(p);
	    return (p);
	}
    private:
	Init<EOT> & init;
    };

    /**
       Initializer for fixed length representations with a single type
    */
    template <class EOT>
    class InitFixedLength: public Init<EOT>
    {
    public:

	typedef typename EOT::AtomType AtomType;

        InitFixedLength(unsigned _combien, RndGenerator<AtomType>& _generator)
            : combien(_combien), generator(_generator) {}

        virtual void operator()(EOT& chrom)
        {
            chrom.resize(combien);
            std::generate(chrom.begin(), chrom.end(), generator);
            chrom.invalidate();
        }

    private :
        unsigned combien;
        /// generic wrapper for Functor (s), to make them have the function-pointer style copy semantics
        STLF<AtomType> generator;
    };

    /**
       Initializer for variable length representations with a single type
    */
    template <class EOT>
    class InitVariableLength: public Init<EOT>
    {
    public:
	typedef typename EOT::AtomType AtomType; 

	//   /** Ctor from a generator */
	//   InitVariableLength(unsigned _minSize, unsigned _maxSize, F<typename EOT::AtomType> & _generator = Gen())
	//     : offset(_minSize), extent(_maxSize - _minSize), 
	// 			 repGenerator( InitGenerator<typename EOT::AtomType>(*(new Init<EOT>)) ), 
	// 			 generator(_generator)
	//   {
	//     if (_minSize >= _maxSize)
	//       throw std::logic_error("InitVariableLength: minSize larger or equal to maxSize");
	//   }

	/** Ctor from an Init */
	InitVariableLength(unsigned _minSize, unsigned _maxSize, Init<AtomType> & _init)
	    : offset(_minSize), extent(_maxSize - _minSize), init(_init)
	{
	    if (_minSize >= _maxSize)
		throw std::logic_error("InitVariableLength: minSize larger or equal to maxSize");
	}


	virtual void operator()(EOT& _chrom)
	{
	    _chrom.resize(offset + rng.random(extent));
	    typename std::vector<AtomType>::iterator it;
	    for (it=_chrom.begin(); it<_chrom.end(); it++)
		init(*it);
	    _chrom.invalidate();
	}

	// accessor to the atom initializer (needed by operator constructs sometimes)
	Init<AtomType> & atomInit() {return init;}

    private :
	unsigned offset;
	unsigned extent;
	Init<AtomType> & init;
    };


    /**
       Initializer for permutation (integer-based) representations.
    */
    template <class EOT>
    class InitPermutation: public Init<EOT>
    {
    public:

	typedef typename EOT::AtomType AtomType;

        InitPermutation(unsigned _chromSize, unsigned _startFrom=0)
            : chromSize(_chromSize), startFrom(_startFrom){}

        virtual void operator()(EOT& chrom)
        {
            chrom.resize(chromSize);            
            for(unsigned idx=0;idx <chrom.size();idx++)
		chrom[idx]=idx+startFrom;  

            std::random_shuffle(chrom.begin(), chrom.end(),gen);            
            chrom.invalidate();
        }

    private :
        unsigned chromSize;
        unsigned startFrom;
        UF_random_generator<unsigned int> gen;
    };
    /** @example t-InitPermutation.cpp
     */


    /**
       InitAdaptor changes the place in the hierarchy
       from Init to MonOp. This is mainly a type conversion,
       nothing else
    
       @see Init, MonOp
    */
    template <class EOT>
    class InitAdaptor : public MonOp<EOT>
    {
    public :
        InitAdaptor(Init<EOT>& _init) : init(_init) {}

        bool operator()(EOT& _eot)
        {
            init(_eot);
            return true;
        }
    private :

        Init<EOT>& init;
    };

}

#endif
/** @}*/
