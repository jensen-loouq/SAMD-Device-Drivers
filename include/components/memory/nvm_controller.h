#ifndef COREZERO_NVMCONTROLLER_H
#define COREZERO_NVMCONTROLLER_H

#include <sam.h>

namespace ll_system
{
	namespace memory
	{
		class nonvolatilememory_controller
		{
		public:
			static inline void SetReadWaitStates(unsigned long waitStates)
			{
				NVMCTRL->CTRLB.bit.RWS = waitStates;
			}
		};
	}
}

#endif // !COREZERO_NVMCONTROLLER_H
