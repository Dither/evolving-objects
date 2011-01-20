//-----------------------------------------------------------------------------

// to avoid long name warnings
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <stdexcept>  // runtime_error 

//-----------------------------------------------------------------------------
// tt.cpp: 
//
//-----------------------------------------------------------------------------


// general
#include <eo>
//-----------------------------------------------------------------------------

struct Dummy : public EO<double>
{
    typedef double Type;
  void printOn(std::ostream & _os) const
  {
      _os << " - ";
      EO<double>::printOn(_os);
  }
};


struct DummyPop : public Pop<Dummy>
{
public :
    DummyPop(int s=0) { resize(s); }
};

//-----------------------------------------------------------------------------

int the_main(int argc, char **argv)
{ 
  Parser parser(argc, argv);
  ValueParam<unsigned int> parentSizeParam(10, "parentSize", "Parent size",'P');
  parser.processParam( parentSizeParam );
    unsigned int pSize = parentSizeParam.value();

  ValueParam<unsigned int> offsrpringSizeParam(10, "offsrpringSize", "Offsrpring size",'O');
  parser.processParam( offsrpringSizeParam );
    unsigned int oSize = offsrpringSizeParam.value();

  ValueParam<unsigned int> tournamentSizeParam(2, "tournamentSize", "Deterministic tournament size",'T');
  parser.processParam( tournamentSizeParam );
    unsigned int tSize = tournamentSizeParam.value();

  ValueParam<double> tournamentRateParam(0.75, "tournamentRate", "Stochastic tournament rate",'R');
  parser.processParam( tournamentRateParam );
    double tRate = tournamentRateParam.value();

  ValueParam<double> sParentsElitismRateParam(0.1, "sParentsElitismRateParam", "Strong elitism rate for parents",'E');
  parser.processParam( sParentsElitismRateParam );
    double sParentsElitismRate = sParentsElitismRateParam.value();

  ValueParam<double> sParentsEugenismRateParam(0, "sParentsEugenismRateParam", "Strong Eugenism rate",'e');
  parser.processParam( sParentsEugenismRateParam );
    double sParentsEugenismRate = sParentsEugenismRateParam.value();

  ValueParam<double> sOffspringElitismRateParam(0, "sOffspringElitismRateParam", "Strong elitism rate for parents",'E');
  parser.processParam( sOffspringElitismRateParam );
    double sOffspringElitismRate = sOffspringElitismRateParam.value();

  ValueParam<double> sOffspringEugenismRateParam(0, "sOffspringEugenismRateParam", "Strong Eugenism rate",'e');
  parser.processParam( sOffspringEugenismRateParam );
    double sOffspringEugenismRate = sOffspringEugenismRateParam.value();

    if (parser.userNeedsHelp())
      {
        parser.printHelp(std::cout);
        exit(1);
      }

    unsigned i;

    std::cout << "Testing the replacements\nParents SIze = " << pSize 
	 << " and offspring size = " << oSize << std::endl;

    rng.reseed(42);


    DummyPop orgParents(pSize);
    DummyPop orgOffspring(oSize);

    // initialize so we can recognize them later!
    for (i=0; i<pSize; i++)
      orgParents[i].fitness(2*i+1);
    for (i=0; i<oSize; i++)
      orgOffspring[i].fitness(2*i);

std::cout << "Initial parents (odd)\n" << orgParents << "\n And initial offsprings (even)\n" << orgOffspring << std::endl;

    // now the ones we're going to play with
    DummyPop parents(0);
    DummyPop offspring(0);

// the replacement procedures under test
    GenerationalReplacement<Dummy> genReplace;
    PlusReplacement<Dummy> plusReplace;
    EPReplacement<Dummy> epReplace(tSize);
    CommaReplacement<Dummy> commaReplace;
    WeakElitistReplacement<Dummy> weakElitistReplace(commaReplace);
    // the SSGA replacements
    SSGAWorseReplacement<Dummy> ssgaWorseReplace;
    SSGADetTournamentReplacement<Dummy> ssgaDTReplace(tSize);
    SSGAStochTournamentReplacement<Dummy> ssgaDSReplace(tRate);

    // here we go
    // Generational
    parents = orgParents;
    offspring = orgOffspring;

    std::cout << "GenerationalReplacement\n";
    std::cout << "=========================\n";
    genReplace(parents, offspring);
std::cout << "Parents (originally odd)\n" << parents << "\n And offsprings (orogonally even\n" << offspring << std::endl;

    // Plus
    parents = orgParents;
    offspring = orgOffspring;

    std::cout << "PlusReplacement\n";
    std::cout << "=================\n";
    plusReplace(parents, offspring);
std::cout << "Parents (originally odd)\n" << parents << "\n And offsprings (originally even)\n" << offspring << std::endl;

    // EP (proche d'un PLUS
    parents = orgParents;
    offspring = orgOffspring;

    std::cout << "EPReplacement\n";
    std::cout << "===============\n";
    epReplace(parents, offspring);
std::cout << "Parents (originally odd)\n" << parents << "\n And offsprings (originally even)\n" << offspring << std::endl;

    // Comma
    parents = orgParents;
    offspring = orgOffspring;

    if (parents.size() > offspring.size() )
	std::cout << "Skipping Comma Replacement, more parents than offspring\n";
    else
      {
	std::cout << "CommaReplacement\n";
	std::cout << "==================\n";
	commaReplace(parents, offspring);
	std::cout << "Parents (originally odd)\n" << parents << "\n And offsprings (originally even)\n" << offspring << std::endl;

	// Comma with weak elitism
	parents = orgParents;
	offspring = orgOffspring;

	std::cout << "The same, with WEAK elitism\n";
	std::cout << "===========================\n";
	weakElitistReplace(parents, offspring);
	std::cout << "Parents (originally odd)\n" << parents << "\n And offsprings (originally even)\n" << offspring << std::endl;
      }

	// preparing SSGA replace worse
	parents = orgParents;
	offspring = orgOffspring;

    if (parents.size() < offspring.size() )
	std::cout << "Skipping all SSGA Replacements, more offspring than parents\n";
    else
      {
	std::cout << "SSGA replace worse\n";
	std::cout << "==================\n";
	ssgaWorseReplace(parents, offspring);
	std::cout << "Parents (originally odd)\n" << parents << "\n And offsprings (originally even)\n" << offspring << std::endl;

    // SSGA deterministic tournament
	parents = orgParents;
	offspring = orgOffspring;

	std::cout << "SSGA deterministic tournament\n";
	std::cout << "=============================\n";
	ssgaDTReplace(parents, offspring);
	std::cout << "Parents (originally odd)\n" << parents << "\n And offsprings (originally even)\n" << offspring << std::endl;

    // SSGA stochastic tournament
	parents = orgParents;
	offspring = orgOffspring;

	std::cout << "SSGA stochastic tournament\n";
	std::cout << "==========================\n";
	ssgaDTReplace(parents, offspring);
	std::cout << "Parents (originally odd)\n" << parents << "\n And offsprings (originally even)\n" << offspring << std::endl;
      }

    // the general replacement
    DeterministicSaDReplacement<Dummy> sAdReplace(sParentsElitismRate, sParentsEugenismRate, sOffspringElitismRate, sOffspringEugenismRate);// 10% parents survive

    parents = orgParents;
    offspring = orgOffspring;

    std::cout << "General - strong elitism\n";
    std::cout << "========================\n";
    sAdReplace(parents, offspring);
    std::cout << "Parents (originally odd)\n" << parents << "\n And offsprings (originally even)\n" << offspring << std::endl;


    return 1;
}

int main(int argc, char **argv)
{
    try
    {
        the_main(argc, argv);
    }
    catch(std::exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }

}
