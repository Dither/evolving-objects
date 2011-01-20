// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// Merge.h
//   Base class for elitist-merging classes
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

#ifndef Merge_h
#define Merge_h

//-----------------------------------------------------------------------------

#include <stdexcept>

// EO includes
#include <Pop.h>     // Pop
#include <Functor.h>  // Merge

namespace eo
{

    /**
     * Merge: Base class for elitist replacement algorithms. 
     * Merges the old population (first argument), with the new generation
     *
     * Its signature is exactly
     * that of the selection base Select, but its purpose is to merge the 
     * two populations into one (the second argument).
     * Note that the algorithms assume that the second argument denotes the 
     * next generation.
     *
     * @ingroup Core
     * @ingroup Replacors
     */

    template<class Chrom> class Merge: public BF<const Pop<Chrom>&, Pop<Chrom>&, void>
    {};

    /**
       Straightforward elitism class, specify the number of individuals to copy
       into new geneneration or the rate w.r.t. pop size
 
       @ingroup Replacors
    */
    template <class EOT> class Elitism : public Merge<EOT>
    {
    public :
	Elitism(double  _rate, bool _interpret_as_rate = true):
	    rate(0), combien(0)
	{
	    if (_interpret_as_rate)
		{
		    if ( (_rate<0) || (_rate>1) )
			throw std::logic_error("Elitism: rate shoud be in [0,1]");
		    rate = _rate;
		}
	    else
		{
		    if (_rate<0)
			throw std::logic_error("Negative number of offspring in Elitism!");
		    combien = (unsigned int)_rate;
		    if (combien != _rate)
			log << warnings << "Warning: Number of guys to merge in Elitism was rounded" << std::endl;
		}
	}
  
	void operator()(const Pop<EOT>& _pop, Pop<EOT>& _offspring)
	{
	    if ((combien == 0) && (rate == 0.0))
		return;
	    unsigned combienLocal;
	    if (combien == 0)	   // rate is specified
		combienLocal = (unsigned int) (rate * _pop.size());
	    else
		combienLocal = combien;
    
	    if (combienLocal > _pop.size())
		throw std::logic_error("Elite larger than population");
    
	    std::vector<const EOT*> result;
	    _pop.nth_element(combienLocal, result);
    
	    for (size_t i = 0; i < result.size(); ++i)
		{
		    _offspring.push_back(*result[i]);
		}
	}
  
    private :
	double rate;
	unsigned combien;
    };

    /**
       No elite
       @ingroup Replacors
    */
    template <class EOT> class NoElitism : public Elitism<EOT>
    {
    public :
        NoElitism() : Elitism<EOT>(0) {}
    };

    /**
       Very elitist class, copies entire population into next gen
       @ingroup Replacors
    */
    template <class EOT> class Plus : public Merge<EOT>
    {
    public :
	void operator()(const Pop<EOT>& _pop, Pop<EOT>& _offspring)
	{
	    _offspring.reserve(_offspring.size() + _pop.size());

	    for (size_t i = 0; i < _pop.size(); ++i)
		{
		    _offspring.push_back(_pop[i]);
		}
	}

    private :
    };

}

//-----------------------------------------------------------------------------

#endif 
