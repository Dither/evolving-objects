// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// InvalidateOps.h
// (c) Maarten Keijzer 2001
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
             Marc.Schoenauer@polytechnique.fr
             mak@dhi.dk
 */
//-----------------------------------------------------------------------------

#ifndef _InvalidateOps_h
#define _InvalidateOps_h

#include <Op.h>

namespace eo
{

    /** @addtogroup Utilities

	One of the invalidator operators. Use this one as a 'hat' on an operator
	that is defined to work on a generic datatype. This functor will then check
	the return type of the operator and invalidate the fitness of the individual.

	This functor is used in algorithms that work with straight MonOp, BinOp
	or QuadOp operators, for instance SGA. Note that GenOp derived operators
	generally do invalidate the fitness of the objects they have changed.

	Return value means "Has_Changed" and not "Needs_To_Be_Invalidated"
	as successive invalidation are not really a problem
    */

    template <class EOT>
    class InvalidateMonOp : public MonOp<EOT>
    {
    public:
	InvalidateMonOp(MonOp<EOT>& _op) : op(_op) {}

	bool operator()(EOT& _eo)
	{
	    if (op(_eo))
		{
		    _eo.invalidate();
		    return true;
		}

	    return false; 
	}

    private:
	MonOp<EOT>& op;
    };

    /**
       One of the invalidator operators. Use this one as a 'hat' on an operator
       that is defined to work on a generic datatype. This functor will then check
       the return type of the operator and invalidate the fitness of the individual.

       This functor is used in algorithms that work with straight MonOp, BinOp
       or QuadOp operators, for instance SGA. Note that GenOp derived operators
       generally do invalidate the fitness of the objects they have changed.

       Return value means "Has_Changed" and not "Needs_To_Be_Invalidated"
       as successive invalidation are not really a problem
    */

    template <class EOT>
    class InvalidateBinOp : public BinOp<EOT>
    {
    public:
	InvalidateBinOp(BinOp<EOT>& _op) : op(_op) {}

	bool operator()(EOT& _eo, const EOT& _eo2)
	{
	    if (op(_eo, _eo2))
		{
		    _eo.invalidate();
		    return true;
		}

	    return false;
	}

    private:
	BinOp<EOT>& op;
    };

    /**
       One of the invalidator operators. Use this one as a 'hat' on an operator
       that is defined to work on a generic datatype. This functor will then check
       the return type of the operator and invalidate the fitness of the individual.

       This functor is used in algorithms that work with straight MonOp, BinOp
       or QuadOp operators, for instance SGA. Note that GenOp derived operators
       generally do invalidate the fitness of the objects they have changed.

       Return value means "Has_Changed" and not "Needs_To_Be_Invalidated"
       as successive invalidation are not really a problem
    */

    template <class EOT>
    class InvalidateQuadOp : public QuadOp<EOT>
    {
    public:
	InvalidateQuadOp(QuadOp<EOT>& _op) : op(_op) {}

	bool operator()(EOT& _eo1, EOT& _eo2)
	{
	    if (op(_eo1, _eo2))
		{
		    _eo1.invalidate();
		    _eo2.invalidate();
		    return true;
		}
	    return false;
	}

    private:
	QuadOp<EOT>& op;
    };

}

#endif
