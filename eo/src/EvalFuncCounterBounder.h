#ifndef EvalFuncCounterBounder_H
#define EvalFuncCounterBounder_H

#include <EvalFunc.h>
#include <utils/Param.h>

namespace eo
{

    /** @addtogroup Evaluation
     * @{
     */

    /** The exception raised by EvalFuncCounterBounder 
     * when the maximum number of allowed evaluations is reached.
     */
    class EvalFuncCounterBounderException : public std::exception
    {
    public:
	EvalFuncCounterBounderException(unsigned long threshold) : _threshold(threshold){}

	const char* what() const throw()
	{
	    std::ostringstream ss;
	    ss << "STOP in EvalFuncCounterBounderException: the maximum number of evaluation has been reached (" << _threshold << ").";
	    return ss.str().c_str();
	}

    private:
	unsigned long _threshold;
    };

    /** Counts the number of evaluations actually performed and throw an EvalFuncCounterBounderException
     * when the maximum number of allowed evaluations is reached.
     *
     * This eval counter permits to stop a search during a generation, without waiting for a continue to be
     * checked at the end of the loop. Useful if you have 10 individuals and 10 generations, 
     * but want to stop after 95 evaluations.
     */
    template < typename EOT >
    class EvalFuncCounterBounder : public EvalFuncCounter< EOT >
    {
    public :
	EvalFuncCounterBounder(EvalFunc<EOT>& func, unsigned long threshold, std::string name = "Eval. ")
	    : EvalFuncCounter< EOT >( func, name ), _threshold( threshold )
	{}

	using EvalFuncCounter< EOT >::value;

	virtual void operator()(EOT& eo)
	{
	    if (eo.invalid())
		{
		    value()++;

		    if (_threshold > 0 && value() >= _threshold)
			{
			    throw EvalFuncCounterBounderException(_threshold);
			}

		    func(eo);
		}
	}

    private :
	unsigned long _threshold;
    };

}

/** @} */
#endif
