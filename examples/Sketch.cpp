/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

/*
 Name:		SAMD_Timer.ino
 Created:	6/29/2019 2:20:53 AM
 Author:	LooUQ Inc
*/

#include <Arduino.h>
#include "Timer.h"
//Beginning of Auto generated function prototypes by Atmel Studio
void onElapse();
//End of Auto generated function prototypes by Atmel Studio



void onElapse()
{
	static int toggle = 0;
	digitalWrite(LED_BUILTIN, toggle);
	toggle = toggle ? 0 : 1;
}



// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
	Serial.begin(115200);

	auto timer_1 = new System::Timers::Timer(5);

	timer_1->Initialize();
	timer_1->Elapsed += onElapse;
	timer_1->Start(500);
	
	TC5->COUNT16.READREQ.reg |= TC_READREQ_ADDR(TC_COUNT16_COUNT_OFFSET);
}

// the loop function runs over and over again until power down or reset
void loop() {
	delay(1000);

#ifdef DEBUG

	//Serial.print("COUNT - ");
	//TC5->COUNT16.READREQ.bit.RREQ = 1;
	//while (TC5->COUNT16.STATUS.bit.SYNCBUSY)
		//;
	//Serial.println(TC5->COUNT16.COUNT.reg);

#endif // DEBUG
}
