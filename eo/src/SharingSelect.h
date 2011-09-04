// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// SharingSelect.h
// (c) GeNeura Team, 1998, Maarten Keijzer 2000, Marc Schoenauer 2001
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

    Contact: Marc.Schoenauer@polytechnique.fr
             mak@dhi.dk
 */
//-----------------------------------------------------------------------------

#ifndef SharingSelect_h
#define SharingSelect_h

//-----------------------------------------------------------------------------

#include <SelectFromWorth.h>
#include <Sharing.h>

namespace eo
{

    /** SharingSelect: select an individual by roulette wheel 
     * on its SHARED fitness. It is an RouletteWorthSelect, 
     * i.e. a selector using a std::vector of worthes
     * rather than the raw fitness (see SelectFromWorth.h)
     * It uses an internal Sharing object which is 
     * an Perf2Worth<EOT, double>
     * @ingroup Selectors
     */

    template <class EOT> 
    class SharingSelect: public RouletteWorthSelect<EOT, double> 
    {
    public:
	/** Ctor:
	 *  @param _sigma the sigma_share
	 *  @param _dist the distance object to use
	 */
	SharingSelect(double _sigma, Distance<EOT> & _dist): 
	    RouletteWorthSelect<EOT, double>(sharing), sharing(_sigma, _dist) {}

    private :
	Sharing<EOT> sharing;	   // derived from Perf2Worth
    };
    /** @example t-Sharing.cpp
     */

}

#endif
