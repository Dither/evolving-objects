// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// OpSelMason.h
// (c) GeNeura Team, 1999
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

#ifndef _OPSELMASON_H
#define _OPSELMASON_H

#include <OpFactory.h>	// for Factory and OpFactory

#include <map>

namespace eo
{

    /** EO Mason, or builder, for operator selectors. A builder must allocate memory
	to the objects it builds, and then deallocate it when it gets out of scope

	@ingroup Utilities
    */
    template<class Class>
    class OpSelMason: public Factory<OpSelector<Class> > {

    public:
	typedef std::vector<Op<Class>* > vOpP;
	typedef map<OpSelector<Class>*, vOpP > MEV;

	/// @name ctors and dtors
	//{@
	/// constructor
	OpSelMason( OpFactory<Class>& _opFact): operatorFactory( _opFact ) {};

	/// destructor
	virtual ~OpSelMason() {};
	//@}

	/** Factory methods: creates an object from an std::istream, reading from
	    it whatever is needed to create the object. The format is
	    opSelClassName\\
	    rate 1 operator1\\
	    rate 2 operator2\\
	    ...\\
	    Stores all operators built in a database (#allocMap#), so that somebody 
	    can destroy them later. The Mason is in charge or destroying the operators,
	    since the built object can´t do it itself. The objects built must be destroyed
	    from outside, using the "destroy" method
	*/
	virtual OpSelector<Class>* make(std::istream& _is) {

	    std::string opSelName;
	    _is >> opSelName;
	    OpSelector<Class>* opSelectorP;
	    // Build the operator selector
	    if ( opSelName == "ProportionalOpSel" ) {
		opSelectorP = new ProportionalOpSel<Class>();
	    }

	    // Temp std::vector for storing pointers
	    vOpP tmpPVec;
	    // read operator rate and name
	    while ( _is ) {
		float rate;
		_is >> rate;
		if ( _is ) {
		    Op<Class>* op = operatorFactory.make( _is );	// This reads the rest of the line
		    // Add the operators to the selector, don´t pay attention to the IDs
		    opSelectorP->addOp( *op, rate );
		    // Keep it in the store, to destroy later
		    tmpPVec.push_back( op );
		} // if
	    } // while

	    // Put it in the map
	    allocMap.insert( MEV::value_type( opSelectorP, tmpPVec ) );

	    return opSelectorP;
	};

	///@name Object methods
	//@{
	/** Return the class id */
	virtual std::string className() const { return "OpSelMason"; }

	//@}

    private:
	map<OpSelector<Class>*,std::vector<Op<Class>* > > allocMap;
	OpFactory<Class>& operatorFactory;
    };

}

#endif
