// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// Updater.h
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

#ifndef _Updater_h
#define _Updater_h

#include <string>
#include <Functor.h>
#include <utils/State.h>
#include <utils/Param.h>

namespace eo
{

    template <class EOT> class CheckPoint;

    /**
       Updater is a generic procudere for updating whatever you want.
       Yet again an empty name

       @ingroup Utilities
    */
    class Updater : public F<void>
    {
    public:
	virtual void lastCall() {}
	virtual std::string className(void) const { return "Updater"; }
   
	template <class EOT> 
	Updater& addTo(CheckPoint<EOT>& cp)        { cp.add(*this);  return *this; }
    };

    /**
       an Updater that simply increments a counter

       @ingroup Utilities
    */
    template <class T>
    class Incrementor : public Updater
    {public :
	/** Default Ctor - requires a reference to the thing to increment */
	Incrementor(T& _counter, T _stepsize = 1) : counter(_counter), stepsize(_stepsize) {}

	/** Simply increments */
	virtual void operator()()
	{
	    counter += stepsize;
	}

	virtual std::string className(void) const { return "Incrementor"; }
    private:
	T& counter;
	T stepsize;
    };

    /** an Updater that is an ValueParam (and thus OWNS its counter)
     *  Mandatory for generation counter in make_checkpoint

     @ingroup Utilities
    */
    template <class T>
    class IncrementorParam : public Updater, public ValueParam<T>
    {
    public:

	using ValueParam<T>::value;

	/** Default Ctor : a name and optionally an increment*/
	IncrementorParam( std::string _name, T _stepsize = 1) :
	    ValueParam<T>(T(0), _name), stepsize(_stepsize) {}

	/** Ctor with a name and non-zero initial value
	 *  and mandatory stepSize to remove ambiguity
	 */
	IncrementorParam( std::string _name, T _countValue, T _stepsize) :
	    ValueParam<T>(_countValue, _name), stepsize(_stepsize) {}

	/** Simply increments */
	virtual void operator()()
	{
	    value() += stepsize;
	}

	virtual std::string className(void) const { return "IncrementorParam"; }

    private:
	T stepsize;
    };

#include <time.h>

    /**
       an Updater that saves a state every given time interval

       @ingroup Utilities
    */
    class TimedStateSaver : public Updater
    {
    public :
	TimedStateSaver(time_t _interval, const State& _state, std::string _prefix = "state", std::string _extension = "sav") : state(_state),
																    interval(_interval), last_time(time(0)), first_time(time(0)),
																    prefix(_prefix), extension(_extension) {}

	void operator()(void);

	virtual std::string className(void) const { return "TimedStateSaver"; }
    private :
	const State& state;

	const time_t interval;
	time_t last_time;
	const time_t first_time;
	const std::string prefix;
	const std::string extension;
    };

    /**
       an Updater that saves a state every given generations

       @ingroup Utilities
    */
    class CountedStateSaver : public Updater
    {
    public :
	CountedStateSaver(unsigned _interval, const State& _state, std::string _prefix, bool _saveOnLastCall, std::string _extension = "sav", unsigned _counter = 0)
	    : state(_state), interval(_interval), counter(_counter),
	      saveOnLastCall(_saveOnLastCall),
	      prefix(_prefix), extension(_extension) {}

	CountedStateSaver(unsigned _interval, const State& _state, std::string _prefix = "state", std::string _extension = "sav", unsigned _counter = 0)
	    : state(_state), interval(_interval), counter(_counter),
	      saveOnLastCall(true),
	      prefix(_prefix), extension(_extension) {}

	virtual void lastCall(void);
	void operator()(void);

	virtual std::string className(void) const { return "CountedStateSaver"; }
    private :
	void doItNow(void);

	const State& state;
	const unsigned interval;
	unsigned counter;
	bool saveOnLastCall;

	const std::string prefix;
	const std::string extension;
    };

}

#endif
