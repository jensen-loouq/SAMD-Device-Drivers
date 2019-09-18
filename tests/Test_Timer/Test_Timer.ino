/*
 Name:		Test_Timer.ino
 Created:	7/1/2019 9:03:32 PM
 Author:	LooUQ Inc
*/

// the setup function runs once when you press reset or power the board
#include <CoreZero.h>
using namespace CoreZero::Timers;

void onElapsed()
{
	static int toggle = 0;
	digitalWrite(LED_BUILTIN, toggle);
	toggle = toggle ? 0 : 1;
}


void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	/* Timer */
	Timer& timer = *new Timer(5);
	
	timer.Initialize();
	timer.Elapsed += onElapsed;
	timer.Start(2000);
}

// the loop function runs over and over again until power down or reset
void loop() {
  
}
