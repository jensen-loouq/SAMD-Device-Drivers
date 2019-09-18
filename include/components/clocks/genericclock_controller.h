/******************************************************************************
*	\file genericclock_controller.h
*	\author Jensen Miller <jensen@loouq.com>
*	
*	Copyright (c) 2019 LooUQ Incorporated
*
*	License: The GNU License
*
*	This file is part of CoreZero.
*
*   CoreZero is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   CoreZero is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with CoreZero.  If not, see <https://www.gnu.org/licenses/>.
*
`*****************************************************************************/
#ifndef COREZERO_GENERICCLOCKCONTROLLER_H
#define COREZERO_GENERICCLOCKCONTROLLER_H

#include <sam.h>
#include "mainclock.h"

namespace ll_system
{
	namespace clocks
	{
		class genericclock_controller
		{
		public:
			enum Source
			{
				X32kHz = GCLK_GENCTRL_SRC_XOSC32K_Val,
				ULP32kHz = GCLK_GENCTRL_SRC_OSCULP32K_Val,
				DFLL48MHz = GCLK_GENCTRL_SRC_DFLL48M_Val,
				DPLL96MHz = GCLK_GENCTRL_SRC_DPLL96M_Val
			};

		public:
			static inline void SWReset(void)
			{
				GCLK->CTRLA.bit.SWRST = 1ul;
				while (GCLK->SYNCBUSY.bit.SWRST)
					;
			}

			static inline void SetGenericClockSource(unsigned long genericClkId, unsigned long sourceClock
				, bool enable_ = true
				, bool improveDutyCycle = false
				, bool outputOffValue = false
				, bool outputEnable = false
				, unsigned long genDiv = 0)
			{
				GCLK->GENCTRL[genericClkId].reg = GCLK_GENCTRL_SRC(sourceClock)
					| (enable_ ? GCLK_GENCTRL_GENEN : 0ul)
					| (improveDutyCycle ? GCLK_GENCTRL_IDC : 0ul)
					| (outputOffValue ? GCLK_GENCTRL_OOV : 0ul)
					| (outputEnable ? GCLK_GENCTRL_OE : 0ul)
					| (genDiv ? GCLK_GENCTRL_DIV(genDiv) : 0ul);

				while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL((1ul << genericClkId)))
					;
			}

			static inline void SetPeripheralChannelControl(unsigned peripheralIndex, unsigned generatorSelection
				, bool channelEnable = true
				, bool writeLock = false)
			{
				GCLK->PCHCTRL[peripheralIndex].reg = GCLK_PCHCTRL_GEN(generatorSelection)
					| (channelEnable ? GCLK_PCHCTRL_CHEN : 0ul)
					| (writeLock ? GCLK_PCHCTRL_WRTLOCK : 0ul);
				while (!GCLK->PCHCTRL[peripheralIndex].bit.CHEN)
					;
			}
		};

		template<>
		void main_clock::EnableClock<genericclock_controller>()
		{
			MCLK->APBAMASK.bit.GCLK_ = 1ul;
		}
	}
}

#endif // !COREZERO_GENERICCLOCKCONTROLLER_H
