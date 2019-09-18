/******************************************************************************
*	\file mainclock.h
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