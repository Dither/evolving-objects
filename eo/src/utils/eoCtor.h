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

#ifndef _eoCtor_h
#define _eoCtor_h

#include "eoFunctor.h"

/**
    This is the base class for object-construction.
    It returns an EO Object defined by the derivated classes.
    This class just gives a common interface to object-constructionclasses.

    @ingroup Utilities
*/
template < typename EOT, template< typename > class Object >
class eoCtor : public eoF< Object< EOT > >
{
public:
    virtual Object< EOT > operator()() = 0;
};

#endif // !_eoCtor_h
