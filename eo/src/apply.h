// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// eoApply.h
// (c) Maarten Keijzer 2000
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
	     mak@dhi.dk
 */
//-----------------------------------------------------------------------------

#ifndef _apply_h
#define _apply_h

#include <utils/eoParallel.h>
#include <utils/eoParser.h>
#include <utils/eoLogger.h>
#include <eoFunctor.h>
#include <vector>
#include <omp.h>
#include <numeric> // accumulate

/**
  Applies a unary function to a std::vector of things.

  @ingroup Utilities
*/
template <class EOT>
void apply(eoUF<EOT&, void>& _proc, std::vector<EOT>& _pop)
{
    size_t size = _pop.size();

#ifdef _OPENMP

    std::vector< double > times_per_eval(size, 0);
    std::vector< double > times_per_thread(eo::parallel.nthreads(), 0);

    bool enableResults = eo::parallel.enableResults();

    if (!eo::parallel.isDynamic())
	{
#pragma omp parallel for if(eo::parallel.isEnabled()) firstprivate(enableResults)
	    //doesnot work with gcc 4.1.2
	    //default(none) shared(_proc, _pop, size)
	    for (size_t i = 0; i < size; ++i)
		{
		    if ( enableResults )
			{
			    times_per_eval[i] = omp_get_wtime(); // first measure
			}

		    _proc(_pop[i]); // evaluation

		    if ( enableResults )
			{
			    times_per_eval[i] = omp_get_wtime() - times_per_eval[i]; // last measure
			    times_per_thread[omp_get_thread_num()] += times_per_eval[i];
			}
		}
	}
    else
	{
#pragma omp parallel for schedule(dynamic) if(eo::parallel.isEnabled()) firstprivate(enableResults)
	    //doesnot work with gcc 4.1.2
	    //default(none) shared(_proc, _pop, size)
	    for (size_t i = 0; i < size; ++i)
		{
		    if ( enableResults )
			{
			    times_per_eval[i] = omp_get_wtime(); // first measure
			}

		    _proc(_pop[i]); // evaluation

		    if ( enableResults )
			{
			    times_per_eval[i] = omp_get_wtime() - times_per_eval[i]; // last measure
			    times_per_thread[omp_get_thread_num()] += times_per_eval[i];
			}
		}
	}

    if ( enableResults )
	{
	    {
		eoLogger log( eo::file( eo::parallel.prefix() + "_times_per_eval.txt" ) );
		log << times_per_eval.size() << ' ';
		std::copy(times_per_eval.begin(), times_per_eval.end(), std::ostream_iterator<double>(log, " "));
		log << std::endl;
	    }

	    {
		eoLogger log( eo::file( eo::parallel.prefix() + "_times_per_thread.txt" ) );
		log << times_per_thread.size() << ' ';
		std::copy(times_per_thread.begin(), times_per_thread.end(), std::ostream_iterator<double>(log, " "));
		log << std::endl;
	    }

	    {
		eoLogger log( eo::file( eo::parallel.prefix() + "_accumulated_sequential_times.txt" ) );
		log << times_per_eval.size() << ' ' << std::accumulate( times_per_eval.begin(), times_per_eval.end(), 0.0) << std::endl;
	    }
	}

#else // _OPENMP

    for (size_t i = 0; i < size; ++i) { _proc(_pop[i]); }

#endif // !_OPENMP
}

#endif // !_apply_h


//// DEPRECATED ////
//
// template <class EOT>
// void apply(eoUF<EOT&, void>& _proc, std::vector<EOT>& _pop)
// {
//     size_t size = _pop.size();

//     eoLogger log;
//     log << eo::file("evaluation_times.txt");

// #pragma omp parallel for schedule(dynamic) if(eo::parallel.isEnabled())
//     for (size_t i = 0; i < size; ++i)
// 	{
// 	    double t1 = omp_get_wtime();
// 	    _proc(_pop[i]);
// 	    double t2 = omp_get_wtime();

// #pragma omp critical
// 	    log << t2-t1 << std::endl;
// 	}
// }

/**
  This is a variant of apply<EOT> which is called in parallel
  thanks to OpenMP.

  @ingroup Utilities
*/
// template <class EOT>
// void omp_apply(eoUF<EOT&, void>& _proc, std::vector<EOT>& _pop)
// {
//     size_t size = _pop.size();
// #pragma omp parallel for if(eo::parallel.isEnabled())
//     //doesnot work with gcc 4.1.2
//     //default(none) shared(_proc, _pop, size)
//     for (size_t i = 0; i < size; ++i)
//     {
// 	_proc(_pop[i]);
//     }
// }

/**
  And now we are using the dynamic scheduling.

  @ingroup Utilities
*/
// template <class EOT>
// void omp_dynamic_apply(eoUF<EOT&, void>& _proc, std::vector<EOT>& _pop)
// {
//     size_t size = _pop.size();
// #pragma omp parallel for if(eo::parallel.isEnabled()) schedule(dynamic)
//     //doesnot work with gcc 4.1.2
//     //default(none) shared(_proc, _pop, size)
//     for (size_t i = 0; i < size; ++i)
//     {
// 	_proc(_pop[i]);
//     }
// }
