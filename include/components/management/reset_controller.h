#ifndef COREZERO_RESETCONTROLLER_H
#define COREZERO_RESETCONTROLLER_H

#include <sam.h>

namespace ll_system
{
	namespace management
	{
		enum ResetCause
		{
			PowerOnReset = RSTC_RCAUSE_POR,
			BrownOut12 = RSTC_RCAUSE_BOD12,
			BrownOut33 = RSTC_RCAUSE_BOD33,
			ExternalReset = RSTC_RCAUSE_EXT,
			WatchdogReset = RSTC_RCAUSE_WDT,
			SystemResetRequest = RSTC_RCAUSE_SYST,
			BackupReset = RSTC_RCAUSE_BACKUP
		};

		enum BackupExit
		{
			RealTimerCounterInterrupt = RSTC_BKUPEXIT_RTC,
			BatteryBackupPowerSwitch = RSTC_BKUPEXIT_BBPS,
			ExternalWake = RSTC_BKUPEXIT_EXTWAKE
		};

		class reset_controller
		{
		public:
			static ResetCause GetResetCause() noexcept
			{
				return (ResetCause)(RSTC->RCAUSE.reg & RSTC_RCAUSE_MASK);
			}


			static BackupExit GetBackupExitSource() noexcept
			{
				return (BackupExit)(RSTC->BKUPEXIT.reg & RSTC_BKUPEXIT_MASK);
			}

		private:
			static void MCLK_enable_bus_clock() noexcept
			{
				if (MCLK->APBAMASK.bit.RSTC_ == 0)
					MCLK->APBAMASK.bit.RSTC_ = 1;
			}
		};
	}
}

#endif // !COREZERO_RESETCONTROLLER_H
