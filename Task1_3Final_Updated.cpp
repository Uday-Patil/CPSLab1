/* Note: This is COMPLETED SOURCE CODE of the Exercise 1-3 (Drive on Command) of Cyber Physical Systems Lab1. 
	1. This program reads the value of the touch sensor and turns on (turns off resp.) both main motors when the touch sensor is pressed. 
*/

#include "ev3dev.h"
#include <iostream>

void waitForRelease();
using namespace ev3dev;

int main()
{
	std::cout << "Hello CPS Course." << std::endl;
	std::cout << "Press main button to leave ..." << std::endl;
	// Wait for main button is pressed

	motor M1 = motor(OUTPUT_A, motor::motor_large);
	motor M2 = motor(OUTPUT_B, motor::motor_large);
	touch_sensor t1 = touch_sensor(INPUT_1);
	int flag=2;		// Flag for checking EVEN-ODD functionality !!
	
	M1.set_duty_cycle_sp(50);
	M2.set_duty_cycle_sp(50);
		
	do {
	
	if (t1.is_pressed()==1 && flag%2==0) {
	/* Wait for press and release */
		waitForRelease();

		M1.run_direct();
		M2.run_direct();
		flag = 1;
	}
	
	if (t1.is_pressed() == 1 && flag%2!=0) {
			
		M1.stop();
		M2.stop();
		flag = 2;
	/* Wait for press and release */	
		waitForRelease();
	} 

	} while ( !button::enter.pressed() );
	
	M1.stop();
	M2.stop();
	return 0;
}


/*	FUNCTION 1: Wait until Touch sensor is released */
void waitForRelease() {
	touch_sensor t1 = touch_sensor(INPUT_1);
	do {

		} while (t1.is_pressed() != 0);
}