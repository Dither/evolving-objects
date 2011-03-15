// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 * Caner Candan <caner@candan.fr>, http://caner.candan.fr
 */

#ifndef _eoPopCtor_h
#define _eoPopCtor_h

#include "eoCtor.h"
#include "eoPop.h"

/**
    This class allows to deleguate the building of a population.
    It can be useful whether you wish that another class built a population but still in keeping control of the size and some others parameters that you may need.
    It returns a population of individuals.

    @ingroup Utilities
*/
template < typename EOT >
class eoPopCtor : public eoCtor< EOT, eoPop >
{
public:
    eoPopCtor( const size_t size ) : _size( size ) {}

    eoPop< EOT > operator()()
    {
	eoPop< EOT > pop;
	pop.resize(_size);
	return pop;
    }

private:
    const size_t _size;
};

template < typename EOT >
class eoDummyPopCtor : public eoCtor< EOT, eoPop >
{
public:
    eoPop< EOT > operator()() { return eoPop< EOT >(); }
};

#endif // !_eoPopCtor_h
