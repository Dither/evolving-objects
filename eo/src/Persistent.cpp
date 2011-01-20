#ifdef _MSC_VER
// to avoid long name warnings
#pragma warning(disable:4786)
#endif 

#include <Persistent.h>

namespace eo
{

    //Implementation of these objects

    std::istream & operator >> ( std::istream& _is, Persistent& _o ) {
	_o.readFrom(_is);
	return _is;
    }

}
