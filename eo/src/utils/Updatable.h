// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// Updatable.h
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

#ifndef _Updatable_h
#define _Updatable_h

#include <utils/Updater.h>

namespace eo
{

    /**
       Updatable is a generic class for adding updatation to an existing class
       Just says it has an update() method

       @ingroup Utilities
    */
    class Updatable
    {
    public:

	/** @brief Virtual destructor */
	virtual ~Updatable() {};

	virtual void update() = 0;
    };



    /**
       A base class to actually update an Updatable object

       @ingroup Utilities
    */
    class DynUpdater : public Updater
    {public :
	DynUpdater(Updatable & _toUpdate) : toUpdate(_toUpdate) {};

	virtual void operator()()
	{
	    toUpdate.update();
	}

    private:
	Updatable& toUpdate;
    };

    /**
       An Updater to update an Updatable object every given time interval

       @ingroup Utilities
    */
    class TimedDynUpdate : public DynUpdater
    {
    public :
	TimedDynUpdate(Updatable & _toUpdate, time_t _interval) :
	    DynUpdater(_toUpdate),
	    interval(_interval), last_time(time(0)), first_time(time(0)) {}

	void operator()(void)
	{
	    time_t now = time(0);

	    if (now >= last_time + interval)
		{
		    last_time = now;
		    DynUpdater::operator() ();
		}
	}
    private :
	const time_t interval;
	time_t last_time;
	const time_t first_time;
    };

    /**
       An Updater to update an Updatable object every given tic

       @ingroup Utilities
    */
    class CountedDynUpdate : public DynUpdater
    {
    public :
	CountedDynUpdate(Updatable & _toUpdate, unsigned _interval)
	    : DynUpdater(_toUpdate), interval(_interval), counter(0) {}

	void operator()(void)
	{
	    if (++counter % interval == 0)
		{
		    DynUpdater::operator() ();
		}
	}
    private :
	const unsigned interval;
	unsigned counter;
    };

}

#endif
