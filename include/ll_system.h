#ifndef COREZERO_LL_SYSTEM_H
#define COREZERO_LL_SYSTEM_H

#pragma once

#include <sam.h>

#include "../lib/compile_time/CTString.h"
#include "ll_component.h"

#include "components/management/power_manager.h"
#include "components/clocks/mainclock.h"
#include "components/clocks/oscillator_dfll.h"
#include "components/clocks/genericclock_controller.h"
#include "components/memory/nvm_controller.h"
#include "components/timers/timer.h"
#include "components/management/reset_controller.h"



typedef enum
{
	AtmelAVR8,
	AtmelAVR32,
	AtmelSAM,
	IntelQuark
} Platform_E;

namespace ll_system
{
	static inline void initialize(void) noexcept
	{
		using memory::nonvolatilememory_controller;
		using namespace clocks;

		SystemInit();

		// 0 Read States
		nonvolatilememory_controller::SetReadWaitStates(NVMCTRL_CTRLB_RWS_HALF_Val);

		//	Enable Generic Clocks		
		main_clock::EnableClock<genericclock_controller>();

		//	Setup External crystal oscillator
		oscillator_controller<Is32kOscillator>::EnableExternal32kOscillator();

		//	Reset the Generic Clock Controller
		genericclock_controller::SWReset();

		//	Source GENERATOR_1 with external crystal oscillator
		genericclock_controller::SetGenericClockSource(1ul, genericclock_controller::X32kHz);



		//
		//	DFLL48M
		//
		genericclock_controller::SetPeripheralChannelControl(OSCCTRL_GCLK_ID_DFLL48, 1ul);

		dfll48m_oscillator::WaitForReady();

		dfll48m_oscillator::Disable();



		//	Set DFLL coarse & fine max step. Multiply factor 0.
		dfll48m_oscillator::SetCoarseMaxStep(1ul);
		dfll48m_oscillator::SetFineMaxStep(1ul);
		dfll48m_oscillator::SetDFLLMultiplyFactor(0ul);
		dfll48m_oscillator::ApplyMultiplierSettings();

		dfll48m_oscillator::Enable();


		//	Source GENERATOR_0 with DFLL 48MHz oscillator for main clock.
		genericclock_controller::SetGenericClockSource(0ul, genericclock_controller::DFLL48MHz, true, true);

		//	Create a 1Mhz clock on GENERATOR_3
		genericclock_controller::SetGenericClockSource(3ul, genericclock_controller::DFLL48MHz, true, false, false, false, 48ul);

		//
		//	Main Clock
		//

		//	Generator 0 sources main clock - Run CPU @ 48MHz
		main_clock::SetCPUClkDivision(DIV1);

		//	The main voltage regulator is the LDO
		//SUPC->VREG.bit.SEL = 0;
	}





	//template <Platform_E _Platform>
	//struct __system__;

	//template<>
	//struct __system__<AtmelSAM>
	//{
	//	typedef enum
	//	{
	//		D51,
	//		R34
	//	} ProductSeries_E;

	//	typedef enum
	//	{
	//		G = 0x30,
	//		J = 0x40,
	//		N = 0x64,
	//		P = 0x80
	//	} PinCount_E;

	//	typedef enum
	//	{
	//		F18 = 0x100,
	//		F19 = 0x200,
	//		F20 = 0x400
	//	} FlashMemDensity_E;

	//	template <ProductSeries_E _Series, PinCount_E _PinCount, FlashMemDensity_E _FlashMemoryDensity>
	//	struct Configuration
	//	{

	//	};
	//};
}


#endif // !COREZERO_LL_SYSTEM_H