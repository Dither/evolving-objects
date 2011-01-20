/* -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

    t-Random.cpp
      Test program for random generator

    (c) GeNeura Team, 1999

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact: todos@geneura.ugr.es, http://geneura.ugr.es

*/

/**
CVS Info: $Date: 2003-02-27 19:20:24 $  $Author: okoenig $ $Revision: 1.13 $
*/

//-----------------------------------------------------------------------------

#include <iostream>   // cout
#include <fstream>  // ostrstream, istrstream
#include <utils/RndGenerators.h>         // Bin
//#include <Normal.h>
//#include <NegExp.h>

//-----------------------------------------------------------------------------

using namespace eo;

int main() {
  UniformGenerator<float> u1(-2.5,3.5);
  UniformGenerator<double> u2(0.003, 0.05 );
  UniformGenerator<unsigned long> u3( 10000U, 10000000U);

  try
  { // throws an error
    UniformGenerator<unsigned long> utest( 10000000U, 10000U);
    throw; // if this succeeds something is wrong, make sure that that is noticed
  }
  catch (std::logic_error& e)
  {
    std::cout << e.what() << std::endl;
  }

  std::ofstream os("t-Random.out");

  for ( unsigned i = 0; i < 100; i ++)
  {
    os << u1() << "\t" << u2() << "\t" << u3() << std::endl;
  }

  return 0; // to avoid VC++ complaints

}

//-----------------------------------------------------------------------------
