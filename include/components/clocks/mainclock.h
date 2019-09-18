#ifndef COREZERO_MAINCLOCK_H
#define COREZERO_MAINCLOCK_H

#include <sam.h>
typedef unsigned char uint8;
typedef unsigned long uint32;

namespace ll_system
{
	namespace clocks
	{
		enum CPUClockDivFactor
		{
			DIV1,
			DIV2,
			DIV4,
			DIV8,
			DIV16,
			DIV32,
			DIV64,
			DIV128
		};

		enum ClockMask
		{
			AHB,
			APBA,
			APBB,
			APBC,
			APBD,
			APBE
		};

		class main_clock
		{
		public:
			static void SetCPUClkDivision(CPUClockDivFactor divFactor)
			{
				MCLK->CPUDIV.reg = (uint8)divFactor;
			}

			template <typename COMPONENT>
			static void EnableClock();
		};
	}
}

#endif