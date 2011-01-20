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
*/

#ifndef _EvalUserTimeThrowException_h
#define _EvalUserTimeThrowException_h

#include <sys/time.h>
#include <sys/resource.h>

#include <Exceptions.h>

namespace eo
{

    /** Check at each evaluation if a given CPU user time contract has been reached.
     *
     * Throw an MaxTimeException if the given max time has been reached.
     * Usefull if you want to end the search independently of generations.
     * This class uses (almost-)POSIX headers.
     * It uses a computation of the user time used on the CPU. For a wallclock time measure, see EvalTimeThrowException
     *
     * @ingroup Evaluation
     */
    template< class EOT >
    class EvalUserTimeThrowException : public EvalFuncCounter< EOT >
    {
    public:
	EvalUserTimeThrowException( EvalFunc<EOT> & func, long max ) : _max(max), EvalFuncCounter<EOT>( func, "CPU-user") {}

	virtual void operator() ( EOT & eo )
	{
	    if( eo.invalid() ) {

		getrusage(RUSAGE_SELF,&_usage);

		if( _usage.ru_utime.tv_sec >= _max ) {
		    throw MaxTimeException( _usage.ru_utime.tv_sec );
		} else {
		    func(eo);
		}
	    }
	}

    protected:
	long _max;
	struct rusage _usage;
    };

}

#endif // !_EvalUserTimeThrowException_h
