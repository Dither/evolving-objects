/* (c) Marc Schoenauer, Maarten Keijzer and GeNeura Team, 2000

This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your option) any
later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this library; if not, write to the Free Software Foundation, Inc., 59
Temple Place, Suite 330, Boston, MA 02111-1307 USA

Contact: http://eodev.sourceforge.net
         todos@geneura.ugr.es, http://geneura.ugr.es
         Marc.Schoenauer@polytechnique.fr
         mkeijzer@dhi.dk
*/

#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <sstream>
#include <string>

#include "Param.h"
#include "Object.h"
#include "Persistent.h"

namespace eo
{

/** Parameter saving and loading

    ParameterLoader is an abstract class that can be used as a base for your own
    parameter loading and saving. The command line parser Parser is derived from
    this class.

    @ingroup Parameters
*/
    class ParameterLoader
    {
    public :

	/** Need a virtual destructor */
	virtual ~ParameterLoader();

	/** Register a parameter and set its value if it is known

	    @param param      the parameter to process
	    @param section    the section where this parameter belongs
	*/
	virtual void processParam(Param& param, std::string section = "") = 0;

	/**
	 * checks if _param has been actually entered
	 */
	virtual bool isItThere(Param& _param) const = 0;

	/**
	 * Construct a Param and sets its value. The loader will own the memory thus created
	 *
	 * @param _defaultValue       The default value
	 * @param _longName           Long name of the argument
	 * @param _description        Description of the parameter. What is useful for.
	 * @param _shortHand          Short name of the argument (Optional)
	 * @param _section            Name of the section where the parameter belongs
	 * @param _required           If it is a necessary parameter or not
	 */
	template <class ValueType>
	ValueParam<ValueType>& createParam(ValueType _defaultValue,
					     std::string _longName,
					     std::string _description,
					     char _shortHand = 0,
					     std::string _section = "",
					     bool _required = false)
	    {
		ValueParam<ValueType>* p = new ValueParam<ValueType>(_defaultValue,
									 _longName,
									 _description,
									 _shortHand,
									 _required);
		ownedParams.push_back(p);
		processParam(*p, _section);
		return *p;
	    }


    private :

	std::vector<Param*> ownedParams;
    };



/**
   Parser: command line parser and configuration file reader
   This class is persistent, so it can be stored and reloaded to restore
   parameter settings.

   @ingroup Parameters
*/
    class Parser : public ParameterLoader, public Object, public Persistent
    {

    public:

	/**
	 * Constructor
	 * a complete constructor that reads the command line an optionally reads
	 * a configuration file.

	 *
	 * myEo --param-file=param.rc     will then load using the parameter file param.rc
	 *
	 * @param _argc                   command line arguments count
	 * @param _argv                   command line parameters
	 * @param  _programDescription    Description of the work the program does
	 * @param _lFileParamName         Name of the parameter specifying the configuration file (--param-file)
	 * @param _shortHand              Single charachter shorthand for specifying the configuration file
	 */
	Parser ( unsigned _argc, char **_argv , std::string _programDescription = "",
		   std::string _lFileParamName = "param-file", char _shortHand = 'p');

	/**
	   Processes the parameter and puts it in the appropriate section for readability
	*/
	void processParam(Param& param, std::string section = "");

	/** Read from a stream
	 * @param is the input stream
	 */
	void readFrom(std::istream& is);

	/** Pint on a stream
	 * @param os the output stream
	 */
	void printOn(std::ostream& os) const;

	/// className for readibility
	std::string className(void) const { return "Parser"; }

	/// true if the user made an error or asked for help
	bool userNeedsHelp(void);
	/**
	 * Prints an automatic help in the specified output using the information
	 * provided by parameters
	 */
	void printHelp(std::ostream& os);

	std::string ProgramName() { return programName; }

	/** Has param been entered by user?

	    Checks if _param has been actually entered by the user
	*/
	virtual bool isItThere(Param& _param) const
	    { return getValue(_param).first; }

	/**
	 * get a handle on a param from its longName
	 *
	 * if not found, returns 0 (null pointer :-)
	 *
	 * Not very clean (requires hard-coding of the long name twice!)
	 * but very useful in many occasions...
	 */
	Param * getParamWithLongName(const std::string& _name) const;



	/** Get or create parameter

	    It seems finally that the easiest use of the above method is
	    through the following, whose interface is similar to that of the
	    widely-used createParam.

	    For some (probably very stupid) reason, I failed to put it in the
	    .cpp. Any hint???
	*/
	template <class ValueType>
	ValueParam<ValueType>& getORcreateParam(ValueType _defaultValue,
						  std::string _longName,
						  std::string _description,
						  char _shortHand = 0,
						  std::string _section = "",
						  bool _required = false)
	    {
		Param* ptParam = getParamWithLongName(_longName);
		if (ptParam) {
		    // found
		    ValueParam<ValueType>* ptTypedParam(
			dynamic_cast<ValueParam<ValueType>*>(ptParam));
		    return *ptTypedParam;
		} else {
		    // not found -> create it
		    return createParam(_defaultValue, _longName, _description,
				       _shortHand, _section, _required);
		}
	    }



	/** Set parameter value or create parameter

	    This makes sure that the specified parameter has the given value.
	    If the parameter does not exist yet, it is created.

	    This requires that operator<< is defined for ValueType.


	    @param _defaultValue Default value.
	    @param _longName     Long name of the argument.
	    @param _description  Description of the parameter.
	    @param _shortHand    Short name of the argument (Optional)
	    @param _section      Name of the section where the parameter belongs.
	    @param _required     Is the parameter mandatory?
	    @return Corresponding parameter.
	*/
	template <class ValueType>
	ValueParam<ValueType>& setORcreateParam(ValueType _defaultValue,
						  std::string _longName,
						  std::string _description,
						  char _shortHand = 0,
						  std::string _section = "",
						  bool _required = false)
	    {
		ValueParam<ValueType>& param = createParam(_defaultValue, _longName, _description,
							     _shortHand, _section, _required);
		std::ostringstream os;
		os << _defaultValue;
		if(isItThere(param)) {
		    param.setValue(os.str());
		} else {
		    longNameMap[_longName] = os.str();
		    shortNameMap[_shortHand] = os.str();
		}
		return param;
	    }



	/** accessors to the stopOnUnknownParam value */
	void setStopOnUnknownParam(bool _b) {stopOnUnknownParam.value()=_b;}
	bool getStopOnUnknownParam() {return stopOnUnknownParam.value();}

	/** Prefix handling */
	void setPrefix(const std:: string & _prefix) {prefix = _prefix;}

	void resetPrefix() {prefix = "";}

	std::string getPrefix() {return prefix;}

    private:

	void doRegisterParam(Param& param);

	std::pair<bool, std::string> getValue(Param& _param) const;

	void updateParameters();

	typedef std::multimap<std::string, Param*> MultiMapType;

	// used to store all parameters that are processed
	MultiMapType params;

	std::string programName;
	std::string programDescription;

	typedef std::map<char, std::string> ShortNameMapType;
	ShortNameMapType shortNameMap;

	typedef std::map<std::string, std::string> LongNameMapType;
	LongNameMapType longNameMap;

	// flag that marks if the user need to know that there was a problem
	// used to display the message about "-h" only once
	bool needHelpMessage;

	ValueParam<bool>   needHelp;
	ValueParam<bool>   stopOnUnknownParam;

	mutable std::vector<std::string> messages;

	std::string prefix;   // used for all created params - in processParam

    };

}

#endif // PARSER_H

// Local Variables:
// coding: iso-8859-1
// mode:C++
// c-file-style: "Stroustrup"
// comment-column: 35
// fill-column: 80
// End:
