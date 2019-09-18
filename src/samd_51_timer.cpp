#include "Timer.h"

#if SAMD51_SERIES

#define GetTimerCounter(N) \
	(TC ## N)


#define WaitSync(OP) \
	while (m_tc->COUNT16.SYNCBUSY.bit.OP)

/* --------------------------------- Timers ---------------------------------*/

System::Timers::Timer* g_Timer[TC_INST_NUM] = { nullptr };

void TCn_Handler(unsigned n) {
	if (g_Timer[n] != nullptr) {
		g_Timer[n]->Elapsed();
		g_Timer[n]->reload();
	}
}

/*****************************************************************************/



/* -------------------------- Native Timer Handlers ------------------------ */
#define HANDLE_TC(N) TCn_Handler(N); TC##N->COUNT16.INTFLAG.bit.OVF = 1

void TC0_Handler() { HANDLE_TC(0); }
void TC1_Handler() { HANDLE_TC(1); }
void TC2_Handler() { HANDLE_TC(2); }
void TC3_Handler() { HANDLE_TC(3); }
void TC4_Handler() { HANDLE_TC(4); }
void TC5_Handler() { HANDLE_TC(5); }

/*****************************************************************************/



namespace System
{
	namespace Timers
	{
		constexpr const float FrequencyCorrective = 0.9766f;



		Timer::~Timer()
		{
			// Reset Clock Registers
			m_tc->COUNT16.CTRLA.bit.SWRST = 1;
			WaitSync(SWRST)
				;

			// Set the global instance back to null
			g_Timer[m_timerId] = nullptr;
		}



		void Timer::Initialize()
		{
			g_Timer[m_timerId] = this;
			/* Configure Dependencies */
			config_gclk();
			config_nvic();

			// Get the timer by ID
			m_tc = GetTimerCounter(5);

			// Reset the Timer/Counter
			m_tc->COUNT16.CTRLA.bit.SWRST = 1;
			WaitSync(SWRST)
				;

			// Disable the Timer/Counter	
			m_tc->COUNT16.CTRLA.bit.ENABLE = 0;
			WaitSync(ENABLE)
				;

			// Peripheral Clock Masking - Enable TCn
			MCLK->APBCMASK.bit.TC5_ = 1;

			// Select x-bit counter mode
			m_tc->COUNT16.CTRLA.bit.MODE = TC_CTRLA_MODE_COUNT16_Val;
			WaitSync(STATUS)
				;

			// Select wave generation operation
			m_tc->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_NFRQ_Val;

			//	Setup Prescaler
			m_tc->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024 | TC_CTRLA_PRESCSYNC_PRESC;

			// Set interrupt
			m_tc->COUNT16.INTENSET.bit.OVF = 1;
		}



		void Timer::Start(unsigned interval_)
		{
			m_interval = interval_;
			m_tc->COUNT16.CTRLBCLR.bit.ONESHOT = 1;
			WaitSync(CTRLB)
				;

			load(m_interval);

			SetDirection(Timer::Down);

			m_tc->COUNT16.CTRLA.bit.ENABLE = 1;
			WaitSync(ENABLE)
				;
		}



		void Timer::StartOnce(unsigned initialCount)
		{
			m_tc->COUNT16.CTRLBSET.bit.ONESHOT = 1;
			WaitSync(CTRLB)
				;

			// Set Count
			load(initialCount);

			SetDirection(Timer::Down);

			m_tc->COUNT16.CTRLA.bit.ENABLE = 1;
			WaitSync(ENABLE)
				;
		}



		void Timer::Pause()
		{
			issue_command(Command::Stop);
			while (m_tc->COUNT16.STATUS.bit.STOP)
				;
		}



		/**
		 *	\brief Resume the timer operation.
		 */
		void Timer::Resume() noexcept
		{
			if (m_tc->COUNT16.STATUS.bit.STOP)
			{
				issue_command(Command::Re_Trigger);
				while (!m_tc->COUNT16.STATUS.bit.STOP)
					;
			}
		}



		/**
		 *	\brief Restart the timer.
		 */
		void Timer::Restart() noexcept
		{
			issue_command(Command::Re_Trigger);
		}



		/**
		 *	\brief Set the direction of the timer (Count up/down).
		 *
		 *	\param[in] dir The direction to count.
		 */
		void Timer::SetDirection(const Direction dir) noexcept
		{
			switch (dir)
			{
			case Up:
				m_tc->COUNT16.CTRLBCLR.bit.DIR = 1;
				WaitSync(CTRLB);
				break;
			case Down:
				m_tc->COUNT16.CTRLBSET.bit.DIR = 1;
				WaitSync(CTRLB);
				break;
			}
			return;
		}



		/**
		 *	\brief Configure a generic clock for the timer.
		 */
		void Timer::config_gclk() noexcept
		{
			GCLK->PCHCTRL[0x1E].reg = GCLK_PCHCTRL_GEN_GCLK5_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);
			while (GCLK->SYNCBUSY.bit.GENCTRL5)
				;
		}



		/**
		 *	\brief Configure the NVIC interrupt for the timer.
		 */
		void Timer::config_nvic() const noexcept
		{
			NVIC_EnableIRQ(IRQn::TC5_IRQn);
		}



		/**
		 *	\brief Issue a command to the timer.
		 *
		 *	\param[in] command_ The command to issue.
		 */
		void Timer::issue_command(const Command command_) noexcept
		{
			m_tc->COUNT16.CTRLBSET.bit.CMD = (uint8_t)command_;
		}



		/**
		 *	\brief Load the count register.
		 *
		 *	\param[in] countVal The value to load count with
		 */
		void Timer::load(unsigned countVal) const noexcept
		{
			// Set Count
			m_tc->COUNT16.COUNT.reg = (uint16_t)(countVal * FrequencyCorrective);
			WaitSync(COUNT)
				;
		}



		void Timer::reload() noexcept
		{
			if (!m_runOnce)
				load(m_interval);
		}
	}
}


#endif