#ifndef COREZERO_POWERMANAGER_H
#define COREZERO_POWERMANAGER_H

#include <sam.h>
#include "../../../lib/functional/Event.h"

namespace ll_system
{
	namespace management
	{
		//typedef enum
		//{
		//	CoreSwitchable,
		//	Backup,
		//	SystemRAM,
		//	BackupRAM
		//} PowerDomain;

		//using SleepModeEntryReady_EventHandler = delegate<void()>;
		//using PerformanceReady_EventHandler = delegate<void()>;

		//class power_manager
		//{
		//public:
		//	/// Sleep Modes for the MCU. The CPU is stopped.
		//	enum SleepMode
		//	{
		//		Idle = PM_SLEEPCFG_SLEEPMODE_IDLE2,			///< Synchronous clocks are stopped except when requested. Logic is retained.
		//		Standby = PM_SLEEPCFG_SLEEPMODE_STANDBY,	///< Peripherals are stopped, logic is retained, PD gating can be used to fully or partially turn of the PDSYSRAM power domain.
		//		Hibernate = PM_SLEEPCFG_SLEEPMODE_HIBERNATE,///< Core power domain is OFF. Backup PD is kept powered to allow few features to run
		//		Backup = PM_SLEEPCFG_SLEEPMODE_BACKUP,		///< Only the backup PD is kept powered to allow few featuers to run. The backup RAM PD can be retained
		//		OFF = PM_SLEEPCFG_SLEEPMODE_OFF				///< The entire device is powered off.
		//	};

		//	enum FastWakeup
		//	{
		//		No = PM_STDBYCFG_FASTWKUP(0x0),
		//		NVM = PM_STDBYCFG_FASTWKUP(0x1),
		//		MainVoltageRegulator = PM_STDBYCFG_FASTWKUP(0x2),
		//		Both = PM_STDBYCFG_FASTWKUP(0x3)
		//	};

		//	enum RAMConfig
		//	{
		//		Retain = PM_STDBYCFG_RAMCFG_RET,
		//		Retain32Kb = PM_STDBYCFG_RAMCFG_PARTIAL,
		//		Off = PM_STDBYCFG_RAMCFG_OFF
		//	};

		//public:
		//	static inline void EnterSleepMode() { while (!IsSleepReady()); __WFI; }

		//public:
		//	static event<SleepModeEntryReady_EventHandler> SleepModeEntryReady;
		//	static event<PerformanceReady_EventHandler> PerformanceReady;

		//public:
		//	static void AllowIORetention(bool enable) noexcept;
		//	static void AllowSleepReadyInterrupt(bool enable) noexcept;

		//	static void SetSleepMode(const SleepMode sleepMode) noexcept;
		//	static SleepMode GetSleepMode() noexcept;

		//	static bool IsSleepReady() noexcept;
		//	static void ConfigureStandby(const RAMConfig ramConfig, const FastWakeup fastWakeup) noexcept;
		//	static void ConfigureHibernation(const RAMConfig ramConfig, const RAMConfig backupRamConfig) noexcept;
		//	static void ConfigureBackup(const RAMConfig backupRamConfig) noexcept;

		//private:
		//	static void clear_interrupt_flag() noexcept;
		//	static void MCLK_enable_bus_clock() noexcept;
		//};
	}
}

#endif // !COREZERO_POWERMANAGER_H
