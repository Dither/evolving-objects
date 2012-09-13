# include <mpi/eoMpi.h>

#include <stdexcept>
#include <iostream>
#include <sstream>

#include <eo>
#include <es.h>
#include <ga.h>

#include <eoInt.h>

using namespace std;
using namespace eo::mpi;

typedef eoReal<double> Real;
typedef eoBit<double> Bit;
typedef eoInt<double> Int;
typedef Int Indi;

int main(int argc, char **argv)
{
    Node::init( argc, argv );


    return 0;
}
