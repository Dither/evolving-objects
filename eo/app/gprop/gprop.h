//-----------------------------------------------------------------------------
// gprop.h
//-----------------------------------------------------------------------------

#ifndef gprop_h
#define gprop_h

//-----------------------------------------------------------------------------

#include <iostream>               // istream ostream
#include <string>                 // string
#include <EO.h>                   // EO
#include <eoOp.h>                 // eoMonOp eoQuadraticOp
#include <eoEvalFuncPtr.h>        // eoEvalFunc
#include <eoInit.h>               // eoInit
#include <utils/rnd_generators.h> // normal_generator
#include "mlp.h"                  // mlp::net mlp::set
#include "qp.h"                   // qp::set
#include "mse.h"                  // mse::error

//-----------------------------------------------------------------------------
// phenotype
//-----------------------------------------------------------------------------

struct phenotype
{
  unsigned trn_ok, val_ok, tst_ok;
  double mse_error;

  static unsigned trn_max, val_max, tst_max;

  phenotype(const double& _mse_error = 0): mse_error(_mse_error) {}

  operator double(void) const { return mse_error; }

  friend bool operator<(const phenotype& a, const phenotype& b)
  {
    return a.mse_error < b.mse_error;
  }

  friend ostream& operator<<(ostream& os, const phenotype& p)
  {
    return os << p.trn_ok << "/" << p.trn_max << " "
	      << p.val_ok << "/" << p.val_max << " "
	      << p.tst_ok << "/" << p.tst_max << " "
	      << p.mse_error;
  }
  
  friend istream& operator>>(istream& is, phenotype& p)
  {
    return is;
  }
};

unsigned phenotype::trn_max = 0, phenotype::val_max = 0, phenotype::tst_max = 0;

//-----------------------------------------------------------------------------
// genotype
//-----------------------------------------------------------------------------

typedef mlp::net genotype;

//-----------------------------------------------------------------------------
// Chrom
//-----------------------------------------------------------------------------

class Chrom: public EO<phenotype>, public genotype
{
public:
  Chrom(): genotype(25, 2) {}

  string className() const { return "Chrom"; }

  void printOn (ostream& os) const 
  { 
    // os << static_cast<genotype>(*this) << " " << fitness(); 
    os << fitness();
  }
  
  void readFrom (istream& is) 
  {
    invalidate();
  }
};

//-----------------------------------------------------------------------------
// eoChromInit
//-----------------------------------------------------------------------------

class eoInitChrom: public eoInit<Chrom>
{
public:
  void operator()(Chrom& chrom)
  {
    chrom.reset();
    chrom.invalidate();
  }
};

//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------

mlp::set trn_set, val_set, tst_set;

//-----------------------------------------------------------------------------
// eoChromMutation
//-----------------------------------------------------------------------------

class eoChromMutation: public eoMonOp<Chrom>
{
public:
  eoChromMutation(eoValueParam<unsigned>& _generation):
    generation(_generation) {}
  
  void operator()(Chrom& chrom)
  {
    mse::net tmp(chrom);
    tmp.train(trn_set, 10, 0, 0.001);
  }
  
private:
  eoValueParam<unsigned>& generation;
};

//-----------------------------------------------------------------------------
// eoChromXover
//-----------------------------------------------------------------------------

class eoChromXover: public eoQuadraticOp<Chrom>
{
public:
  void operator()(Chrom& chrom1, Chrom& chrom2)
  {
  }
};

//-----------------------------------------------------------------------------
// eoChromEvaluator
//-----------------------------------------------------------------------------

unsigned correct(const mlp::net& net, const qp::set& set)
{
  unsigned sum = 0;
  
  for (qp::set::const_iterator s = set.begin(); s != set.end(); ++s)
    {
      unsigned partial = 0;
      
      for (unsigned i = 0; i < s->output.size(); ++i)
        if (s->output[i] < 0.5 && net(s->input)[i] < 0.5 ||
            s->output[i] > 0.5 && net(s->input)[i] > 0.5)
          ++partial;
      
      if (partial == s->output.size())
        ++sum;
    }

  return sum;
}



phenotype eoChromEvaluator(const Chrom& chrom)
{
  //  extern mlp::set trn_set, val_set, tst_set;

  phenotype p;
  p.trn_ok = correct(chrom, trn_set);
  p.val_ok = correct(chrom, val_set);
  p.tst_ok = correct(chrom, tst_set);
  p.mse_error = mse::error(chrom, val_set);

  return p;
};

//-----------------------------------------------------------------------------

#endif // gprop_h

// Local Variables: 
// mode:C++ 
// End: