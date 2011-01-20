#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sstream>

#include <utils/State.h>
#include <utils/Updater.h>

namespace eo
{

    using namespace std;

    void TimedStateSaver::operator()(void)
    {
	time_t now = time(0);

	if (now >= last_time + interval)
	    {
		last_time = now;
		ostringstream os;
		os << prefix << (now - first_time) << '.' << extension;
		state.save(os.str());
	    }
    }

    void CountedStateSaver::doItNow(void)
    {
	ostringstream os;
        os << prefix << counter << '.' << extension;
        state.save(os.str());
    }

    void CountedStateSaver::operator()(void)
    {
	if (++counter % interval == 0)
	    doItNow();
    }

    void CountedStateSaver::lastCall(void)
    {
	if (saveOnLastCall)
	    doItNow();
    }

}
