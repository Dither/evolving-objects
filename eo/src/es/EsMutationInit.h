// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; fill-column: 80 -*-

//-----------------------------------------------------------------------------
// EsMutationInit.h
// (c) GeNeura Team, 1998 - EEAAX 1999 - Maarten Keijzer 2000
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

#ifndef _EsMutationInit_h
#define _EsMutationInit_h

#include <utils/Parser.h>

namespace eo
{

    /** Initialize Mutation operator

	@ingroup Real
	@ingroup Variators

	Proxy class that is used for initializing the mutation operator. It provides an
	interface between EsMutate and the abstract parameterLoader. It also provides
	the names for the parameters in this class as virtual protected member
	functions.

	If you have more than a single ES in a project that need different names in the
	configuration files, you might consider overriding this class to change the
	names.

	@see EsMutate
    */
    class EsMutationInit
    {
    public :

	/** Constructor

	    @param _parser Parser to read parameters from.
	    @param _section Parser section for \f$\tau\f$-parameters.
	*/
	EsMutationInit(Parser& _parser,
			 std::string _section="ES mutation parameters" ) :
	    parser(_parser), repSection(_section),
	    TauLclParam(0), TauGlbParam(0), TauBetaParam(0) {}

	/** Virtual destructor */
	virtual ~EsMutationInit() {}

	/** local tau */
	double TauLcl(void)
	{
	    if (TauLclParam == 0)
		{
		    TauLclParam = &parser.getORcreateParam(1.0, TauLclName(),
							   "Local Tau (before normalization)",
							   TauLclShort(), section());
		}
	    return TauLclParam->value();
	}

	/** global tau */
	double TauGlb(void)
	{
	    if (TauGlbParam == 0)
		{
		    TauGlbParam = &parser.getORcreateParam(1.0, TauGlbName(),
							   "Global Tau (before normalization)",
							   TauGlbShort(), section());
		}
	    return TauGlbParam->value();
	}

	/** correlation's tau */
	double TauBeta(void)
	{
	    if (TauBetaParam == 0)
		{
		    TauBetaParam = &parser.getORcreateParam(0.0873, TauBetaName(),
							    "Beta", TauBetaShort(), section());
		}
	    return TauBetaParam->value();
	}

    protected :

	virtual std::string section(void)                { return repSection; }

	virtual std::string TauLclName(void) const       { return "TauLoc"; }
	virtual char   TauLclShort(void) const           { return 'l'; }

	virtual std::string TauGlbName(void) const       { return "TauGlob"; }
	virtual char   TauGlbShort(void) const           { return 'g'; }

	virtual std::string TauBetaName(void) const      { return "Beta"; }
	virtual char   TauBetaShort(void) const          { return 'b'; }

    private:

	Parser& parser;
	std::string repSection;
	ValueParam<double>* TauLclParam;
	ValueParam<double>* TauGlbParam;
	ValueParam<double>* TauBetaParam;
    };

}

#endif
