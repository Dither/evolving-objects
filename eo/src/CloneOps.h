// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-
//-----------------------------------------------------------------------------
// CloneOps.h
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
    CVS Info: $Date: 2003-02-27 19:26:09 $ $Header: /home/nojhan/dev/eodev/eodev_cvs/eo/src/CloneOps.h,v 1.2 2003-02-27 19:26:09 okoenig Exp $ $Author: okoenig $ 
 */
//-----------------------------------------------------------------------------

#ifndef _CloneOps_H
#define _CloneOps_H

#include <Op.h>

namespace eo
{

    /**
     * The different null-variation operators (i.e. they do nothing)
     * 
     * QuadCloneOp at least is useful to emulate the standard 
     *               crossover(pCross) + mutation(pMut) 
     *               within the GenOp framework
     * MonCloneOp will probably be useful as the copy operator
     * BinCloneOp will certainly never been used - but let's be complete :-)
     *
     * @addtogroup Core
     * @{
     */

    /**
       Mon clone: one argument
    */
    template <class EOT>
    class MonCloneOp: public MonOp<EOT>
    {
    public:
	/// Ctor
	MonCloneOp() : MonOp<EOT>() {}
	virtual std::string className() const {return "MonCloneOp";}
	virtual bool operator()(EOT&){return false;}
    };


    /** Binary clone: two operands, only the first could be modified
     */
    template<class EOT>
    class BinCloneOp: public BinOp<EOT>
    {
    public:
	/// Ctor
	BinCloneOp() : BinOp<EOT>() {}
	virtual std::string className() const {return "BinCloneOp";}
	virtual bool operator()(EOT&, const EOT&){return false;}
    };

    /** Quad clone: two operands, both could be modified - but are not!
     */
    template<class EOT>
    class QuadCloneOp: public QuadOp<EOT>
    {
    public:
	/// Ctor
	QuadCloneOp():QuadOp<EOT>() {}
	virtual std::string className() const {return "QuadCloneOp";}
	virtual bool operator()(EOT& , EOT& ) {return false;}
    };

}

#endif
/** @} */
