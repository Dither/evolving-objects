/* -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

    t-eobin.cpp
      This program tests the the binary cromosomes and several genetic operators
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
//-----------------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>   // std::cout
#include <sstream>

#include <eo>         // general EO
#include <ga.h>	      // bitstring representation & operators
#include <utils/RndGenerators.h>
#include "binary_value.h"

using namespace eo;

//-----------------------------------------------------------------------------

typedef Bit<double> Chrom;

//-----------------------------------------------------------------------------

void main_function()
{
  const unsigned SIZE = 8;
  unsigned i, j;
  BooleanGenerator gen;

  Chrom chrom(SIZE), chrom2;
  chrom.fitness(binary_value(chrom)); chrom2.fitness(binary_value(chrom2));

  std::cout << "chrom:  " << chrom << std::endl;
  chrom[0] = chrom[SIZE - 1] = true; chrom.fitness(binary_value(chrom));
  std::cout << "chrom:  " << chrom << std::endl;
  chrom[0] = chrom[SIZE - 1] = false; chrom.fitness(binary_value(chrom));
  std::cout << "chrom:  " << chrom << std::endl;
  chrom[0] = chrom[SIZE - 1] = true; chrom.fitness(binary_value(chrom));

  std::cout << "chrom.className() = " << chrom.className() << std::endl;

  std::cout << "chrom:  " << chrom << std::endl
       << "chrom2: " << chrom2 << std::endl;

  std::ostringstream os;
  os << chrom;
  std::istringstream is(os.str());
  is >> chrom2; chrom.fitness(binary_value(chrom2));

  std::cout << "\nTesting reading, writing\n";
  std::cout << "chrom:  " << chrom << "\nchrom2: " << chrom2 << '\n';

  std::fill(chrom.begin(), chrom.end(), false);
  std::cout << "--------------------------------------------------"
       << std::endl << "MonOp's aplied to .......... " << chrom << std::endl;

  InitFixedLength<Chrom>
      random(chrom.size(), gen);

  random(chrom); chrom.fitness(binary_value(chrom));
  std::cout << "after BinRandom ............ " << chrom << std::endl;

  OneBitFlip<Chrom> bitflip;
  bitflip(chrom); chrom.fitness(binary_value(chrom));
  std::cout << "after BitFlip .............. " << chrom << std::endl;

  BitMutation<Chrom> mutation(0.5);
  mutation(chrom); chrom.fitness(binary_value(chrom));
  std::cout << "after BinMutation(0.5) ..... " << chrom << std::endl;

  BitInversion<Chrom> inversion;
  inversion(chrom); chrom.fitness(binary_value(chrom));
  std::cout << "after BinInversion ......... " << chrom << std::endl;

  BitNext<Chrom> next;
  next(chrom); chrom.fitness(binary_value(chrom));
  std::cout << "after BinNext .............. " << chrom << std::endl;

  BitPrev<Chrom> prev;
  prev(chrom); chrom.fitness(binary_value(chrom));
  std::cout << "after BinPrev .............. " << chrom << std::endl;

  std::fill(chrom.begin(), chrom.end(), false); chrom.fitness(binary_value(chrom));
  std::fill(chrom2.begin(), chrom2.end(), true); chrom2.fitness(binary_value(chrom2));
  std::cout << "--------------------------------------------------"
       << std::endl << "BinOp's aplied to ... "
       << chrom << " " << chrom2 << std::endl;

  OnePtBitXover<Chrom> xover;
  std::fill(chrom.begin(), chrom.end(), false);
  std::fill(chrom2.begin(), chrom2.end(), true);
  xover(chrom, chrom2);
  chrom.fitness(binary_value(chrom)); chrom2.fitness(binary_value(chrom2));
  std::cout << "BinCrossover ........ " << chrom << " " << chrom2 << std::endl;

  for (i = 1; i < SIZE; i++)
    {
      NPtsBitXover<Chrom> nxover(i);
      std::fill(chrom.begin(), chrom.end(), false);
      std::fill(chrom2.begin(), chrom2.end(), true);
      nxover(chrom, chrom2);
      chrom.fitness(binary_value(chrom)); chrom2.fitness(binary_value(chrom2));
      std::cout << "BinNxOver(" << i << ") ........ "
	   << chrom << " " << chrom2 << std::endl;
    }

  for (i = 1; i < SIZE / 2; i++)
    for (j = 1; j < SIZE / 2; j++)
      {
	BitGxOver<Chrom> gxover(i, j);
	std::fill(chrom.begin(), chrom.end(), false);
	std::fill(chrom2.begin(), chrom2.end(), true);
	gxover(chrom, chrom2);
	chrom.fitness(binary_value(chrom)); chrom2.fitness(binary_value(chrom2));
	std::cout  << "BinGxOver(" << i << ", " << j << ") ..... "
	      << chrom << " " << chrom2 << std::endl;
      }

    // test SGA algorithm
    GenContinue<Chrom> continuator1(50);
    FitContinue<Chrom> continuator2(65535.f);

    CombinedContinue<Chrom> continuator(continuator1, continuator2);

    CheckPoint<Chrom> checkpoint(continuator);

    StdoutMonitor monitor;

    checkpoint.add(monitor);

    SecondMomentStats<Chrom> stats;

    monitor.add(stats);
    checkpoint.add(stats);

    ProportionalSelect<Chrom> select;
    EvalFuncPtr<Chrom>  eval(binary_value);

    SGA<Chrom> sga(select, xover, 0.8f, bitflip, 0.1f, eval, checkpoint);

    InitFixedLength<Chrom> init(16, gen);
    Pop<Chrom> pop(100, init);

    apply<Chrom>(eval, pop);

    sga(pop);

    pop.sort();

    std::cout << "Population " << pop << std::endl;

    std::cout << "\nBest: " << pop[0].fitness() << '\n';

  /*

    Commented this out, waiting for a definite decision what to do with the mOp's

    // Check multiOps
    eoMultiMonOp<Chrom> mOp( &next );
    mOp.adOp( &bitflip );
    std::cout << "before multiMonOp............  " << chrom << std::endl;
    mOp( chrom );
    std::cout << "after multiMonOp .............. " << chrom << std::endl;

    BinGxOver<Chrom> gxover(2, 4);
    eoMultiBinOp<Chrom> mbOp( &gxover );
    mOp.adOp( &bitflip );
    std::cout << "before multiBinOp............  " << chrom << " " << chrom2 << std::endl;
    mbOp( chrom, chrom2 );
    std::cout << "after multiBinOp .............. " << chrom << " " << chrom2 <<std::endl;
  */
}

//-----------------------------------------------------------------------------
// For MSVC memory lead detection
#ifdef _MSC_VER
#include <crtdbg.h>
#endif

int main()
{
#ifdef _MSC_VER
  //  rng.reseed(42);
    int flag = _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
     flag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flag);
//   _CrtSetBreakAlloc(100);
#endif

    try
    {
	main_function();
    }
    catch(std::exception& e)
    {
	std::cout << "Exception: " << e.what() << '\n';
    }

}
