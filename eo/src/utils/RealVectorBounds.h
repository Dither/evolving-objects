// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// RealVectorBounds.h
// (c) Marc Schoenauer 2001, Maarten Keijzer 2000, GeNeura Team, 1998
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

#ifndef _RealVectorBounds_h
#define _RealVectorBounds_h

#include <stdexcept>		   // std::exceptions!
#include <utils/RNG.h>
#include <utils/RealBounds.h>

namespace eo
{

    /**
       Vector type for bounds (see RealBounds.h for scalar types)
       ------------
       Class RealVectorBounds implements the std::vectorized version:
       it is basically a std::vector of RealBounds * and forwards all request
       to the elements of the std::vector.

       This file also contains the global variables and DummyVectorNoBounds
       that are used as defaults in ctors (i.e. when no
       bounds are given, it is assumed unbounded values)

       THe 2 main classes defined here are

       RealBaseVectorBounds, base class that handles all useful functions
       RealVectorBounds which derives from the preceding *and* Persistent
       and also has a mechanism for memory handling of the pointers
       it has to allocate

       @ingroup Bounds
    */
    class RealBaseVectorBounds : public std::vector<RealBounds *>
    {
    public:
	// virtual desctructor (to avoid warning?)
	virtual ~RealBaseVectorBounds(){}

	/** Default Ctor.
	 */
	RealBaseVectorBounds() : std::vector<RealBounds *>(0) {}

	/** Ctor: same bounds for everybody, given as an RealBounds
	 */
	RealBaseVectorBounds(unsigned _dim, RealBounds & _bounds) :
	    std::vector<RealBounds *>(_dim, &_bounds)
	{}

	/** Ctor, particular case of dim-2
	 */
	RealBaseVectorBounds(RealBounds & _xbounds, RealBounds & _ybounds) : 
	    std::vector<RealBounds *>(0)
	{
	    push_back( &_xbounds);
	    push_back( &_ybounds);
	}

	/** test: is i_th component bounded
	 */
	virtual bool isBounded(unsigned _i)
	{
	    return (*this)[_i]->isBounded();
	}

	/** test: bounded iff all are bounded
	 */
	virtual bool isBounded(void)
	{
	    for (unsigned i=0; i<size(); i++)
		if (! (*this)[i]->isBounded())
		    return false;
	    return true;
	}

	/** Self-test: true iff i_th component has no bounds at all
	 */
	virtual bool hasNoBoundAtAll(unsigned _i)
	{
	    return (*this)[_i]->hasNoBoundAtAll();
	}

	/** Self-test: true iff all components have no bound at all
	 */
	virtual bool hasNoBoundAtAll(void)
	{
	    for (unsigned i=0; i<size(); i++)
		if (! (*this)[i]->hasNoBoundAtAll())
		    return false;
	    return true;
	}

	virtual bool isMinBounded(unsigned _i)
	{ return (*this)[_i]->isMinBounded();};

	virtual bool isMaxBounded(unsigned _i)
	{ return (*this)[_i]->isMaxBounded();};

	/** Folds a real value back into the bounds - i_th component
	 */
	virtual void foldsInBounds(unsigned _i, double & _r)
	{
	    (*this)[_i]->foldsInBounds(_r);
	}

	/** Folds all variables of a std::vector of real values into the bounds
	 */
	virtual void foldsInBounds(std::vector<double> & _v)
	{
	    for (unsigned i=0; i<size(); i++)
		{
		    (*this)[i]->foldsInBounds(_v[i]);
		}
	}

	/** Truncates a real value to the bounds - i_th component
	 */
	virtual void truncate(unsigned _i, double & _r)
	{
	    (*this)[_i]->truncate(_r);
	}

	/** truncates all variables of a std::vector of real values to the bounds
	 */
	virtual void truncate(std::vector<double> & _v)
	{
	    for (unsigned i=0; i<size(); i++)
		{
		    (*this)[i]->truncate(_v[i]);
		}
	}

	/** test: is i_th component within the bounds?
	 */
	virtual bool isInBounds(unsigned _i, double _r)
	{ return (*this)[_i]->isInBounds(_r); }

	/** test: are ALL components within the bounds?
	 */
	virtual bool isInBounds(std::vector<double> _v)
	{
	    for (unsigned i=0; i<size(); i++)
		if (! isInBounds(i, _v[i]))
		    return false;
	    return true;
	}

	/** Accessors: will raise an std::exception if these do not exist
	 */
	virtual double minimum(unsigned _i) {return (*this)[_i]->minimum();}
	virtual double maximum(unsigned _i) {return (*this)[_i]->maximum();}
	virtual double range(unsigned _i) {return (*this)[_i]->range();}

	/** Computes the average range
	 *  An std::exception will be raised if one of the component is unbounded
	 */
	virtual double averageRange()
	{
	    double r=0.0;
	    for (unsigned i=0; i<size(); i++)
		r += range(i);
	    return r/size();
	}

	/** Generates a random number in i_th range
	 *  An std::exception will be raised if one of the component is unbounded
	 */
	virtual double uniform(unsigned _i, Rng & _rng = rng)
	{
	    (void)_rng;

	    double r= (*this)[_i]->uniform();
	    return r;
	}

	/** fills a std::vector with uniformly chosen variables in bounds
	 *  An std::exception will be raised if one of the component is unbounded
	 */
	void uniform(std::vector<double> & _v, Rng & _rng = rng)
	{
	    _v.resize(size());
	    for (unsigned i=0; i<size(); i++)
		{
		    _v[i] = uniform(i, _rng);
		}
	}

	/**
	 * Write object. It's called printOn since it prints the object on a stream.
	 * @param _os A std::ostream.
	 */
	virtual void printOn(std::ostream& _os) const
	{
	    for (unsigned i=0; i<size(); i++)
		{
		    operator[](i)->printOn(_os);
		    _os << ";";
		}
	}
    };

    ////////////////////////////////////////////////////////////////////
    /** Now a derived class, for parser reading
     * It holds some of the bounds (and destroy them when dying)

     @ingroup Bounds
    */
    class RealVectorBounds : public RealBaseVectorBounds, public Persistent
    {
    public:
	/** Default Ctor will call base class default ctor
	 */
	RealVectorBounds():RealBaseVectorBounds() {}

	/** Ctor: same bounds for everybody, given as an RealBounds
	 */
	RealVectorBounds(unsigned _dim, RealBounds & _bounds) :
	    RealBaseVectorBounds(_dim, _bounds), factor(1,_dim), ownedBounds(0)
	{}

	/** Ctor, particular case of dim-2
	 */
	RealVectorBounds(RealBounds & _xbounds, RealBounds & _ybounds) :
	    RealBaseVectorBounds(_xbounds, _ybounds), factor(2,1), ownedBounds(0)
	{}

	/** Simple bounds = minimum and maximum (allowed)
	 */
	RealVectorBounds(unsigned _dim, double _min, double _max) :
	    RealBaseVectorBounds(), factor(1, _dim), ownedBounds(0)
	{
	    if (_max-_min<=0)
		throw std::logic_error("Void range in RealVectorBounds");
	    RealBounds *ptBounds = new RealInterval(_min, _max);
	    // handle memory once
	    ownedBounds.push_back(ptBounds);
	    // same bound for everyone
	    for (unsigned int i=0; i<_dim; i++)
		push_back(ptBounds);
	}

	/** Ctor: different bounds for different variables, std::vectors of double
	 */
	RealVectorBounds(std::vector<double> _min, std::vector<double> _max) :
	    factor(_min.size(), 1), ownedBounds(0)
	{
	    if (_max.size() != _min.size())
		throw std::logic_error("Dimensions don't match in RealVectorBounds");
	    // the bounds
	    RealBounds *ptBounds;
	    for (unsigned i=0; i<_min.size(); i++)
		{
		    ptBounds = new RealInterval(_min[i], _max[i]);
		    ownedBounds.push_back(ptBounds);
		    push_back(ptBounds);
		}
	}

	/** Ctor from a std::string
	 * and don't worry, the readFrom(std::string) starts by setting everything to 0!
	 */
	RealVectorBounds(std::string _s) : RealBaseVectorBounds()
	{
	    readFrom(_s);
	}

	/** Dtor: destroy all ownedBounds - BUG ???*/
	virtual ~RealVectorBounds()
	{
	    //     std::cout << "Dtor, avec size = " << ownedBounds.size() << std::endl;
	    //     for (unsigned i = 0; i < ownedBounds.size(); ++i)
	    //     {
	    //         delete ownedBounds[i];
	    //     }
	}


	// methods from Persistent
	/**
	 * Read object from a stream
	 * only calls the readFrom(std::string) - for param reading
	 * @param _is A std::istream.
	 */
	virtual void readFrom(std::istream& _is) ;

	/**
	 * Read object from a std::string
	 * @param _s A std::istream.
	 */
	virtual void readFrom(std::string _s) ;

	/** overload printOn method to save space */
	virtual void printOn(std::ostream& _os) const
	{
	    if (factor[0]>1)
		_os << factor[0] ;
	    operator[](0)->printOn(_os);

	    // other bounds
	    unsigned int index=factor[0];
	    if (factor.size()>1)
		for (unsigned i=1; i<factor.size(); i++)
		    {
			_os << ";";
			if (factor[i] > 1)
			    _os << factor[i];
			operator[](index)->printOn(_os);
			index += factor[i];
		    }
	}

	/** Eventually increases the size by duplicating last bound */
	void adjust_size(unsigned _dim);

	/** need to rewrite copy ctor and assignement operator
	 *  because of ownedBounds */
	RealVectorBounds(const RealVectorBounds &);

    private:// WARNING: there is no reason for both std::vector below
        //to be synchronized in any manner
	std::vector<unsigned int> factor;	   // std::list of nb of "grouped" bounds
	std::vector<RealBounds *> ownedBounds;
	// keep this one private
	RealVectorBounds& operator=(const RealVectorBounds&);
    };

    //////////////////////////////////////////////////////////////
    /** the dummy unbounded RealVectorBounds: usefull if you don't need bounds!
     * everything is inlined.
     * Warning: we do need this class, and not only a std::vector<RealNoBounds *>

     @ingroup Bounds
    */
    class RealVectorNoBounds: public RealVectorBounds
    {
    public:
	// virtual desctructor (to avoid warning?)
	virtual ~RealVectorNoBounds(){}

	/**
	 * Ctor: nothing to do, but beware of dimension: call base class ctor
	 */
	RealVectorNoBounds(unsigned _dim) :
	    RealVectorBounds( (_dim?_dim:1), DummyRealNoBounds)
	{}

	virtual bool isBounded(unsigned)  {return false;}
	virtual bool isBounded(void)   {return false;}

	virtual bool hasNoBoundAtAll(unsigned)  {return true;}
	virtual bool hasNoBoundAtAll(void)  {return true;}

	virtual bool isMinBounded(unsigned)   {return false;}
	virtual bool isMaxBounded(unsigned)   {return false;}

	virtual void foldsInBounds(unsigned, double &) {return;}
	virtual void foldsInBounds(std::vector<double> &) {return;}

	virtual void truncate(unsigned, double &) {return;}
	virtual void truncate(std::vector<double> &) {return;}

	virtual bool isInBounds(unsigned, double) {return true;}
	virtual bool isInBounds(std::vector<double>) {return true;}

	// accessors
	virtual double minimum(unsigned)
	{
	    throw std::logic_error("Trying to get minimum of RealVectorNoBounds");
	}
	virtual double maximum(unsigned)
	{
	    throw std::logic_error("Trying to get maximum of RealVectorNoBounds");
	}
	virtual double range(unsigned)
	{
	    throw std::logic_error("Trying to get range of RealVectorNoBounds");
	}

	virtual double averageRange()
	{
	    throw std::logic_error("Trying to get average range of RealVectorNoBounds");
	}

	// random generators
	virtual double uniform(unsigned, Rng & _rng = rng)
	{
	    (void)_rng;

	    throw std::logic_error("No uniform distribution on RealVectorNoBounds");
	}

	// fills a std::vector with uniformly chosen variables in bounds
	void uniform(std::vector<double> &, Rng & _rng = rng)
	{
	    (void)_rng;

	    throw std::logic_error("No uniform distribution on RealVectorNoBounds");
	}

    };

    /** one object for all - see RealBounds.cpp
	@ingroup Bounds
    */
    extern RealVectorNoBounds DummyVectorNoBounds;

}

#endif
