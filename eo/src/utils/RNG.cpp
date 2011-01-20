#ifdef _MSC_VER
// to avoid long name warnings
#pragma warning(disable:4786)
#endif

#include <ctime>
#include "RNG.h"

namespace eo
{

    // initialize static constants
    const uint32_t Rng::K(0x9908B0DFU);
    const int Rng::M(397);
    const int Rng::N(624);

    // global random number generator object
    Rng rng(static_cast<uint32_t>(time(0)));

}
