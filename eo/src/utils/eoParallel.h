// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

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
    Caner Candan <caner.candan@thalesgroup.com>
*/

/** @defgroup Parallel Parallel
 * @ingroup Utilities
 @{
*/

#ifndef eoParallel_h
#define eoParallel_h

#include "eoObject.h"
#include "eoParser.h"

/**
 * eoParallel
 * Class providing parameters for parallelization
 * Use of a global variable eo::parallel to easily use the parallelization parameters anywhere
 */
class eoParallel : public eoObject
{
public:
    eoParallel();
    ~eoParallel();

    virtual std::string	className() const;

#if defined(WITH_OMP) or defined(WITH_MPI)
    inline bool doMeasure() const { return _doMeasure.value(); }
    inline bool isDynamic() const { return _isDynamic.value(); }
#endif // !WITH_OMP or WITH_MPI

#ifdef WITH_OMP
    inline bool isEnabled() const { return _isEnabled.value(); }
    std::string prefix() const;
    inline unsigned int nthreads() const { return _nthreads.value(); }
    inline bool enableResults() const { return _enableResults.value(); }
#endif // !WITH_OMP

#ifdef WITH_MPI
    inline unsigned int packetSize() const { return _packetSize.value(); }
#endif // !WITH_MPI

    friend void make_parallel(eoParser&);

private:
    void _createParameters( eoParser& );

private:
    double _t_start;

#if defined(WITH_OMP) or defined(WITH_MPI)
    eoValueParam<bool> _isDynamic;
    eoValueParam<bool> _doMeasure;
#endif // !WITH_OMP or WITH_MPI

#ifdef WITH_OMP
    eoValueParam<bool> _isEnabled;
    eoValueParam<std::string> _prefix;
    eoValueParam<unsigned int> _nthreads;
    eoValueParam<bool> _enableResults;
#endif // !WITH_OMP

#ifdef WITH_MPI
    eoValueParam<unsigned int> _packetSize;
#endif // !WITH_MPI
};

void make_parallel(eoParser&);

namespace eo
{
    /**
     * parallel is an external global variable defined in order to use where ever you want the parallel parameters
     */
    extern eoParallel parallel;
}

/** @} */

#endif // !eoParallel_h
