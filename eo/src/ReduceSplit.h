// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// ReduceSplit.h
//   Base class for population-reducing classes - retaining the poor losers
// (c) GeNeura Team, 1998, Marc Schoenauer, 2002
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

   Contact: Marc.Schoenauer@inria.fr
 */
//-----------------------------------------------------------------------------

#ifndef ReduceSplit_h
#define ReduceSplit_h

//-----------------------------------------------------------------------------

#include <iostream>

// EO includes
#include <Pop.h>     // Pop
#include <Functor.h>  // Reduce
#include <utils/selectors.h>

namespace eo
{

    /** @addtogroup Replacors
     * @{
     */

    /**
     * ReduceSplit: reduce the pop to the specified size
     *                  AND eventually returns the eliminated guys
     */
    template<class EOT> class ReduceSplit: public BF<Pop<EOT>&, Pop<EOT> &, void >
    {};

    /** deterministic truncation method using sort */
    template <class EOT> 
    class TruncateSplit : public ReduceSplit<EOT>
    {
    public:
	/** Ctor: must provide amount of reduction, 
	    and whether or not you need to return the eliminated guys
	*/
	TruncateSplit(HowMany _howMany, bool _returnEliminated = false):
	    howMany(_howMany), returnEliminated(_returnEliminated) {}
    
	/** do the jonb */
	void operator()(Pop<EOT>& _newgen, Pop<EOT> & _eliminated)
	{
	    unsigned popSize = _newgen.size();
	    unsigned eliminated = howMany(popSize);
	    if (!eliminated)   // nothing to do
		return ;
	    unsigned newsize = popSize - eliminated;
	    if (newsize < 0)
		throw std::logic_error("TruncateSplit: Cannot truncate to a larger size!\n");
    
	    _newgen.nth_element(newsize);

	    // save poor losers if necessary
	    if (returnEliminated)
		for (unsigned i=0; i<eliminated; i++)
		    _eliminated.push_back(_newgen[newsize+i]);
	    // truncate
	    _newgen.resize(newsize);
	    return ;
	}

    private:
	HowMany howMany;
	bool returnEliminated;
    };

    /** a ReduceSplit class that does not sort, but repeatidely kills the worse.
	To be used in SSGA-like replacements (e.g. see SSGAWorseReplacement)
    */
    template <class EOT> 
    class LinearTruncateSplit : public ReduceSplit<EOT>
    {
    public:
	/** Ctor: must provide amount of reduction, 
	    and whether or not you need to return the eliminated guys
	*/
	LinearTruncateSplit(HowMany _howMany, bool _returnEliminated = false):
	    howMany(_howMany), returnEliminated(_returnEliminated) {}
    
	/** do the job */
	void operator()(Pop<EOT>& _newgen, Pop<EOT> & _eliminated)
	{
	    unsigned popSize = _newgen.size();
	    unsigned eliminated = howMany(popSize);
	    if (!eliminated)   // nothing to do
		return ;
	    unsigned newsize = popSize - eliminated;
	    if (newsize < 0)
		throw std::logic_error("LinearTruncateSplit: Cannot truncate to a larger size!\n");

	    _eliminated.reserve(_eliminated.size()+eliminated); //in case not empty?
	    for (unsigned i=0; i<eliminated; i++)
		{
		    typename Pop<EOT>::iterator it = _newgen.it_worse_element();
		    if (returnEliminated)
			_eliminated.push_back(*it);
		    _newgen.erase(it);	    
		}
	}

    private:
	HowMany howMany;
	bool returnEliminated;
    };

    /** random truncation - batch version */
    template <class EOT> 
    class RandomSplit : public ReduceSplit<EOT>
    {
    public:
	/** Ctor: must provide amount of reduction, 
	    and whether or not you need to return the eliminated guys
	*/
	RandomSplit(HowMany _howMany, bool _returnEliminated = false):
	    howMany(_howMany), returnEliminated(_returnEliminated) {}
    
	/** do the job */
	void operator()(Pop<EOT>& _newgen, Pop<EOT> & _eliminated)
	{
	    unsigned popSize = _newgen.size();
	    unsigned eliminated = howMany(popSize);
	    if (!eliminated)   // nothing to do
		return ;
	    unsigned newsize = popSize - eliminated;
	    if (newsize < 0)
		throw std::logic_error("RandomSplit: Cannot truncate to a larger size!\n");

	    _newgen.shuffle();

	    // save poor losers if necessary
	    if (returnEliminated)
		for (unsigned i=0; i<eliminated; i++)
		    _eliminated.push_back(_newgen[newsize+i]);
	    // truncate
	    _newgen.resize(newsize);
	    return ;
	}

    private:
	HowMany howMany;
	bool returnEliminated;
    };


    /** random truncation - linear version */
    template <class EOT> 
    class LinearRandomSplit : public ReduceSplit<EOT>
    {
    public:
	/** Ctor: must provide amount of reduction, 
	    and whether or not you need to return the eliminated guys
	*/
	LinearRandomSplit(HowMany _howMany, bool _returnEliminated = false):
	    howMany(_howMany), returnEliminated(_returnEliminated) {}
    
	/** do the job */
	void operator()(Pop<EOT>& _newgen, Pop<EOT> & _eliminated)
	{
	    unsigned popSize = _newgen.size();
	    unsigned eliminated = howMany(popSize);
	    if (!eliminated)   // nothing to do
		return ;
	    unsigned newsize = popSize - eliminated;
	    if (newsize < 0)
		throw std::logic_error("LinearRandomSplit: Cannot truncate to a larger size!\n");

	    _eliminated.reserve(_eliminated.size()+eliminated); //in case not empty?
	    for (unsigned i=0; i<eliminated; i++)
		{
		    unsigned loser=random(_newgen.size());
		    typename Pop<EOT>::iterator it = _newgen.begin()+loser;
		    if (returnEliminated)
			_eliminated.push_back(*it);
		    _newgen.erase(it);	    
		}
	    return ;
	}

    private:
	HowMany howMany;
	bool returnEliminated;
    };


    /** a ReduceSplit class based on a repeated deterministic (reverse!) tournament
	To be used in SSGA-like replacements (e.g. see SSGADetTournamentReplacement)
    */
    template <class EOT> 
    class DetTournamentTruncateSplit : public ReduceSplit<EOT>
    {
    public:
	/** Ctor: must provide amount of reduction, 
	    and whether or not you need to return the eliminated guys
	*/
	DetTournamentTruncateSplit(unsigned _t_size, HowMany _howMany, 
				     bool _returnEliminated = false):
	    t_size(_t_size), howMany(_howMany), 
	    returnEliminated(_returnEliminated) 
	{
	    if (t_size < 2)
		{ 
		    eo::log << eo::warnings << "Warning, Size for DetTournamentTruncateSplit adjusted to 2" << std::endl;
		    t_size = 2;
		}
	}

	/** Performs repeated inverse_deterministic_tournament on the pop */
	void operator()(Pop<EOT>& _newgen, Pop<EOT> & _eliminated)
	// BUG???  void operator()(Pop<EOT>& _newgen, unsigned _newsize)
	{
	    unsigned popSize = _newgen.size();
	    unsigned eliminated = howMany(popSize);
	    if (!eliminated)   // nothing to do
		return ;
	    unsigned newsize = popSize - eliminated;
	    if (newsize < 0)
		throw std::logic_error("DetTournamentTruncateSplit: Cannot truncate to a larger size!\n");


	    _eliminated.reserve(_eliminated.size()+eliminated); //in case not empty?
	    for (unsigned i=0; i<eliminated; i++)
		{
		    typename Pop<EOT>::iterator it = inverse_deterministic_tournament(_newgen.begin(), _newgen.end(), t_size);
		    if (returnEliminated)
			_eliminated.push_back(*it);
		    _newgen.erase(it);
		}
	}

    private:
	unsigned t_size;
	HowMany howMany;
	bool returnEliminated;
    };

    /** a ReduceSplit class based on a repeated deterministic (reverse!) tournament
	To be used in SSGA-like replacements (e.g. see SSGAStochTournamentReplacement)
    */
    template <class EOT> 
    class StochTournamentTruncateSplit : public Reduce<EOT>
    {
    public:
	/** Ctor: must provide amount of reduction, 
	    and whether or not you need to return the eliminated guys
	*/
	StochTournamentTruncateSplit(double _t_rate, HowMany _howMany, 
				       bool _returnEliminated = false):
	    t_rate(_t_rate), howMany(_howMany), 
	    returnEliminated(_returnEliminated) 
	{
	    if (t_rate <= 0.5)
		{ 
		    eo::log << eo:warnings << "Warning, Rate for StochTournamentTruncateSplit adjusted to 0.51" << std::endl;
		    t_rate = 0.51;
		}
	    if (t_rate > 1)
		{
		    eo::log << eo::warnings << "Warning, Rate for StochTournamentTruncateSplit adjusted to 1" << std::endl;
		    t_rate = 1;
		}
	}

	/** Performs repeated inverse_stochastic_tournament on the pop */
	void operator()(Pop<EOT>& _newgen, Pop<EOT> & _eliminated)
	//BUG???  void operator()(Pop<EOT>& _newgen, unsigned _newsize)
	{
	    /* old version
	       if (!_eliminated.size())	   // nothing to do
	       return;
	       unsigned oldSize = _newgen.size();
	       unsigned newSize = oldSize - _eliminated.size();
	       unsigned eliminated = howMany(popSize);
	       if (newSize < 0)
	       throw std::logic_error("StochTournamentTruncateSplit: Cannot truncate to a larger size!\n");

	       end of old version    */

	    unsigned popSize = _newgen.size();
	    unsigned eliminated = howMany(popSize);
	    if (!eliminated)   // nothing to do
		return ;
	    unsigned newsize = popSize - eliminated;
	    if (newsize < 0)
		throw std::logic_error("DetTournamentTruncateSplit: Cannot truncate to a larger size!\n");



	    _eliminated.reserve(_eliminated.size()+eliminated); //in case not empty?
	    for (unsigned i=0; i<_eliminated.size(); i++)
		{
		    typename Pop<EOT>::iterator it = inverse_stochastic_tournament(_newgen.begin(), _newgen.end(), t_rate);
		    if (returnEliminated)
			_eliminated.push_back(*it);
		    _newgen.erase(it);
		}
	}


    private:
	double t_rate;
	HowMany howMany;
	bool returnEliminated;
    };

    //-----------------------------------------------------------------------------

}

/** @} */
#endif
