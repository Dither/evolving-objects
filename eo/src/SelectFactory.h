// SelectFactory.h
// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// SelectFactory.h
// (c) GeNeura Team, 1998
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
 */
//-----------------------------------------------------------------------------

#ifndef _SELECTFACTORY_H
#define _SELECTFACTORY_H

#include <Factory.h>
#include <RandomSelect.h>
#include <eoTournament.h>

//-----------------------------------------------------------------------------

namespace eo
{

    /** EO Factory.An instance of the factory class to create selectors, that is,
	Select objects

	@see Select
	@ingroup Selectors
	@ingroup Utilities
    */
    template< class EOT>
    class SelectFactory: public Factory<Select< EOT> > {
	
    public:
	
	/// @name ctors and dtors
	//{@
	/// constructor
	SelectFactory( ) {}
	
	/// destructor
	virtual ~SelectFactory() {}
	//@}

	/** Another factory methods: creates an object from an std::istream, reading from
	    it whatever is needed to create the object. Usually, the format for the std::istream will be\\
	    objectType parameter1 parameter2 ... parametern\\
	*/
	virtual Select<EOT>* make(std::istream& _is) {
	    Select<EOT> * selectPtr;
	    std::string objectTypeStr;
	    _is >> objectTypeStr;
	    // All selectors have a rate, the proportion of the original population
	    float rate;
	    _is >> rate;
	    if  ( objectTypeStr == "eoTournament") {
		// another parameter is necessary
		unsigned tSize;
		_is >> tSize;
		selectPtr = new eoTournament<EOT>( rate, tSize );
	    } else 	{
		if ( objectTypeStr == "RandomSelect" ) {
		    selectPtr = new RandomSelect<EOT>( rate );
		} else {
		    throw std::runtime_error( "Incorrect selector type" );
		}
	    }
	    return selectPtr;
	}

	///@name Object methods
	//@{
	void printOn( std::ostream& _os ) const {};
	void readFrom( std::istream& _is ){};

	/** className is inherited */
	//@}
	
    };

}

#endif
