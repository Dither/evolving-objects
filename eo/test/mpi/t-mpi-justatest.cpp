# include <mpi/eoMpi.h>

#include <stdexcept>
#include <iostream>
#include <sstream>

#include <eo>
#include <es.h>

#include <eoInt.h>

#include "SerializableEOReal.h"
#include "SerializableEOBit.h"

#include "real_value.h"

using namespace std;
using namespace eo::mpi;

typedef eoserial::eoReal<double> Real;
typedef eoserial::eoBit<double> Bit;
typedef eoserial::eoInt<double> Int;
typedef Int Indi;

int main(int argc, char **argv)
{
    Node::init( argc, argv );


    return 0;
}
