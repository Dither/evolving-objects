// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

//-----------------------------------------------------------------------------
// eoLogger.h
// (c) GeNeura Team, 1998 - EEAAX 1999, Maarten Keijzer 2000
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

    Authors: todos@geneura.ugr.es, http://geneura.ugr.es
             Marc.Schoenauer@polytechnique.fr
             mak@dhi.dk
	     Johann Dréo <nojhan@gmail.com>
	     Caner Candan <caner@candan.fr>
 */
//-----------------------------------------------------------------------------

/** Here's an example explaning how to use eoLogger:

   #include <iostream>
   #include <utils/eoLogger.h>
   #include <utils/eoParserLogger.h>

   int	main(int ac, char** av)
   {
   // We are declaring first an overload of eoParser class using Logger
   // component.
   eoParserLogger	parser(ac, av);

   // This call is important to allow -v parameter to change user level.
   make_verbose(parser);

   // At this time we are switching to warning message and messages
   // which are going to follow it are going to be warnings message too.
   // These messages can be displayed only if the user level (sets with
   // eo::setlevel function) is set to eo::warnings.
   eo::log << eo::warnings;

   // With the following eo::file function we are defining that
   // all future logs are going to this new file resource which is
   // test.txt
   eo::log << eo::file("test.txt") << "In FILE" << std::endl;

   // Now we are changing again the resources destination to cout which
   // is the standard output.
   eo::log << std::cout << "In COUT" << std::endl;

   // Here are 2 differents examples of how to set the errors user level
   // in using either a string or an identifier.
   eo::log << eo::setlevel("errors");
   eo::log << eo::setlevel(eo::errors);

   // Now we are writting a quiet message.
   eo::log << eo::quiet << "1) Must be in quiet mode" << std::endl;

   // And so on...
   eo::log << eo::setlevel(eo::warnings) << eo::warnings << "2) Must be in warnings mode" << std::endl;

   eo::log << eo::setlevel(eo::logging);

   eo::log << eo::errors;
   eo::log << "3) Must be in errors mode";
   eo::log << std::endl;

   eo::log << eo::debug << 4 << ')'
   << " Must be in debug mode\n";

   return 0;
   }

 */

#ifndef eoLogger_h
# define eoLogger_h

# include <map>
# include <vector>
# include <string>
# include <iosfwd>

# include "eoObject.h"

//-----------------------------------------------------------------------------

namespace	eo
{
    /**
     * Levels contents all the available levels in eoLogger
     */
    enum Levels {quiet = 0,
		 errors,
		 warnings,
		 progress,
		 logging,
		 debug};

    /**
     * file
     * this structure combined with the friend operator<< below is an easy way to select a file like output.
     */
    struct	file
    {
	file(const std::string f);
	const std::string	_f;
    };

    /**
     * setlevel
     * this structure combined with the friend operator<< below is an easy way to set a verbose level.
     */
    struct	setlevel
    {
	setlevel(const std::string v);
	setlevel(const Levels lvl);
	const std::string	_v;
	const Levels	_lvl;
    };
}

/**
 * eoLogger
 * Class providing a verbose management through EO
 * Use of a global variable eo::log to easily use the logger like std::cout
 */
class	eoLogger : public eoObject,
		   public std::ostream
{
public:
    eoLogger();
    ~eoLogger();

    virtual std::string	className() const;

    void	addLevel(std::string name, eo::Levels level);
    void	printLevels() const;

private:
    /**
     * outbuf
     * this class is inherits of std::streambuf which is used by eoLogger to write the buffer in an output.
     */
    class	outbuf : public std::streambuf
    {
    public:
	outbuf(const int& fd,
	       const eo::Levels& contexlvl,
	       const eo::Levels& selectedlvl);
    protected:
	virtual int	overflow(int_type c);
    private:
	const int&		_fd;
	const eo::Levels&	_contexLevel;
	const eo::Levels&	_selectedLevel;
    };

private:
    /**
     * MapLevel is the type used by the map member _levels.
     */
    typedef std::map<std::string, eo::Levels>	MapLevel;

public:
    /**
     * operator<< used there to set a verbose mode.
     */
    friend eoLogger&	operator<<(eoLogger&, const eo::Levels);

    /**
     * operator<< used there to set a filename through the class file.
     */
    friend eoLogger&	operator<<(eoLogger&, eo::file);

    /**
     * operator<< used there to set a verbose level through the class setlevel.
     */
    friend eoLogger&	operator<<(eoLogger&, eo::setlevel);

    /**
     * operator<< used there to be able to use std::cout to say that we wish to redirect back the buffer to the standard output.
     */
    friend eoLogger&	operator<<(eoLogger&, std::ostream&);

private:
    /**
     * _selectedLevel is the member storing verbose level setted by the user thanks to operator()
     */
    eo::Levels	_selectedLevel;
    eo::Levels	_contexLevel;

    /**
     * _fd in storing the file descriptor at this place we can disable easily the buffer in
     * changing the value at -1. It is used by operator <<.
     */
    int		_fd;

    /**
     * _obuf std::ostream mandates to use a buffer. _obuf is a outbuf inheriting of std::streambuf.
     */
    outbuf	_obuf;

    /**
     * _levels contains all the existing level order by position
     */
    MapLevel	_levels;

    /**
     * _levelsOrder is just a list to keep the order of levels
     */
    std::vector<std::string>	_sortedLevels;

    /**
     * _standard_io_streams
     */
    std::map< std::ostream*, int >	_standard_io_streams;
};

namespace	eo
{
    /**
     * log is an external global variable defined to easily use a same way than std::cout to write a log.
     */
    extern eoLogger	log;
}

#endif // !eoLogger_h
