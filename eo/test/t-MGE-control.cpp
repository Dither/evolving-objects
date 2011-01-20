//-----------------------------------------------------------------------------
// t-eoMGE.cpp
//-----------------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef __GNUG__
// to avoid long name warnings
#pragma warning(disable:4786)
#endif // __GNUG__

#include "eo"
#include "ga/BitOp.h"

#include "RoyalRoad.h"

// Viri
#include "VirusOp.h"
#include "eoVirus.h"
#include "eoInitVirus.h"

using namespace eo;

//-----------------------------------------------------------------------------

typedef eoVirus<float> Chrom;

//-----------------------------------------------------------------------------

int main()
{
  const unsigned POP_SIZE = 10, CHROM_SIZE = 12;
  unsigned i;
  BooleanGenerator gen;

  // the populations:
  Pop<Chrom> pop;

  // Evaluation
  RoyalRoad<Chrom> rr( 8 );
  EvalFuncCounter<Chrom> eval( rr );

  eoInitVirus<float> random(CHROM_SIZE, gen);
  for (i = 0; i < POP_SIZE; ++i) {
      Chrom chrom;
      random(chrom);
      eval(chrom);
      pop.push_back(chrom);
  }

  std::cout << "population:" << std::endl;
  for (i = 0; i < pop.size(); ++i)
    std::cout << "\t" << pop[i] << " " << pop[i].fitness() << std::endl;

  // selection
  StochTournamentSelect<Chrom> lottery(0.9 );

  // breeder
  OneBitFlip<Chrom> vm;
  UBitXover<Chrom> xover;
  ProportionalOp<Chrom> propSel;
  GeneralBreeder<Chrom> breeder( lottery, propSel );
  propSel.add(vm, 0.2);
  propSel.add(xover, 0.8);

  // Replace a single one
  CommaReplacement<Chrom> replace;

  // Terminators
  GenContinue<Chrom> continuator1(10);
  FitContinue<Chrom> continuator2(CHROM_SIZE);
  CombinedContinue<Chrom> continuator(continuator1, continuator2);
  CheckPoint<Chrom> checkpoint(continuator);
  StdoutMonitor monitor;
  checkpoint.add(monitor);
  SecondMomentStats<Chrom> stats;
  PopStat<Chrom> dumper( 10 );
  monitor.add(stats);
  checkpoint.add(dumper);
  checkpoint.add(stats);

  // GA generation
  EasyEA<Chrom> ea(checkpoint, eval,  breeder, replace );

  // evolution
  try
    {
      ea(pop);
    }
  catch (std::exception& e)
    {
	std::cout << "exception: " << e.what() << std::endl;;
	exit(EXIT_FAILURE);
    }

  std::cout << "pop" << std::endl;
  for (i = 0; i < pop.size(); ++i)
    std::cout << "\t" <<  pop[i] << " " << pop[i].fitness() << std::endl;

  std::cout << "\n --> Number of Evaluations = " << eval.getValue() << std::endl;
  return 0;
}

//-----------------------------------------------------------------------------
