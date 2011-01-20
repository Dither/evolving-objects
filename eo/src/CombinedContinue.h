// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// CombinedContinue.h
// (c) Maarten Keijzer, GeNeura Team, 1999, 2000
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

#ifndef _CombinedContinue_h
#define _CombinedContinue_h

#include <Continue.h>

namespace eo
{

    /** 
	Combined continuators - logical AND:
	Continues until one of the embedded continuators says halt!

	20/11/00 MS: Changed the 2-continuator construct to a std::vector<Continue<EOT> >
	to be consistent with other Combined constructs
	and allow to easily handle more than 2 continuators

	02/2003 Ramón Casero Cañas - added the removeLast() method 

	@ingroup Combination
    */
    template< class EOT>
    class CombinedContinue: public Continue<EOT> {
    public:

	/// Define Fitness
	typedef typename EOT::Fitness FitnessType;

	/// Ctor, make sure that at least on continuator is present
	CombinedContinue( Continue<EOT>& _cont)
	    : Continue<EOT> ()
	{
	    continuators.push_back(&_cont);
	}

	/// Ctor - for historical reasons ... should disspear some day
	CombinedContinue( Continue<EOT>& _cont1, Continue<EOT>& _cont2)
	    : Continue<EOT> ()
	{
	    continuators.push_back(&_cont1);
	    continuators.push_back(&_cont2);
	}

	void add(Continue<EOT> & _cont)
	{
	    continuators.push_back(&_cont);
	}

	///////////// RAMON'S CODE ///////////////
	void removeLast(void)
	{
	    continuators.pop_back();
	}
	///////////// RAMON'S CODE (end) ///////////////


	/** Returns false when one of the embedded continuators say so (logical and)
	 */
	virtual bool operator() ( const Pop<EOT>& _pop )
	{
	    for (unsigned i = 0; i < continuators.size(); ++i)
		if ( !(*continuators[i])(_pop) ) return false;
	    return true;
	}

	virtual std::string className(void) const { return "CombinedContinue"; }

    private:
	std::vector<Continue<EOT>*>    continuators;
    };

}

#endif

