#ifdef _MSC_VER
// to avoid long name warnings
#pragma warning(disable:4786)
#endif

#include <FunctorStore.h>
#include <Functor.h>

namespace eo
{

    /// clears the memory
    FunctorStore::~FunctorStore()
    {
	for(size_t i = 0; i < vec.size(); ++i) {
	    delete vec[i];
	}
    }

}
