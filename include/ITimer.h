#ifndef __I_TIMER_H_INCLUDED__
#define __I_TIMER_H_INCLUDED__

#include "IUnknown.h"

namespace irr
{

//! Interface for getting the current time
class ITimer : public IUnknown
{
public:

	//! destructor
	virtual ~ITimer() {}

	//! returns current time in milliseconds. This value does not start
	//! with 0 when the application starts. For example in one implementation
	//! the value returne could be the amount of milliseconds which
	//! have elapsed since the system was started.
	virtual u32 getTime() = 0;
};

} // end namespace

#endif

