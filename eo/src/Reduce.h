// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// Reduce.h
//   Base class for population-merging classes
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

#ifndef Reduce_h
#define Reduce_h

//-----------------------------------------------------------------------------

#include <iostream>

// EO includes
#include <Pop.h>     // Pop
#include <Functor.h>  // Reduce
#include <utils/selectors.h>

namespace eo
{

    /**
     * Reduce: .reduce the new generation to the specified size
     At the moment, limited to truncation - with 2 different methods,
     one that sorts the whole population, and one that repeatidely kills 
     the worst. Ideally, we should be able to choose at run-time!!!

     @ingroup Replacors
    */
    template<class EOT> class Reduce: public BF<Pop<EOT>&, unsigned, void>
    {};

    /** truncation method using sort 
	@ingroup Replacors
    */
    template <class EOT> class Truncate : public Reduce<EOT>
    {
	void operator()(Pop<EOT>& _newgen, unsigned _newsize)
	{
	    if (_newgen.size() == _newsize)
		return;
	    if (_newgen.size() < _newsize)
		throw std::logic_error("Truncate: Cannot truncate to a larger size!\n");
        
	    _newgen.sort();
	    _newgen.resize(_newsize);
	}
    };

    /** random truncation 
	@ingroup Replacors
	* */
    template <class EOT> class RandomReduce : public Reduce<EOT>
    {
	void operator()(Pop<EOT>& _newgen, unsigned _newsize)
	{
	    if (_newgen.size() == _newsize)
		return;
	    if (_newgen.size() < _newsize)
		throw std::logic_error("RandomReduce: Cannot truncate to a larger size!\n");

	    // shuffle the population, then trucate
	    _newgen.shuffle();
	    _newgen.resize(_newsize);
	}
    };

    /** 
	EP truncation method (some global stochastic tournament +  sort)
	Softer selective pressure than pure truncate
	@ingroup Replacors
    */
    template <class EOT> class EPReduce : public Reduce<EOT>
    {
    public:
	typedef typename EOT::Fitness Fitness; 

	EPReduce(unsigned _t_size  ):
	    t_size(_t_size)
	{
	    if (t_size < 2)
		{ 
		    log << warnings << "Warning: EP tournament size should be >= 2. Adjusted" << std::endl;
		    t_size = 2;
		}
	}

	/// helper struct for comparing on std::pairs
	// compares the scores
	// uses the fitness if scores are equals ????
	typedef std::pair<float, typename Pop<EOT>::iterator>  EPpair;
	struct Cmp {
	    bool operator()(const EPpair a, const EPpair b) const
	    { 
		if (b.first == a.first)
		    return  (*b.second < *a.second);
		return b.first < a.first; 
	    }
	};
  
  
	void operator()(Pop<EOT>& _newgen, unsigned _newsize)
	{
	    unsigned int presentSize = _newgen.size();
    
	    if (presentSize == _newsize)
		return;
	    if (presentSize < _newsize)
		throw std::logic_error("Truncate: Cannot truncate to a larger size!\n");
	    std::vector<EPpair> scores(presentSize);
	    for (unsigned i=0; i<presentSize; i++)
		{
		    scores[i].second = _newgen.begin()+i;
		    Fitness fit = _newgen[i].fitness();
		    for (unsigned itourn = 0; itourn < t_size; ++itourn)
			{
			    const EOT & competitor = _newgen[rng.random(presentSize)];
			    if (fit > competitor.fitness())
				scores[i].first += 1;
			    else if (fit == competitor.fitness())
				scores[i].first += 0.5;
			}
		}

	    // now we have the scores
	    typename std::vector<EPpair>::iterator it = scores.begin() + _newsize;
	    std::nth_element(scores.begin(), it, scores.end(), Cmp());
	    // sort(scores.begin(), scores.end(), Cmp());
	    unsigned j;
	    // 	std::cout << "Les scores apres tri\n";
	    // 	for (j=0; j<scores.size(); j++)
	    // 	  {
	    // 	    std::cout << scores[j].first << " " << *scores[j].second << std::endl;
	    // 	  }
	    Pop<EOT> tmPop;
	    for (j=0; j<_newsize; j++)
		{
		    tmPop.push_back(*scores[j].second);
		}
	    _newgen.swap(tmPop);
	    // erase does not work, but I'm sure there is a way in STL to mark
	    // and later delete all inside a std::vector ??????
	    // this would avoid all copies here

	    // 	it = scores.begin() + _newsize;
	    // 	while (it < scores.end())
	    // 	  _newgen.erase(it->second);
	}
    private:
	unsigned t_size;
    };

    /** a truncate class that does not sort, but repeatidely kills the worse.
	To be used in SSGA-like replacements (e.g. see SSGAWorseReplacement)
	@ingroup Replacors
    */
    template <class EOT> 
    class LinearTruncate : public Reduce<EOT>
    {
	void operator()(Pop<EOT>& _newgen, unsigned _newsize)
	{
	    unsigned oldSize = _newgen.size();
	    if (oldSize == _newsize)
		return;
	    if (oldSize < _newsize)
		throw std::logic_error("LinearTruncate: Cannot truncate to a larger size!\n");
	    for (unsigned i=0; i<oldSize - _newsize; i++)
		{
		    typename Pop<EOT>::iterator it = _newgen.it_worse_element();
		    _newgen.erase(it);	    
		}
	}
    };

    /** a truncate class based on a repeated deterministic (reverse!) tournament
	To be used in SSGA-like replacements (e.g. see SSGADetTournamentReplacement)
	@ingroup Replacors
    */
    template <class EOT> 
    class DetTournamentTruncate : public Reduce<EOT>
    {
    public:
	DetTournamentTruncate(unsigned _t_size):
	    t_size(_t_size)
	{
	    if (t_size < 2)
		{ 
		    log << warnings << "Warning, Size for DetTournamentTruncate adjusted to 2" << std::endl;
		    t_size = 2;
		}
	}

	void operator()(Pop<EOT>& _newgen, unsigned _newsize)
	{
	    unsigned oldSize = _newgen.size();
	    if (_newsize == 0)
		{
		    _newgen.resize(0);
		    return;
		}
	    if (oldSize == _newsize)
		return;
	    if (oldSize < _newsize)
		throw std::logic_error("DetTournamentTruncate: Cannot truncate to a larger size!\n");

	    // Now OK to erase some losers
	    for (unsigned i=0; i<oldSize - _newsize; i++)
		{
		    //OLDCODE EOT & eo = inverse_deterministic_tournament<EOT>(_newgen, t_size);
		    //OLDCODE _newgen.erase(&eo);
	
		    // Jeroen Eggermont stdc++v3  patch
		    // in the new code from stdc++v3 an iterator from a container<T> is no longer an pointer to T
		    // Because eo already contained a fuction using Pop<EOT>::iterator's we will use the following
	
		    _newgen.erase( inverse_deterministic_tournament(_newgen.begin(), _newgen.end(), t_size) );
	
		}
	}
    private:
	unsigned t_size;
    };

    /** a truncate class based on a repeated deterministic (reverse!) tournament
	To be used in SSGA-like replacements (e.g. see SSGAStochTournamentReplacement)
	@ingroup Replacors
    */
    template <class EOT> 
    class StochTournamentTruncate : public Reduce<EOT>
    {
    public:
	StochTournamentTruncate(double _t_rate):
	    t_rate(_t_rate)
	{
	    if (t_rate <= 0.5)
		{ 
		    log << warnings << "Warning, Rate for StochTournamentTruncate adjusted to 0.51" << std::endl;
		    t_rate = 0.51;
		}
	    if (t_rate > 1)
		{
		    log << warnings << "Warning, Rate for StochTournamentTruncate adjusted to 1" << std::endl;
		    t_rate = 1;
		}
	}

	void operator()(Pop<EOT>& _newgen, unsigned _newsize)
	{
	    unsigned oldSize = _newgen.size();
	    if (_newsize == 0)
		{
		    _newgen.resize(0);
		    return;
		}
	    if (oldSize == _newsize)
		return;
	    if (oldSize < _newsize)
		throw std::logic_error("StochTournamentTruncate: Cannot truncate to a larger size!\n");
	    // Now OK to erase some losers
	    for (unsigned i=0; i<oldSize - _newsize; i++)
		{
		    //OLDCODE EOT & eo = inverse_stochastic_tournament<EOT>(_newgen, t_rate);
		    //OLDCODE _newgen.erase(&eo);
	
		    // Jeroen Eggermont stdc++v3  patch
		    // in the new code from stdc++v3 an iterator from a container<T> is no longer an pointer to T
		    // Because eo already contained a fuction using Pop<EOT>::iterator's we will use the following
	
		    _newgen.erase( inverse_stochastic_tournament(_newgen.begin(), _newgen.end(), t_rate) );
	
	
		}
	}

    private:
	double t_rate;
    };

}

//-----------------------------------------------------------------------------

#endif
