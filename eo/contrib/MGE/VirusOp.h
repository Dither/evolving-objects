/*
   ViruOp.h
   (c) GeNeura Team 2001, Marc Schoenauer 2000

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
             Marc.Schoenauer@polytechnique.fr
CVS Info: $Date: 2001-05-17 10:08:25 $ $Header: /home/nojhan/dev/eodev/eodev_cvs/eo/contrib/MGE/VirusOp.h,v 1.1 2001-05-17 10:08:25 jmerelo Exp $ $Author: jmerelo $
*/

#ifndef VirusOp_h
#define VirusOp_h

//-----------------------------------------------------------------------------

#include <iostream>    // ostream, istream
#include <functional>  // bind2nd
#include <string>      // string

#include <utils/eoRNG.h>
#include <MGE/eoVirus.h>

/** VirusBitFlip --> changes 1 bit
*/

template<class FitT>
class VirusBitFlip: public eoMonOp<eoVirus<FitT> > {
 public:
  /// The class name.
  virtual string className() const { return "VirusBitFlip"; };

  /**
   * Change one bit.
   * @param chrom The cromosome which one bit is going to be changed.
   */
  bool operator()(eoVirus<FitT>& _chrom) {
      unsigned i = eo::rng.random(_chrom.size());
      _chrom.virusBitSet(i, _chrom.virusBit(i) ? false : true );
      return true;
  }
};

template<class FitT>
class VirusMutation: public eoMonOp<eoVirus<FitT> > {
 public:
  /// The class name.
  virtual string className() const { return "VirusMutation"; };

  /**
   * Change one bit.
   * @param chrom The cromosome which one bit is going to be changed.
   */
  bool operator()(eoVirus<FitT>& _chrom) {
	// Search for virus bits
	vector<unsigned> bitsSet;
	for ( unsigned i = 0; i < _chrom.size(); i ++ ) {
	  if ( _chrom.virusBit(i) ) {
		bitsSet.push_back( i );
	  }
	}
	if ( !bitsSet.size() ) {
	  return false;
	}
    unsigned flipSite = eo::rng.random(bitsSet.size());
	unsigned flipValue = bitsSet[ flipSite ];
	_chrom[flipValue] = _chrom[flipValue] ? false : true;
	return true;
  }
};

/// Works for 1-bit virus; shifts the one to the right or left
template<class FitT>
class VirusShiftMutation: public eoMonOp<eoVirus<FitT> > {
 public:

  /// Ctor
  VirusShiftMutation( ) {};

  /// The class name.
  virtual string className() const { return "VirusShiftMutation"; };

  /**
   * Change one bit.
   * @param chrom The cromosome which one bit is going to be changed.
   */
  bool operator()(eoVirus<FitT>& _chrom) {
	// Search for virus bits
	eoBooleanGenerator gen;
	for ( unsigned i = 0; i < _chrom.size(); i ++ ) {
	  if ( _chrom.virusBit(i) ) {
		if ( gen() ) {
		  if ( i + 1 < _chrom.size() ) {
			_chrom.virusBitSet(i+1,true);
			_chrom.virusBitSet(i, false);
		  }
		} else {
		  if ( i - 1 > 0 ) {
			_chrom.virusBitSet(i-1,true);
			_chrom.virusBitSet(i, false);
		  }
		}
	  }
	}
	return true;
  }

 private:
};


template<class FitT>
class VirusTransmission: public eoBinOp<eoVirus<FitT> > {
 public:
  /// The class name.
  virtual string className() const { return "VirusTransmission"; };

  /**
   * Change one bit.
   * @param _chrom The "receptor" chromosome
   * @param _chrom2 The "donor" chromosome
   */
  bool operator()(eoVirus<FitT>& _chrom,const eoVirus<FitT>& _chrom2) {
	// Search for virus bits
	for ( unsigned i = 0; i < _chrom.size(); i ++ ) {
	  _chrom.virusBitSet(i, _chrom2.virusBit(i) );
	}
	return true;
  }
};

#endif //VirusOp_h