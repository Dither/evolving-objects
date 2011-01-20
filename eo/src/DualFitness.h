/*

(c) 2010 Thales group

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; version 2 
    of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

Contact: http://eodev.sourceforge.net

Authors:
Johann Dréo <johann.dreo@thalesgroup.com>

*/

#ifndef _DualFitness_h_
#define _DualFitness_h_

#include <functional>
#include <iostream>
#include <utility> // for std::pair
#include <string>

#include <utils/Stat.h>
#include <utils/Logger.h>

namespace eo
{

    /** @addtogroup Evaluation
     * @{
     */

    //! A fitness class that permits to compare feasible and unfeasible individuals and guaranties that a feasible individual will always be better than an unfeasible one.
    /**
     * Use this class as fitness if you have some kind of individuals 
     * that must be always considered as better than others while having the same fitness type.
     *
     * Wraps a scalar fitness _values such as a double or int, with the option of
     * maximizing (using less<BaseType>, @see MaximizingDualFitness)
     * or minimizing (using greater<BaseType>, @see MinimizingDualFitness).
     *
     * Suitable constructors, assignments and casts are defined to work
     * with those quantities as if they were a pair of: a BaseType and a boolean.
     *
     * When changing the fitness, you can use:
     *     individual.fitness( std::make_pair<BaseType,bool>( fitness, feasibility ) );
     * 
     * Be aware that, when printing or reading an eDualFitness instance on a iostream,
     * friend IO classes use a space separator.
     *
     * This class overrides operator<() to use the Compare template argument and handle feasibility.
     * Over operators are coded using this sole function.
     *
     * Standard arithmetic operators are provided to add or substract dual fitnesses.
     * They behave as expected for the fitness value and gives priority to unfeasible fitness
     * (i.e. when adding or substracting dual fitness, the only case when the result will be
     *  a feasible fitness is when both are feasible, else the result is an unfeasibe fitness)
     */
    template <class BaseType, class Compare >
    class DualFitness
    {
    protected:
	//! Scalar type of the fitness (generally a double)
	BaseType _value;

	//! Flag that marks if the individual is feasible
	bool _is_feasible;

    public:

	//! Empty initialization
	/*!
	 * Unfeasible by default
	 */
	DualFitness() : 
	    _value(), 
	    _is_feasible(false) 
	{}

	//! Copy constructor
	DualFitness(const DualFitness& other) :
	    _value(other._value),
	    _is_feasible(other._is_feasible) 
	{}

	//! Constructor from explicit value/feasibility
	DualFitness(const BaseType& v, const bool& is_feasible) : 
	    _value(v),
	    _is_feasible(is_feasible)
	{}

	//! From a std::pair (first element is the value, second is the feasibility)
	DualFitness(const std::pair<BaseType,bool>& dual) :
	    _value(dual.first),
	    _is_feasible(dual.second)
	{}

	inline bool is_feasible() const
	{
	    return _is_feasible;
	}

	inline BaseType value() const
	{
	    return _value;
	}

	//! Copy operator from a std::pair
	DualFitness& operator=(const std::pair<BaseType,bool>& v)
	{
	    _value = v.first;
	    _is_feasible = v.second;
	    return *this; 
	}
 
	//! Copy operator from another DualFitness
	template <class F, class Cmp>
	DualFitness<F,Cmp> & operator=(const DualFitness<BaseType, Compare>& other )
	{
	    if (this != &other) {
		this->_value = other._value; 
		this->_is_feasible = other._is_feasible;
	    }
	    return *this;
	}


	//! Comparison that separate feasible individuals from unfeasible ones. Feasible are always better
	/*!
	 * Use less as a default comparison operator 
	 * (see the "Compare" template of the class to change this behaviour,
	 * @see MinimizingDualFitness for an example).
	 */
	bool operator<(const DualFitness& other) const
	{ 
	    // am I better (less, by default) than the other ?

	    // if I'm feasible and the other is not
	    if( this->_is_feasible && !other._is_feasible ) {
		// no, the other has a better fitness
		return false;

	    } else if( !this->_is_feasible && other._is_feasible ) {
		// yes, a feasible fitness is always better than an unfeasible one
		return true;

	    } else { 
		// the two fitness are of the same type
		// lets rely on the comparator
		return Compare()(_value, other._value);
	    }
	}

	//! Greater: if the other is lesser than me
	bool operator>( const DualFitness<BaseType, Compare>& other ) const  { return other < *this; }

	//! Less or equal: if the other is not lesser than me
	bool operator<=( const DualFitness<BaseType, Compare>& other ) const { return !(other < *this); }

	//! Greater or equal: if the other is not greater than me
	bool operator>=(const DualFitness<BaseType, Compare>& other ) const { return !(*this < other); }

    public:

	//! Add a given fitness to the current one
	template <class F, class Cmp>
	friend
	DualFitness<F,Cmp> & operator+=( DualFitness<F,Cmp> & from, const DualFitness<F,Cmp> & that )
	{
	    from._value += that._value;

	    // true only if the two are feasible, else false
	    from._is_feasible = from._is_feasible && that._is_feasible;

	    return from;
	}

	//! Substract a given fitness to the current one
	template <class F, class Cmp>
	friend
	DualFitness<F,Cmp> & operator-=( DualFitness<F,Cmp> & from, const DualFitness<F,Cmp> & that )
	{
	    from._value -= that._value;

	    // true only if the two are feasible, else false
	    from._is_feasible = from._is_feasible && that._is_feasible;

	    return from;
	}

	// Add this fitness's value to that other, and return a _new_ instance with the result.
	template <class F, class Cmp>
	DualFitness<F,Cmp> operator+(const DualFitness<F,Cmp> & that) 
	{
	    DualFitness<F,Cmp> from( *this );
	    return from += that;
	}

	// Add this fitness's value to that other, and return a _new_ instance with the result.
	template <class F, class Cmp>
	DualFitness<F,Cmp> operator-(const DualFitness<F,Cmp> & that) 
	{
	    DualFitness<F,Cmp> from( *this );
	    return from -= that;
	}

	//! Print an DualFitness instance as a pair of numbers, separated by a space
	template <class F, class Cmp>
	friend
	std::ostream& operator<<(std::ostream& os, const DualFitness<F, Cmp>& f)
	{
	    os << f._value << " " << f._is_feasible;
	    return os;
	}

	//! Read an DualFitness instance as a pair of numbers, separated by a space
	template <class F, class Cmp>
	friend 
	std::istream& operator>>(std::istream& is, DualFitness<F, Cmp>& f)
	{
	    F value;
	    is >> value;

	    bool feasible;
	    is >> feasible;

	    f = std::make_pair<F,bool>( value, feasible );
	    return is;
	}
    };

    //! Compare dual fitnesses as if we were maximizing
    typedef DualFitness<double, std::less<double> >    MaximizingDualFitness;

    //! Compare dual fitnesses as if we were minimizing
    typedef DualFitness<double, std::greater<double> > MinimizingDualFitness;

    //! A predicate that returns the feasibility of a given dual fitness
    /** Use this in STL algorithm that use binary predicates (e.g. count_if, find_if, etc.)
     */
    template< class EOT>
    bool IsFeasible ( const EOT & sol ) { return sol.fitness().is_feasible(); }


    /** Embed two Stat and call the first one on the feasible individuals and
     * the second one on the unfeasible ones, merge the two resulting value in
     * a string, separated by a given marker.
     */
    //template<class EOT, class T>
    template<class EOT, class EOSTAT>
    class DualStatSwitch : public Stat< EOT, std::string >
    {
    public:
	using Stat<EOT,std::string>::value;

	//    DualStatSwitch( Stat<EOT,T> & stat_feasible,  Stat<EOT,T> & stat_unfeasible, std::string sep=" "  ) :
	DualStatSwitch( EOSTAT & stat_feasible,  EOSTAT & stat_unfeasible, std::string sep=" "  ) :
	    _stat_feasible(stat_feasible), 
	    _stat_unfeasible(stat_unfeasible), 
	    _sep(sep),
	    Stat<EOT,std::string>(
				    "?"+sep+"?", 
				    stat_feasible.longName()+sep+stat_unfeasible.longName() 
				    )
	{ }

	virtual void operator()( const Pop<EOT> & pop )
	{
	    Pop<EOT> pop_feasible;
	    pop_feasible.reserve(pop.size());

	    Pop<EOT> pop_unfeasible;
	    pop_unfeasible.reserve(pop.size());

	    for( typename Pop<EOT>::const_iterator ieot=pop.begin(), iend=pop.end(); ieot!=iend; ++ieot ) {
		/*
		  if( ieot->invalid() ) {
		  eo::log << eo::errors << "ERROR: trying to access to an invalid fitness" << std::endl;
		  }
		*/
		if( ieot->fitness().is_feasible() ) {
		    pop_feasible.push_back( *ieot );
		} else {
		    pop_unfeasible.push_back( *ieot );
		}
	    }

	    _stat_feasible( pop_feasible );
	    _stat_unfeasible( pop_unfeasible );

	    std::ostringstream out;
	    out << _stat_feasible.value() << _sep << _stat_unfeasible.value();

	    value() = out.str();
	}

    protected:
	//    Stat<EOT,T> & _stat_feasible;
	//    Stat<EOT,T> & _stat_unfeasible;
	EOSTAT & _stat_feasible;
	EOSTAT & _stat_unfeasible;

	std::string _sep;
    };

}

/** @} */
#endif // _DualFitness_h_

