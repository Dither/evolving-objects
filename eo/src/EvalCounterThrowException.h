/* 
(c) Thales group, 2010

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
    version 2 of the License.

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
Caner Candan <caner.candan@thalesgroup.com>

*/

#ifndef __EvalCounterThrowException_h__
#define __EvalCounterThrowException_h__

#include <EvalFuncCounter.h>
#include <utils/Param.h>
#include <Exceptions.h>

namespace eo
{

    /*!
      Wrap an evaluation function so that an exception may be thrown when the
      algorithm have reached a maximum number of evaluations.

      This may be useful if you want to check this kind of stopping criterion
      at each _evaluation_, instead of using continuators at each _iteration_.

      The class first call the evaluation function, then check the number of
      times it has been called. If the maximum number of evaluation has been
      reached, it throw an MaxEvalException. You can catch this exception
      from your main function, so as to stop everything properly.

      @ingroup Evaluation
    */
    template < typename EOT >
    class EvalCounterThrowException : public EvalFuncCounter< EOT >
    {
    public :
	EvalCounterThrowException( EvalFunc<EOT>& func, unsigned long max_evals, std::string name = "Eval. ")
	    : EvalFuncCounter< EOT >( func, name ), _threshold( max_evals )
	{}

	using EvalFuncCounter< EOT >::value;

	//! Evaluate the individual, then throw an exception if it exceed the max number of evals.
	virtual void operator()(EOT& eo)
	{
	    // bypass already evaluated individuals
	    if (eo.invalid()) {

		// increment the value of the self parameter 
		// (EvalFuncCounter inherits from @see ValueParam) 
		value()++;

		// if we have reached the maximum
		if ( value() >= _threshold ) {

		    // go back through the stack until catched
		    throw MaxEvalException(_threshold);
		}

		// evaluate
		func(eo);

	    } // if invalid
	}

	virtual std::string className() const {return "EvalCounterThrowException";}

    private :
	unsigned long _threshold;
    };

}

#endif // __EvalCounterThrowException_h__

