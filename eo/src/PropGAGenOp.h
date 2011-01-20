// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// PropGAGenOp.h
// (c) Marc.Schoenauer 2005
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
             Marc.Schoenauer@inria.fr
             mak@dhi.dk
 */
//-----------------------------------------------------------------------------

#ifndef _PropGAGenOp_h
#define _PropGAGenOp_h

#include "GenOp.h"
#include "InvalidateOps.h"

namespace eo
{

    ///////////////////////////////////////////////////////////////////////////////
    // class SGAGenOp
    ///////////////////////////////////////////////////////////////////////////////

    /** 
     * PropGAGenOp (for Simple GA, but Proportional) 
     * choice between Crossover, mutation or cloining 
     * with respect to given relatve weights
     *
     * @ingroup Combination
     */
    template<class EOT> 
    class PropGAGenOp : public GenOp<EOT>
    {
    public:
    
	/** Ctor from 
	 *   * weight of clone
	 *   * crossover (with weight)
	 *   * mutation (with weight)
	 */
	PropGAGenOp(double _wClone, QuadOp<EOT>& _cross, double _wCross, 
		      MonOp<EOT>& _mut, double _wMut)
	    : wClone(_wClone),
	      cross(_cross),
	      wCross(_wCross),
	      mut(_mut), 
	      wMut(_wMut) 
	{
	    propOp.add(cross, wCross); // the crossover - with weight wCross
	    propOp.add(mut, wMut); // mutation with weight wMut
	    propOp.add(monClone, wClone);
	}
  
	/** do the job: delegate to op */
	virtual void apply(Populator<EOT>& _pop)
	{
	    propOp.apply(_pop);
	}

	/** inherited from GenOp */
	virtual unsigned max_production(void) {return 2;}

	virtual std::string className() const {return "PropGAGenOp";}


    private:
	double wClone;
	QuadOp<EOT> &cross;   // InvalidateXXX take the boolean output
	double wCross;
	MonOp<EOT> & mut;      // of the XXX op and invalidate the EOT
	double wMut;
	ProportionalOp<EOT> propOp;
	MonCloneOp<EOT> monClone;
    };

}

#endif
