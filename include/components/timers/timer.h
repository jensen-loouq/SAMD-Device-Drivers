#ifndef COREZERO_TIMERCOUNTER_H
#define COREZERO_TIMERCOUNTER_H

#include <sam.h>
#include "../../../lib/functional/Event.h"

void TCn_Handler(unsigned) noexcept;

namespace ll_system
{
	namespace timers
	{
		///	The instances of timers that are available
		constexpr Tc* _Timers[] = TC_INSTS;

		///	Elapsed event handler typing
		using Elapsed_EventHandler = CoreZero::Delegate<void()>;


		template <typename TcCountn>
		class timer;



		/**********************************************************************
		 *	An interface for timers.
		 */
		class i_timer
		{
			friend void ::TCn_Handler(unsigned);
		public:
			virtual ~i_timer() {}

		protected:
			///	Construct an instance from a Timer ID.
			constexpr i_timer(unsigned timerId)
				: m_id(timerId), m_runOnce(false), m_interval(0)
			{}


		protected:
			virtual void load(unsigned) noexcept = 0;



			/******************************************************************
			 *	\brief Reload the COUNT register with the saved interval.
			 */
			virtual void reload() noexcept
			{
				if (!m_runOnce) load(m_interval);
			}

		public:
			///	Signals when the timer elapses.
			CoreZero::Event<Elapsed_EventHandler> Elapsed;


		protected:
			/// Timer ID.
			unsigned m_id;

			/// Whether the timer should run once.
			bool m_runOnce;

			///	Duration.
			unsigned m_interval;

		};

		i_timer* TimerPool[TC_INST_NUM] = { nullptr };

		///	An 8-bit timer.
		using timer8 =	timer<TcCount8>;

		///	A 16-bit timer.
		using timer16 =	timer<TcCount16>;

		///	A 32-bit timer.
		using timer32 =	timer<TcCount32>;
	}
}

#include "variants/timer_8bit.h"
#include "variants/timer_16bit.h"
#include "variants/timer_32bit.h"


//
//	Handling Timers
//
#pragma region TIMER_HANDLING

inline void TCn_Handler(unsigned n) noexcept
{
	if (ll_system::timers::TimerPool[n])
	{
		ll_system::timers::TimerPool[n]->Elapsed();
		ll_system::timers::TimerPool[n]->reload();
	}
}

#define HANDLE_TC(N) \
	TCn_Handler(N); \
	TC##N->COUNT16.INTFLAG.bit.OVF = 1

#ifdef ID_TC0
void TC0_Handler() { HANDLE_TC(0); }
#endif
#ifdef ID_TC1
void TC1_Handler() { HANDLE_TC(1); }
#endif
#ifdef ID_TC2
void TC2_Handler() { HANDLE_TC(2); }
#endif
#ifdef ID_TC3
void TC3_Handler() { HANDLE_TC(3); }
#endif
#ifdef ID_TC4
void TC4_Handler() { HANDLE_TC(4); }
#endif
#ifdef ID_TC5
void TC5_Handler() { HANDLE_TC(5); }
#endif
#ifdef ID_TC6
void TC6_Handler() { HANDLE_TC(6); }
#endif
#ifdef ID_TC7
void TC7_Handler() { HANDLE_TC(7); }
#endif
#ifdef ID_TC8
void TC8_Handler() { HANDLE_TC(8); }
#endif

#pragma endregion

#endif // !COREZERO_TIMERCOUNTER_H
