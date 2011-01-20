/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

   -----------------------------------------------------------------------------
   Perf2Worth.h
   (c) Maarten Keijzer, Marc Schoenauer, 2001

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

#ifndef Perf2Worth_h
#define Perf2Worth_h

#include <utils/Param.h>
#include <Pop.h>
#include <Functor.h>

#include <algorithm>
#include <vector>
#include <string>

namespace eo
{

    /** @brief Base class to transform raw fitnesses into fitness for selection

	@see SelectFromWorth

	@ingroup Selectors
	@ingroup Utilities
    */
    template <class EOT, class WorthT = double>
    class Perf2Worth : public UF<const Pop<EOT>&, void>, public ValueParam<std::vector<WorthT> >
    {
    public:

	using ValueParam<std::vector<WorthT> >::value;

	/** @brief default constructor */
	Perf2Worth(std::string _description = "Worths")
	    : ValueParam<std::vector<WorthT> >(std::vector<WorthT>(0), _description)
        {}

	/** Sort population according to worth, will keep the worths and
	    fitness_cache in sync with the population. */
	virtual void sort_pop(Pop<EOT>& _pop)
	{ // start with a std::vector of indices
	    std::vector<unsigned> indices(_pop.size());

	    unsigned i;
	    for (i = 0; i < _pop.size();++i)
		{ // could use generate, but who cares
		    indices[i] = i;
		}

	    std::sort(indices.begin(), indices.end(), compare_worth(value()));

	    Pop<EOT>      tmp_pop;
	    tmp_pop.resize(_pop.size());
	    std::vector<WorthT>  tmp_worths(value().size());

	    for (i = 0; i < _pop.size(); ++i)
		{
		    tmp_pop[i] = _pop[indices[i]];
		    tmp_worths[i] = value()[indices[i]];
		}

	    std::swap(_pop, tmp_pop);
	    std::swap(value(), tmp_worths);
	}

	/** helper class used to sort indices into populations/worths */
	class compare_worth
	{
	public:

	    compare_worth(const std::vector<WorthT>& _worths) : worths(_worths) {}

	    bool operator()(unsigned a, unsigned b) const {
		return worths[b] < worths[a]; // sort in descending (!) order
	    }

	private:

	    const std::vector<WorthT>& worths;
	};



	virtual void resize(Pop<EOT>& _pop, unsigned sz) {
	    _pop.resize(sz);
	    value().resize(sz);
	};

    };

    /**
       Perf2Worth with fitness cache
       @ingroup Selectors
       @ingroup Utilities
    */
    template <class EOT, class WorthT = typename EOT::Fitness>
    class Perf2WorthCached : public Perf2Worth<EOT, WorthT>
    {
    public:

	using Perf2Worth<EOT, WorthT>::value;

	Perf2WorthCached(std::string _description = "Worths") : Perf2Worth<EOT, WorthT>(_description) {}


	/**
	   Implementation of the operator(), updating a cache of fitnesses. Calls the virtual function
	   calculate_worths when one of the fitnesses has changed. It is not virtual, but derived classes
	   can remove the fitness caching trough the third template element
	*/
	void operator()(const Pop<EOT>& _pop)
	{
	    unsigned i;
	    if (fitness_cache.size() == _pop.size())
		{
		    bool in_sync = true;
		    for (i = 0; i < _pop.size(); ++i)
			{
			    if (fitness_cache[i] != _pop[i].fitness())
				{
				    in_sync = false;
				    fitness_cache[i] = _pop[i].fitness();
				}
			}

		    if (in_sync)
			{ // worths are up to date
			    return;
			}
		}
	    else // just cache the fitness
		{
		    fitness_cache.resize(_pop.size());
		    for (i = 0; i < _pop.size(); ++i)
			{
			    fitness_cache[i] = _pop[i].fitness();
			}
		}

	    // call derived implementation of perf2worth mapping
	    calculate_worths(_pop);
	}

	/** The actual virtual function the derived classes should implement*/
	virtual void calculate_worths(const Pop<EOT>& _pop) = 0;

	/**
	   Sort population according to worth, will keep the worths and fitness_cache in sync with the population.
	*/
	virtual void sort_pop(Pop<EOT>& _pop)
	{ // start with a std::vector of indices
	    std::vector<unsigned> indices(_pop.size());

	    unsigned i;
	    for (i = 0; i < _pop.size();++i)
		{ // could use generate, but who cares
		    indices[i] = i;
		}

	    std::sort(indices.begin(), indices.end(), compare_worth(value()));

	    Pop<EOT>      tmp_pop;
	    tmp_pop.resize(_pop.size());
	    std::vector<WorthT>  tmp_worths(value().size());

#ifdef _MSC_VER
	    std::vector<EOT::Fitness> tmp_cache(_pop.size());
#else
	    std::vector<typename EOT::Fitness> tmp_cache(_pop.size());
#endif
	    for (i = 0; i < _pop.size(); ++i)
		{
		    tmp_pop[i] = _pop[indices[i]];
		    tmp_worths[i] = value()[indices[i]];

		    tmp_cache[i] = fitness_cache[indices[i]];
		}

	    std::swap(_pop, tmp_pop);
	    std::swap(value(), tmp_worths);
	    std::swap(fitness_cache, tmp_cache);
	}

	/** helper class used to sort indices into populations/worths
	 */
	class compare_worth
	{
	public :
	    compare_worth(const std::vector<WorthT>& _worths) : worths(_worths) {}

	    bool operator()(unsigned a, unsigned b) const
	    {
		return worths[b] < worths[a]; // sort in descending (!) order
	    }

	private :

	    const std::vector<WorthT>& worths;
	};

	virtual void resize(Pop<EOT>& _pop, unsigned sz)
	{
	    _pop.resize(sz);
	    value().resize(sz);
	    fitness_cache.resize(sz);
	}

    private :
	std::vector <typename EOT::Fitness> fitness_cache;
    };



    /** A dummy perf2worth, just in case you need it
	@ingroup Selectors
	@ingroup Utilities
    */
    template <class EOT>
    class NoPerf2Worth : public Perf2Worth<EOT, typename EOT::Fitness>
    {
    public:

	using ValueParam< EOT >::value;

	// default behaviour, just copy fitnesses
	void operator()(const Pop<EOT>& _pop) {
	    unsigned i;
	    value().resize(_pop.size());
	    for (i = 0; i < _pop.size(); ++i)
		value()[i]=_pop[i];
	}
    };

}

#endif
