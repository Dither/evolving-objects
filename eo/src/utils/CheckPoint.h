// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// CheckPoint.h
// (c) Maarten Keijzer, Marc Schoenauer and GeNeura Team, 2000
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
             mkeijzer@dhi.dk
 */
//-----------------------------------------------------------------------------

#ifndef _CheckPoint_h
#define _CheckPoint_h

#include <Continue.h>
#include <utils/Updater.h>
#include <utils/Monitor.h>
#include <utils/Stat.h>

namespace eo
{

    /** @defgroup Checkpoints Checkpointing
     *
     * Checkpoints are supposed to be called perodically (for instance at each generation) and
     * will call every functors you put in them.
     *
     * Use them with Stats, Updater and Monitor to get statistics at each generation.
     *
     * @see Stat
     * @see Monitor
     * @see Updater
     *
     * Example of a test program using checkpointing:
     * @include t-eoCheckpointing.cpp
     *
     * @ingroup Utilities
     *
     * @{
     */

    /** CheckPoint is a container class.
	It contains std::vectors of (pointers to) 
	Continue    (modif. MS July 16. 2002)
	Stats, Updater and Monitor
	it is an Continue, so its operator() will be called every generation - 
	and will return the contained-combined-Continue result
	but before that it will call in turn every single 
	{statistics, updaters, monitors} that it has been given,
	and after that, if stopping, all lastCall methods of the above.
    */
    template <class EOT>
    class CheckPoint : public Continue<EOT>
    {
    public :

	CheckPoint(Continue<EOT>& _cont) 
	{
	    continuators.push_back(&_cont);
	}

	bool operator()(const Pop<EOT>& _pop);

	void add(Continue<EOT>& _cont) { continuators.push_back(&_cont); }
	void add(SortedStatBase<EOT>& _stat) { sorted.push_back(&_stat); }
	void add(StatBase<EOT>& _stat) { stats.push_back(&_stat); }
	void add(Monitor& _mon)        { monitors.push_back(&_mon); }
	void add(Updater& _upd)        { updaters.push_back(&_upd); }

	virtual std::string className(void) const { return "CheckPoint"; }
	std::string allClassNames() const ;

    private :

	std::vector<Continue<EOT>*>    continuators;
	std::vector<SortedStatBase<EOT>*>    sorted;
	std::vector<StatBase<EOT>*>    stats;
	std::vector<Monitor*> monitors;
	std::vector<Updater*> updaters;
    };

    template <class EOT>
    bool CheckPoint<EOT>::operator()(const Pop<EOT>& _pop)
    {
	unsigned i;

	std::vector<const EOT*> sorted_pop;
	if (!sorted.empty())
	    {
		_pop.sort(sorted_pop);

		for (i = 0; i < sorted.size(); ++i)
		    {
			(*sorted[i])(sorted_pop);
		    }
	    }

	for (i = 0; i < stats.size(); ++i)
	    (*stats[i])(_pop);

	for (i = 0; i < updaters.size(); ++i)
	    (*updaters[i])();

	for (i = 0; i < monitors.size(); ++i)
	    (*monitors[i])();

	bool bContinue = true;
	for (i = 0; i < continuators.size(); ++i)
	    if ( !(*continuators[i])(_pop) ) 
		bContinue = false;

	if (! bContinue)	   // we're going to stop: lastCall, gentlemen
	    {
		if (!sorted.empty())
		    {
			for (i = 0; i < sorted.size(); ++i)
			    {
				sorted[i]->lastCall(sorted_pop);
			    }
		    }
		for (i = 0; i < stats.size(); ++i)
		    stats[i]->lastCall(_pop);

		for (i = 0; i < updaters.size(); ++i)
		    updaters[i]->lastCall();

		for (i = 0; i < monitors.size(); ++i)
		    monitors[i]->lastCall();
	    }
	return bContinue;
    }

    /** returns a string with all className() 
     *  of data separated with "\n" (for debugging)
     */
    template <class EOT>
    std::string CheckPoint<EOT>::allClassNames() const
    {
	unsigned i;
	std::string s = "\n" + className() + "\n";

	s += "Sorted Stats\n";
	for (i = 0; i < sorted.size(); ++i)
	    s += sorted[i]->className() + "\n";
	s += "\n";

	s += "Stats\n";
	for (i = 0; i < stats.size(); ++i)
	    s += stats[i]->className() + "\n";
	s += "\n";

	s += "Updaters\n";
	for (i = 0; i < updaters.size(); ++i)
	    s += updaters[i]->className() + "\n";
	s += "\n";

	s += "Monitors\n";
	for (i = 0; i < monitors.size(); ++i)
	    s += monitors[i]->className() + "\n";
	s += "\n";

	s += "Continuators\n";
	for (i = 0; i < continuators.size(); ++i)
	    s += continuators[i]->className() + "\n";
	s += "\n";

	return s;
    }

}

/** @} */
#endif
