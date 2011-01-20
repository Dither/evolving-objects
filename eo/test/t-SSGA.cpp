#include <eo>

using namespace eo;

// tests a Steady State GA

// Needed to define this breeder, maybe make it a breeder
template <class EOT>
class BreedOne : public Breed<EOT>
{
public :
  BreedOne(SelectOne<EOT>& _select, GenOp<EOT>& _op) : select(_select), op(_op) {}

  void operator()(const Pop<EOT>& _src, Pop<EOT>& _dest)
  {
    _dest.clear();
    SelectivePopulator<EOT> pop(_src, _dest, select);
    op(pop);
  }

private :
  SelectOne<EOT>& select;
  GenOp<EOT>& op;
};

typedef MinimizingFitness FitnessType;
typedef Vector<FitnessType, unsigned> EoType;

template <class EOT>
class eoMyEval : public EvalFunc<EOT>
{
  public :

  void operator()(EOT& _eo)
  {
    _eo.fitness(*std::max_element(_eo.begin(), _eo.end()));
  }
};

template <class EOT>
class Xover : public BinOp<EOT>
{
  bool operator()(EOT& _eo, const EOT& _eo2)
  {
    unsigned point = rng.random(_eo.size());
    std::copy(_eo2.begin() + point, _eo2.end(), _eo.begin() + point);
    return true;
  }
};

template <class EOT>
class Mutate : public MonOp<EOT>
{
  bool operator()(EOT& _eo)
  {
    unsigned point = rng.random(_eo.size());
    _eo[point] = rng.random(1024);
    return true;
  }
};


int main()
{
  int pop_size = 10;

  GenContinue<EoType> cnt(10);
  CheckPoint<EoType> cp(cnt);


  Xover<EoType> xover;
  Mutate<EoType> mutate;

  ProportionalOp<EoType> opsel;

  opsel.add(xover, 0.8);
  opsel.add(mutate, 0.2);


  DetTournamentSelect<EoType> selector(3);
  BreedOne<EoType> breed(selector, opsel);

  // Replace a single one
  SSGAWorseReplacement<EoType> replace;


//  RandomSelect<EoType> selector;
//  GeneralBreeder<EoType> breed(selector, opsel);
//  PlusReplacement<EoType> replace;


  eoMyEval<EoType> eval;

  EasyEA<EoType> algo(cp, eval, breed, replace);

  UniformGenerator<unsigned> unif(0,1024);
  InitFixedLength<EoType> init(20, unif);

  Pop<EoType> pop(pop_size, init);

  // evaluate
  apply<EoType>(eval, pop);

  BestFitnessStat<EoType>  best("Best_Fitness");
	AverageStat<EoType> avg("Avg_Fitness");
  StdoutMonitor mon;

  cp.add(best);
  cp.add(avg);

//  cp.add(mon);

  mon.add(best);
  mon.add(avg);

  // and run
  algo(pop);

}
