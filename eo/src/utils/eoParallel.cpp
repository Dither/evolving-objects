// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

/*

(c) Thales group, 2010

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
    version 2 of the license.

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

#ifdef WITH_OMP
#include <omp.h>
#endif // !WITH_OMP

#include "eoParallel.h"
#include "eoLogger.h"

eoParallel::eoParallel() :
    _t_start(0)

#if defined(WITH_OMP) or defined(WITH_MPI)
    , _isDynamic( true, "parallelize-dynamic", "Enable dynamic memory shared parallelization", '\0' )
    , _doMeasure( false, "parallelize-do-measure", "Do some measures during execution", '\0' )
#endif // !WITH_OMP or WITH_MPI

#ifdef WITH_OMP
    , _isEnabled( false, "parallelize-loop", "Enable memory shared parallelization into evaluation's loops", '\0' )
    , _prefix( "results", "parallelize-prefix", "Here's the prefix filename where the results are going to be stored", '\0' )
    , _nthreads( 0, "parallelize-nthreads", "Define the number of threads you want to use, nthreads = 0 means you want to use all threads available", '\0' )
    , _enableResults( false, "parallelize-enable-results", "Enable the generation of results", '\0' )
#endif // !WITH_OMP

#ifdef WITH_MPI
    , _packetSize( 1U, "parallelize-packet-size", "Number of elements which should be sent in a single message during a parallel evaluation based on message passing.", '\0')
#endif // !WITH_MPI
{
    // empty
}

eoParallel::~eoParallel()
{
#ifdef WITH_OMP
    if ( doMeasure() )
        {
            double _t_end = omp_get_wtime();
            eoLogger log;
            log << eo::file("measure_" + prefix()) << _t_end - _t_start << std::endl;
        }
#endif // !WITH_OMP
}

std::string eoParallel::className() const
{
    return "eoParallel";
}

#ifdef WITH_OMP
std::string eoParallel::prefix() const
{
    std::string value( _prefix.value() );

    if ( _isEnabled.value() )
        {
            if ( _isDynamic.value() )
                {
                    value += "_dynamic.out";
                }
            else
                {
                    value += "_parallel.out";
                }
        }
    else
        {
            value += "_sequential.out";
        }

    return value;
}
#endif // !WITH_OMP

void eoParallel::_createParameters( eoParser& parser )
{
    std::string section;

#if defined(WITH_OMP) or defined(WITH_MPI)
    section = "Parallelization";
    parser.processParam( _isDynamic, section );
    parser.processParam( _doMeasure, section );
#endif // !WITH_OMP or WITH_MPI

#ifdef WITH_OMP
    section = "Parallelization with OpenMP";
    parser.processParam( _isEnabled, section );
    parser.processParam( _prefix, section );
    parser.processParam( _nthreads, section );
    parser.processParam( _enableResults, section );
#endif // !WITH_OMP

#ifdef WITH_MPI
    section = "Parallelization with MPI";
    parser.processParam( _packetSize, section );
#endif // !WITH_MPI
}

void make_parallel(eoParser& parser)
{
    eo::parallel._createParameters( parser );

#ifdef WITH_OMP
    if ( eo::parallel.isEnabled() )
        {
            if ( eo::parallel.nthreads() > 0 )
                {
                    omp_set_num_threads( eo::parallel.nthreads() );
                }
        }

    if ( eo::parallel.doMeasure() )
        {
            eo::parallel._t_start = omp_get_wtime();
        }
#endif // !WITH_OMP
}

eoParallel eo::parallel;
