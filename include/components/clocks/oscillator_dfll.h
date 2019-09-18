/******************************************************************************
*	\file oscillator_dfll.h
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
#ifndef COREZERO_OSCILLATOR_DFLL_H
#define COREZERO_OSCILLATOR_DFLL_H

#include "oscillator_controller.h"

namespace ll_system
{
	namespace clocks
	{
		class dfll48m_oscillator
		{
			static unsigned m_sCoarseMaxStep;
			static unsigned m_sFineMaxStep;
			static unsigned m_sDFLLMulFactor;

		public:
			static inline void WaitForReady()
			{
				while (!OSCCTRL->STATUS.bit.DFLLRDY)
					;
			}		

			static inline void Disable(void)
			{
				OSCCTRL->DFLLCTRL.bit.ENABLE = 0ul;
			}

			static inline void Enable()
			{
				OSCCTRL->DFLLCTRL.bit.ENABLE = 1ul;
			}

			static inline void SetCoarseMaxStep(unsigned coarseMaxStep)
			{
				m_sCoarseMaxStep = coarseMaxStep;
			}

			static inline void SetFineMaxStep(unsigned fineMaxStep)
			{
				m_sFineMaxStep = fineMaxStep;
			}

			static inline void SetDFLLMultiplyFactor(unsigned dfllMulFactor)
			{
				m_sDFLLMulFactor = dfllMulFactor;
			}

			static void ApplyMultiplierSettings()
			{
				OSCCTRL->DFLLMUL.reg = OSCCTRL_DFLLMUL_CSTEP(m_sCoarseMaxStep)
					| OSCCTRL_DFLLMUL_FSTEP(m_sFineMaxStep)
					| OSCCTRL_DFLLMUL_MUL(m_sDFLLMulFactor);
			}
		};

		unsigned dfll48m_oscillator::m_sCoarseMaxStep = 0ul;
		unsigned dfll48m_oscillator::m_sFineMaxStep = 0ul;
		unsigned dfll48m_oscillator::m_sDFLLMulFactor = 0ul;
	}
}

#endif // !COREZERO_OSCILLATOR_DFLL_H
