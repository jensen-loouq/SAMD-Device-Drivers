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
