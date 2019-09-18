#include "Timer.h"

//#if SAMD21_SERIES

#define GetTimerCounter(N) \
	(TC ## N)


#define WaitSync(OP) \
	while (m_tc->COUNT16.STATUS.bit.OP)



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
constexpr Tc* SAMD21_TCs[TC_INST_NUM] = TC_INSTS;
#define HANDLE_TC(N) TCn_Handler(N); TC##N->COUNT16.INTFLAG.bit.OVF = 1

//void TCC0_Handler(void) {}
//void TCC1_Handler(void) {}
//void TCC2_Handler(void) {}
void TC3_Handler() { HANDLE_TC(3); }
void TC4_Handler() { HANDLE_TC(4); }
void TC5_Handler() { HANDLE_TC(5); }

/*****************************************************************************/


namespace System
{
	namespace Timers
	{
		constexpr const float FrequencyCorrective = 31.25f;
		constexpr const unsigned TcOffset = TCC_INST_NUM;

		Timer::~Timer()
		{
			m_tc->COUNT16.CTRLA.bit.SWRST = 1;
			WaitSync(SYNCBUSY)
				; 

			// Set the global instance back to null
			g_Timer[m_timerId] = nullptr;
		}

		void Timer::Initialize()
		{
			g_Timer[m_timerId] = this;
			
			config_gclk();
			config_nvic();

			// Get the timer by ID
			m_tc = SAMD21_TCs[m_timerId - TcOffset];
			
			// Disable the timer/counter
			m_tc->COUNT16.CTRLA.bit.ENABLE = 0;
			WaitSync(SYNCBUSY)
				;
				
			// Power Manager (Enable CLK_TCn_APB)
			PM->APBCMASK.reg |= 0x1ul << (PM_APBCMASK_TCC0_Pos + m_timerId);
		
			// Reset the timer/counter
			//m_tc->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
			//WaitSync(SYNCBUSY)
				//;

			// Select bit mode
			m_tc->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
			WaitSync(SYNCBUSY)
				;

			// Select wave generation operation
			m_tc->COUNT16.CTRLA.bit.WAVEGEN |= TC_CTRLA_WAVEGEN_NFRQ_Val;
			WaitSync(SYNCBUSY)
				;

			// Setup prescaler
			m_tc->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCSYNC_PRESC
				| TC_CTRLA_PRESCALER_DIV256;
			WaitSync(SYNCBUSY)
				;

			// Enable Interrupt
			m_tc->COUNT16.INTENSET.reg = TC_INTENSET_OVF;
			WaitSync(SYNCBUSY)
				;
		}


		void Timer::Start(unsigned interval_)
		{
			// Save the interval
			m_interval = interval_;
			m_runOnce = false;
			
			// Disable one-shot mode
			WaitSync(SYNCBUSY)
				;
			m_tc->COUNT16.CTRLBCLR.reg |= TC_CTRLBCLR_ONESHOT;
			WaitSync(SYNCBUSY)
				;
			
			SetDirection(Timer::Down);
			load(m_interval);


			m_tc->COUNT16.CTRLA.bit.ENABLE = 1;
			WaitSync(SYNCBUSY)
				;
		}


		void Timer::StartOnce(unsigned initialCount)
		{
			m_tc->COUNT16.CTRLBSET.bit.ONESHOT = 1;
			WaitSync(SYNCBUSY)
				;

			// SetCount
			load(initialCount);

			SetDirection(Timer::Down);
			m_runOnce = true;

			m_tc->COUNT16.CTRLA.bit.ENABLE = 1;
			WaitSync(SYNCBUSY)
				;
		}

		void Timer::Pause()
		{
			issue_command(Command::Stop);
			WaitSync(STOP)
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
				m_tc->COUNT16.CTRLBCLR.reg |= TC_CTRLBCLR_DIR;
				WaitSync(SYNCBUSY)
					;
				break;
			case Down:
				m_tc->COUNT16.CTRLBSET.reg |= TC_CTRLBSET_DIR;
				WaitSync(SYNCBUSY)
					;
				break;
			}
			return;
		}



		/**
		 *	\brief Configure a generic clock for the timer.
		 */
		void Timer::config_gclk() noexcept
		{
			/* NOTE: THE GENERIC CLOCK MUST BE CONFIGURED BY PERFORMING A SINGLE 16-BIT WRITE TO THE GCLK CTRL REG */
			GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID( GCLK_CLKCTRL_ID_TCC0_TCC1_Val + m_timerId )
				| GCLK_CLKCTRL_GEN_GCLK3
				| GCLK_CLKCTRL_CLKEN;

			while (GCLK->STATUS.bit.SYNCBUSY)
				;
		}




		/**
		 *	\brief Configure the NVIC interrupt for the timer.
		 */
		void Timer::config_nvic() const noexcept
		{
			NVIC_EnableIRQ((IRQn_Type)((int)IRQn::TCC0_IRQn + m_timerId));
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
			m_tc->COUNT16.COUNT.reg = TC_COUNT16_COUNT_COUNT((uint16_t)(countVal * FrequencyCorrective));
			WaitSync(SYNCBUSY)
				;
		}



		/**
		 *	\brief Reload the COUNT register with the saved interval.
		 */
		void Timer::reload() noexcept
		{
			if (!m_runOnce)
				load(m_interval);
		}
	}
}

//#endif