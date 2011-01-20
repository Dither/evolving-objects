#ifndef __GNUG__
// to avoid long name warnings
#pragma warning(disable:4786)
#endif // __GNUG__

#include <eo>

#include "binary_value.h"

typedef Bin<float> Chrom;

main()
{
  const unsigned POP_SIZE = 8, CHROM_SIZE = 16;
  unsigned i;

// a chromosome randomizer
  BinRandom<Chrom> random;
// the populations: 
  Pop<Chrom> pop; 

   // Evaluation
  EvalFuncPtr<Chrom> eval(  binary_value );
 
  for (i = 0; i < POP_SIZE; ++i)
    {
      Chrom chrom(CHROM_SIZE);
      random(chrom);
      eval(chrom);
      pop.push_back(chrom);
    }
  
  std::cout << "population:" << std::endl;
  for (i = 0; i < pop.size(); ++i)
    std::cout << "\t" << pop[i] << " " << pop[i].fitness() << std::endl;

  
  // selection
  eoLottery<Chrom> lottery;

  // breeder
  BinBitFlip<Chrom> bitflip;
  BinCrossover<Chrom> xover;
  ProportionalOpSel<Chrom> propSel;
  Breeder<Chrom> breeder( propSel );
  propSel.addOp(bitflip, 0.25);
  propSel.addOp(xover, 0.75);
  
  // replacement
  eoInclusion<Chrom> inclusion;

  // Terminators
  eoFitTerm<Chrom> term( pow(2.0, CHROM_SIZE), 1 );

  // GA generation
  EasyEA<Chrom> ea(lottery, breeder, inclusion, eval, term);

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
  
  return 0;
}

