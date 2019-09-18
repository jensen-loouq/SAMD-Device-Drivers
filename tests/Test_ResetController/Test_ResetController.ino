/*
 Name:		Test_ResetController.ino
 Created:	7/1/2019 9:03:55 PM
 Author:	LooUQ Inc
*/

// the setup function runs once when you press reset or power the board
#include <CoreZero.h>
using namespace CoreZero::Management;

ResetController::ResetCause resetCause;

static const char* Map_Causes[] = {
	"PowerOn", "BOD12", "BOD33", "NVM", "External", "Watchdog", "System", "Backup"
};

void setup() {
	Serial.begin(115200);

	resetCause = ResetController::GetResetCause();
}

// the loop function runs over and over again until power down or reset
void loop() {
	Serial.print("Cause of reset: ");
	Serial.println(Map_Causes[(int)log2(resetCause)]);
	delay(5000);
}