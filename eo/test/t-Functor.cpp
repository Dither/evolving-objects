
#include <Init.h>
#include <Counter.h>

void f(Init<int>& func)
{
    int i;
    func(i);
}

class Tester : public Init<int>
{
public :
    void operator()(int& i)
    {
	i=1;
    }
};

#include <iostream>
#include <eoFixedLength.h>
#include <eoVariableLength.h>

using namespace std;

int main(void)
{
    Tester test;

    FunctorStore store;

    /// make a counter and store it in 'store'
    Init<int>& cntr = make_counter(functor_category(test), test, store);

    UnaryFunctorCounter<Init<int> > cntr2(test);

    f(cntr);
    f(cntr2);
    f(cntr2);
    f(test);

    typedef eoVariableLength<double, int> EoType;
    EoType eo;

    eo.push_back(1);
    eo.push_back(2);

    return 1;
}
