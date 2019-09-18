#ifndef __TIMER_32BIT_H__
#define __TIMER_32BIT_H__

#include "../Timer.h"

namespace ll_system
{
    namespace timers
    {
        template <>
		class timer< TcCount32 >
		{

		};
    } // namespace Timers
    
} // namespace System

#endif