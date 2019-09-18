/*
 Name:		Test_PowerManager.ino
 Created:	7/1/2019 9:28:44 PM
 Author:	LooUQ Inc
*/

// the setup function runs once when you press reset or power the board
#include <CoreZero.h>

using namespace CoreZero::Timers;
using namespace CoreZero::Management;

volatile int times = 0;


void OnSleepEntryReady()
{
	times++;
}


void OnElapsed()
{
	Serial.begin(115200);
	Serial.println("Woke-Up!");
	Serial.println(times);
}

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);

	Timer& timer = *new Timer(5);
	timer.Initialize();
	timer.Elapsed += OnElapsed;
	timer.Start(7000);

	PowerManager::SetSleepMode(PowerManager::Idle);
	PowerManager::SleepModeEntryReady += OnSleepEntryReady;
	PowerManager::AllowSleepReadyInterrupt(true);
}

// the loop function runs over and over again until power down or reset
void loop() {
	delay(1500);
	digitalWrite(LED_BUILTIN, LOW);
	delay(1500);
	digitalWrite(LED_BUILTIN, HIGH);

	PowerManager::EnterSleepMode();
}
