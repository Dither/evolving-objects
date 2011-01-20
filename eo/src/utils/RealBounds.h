// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// RealBounds.h
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

#ifndef _RealBounds_h
#define _RealBounds_h

#include <stdexcept>		   // std::exceptions!
#include <utils/RNG.h>

namespace eo
{

    /**
       \defgroup Real Vector of reals

       Set of classes related to continuous black-box optimization problems.

       Here are several examples of test programs using Real, EsSimple, EsStdev or EsFull to build an Evoution Strategies algorithm:
       @include t-ESAll.cpp
       @include t-ESFull.cpp

       @ingroup Representations

    */


    /**
       \class RealBounds RealBounds.h es/RealBounds.h
       \ingroup Real

       Defines bound classes for real numbers.

       Scalar type:
       ------------
       Basic class is RealBounds, a pure virtual.

       The following pure virtual methods are to be used in mutations:
       - void foldsInBounds(double &) that folds any value that falls out of 
       the bounds back into the bounds, by bouncing on the limit (if any)
       - bool isInBounds(double) that simply says whether or not the argument 
       is in the bounds
       - void truncate(double &) that set the argument to the bound value it
       it exceeds it

       So mutation can choose 
       - iterate trying until they fall in bounds, 
       - only try once and "restd::pair" by using the foldsInBounds method
       - only try once and restd::pair using the truncate method (will create a
       huge bias toward the bound if the soluiton is not far from the bounds)

       There is also a uniform() method that generates a uniform value 
       (if possible, i.e. if bounded) in the interval.

       Derived class are 
       RealInterval that holds a minimum and maximum value, 
       RealNoBounds the "unbounded bounds" (-infinity, +infinity)
       RealBelowBound the half-bounded interval [min, +infinity)
       RealAboveBound the   half-bounded interval (-infinity, max]

       THis file also contains the declaration of *the* global object that
       is the unbounded bound

       @ingroup Bounds
    */
    class RealBounds : public Persistent
    { 
    public:
	virtual ~RealBounds(){}

	/** Self-Test: true if ***both*** a min and a max
	 */
	virtual bool isBounded(void) const  = 0;

	/** Self-Test: true if no min ***and*** no max
	 *        hence no further need to test/truncate/fold anything
	 */
	virtual bool hasNoBoundAtAll(void) const  = 0;

	/** Self-Test: bounded from below???
	 */
	virtual bool isMinBounded(void) const = 0;

	/** Self-Test: bounded from above???
	 */
	virtual bool isMaxBounded(void) const = 0;

	/** Test on a value: is it in bounds?
	 */
	virtual bool isInBounds(double)  const = 0;

	/** Put value back into bounds - by folding back and forth
	 */
	virtual void foldsInBounds(double &)  const = 0;

	/** Put value back into bounds - by truncating to a boundary value
	 */
	virtual void truncate(double &)  const = 0;

	/** get minimum value 
	 *  std::exception if does not exist
	 */  
	virtual double minimum()  const = 0 ;
	/** get maximum value
	 *  std::exception if does not exist
	 */  
	virtual double maximum()  const = 0 ;
	/** get range
	 *  std::exception if unbounded
	 */  
	virtual double range()  const = 0;

	/** random generator of uniform numbers in bounds
	 * std::exception if unbounded
	 */
	virtual double uniform(Rng & _rng = rng)  const = 0;

	/** for memory managements - ugly */
	virtual RealBounds * dup()  const = 0;
    };

    /** A default class for unbounded variables

	@ingroup Bounds
    */
    class RealNoBounds : public RealBounds
    {
    public:
	virtual ~RealNoBounds(){}

	virtual bool isBounded(void)  const {return false;}
	virtual bool hasNoBoundAtAll(void) const  {return true;}
	virtual bool isMinBounded(void)  const {return false;}
	virtual bool isMaxBounded(void)  const {return false;}
	virtual void foldsInBounds(double &)  const {return;}
	virtual void truncate(double &)  const {return;}
	virtual bool isInBounds(double)  const {return true;}

	virtual double minimum() const
	{
	    throw std::logic_error("Trying to get minimum of unbounded RealBounds");
	}
	virtual double maximum() const 
	{
	    throw std::logic_error("Trying to get maximum of unbounded RealBounds");
	}
	virtual double range() const 
	{
	    throw std::logic_error("Trying to get range of unbounded RealBounds");
	}

	virtual double uniform(Rng & _rng = rng) const
	{
	    (void)_rng;

	    throw std::logic_error("Trying to generate uniform values in unbounded RealBounds");
	}

	// methods from Persistent
	/**
	 * Read object.
	 * @param _is A std::istream.
	 * but reading should not be done here, because of bound problems
	 * see RealVectorBounds
	 */
	virtual void readFrom(std::istream& _is)
	{
	    (void)_is;

	    throw std::runtime_error("Should not use RealBounds::readFrom");
	}

	/**
	 * Write object. It's called printOn since it prints the object on a stream.
	 * @param _os A std::ostream.
	 */
	virtual void printOn(std::ostream& _os) const
	{
	    _os << "[-inf,+inf]";
	}

	/** for memory managements - ugly */
	virtual RealBounds * dup() const 
	{
	    return new RealNoBounds(*this);
	}

    };

    /** one object for all - see RealBounds.cpp
	@ingroup Bounds
    */
    extern RealNoBounds DummyRealNoBounds;

    /**
     * fully bounded RealBound == interval

     @ingroup Bounds
    */
    class RealInterval : public RealBounds
    {
    public :
	virtual ~RealInterval(){}
  
	/** 
	    Simple bounds = minimum and maximum (allowed)
	*/
	RealInterval(double _min=0, double _max=1) : 
	    repMinimum(_min), repMaximum(_max), repRange(_max-_min) 
	{
	    if (repRange<=0)
		throw std::logic_error("Void range in RealBounds");
	}

	// accessors  
	virtual double minimum() const { return repMinimum; }
	virtual double maximum() const { return repMaximum; }
	virtual double range()  const { return repRange; }

	// description
	virtual bool isBounded(void)  const {return true;}
	virtual bool hasNoBoundAtAll(void)  const {return false;}
	virtual bool isMinBounded(void)  const {return true;}
	virtual bool isMaxBounded(void)  const {return true;}

	virtual double uniform(Rng & _rng = rng) const 
	{
	    return repMinimum + _rng.uniform(repRange);
	}  

	// says if a given double is within the bounds
	virtual bool isInBounds(double _r) const 
	{
	    if (_r < repMinimum)
		return false;
	    if (_r > repMaximum)
		return false;
	    return true;
	}

	// folds a value into bounds
	virtual void foldsInBounds(double &  _r) const 
	{
	    long iloc;
	    double dlargloc = 2 * range() ;

	    if (fabs(_r) > 1.0E9)		// iloc too large!
		{
		    _r = uniform();
		    return;
		}

	    if ( (_r > maximum()) )
		{
		    iloc = (long) ( (_r-minimum()) / dlargloc ) ;
		    _r -= dlargloc * iloc ;
		    if ( _r > maximum() )
			_r = 2*maximum() - _r ;
		}
    
	    if (_r < minimum()) 
		{
		    iloc = (long) ( (maximum()-_r) / dlargloc ) ;
		    _r += dlargloc * iloc ;
		    if (_r < minimum())
			_r = 2*minimum() - _r ;
		}
	}    

	// truncates to the bounds
	virtual void truncate(double & _r) const 
	{
	    if (_r < repMinimum)
		_r = repMinimum;
	    else if (_r > repMaximum)
		_r = repMaximum;
	    return;
	}

	// methods from Persistent
	/**
	 * Read object.
	 * @param _is A std::istream.
	 * but reading should not be done here, because of bound problems
	 * see RealVectorBounds
	 */
	virtual void readFrom(std::istream& _is)
	{
	    (void)_is;

	    throw std::runtime_error("Should not use RealInterval::readFrom");
	}

	/**
	 * Write object. It's called printOn since it prints the object on a stream.
	 * @param _os A std::ostream.
	 */
	virtual void printOn(std::ostream& _os) const
	{
	    _os << "[" << repMinimum << "," << repMaximum << "]";
	}

	/** for memory managements - ugly */
	virtual RealBounds * dup() const 
	{
	    return new RealInterval(*this);
	}

    private :
	double repMinimum;
	double repMaximum;
	double repRange;			   // to minimize operations ???
    };

    /**
     * an RealBound bounded from below only

     @ingroup Bounds
    */
    class RealBelowBound : public RealBounds
    {
    public :
	virtual ~RealBelowBound(){}  
	/** 
	    Simple bounds = minimum
	*/
	RealBelowBound(double _min=0) : 
	    repMinimum(_min)
	{}

	// accessors  
	virtual double minimum() const { return repMinimum; }

	virtual double maximum() const 
	{
	    throw std::logic_error("Trying to get maximum of RealBelowBound");
	}
	virtual double range() const 
	{
	    throw std::logic_error("Trying to get range of RealBelowBound");
	}

	// random generators
	virtual double uniform(Rng & _rng = rng) const
	{
	    (void)_rng;

	    throw std::logic_error("Trying to generate uniform values in RealBelowBound");
	}

	// description
	virtual bool isBounded(void)  const {return false;}
	virtual bool hasNoBoundAtAll(void)  const {return false;}
	virtual bool isMinBounded(void)  const {return true;}
	virtual bool isMaxBounded(void) const  {return false;}

	// says if a given double is within the bounds
	virtual bool isInBounds(double _r) const 
	{
	    if (_r < repMinimum)
		return false;
	    return true;
	}

	// folds a value into bounds
	virtual void foldsInBounds(double &  _r) const 
	{
	    // easy as a pie: symmetry w.r.t. minimum
	    if (_r < repMinimum)	   // nothing to do otherwise
		_r = 2*repMinimum - _r;
	    return ;
	}    

	// truncates to the bounds
	virtual void truncate(double & _r) const 
	{
	    if (_r < repMinimum)
		_r = repMinimum;
	    return;
	}

	// methods from Persistent
	/**
	 * Read object.
	 * @param _is A std::istream.
	 * but reading should not be done here, because of bound problems
	 * see RealVectorBounds
	 */
	virtual void readFrom(std::istream& _is) 
	{
	    (void)_is;

	    throw std::runtime_error("Should not use RealBelowBound::readFrom");
	}

	/**
	 * Write object. It's called printOn since it prints the object on a stream.
	 * @param _os A std::ostream.
	 */
	virtual void printOn(std::ostream& _os) const
	{
	    _os << "[" << repMinimum << ",+inf]";
	}

	/** for memory managements - ugly */
	virtual RealBounds * dup() const 
	{
	    return new RealBelowBound(*this);
	}

    private :
	double repMinimum;
    };

    /**
       An RealBound bounded from above only
    */
    class RealAboveBound : public RealBounds
    {
    public :
	virtual ~RealAboveBound(){}
  
	/** 
	    Simple bounds = minimum
	*/
	RealAboveBound(double _max=0) : 
	    repMaximum(_max)
	{}

	// accessors  
	virtual double maximum() const  { return repMaximum; }

	virtual double minimum() const 
	{
	    throw std::logic_error("Trying to get minimum of RealAboveBound");
	}
	virtual double range() const 
	{
	    throw std::logic_error("Trying to get range of RealAboveBound");
	}

	// random generators
	virtual double uniform(Rng & _rng = rng) const
	{
	    (void)_rng;

	    throw std::logic_error("Trying to generate uniform values in RealAboveBound");
	}

	// description
	virtual bool isBounded(void)  const {return false;}
	virtual bool hasNoBoundAtAll(void)  const {return false;}
	virtual bool isMinBounded(void)  const {return false;}
	virtual bool isMaxBounded(void)  const {return true;}

	// says if a given double is within the bounds
	virtual bool isInBounds(double _r) const 
	{
	    if (_r > repMaximum)
		return false;
	    return true;
	}

	// folds a value into bounds
	virtual void foldsInBounds(double &  _r) const 
	{
	    // easy as a pie: symmetry w.r.t. maximum
	    if (_r > repMaximum)	   // nothing to do otherwise
		_r = 2*repMaximum - _r;
	    return ;
	}    

	// truncates to the bounds
	virtual void truncate(double & _r) const 
	{
	    if (_r > repMaximum)
		_r = repMaximum;
	    return;
	}

	// methods from Persistent
	/**
	 * Read object.
	 * @param _is A std::istream.
	 * but reading should not be done here, because of bound problems
	 * see RealVectorBounds
	 */
	virtual void readFrom(std::istream& _is)
	{
	    (void)_is;

	    throw std::runtime_error("Should not use RealAboveBound::readFrom");
	}

	/**
	 * Write object. It's called printOn since it prints the object on a stream.
	 * @param _os A std::ostream.
	 */
	virtual void printOn(std::ostream& _os) const
	{
	    _os << "[-inf," << repMaximum << "]";
	}

	/** for memory managements - ugly */
	virtual RealBounds * dup() const 
	{
	    return new RealAboveBound(*this);
	}

    private :
	double repMaximum;
    };

    //////////////////////// tentative for a general BOUND class that is constructed from a string

    /** A class that encapsulate all possible IntBounds.
     *  Mandatory in order to read through the parser

     @ingroup Bounds
    */
    class GeneralRealBounds : public RealBounds
    {
    public:
	/** Ctor: from a string, chooses the type of bound */
	GeneralRealBounds(std::string _s = "[-infinity,+infinity]")
	{
	    repBound = getBoundsFromString(_s);
	}

	/** Need a Cpy Ctor because we are allocating memory */
	GeneralRealBounds(const GeneralRealBounds & _b):RealBounds(_b)
	{
	    // replicate the embedded bound (I'm pretty sure there is another
	    // way to do that !!!

	    bool minBounded = _b.isMinBounded();
	    bool maxBounded = _b.isMaxBounded();
	    double minimum, maximum;
	    const RealBounds & bb = _b.theBounds();
	    if (minBounded) minimum = bb.minimum();
	    if (maxBounded) maximum = bb.maximum();

	    if (minBounded && maxBounded)
		repBound = new RealInterval(minimum, maximum);
	    else if (!minBounded && !maxBounded)	// no bound at all
		repBound = new RealNoBounds;
	    else if (!minBounded && maxBounded)
		repBound = new RealAboveBound(maximum);
	    else if (minBounded && !maxBounded)
		repBound = new RealBelowBound(minimum);
	}

	GeneralRealBounds& operator=(const GeneralRealBounds& _b)
	{
	    // replicate the embedded bound (I'm pretty sure there is another
	    // way to do that !!!

	    bool minBounded = _b.isMinBounded();
	    bool maxBounded = _b.isMaxBounded();
	    double minimum, maximum;
	    const RealBounds & bb = _b.theBounds();
	    if (minBounded) minimum = bb.minimum();
	    if (maxBounded) maximum = bb.maximum();

	    // first delete the embedded bounds if necessary
	    if (repBound)
		delete repBound;
	    // now reallocate
	    if (minBounded && maxBounded)
		repBound = new RealInterval(minimum, maximum);
	    else if (!minBounded && !maxBounded)	// no bound at all
		repBound = new RealNoBounds;
	    else if (!minBounded && maxBounded)
		repBound = new RealAboveBound(maximum);
	    else if (minBounded && !maxBounded)
		repBound = new RealBelowBound(minimum);
	    return (*this);
	}


	/** Need a Dtor because we allocate an actual bound  */
	~GeneralRealBounds()
	{
	    delete repBound;
	}

	///// and now all methods from the embedded bounds
	/** Self-Test: true if ***both*** a min and a max
	 */
	virtual bool isBounded(void)  const {return repBound->isBounded();}

	/** Self-Test: true if no min ***and*** no max
	 *        hence no further need to test/truncate/fold anything
	 */
	virtual bool hasNoBoundAtAll(void)  const {return repBound->hasNoBoundAtAll();}

	/** Self-Test: bounded from below???
	 */
	virtual bool isMinBounded(void)  const {return repBound->isMinBounded();}

	/** Self-Test: bounded from above???
	 */
	virtual bool isMaxBounded(void) const {return repBound->isMaxBounded();}

	/** Test on a value: is it in bounds?
	 */
	virtual bool isInBounds(double _x)  const {return repBound->isInBounds(_x);}

	/** Put value back into bounds - by folding back and forth
	 */
	virtual void foldsInBounds(double & _x) const {return repBound->foldsInBounds(_x);}

	/** Put value back into bounds - by truncating to a boundary value
	 */
	virtual void truncate(double & _x)  const {return repBound->truncate(_x);}

	/** get minimum value 
	 *  std::exception if does not exist
	 */  
	virtual double minimum()  const {return repBound->minimum();}
	/** get maximum value
	 *  std::exception if does not exist
	 */  
	virtual double maximum() const {return repBound->maximum();}
	/** get range
	 *  std::exception if unbounded
	 */  
	virtual double range()  const {return repBound->range();}

	/** random generator of uniform numbers in bounds
	 * std::exception if unbounded
	 */
	virtual double uniform(Rng & _rng = rng)  const {(void)_rng; return repBound->uniform();}

	/** for memory managements - ugly */
	virtual RealBounds * dup() const  {return repBound->dup();}

	/** for efficiency, it's better to use the embedded boud directly */
	const RealBounds & theBounds()  const { return *repBound;} 

	/** don't forget the printOn method - 
	 * again that of the embedded bound
	 */
	virtual void printOn(std::ostream& _os) const
	{
	    repBound->printOn(_os);
	}

	/** no readFrom ??? Have to check that later */
	virtual void readFrom(std::istream& _is) 
	{
	    std::string s;
	    _is >> s;
	    if (repBound)
		delete repBound;
	    repBound = getBoundsFromString(s);
	}

    private:
	// reading from a string
	RealBounds * getBoundsFromString(std::string);

	RealBounds * repBound;
    };

}

#endif
