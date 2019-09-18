#ifndef SAMD_TIMER_H
#define SAMD_TIMER_H

#include "Event.h"
#include <samd.h>

void TCn_Handler(unsigned);

namespace System
{
	namespace Timers
	{
		using Elapsed_EventHandler = mbd::delegate<void()>;

		class Timer
		{
			friend void ::TCn_Handler(unsigned);
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

		public:
			constexpr Timer(const unsigned timerId)
				: m_runOnce(false)
				, m_genericClkDiv(0)
				, m_interval(0)
				, m_timerId(timerId)
				, m_tc(nullptr)
			{}

			~Timer();

		public:
			virtual void Initialize() noexcept;
			virtual void Start(unsigned interval_) noexcept;
			virtual void StartOnce(unsigned duration_) noexcept;
			virtual void Pause() noexcept;
			virtual void Resume() noexcept;
			virtual void Restart() noexcept;

		public:
			unsigned GetInterval() const noexcept { return m_interval; }
			void SetInterval(unsigned milliSeconds) noexcept { m_interval = milliSeconds; }
			void SetDirection(const Direction dir) noexcept;
		public:
			mbd::event<Elapsed_EventHandler> Elapsed;

		private:
			void config_gclk() noexcept;
			void config_nvic() const noexcept;			
			void issue_command(const Command command_);
			void load(unsigned count_) const noexcept;
			void reload() noexcept;

		private:
			/// Mode to run once (OneShot).
			bool m_runOnce;

			/// The generic clock divider.
			uint16_t m_genericClkDiv;

			/// The interval until elapse.
			unsigned m_interval;

			/// The timer id.
			unsigned m_timerId;

			/// The timer structure.
			Tc* m_tc;
		};
	}
}


#endif // !SAMD_TIMER_H