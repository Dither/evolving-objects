// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// GeneralBreeder.h 
// (c) Maarten Keijzer and Marc Schoenauer, 2001
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

    Contact: mkeijzer@dhi.dk
             Marc.Schoenauer@polytechnique.fr
 */
//-----------------------------------------------------------------------------

#ifndef GeneralBreeder_h
#define GeneralBreeder_h

//-----------------------------------------------------------------------------

/*****************************************************************************
 * GeneralBreeder: transforms a population using the generalOp construct.
 *****************************************************************************/

#include <Op.h>
#include <GenOp.h>
#include <Populator.h>
#include <SelectOne.h>
#include <Breed.h>
#include <utils/HowMany.h>

namespace eo
{

    /**
       Base class for breeders using generalized operators.

       @ingroup Combination
    */
    template<class EOT>
    class GeneralBreeder: public Breed<EOT>
    {
    public:
	/** Ctor:
	 *
	 * @param _select a selectoOne, to be used for all selections
	 * @param _op a general operator (will generally be an OpContainer)
	 * @param _rate               pour howMany, le nbre d'enfants a generer
	 * @param _interpret_as_rate  <a href="../../tutorial/html/eoEngine.html#howmany">explanation</a>
	 */
	GeneralBreeder(
			 SelectOne<EOT>& _select,
			 GenOp<EOT>& _op,
			 double  _rate=1.0,
			 bool _interpret_as_rate = true) :
	    select( _select ), op(_op),  howMany(_rate, _interpret_as_rate) {}

	/** Ctor:
	 *
	 * @param _select a selectoOne, to be used for all selections
	 * @param _op a general operator (will generally be an OpContainer)
	 * @param _howMany an HowMany <a href="../../tutorial/html/eoEngine.html#howmany">explanation</a>
	 */
	GeneralBreeder(
			 SelectOne<EOT>& _select,
			 GenOp<EOT>& _op,
			 HowMany _howMany ) :
	    select( _select ), op(_op),  howMany(_howMany) {}

	/** The breeder: simply calls the genOp on a selective populator!
	 *
	 * @param _parents the initial population
	 * @param _offspring the resulting population (content -if any- is lost)
	 */
	void operator()(const Pop<EOT>& _parents, Pop<EOT>& _offspring)
	{
	    unsigned target = howMany(_parents.size());

	    _offspring.clear();
	    SelectivePopulator<EOT> it(_parents, _offspring, select);

	    while (_offspring.size() < target)
		{
		    op(it);
		    ++it;
		}

	    _offspring.resize(target);   // you might have generated a few more
	}

	/// The class name.
	virtual std::string className() const { return "GeneralBreeder"; }

    private:
	SelectOne<EOT>& select;
	GenOp<EOT>& op;
	HowMany howMany;
    };

}

#endif
