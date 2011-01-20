//-----------------------------------------------------------------------------
// t-ShiftMutation.cpp
//-----------------------------------------------------------------------------

#include <eo>

#include <Int.h>
#include <ShiftMutation.h>
#include <set>

//-----------------------------------------------------------------------------

typedef Int<double> Chrom;

//-----------------------------------------------------------------------------


double real_value(const Chrom & _chrom)
{
  double sum = 0;
  for (unsigned i = 0; i < _chrom.size(); i++)
      sum += _chrom[i];
  return sum/_chrom.size();
}

// Return true if the given chromosome corresponds to a permutation
bool check_permutation(const Chrom& _chrom){
	unsigned size= _chrom.size();
	std::set<unsigned> verif;
	for(unsigned i=0; i< size; i++){
		if(verif.insert(_chrom[i]).second==false){
			std::cout << " Error: Wrong permutation !" << std::endl;
			std::string s;
			s.append( " Wrong permutation in t-ShiftMutation");
			throw std::runtime_error( s );
			return false;
		}
	}
	return true;
}


int main()
{
  const unsigned POP_SIZE = 3, CHROM_SIZE = 8;
  unsigned i;

   // a chromosome randomizer
  InitPermutation <Chrom> random(CHROM_SIZE);
  
   // the population: 
  Pop<Chrom> pop;
	
  // Evaluation
  EvalFuncPtr<Chrom> eval(  real_value );
 
  for (i = 0; i < POP_SIZE; ++i)
    {
      Chrom chrom(CHROM_SIZE);
      random(chrom);
      eval(chrom);
      pop.push_back(chrom);
    }
       
  // a shift mutation
  ShiftMutation <Chrom> shift;
  
  for (i = 0; i < POP_SIZE; ++i)
    {
      std::cout << " Initial chromosome n°" << i << " : " << pop[i] << "..." <<  std::endl;
      shift(pop[i]);
      std::cout << " ... becomes : " << pop[i] << " after shift mutation" << std::endl;
      check_permutation(pop[i]);
    }
   
  return 0;
}

//-----------------------------------------------------------------------------
