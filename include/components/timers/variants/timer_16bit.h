#ifndef __TIMER_16BIT_H__
#define __TIMER_16BIT_H__

#include "../timer.h"

#define WaitForSync(xBUSY) \
	while (p_timer->SYNCBUSY.bit.xBUSY)



namespace ll_system
{
    namespace timers
    {
		constexpr const float FrequencyCorrective = 0.9766f;

		template <>
		class timer< TcCount16 > : public i_timer
		{
		//
		//	Enumerations
		//
		public:			
			enum Command
			{
				NoAction = 0x0,
				Re_Trigger = 0x1,
				Stop = 0x2,
				Reserved = 0x3
			};

			enum Direction
			{
				Up = 0x0,
				Down = 0x1
			};

			enum WaveformGenerationOp
			{
				NFRQ,
				MFRQ,
				NPWM,
				MPWM
			};


		//
		//	Constructors
		//
		public:
            /// Constructor.
			constexpr timer(unsigned timerId)
				: i_timer(timerId)
				, p_timer(&_Timers[timerId]->COUNT16)
			{}


            /// Destructor.
			~timer() { TimerPool[m_id] = nullptr; }


		//
		//	Methods
		//
		public:
			/******************************************************************
			 *	\brief Runtime initialization of the timer.
			 */
			inline void Initialize(void)
			{
				configure_clocks();

				//	Disable the timer counter
				p_timer->CTRLA.bit.ENABLE = 0ul;
				WaitForSync(ENABLE)
					;

				//	Reset the Timer/Counter
				p_timer->CTRLA.bit.SWRST = 1ul;
				WaitForSync(SWRST)
					;

				//	Set Mode
				p_timer->CTRLA.bit.MODE = TC_CTRLA_MODE_COUNT16_Val;
				WaitForSync(STATUS)
					;

				//	Select wave generation operation
				p_timer->WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_NFRQ_Val;

				//	Setup Prescaler
				p_timer->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024 | TC_CTRLA_PRESCSYNC_PRESC;
				WaitForSync(STATUS)
					;
					
				configure_interrupts();

				TimerPool[m_id] = this;
			}



			/******************************************************************
			 *	\brief Start the timer with a given interval.
			 *
			 *	\param[in] interval_ The interval the timer should elapse over.
			 */
			inline void Start(unsigned interval_)
			{
				m_interval = interval_;
				p_timer->CTRLBCLR.bit.ONESHOT = 1;
				WaitForSync(CTRLB)
					;

				load(m_interval);
				SetDirection(timer::Down);
				p_timer->CTRLA.bit.ENABLE = 1;

				WaitForSync(ENABLE)
					;
			}



			/******************************************************************
			 *	\brief Pause the timer.
			 */
			inline void Pause(void)
			{
				issue_command(Command::Stop);
				while (p_timer->STATUS.bit.STOP)
					;
			}



			/******************************************************************
			 *	\brief Resume the timer operation.
			 */
			inline void Resume(void)
			{
				if (p_timer->STATUS.bit.STOP)
				{
					issue_command(Command::Re_Trigger);
					while (!p_timer->STATUS.bit.STOP)
						;
				}
			}



			/******************************************************************
			 *	\brief Restart the timer.
			 */
			inline void Restart(void)
			{
				issue_command(Command::Re_Trigger);
			}


			/******************************************************************
			 *	\brief Set the direction of the timer (count up/down).
			 *
			 *	\param[in] direction_ The direction to count.
			 */
			inline void SetDirection(const Direction direction_)
			{
				switch (direction_)
				{
				case Up:
					p_timer->CTRLBCLR.bit.DIR = 1;
					WaitForSync(CTRLB);
					return;
				case Down:
					p_timer->CTRLBSET.bit.DIR = 1;
					WaitForSync(CTRLB);
					return;
				}
			}


		private:
			/******************************************************************
			 *	\brief Configure the Clocks
			 */
			inline void configure_clocks(void)
			{
				//	Generic Clock setup
				unsigned long peripheral_index = TC0_GCLK_ID + (unsigned)(m_id / 2);
				GCLK->PCHCTRL[peripheral_index].reg = GCLK_PCHCTRL_GEN(3ul) | GCLK_PCHCTRL_CHEN;				
				while (!GCLK->PCHCTRL[peripheral_index].bit.CHEN)
					;
				
				//	Peripheral Clock Masking
				MCLK->APBCMASK.reg |= 1ul << (MCLK_APBCMASK_TC0_Pos + m_id);
			}



			/******************************************************************
			 *	\brief Configure the timer's interrupts.
			 */
			inline void configure_interrupts(void)
			{
				const IRQn_Type timer_irqn = (IRQn_Type)((unsigned)(IRQn_Type::TC0_IRQn) + m_id);

				NVIC_SetPriority(timer_irqn, 0);
				NVIC_EnableIRQ(timer_irqn);

				p_timer->INTENSET.bit.OVF = 1;
			}



			/******************************************************************
			 *	\brief Issue a command to the timer.
			 *
			 *	\param[in] command_ The command to issue.
			 */
			inline void issue_command(const Command command_)
			{
				p_timer->CTRLBSET.bit.CMD = command_;
			}



			/******************************************************************
			 *	\brief Load the COUNT register with a duration.
			 *
			 *	\param[in] count_ The value to load COUNT with.
			 */
			void load(unsigned count_) noexcept
			{
				p_timer->COUNT.reg = (count_ * FrequencyCorrective);
				WaitForSync(COUNT)
					;
			}


		private:
            /// The timer instance.
			TcCount16 * p_timer;
		};
    }   
}


#endif