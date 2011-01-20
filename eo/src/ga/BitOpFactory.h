// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// BitOpFactory.h
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

#ifndef _BITOPFACTORY_H
#define _BITOPFACTORY_H

#include <Factory.h>
#include <ga/BitOp.h>

//-----------------------------------------------------------------------------

namespace eo
{

    /** EO Factory. An instance of the factory class to create operators that act
	on bitstring chromosomes. Only those chromosomes can instantiate the operators
	that are created here
	@see Select

	@ingroup Variators
    */
    template< class EOT>
    class BitOpFactory: public Factory<EOT>
    {

    public:

	/// @name ctors and dtors
	//{@
	/// constructor
	BitOpFactory( ) {};

	/// destructor
	virtual ~BitOpFactory() {};
	//@}

	/** Another factory method: creates an object from an std::istream, reading from
	    it whatever is needed to create the object. Usually, the format for the std::istream will be\\
	    objectType parameter1 parameter2 ... parametern\\
	    If there are problems, an std::exception is raised; it should be caught at the
	    upper level, because it might be something for that level\\
	    At the same time, it catches std::exceptions thrown at a lower level, which will
	    indicate that whatever is in the stream is for this method to process
	    @param _is an stream from where a single line will be read
	    @throw runtime_std::exception if the object type is not known
	*/
	virtual Op<EOT>* make(std::istream& _is)
	{
	    Op<EOT> * opPtr = NULL;
	    try {
		opPtr = Factory<EOT>::make( _is );
	    } catch ( const std::string& objectTypeStr ) {
		if ( objectTypeStr == "BinBitFlip" ) {
		    opPtr = new OneBitFlip<EOT>( );
		} 
		// handles old operator names as well as new ones
		if ( objectTypeStr == "OneBitFlip" ) {
		    opPtr = new OneBitFlip<EOT>( );
		} 

		// Standard BitFilp Mutation
		if ( objectTypeStr == "BinMutation" ) {
		    float rate;
		    _is >> rate;
		    opPtr = new BitMutation<EOT>( rate );
		} 
		if ( objectTypeStr == "BitMutation" ) {
		    float rate;
		    _is >> rate;
		    opPtr = new BitMutation<EOT>( rate );
		} 

		// Bit inversion
		if ( objectTypeStr == "BinInversion" ) {
		    opPtr = new BitInversion<EOT>( );
		}
		if ( objectTypeStr == "BitInversion" ) {
		    opPtr = new BitInversion<EOT>( );
		}

		// Next binary value
		if ( objectTypeStr == "BinNext" ) {
		    opPtr = new BitNext<EOT>( );
		}
		if ( objectTypeStr == "BitNext" ) {
		    opPtr = new BitNext<EOT>( );
		}

		// Previous binary value
		if ( objectTypeStr == "BinPrev" ) {
		    opPtr = new BitPrev<EOT>( );
		}
		if ( objectTypeStr == "BitPrev" ) {
		    opPtr = new BitPrev<EOT>( );
		}

		// 1 point Xover
		if ( objectTypeStr == "BinCrossover" ) {
		    opPtr = new 1PtBitXover<EOT>( );
		}
		if ( objectTypeStr == "1PtBitXover" ) {
		    opPtr = new 1PtBitXover<EOT>( );
		}

		// Npts Xover
		if ( objectTypeStr == "BinNxOver" ) {
		    unsigned nPoints;
		    _is >> nPoints;
		    opPtr = new NPtsBitXover<EOT>( nPoints );
		}
		if ( objectTypeStr == "NPtsBitXover" ) {
		    unsigned nPoints;
		    _is >> nPoints;
		    opPtr = new NPtsBitXover<EOT>( nPoints );
		}

		// Gene Xover (obsolete)
		if ( objectTypeStr == "BinGxOver" ) {
		    unsigned geneSize, nPoints;
		    _is >> geneSize >> nPoints;
		    opPtr = new BitGxOver<EOT>( geneSize, nPoints );
		}
		if ( objectTypeStr == "BitGxOver" ) {
		    unsigned geneSize, nPoints;
		    _is >> geneSize >> nPoints;
		    opPtr = new BitGxOver<EOT>( geneSize, nPoints );
		}

		// Uniform Xover
		if ( objectTypeStr == "BinUxOver" ) {
		    float rate;
		    _is >> rate;
		    opPtr = new UBitXover<EOT>( rate );
		}
		if ( objectTypeStr == "UBitXover" ) {
		    float rate;
		    _is >> rate;
		    opPtr = new UBitXover<EOT>( rate );
		}

		// nothing read!
		if ( !opPtr ) {	// to be caught by the upper level
		    throw objectTypeStr;
		}
	    }
	    return opPtr;
	};

    };

}

#endif
