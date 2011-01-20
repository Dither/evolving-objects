#ifndef FuncPtrStat_h
#define FuncPtrStat_h

#include <FunctorStore.h>
#include <utils/Stat.h>

namespace eo
{

    /** Wrapper to turn any stand-alone function and into an Stat.
     *
     * The function should take an Pop as argument.
     *
     * @ingroup Stats
     */
    template <class EOT, class T>
    class FuncPtrStat : public Stat<EOT, T>
    {
    public :
	typedef T (*func_t)(const Pop<EOT>&);


	FuncPtrStat(func_t f, std::string _description = "func_ptr")
	    : Stat<EOT, T>(T(), _description), func(f)
        {}

	using Stat<EOT, T>::value;

	void operator()(const Pop<EOT>& pop)
	{
	    value() = func(pop);
	}

    private:
	func_t func;
    };

    /**
     * @ingroup Stats
     */
    template <class EOT, class T>
    FuncPtrStat<EOT, T>& makeFuncPtrStat( T (*func)(const Pop<EOT>&), FunctorStore& store, std::string description = "func") {
	return store.storeFunctor(
				  new FuncPtrStat<EOT, T>( func, description)
				  );
    }

}

#endif
