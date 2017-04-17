#include "ev3dev.h"
#include <iostream>

using namespace ev3dev;

int main()
{
	std::cout << "Hello CPS Course." << std::endl;
	std::cout << "Press main button to leave ..." << std::endl;
	// Wait for main button is pressed

	motor M1 = motor(OUTPUT_A, motor::motor_large);
	motor M2 = motor(OUTPUT_B, motor::motor_large);
	touch_sensor t1 = touch_sensor(INPUT_1);
	int flag=0;
	
	M1.set_duty_cycle_sp(50);
	M2.set_duty_cycle_sp(50);
	
	driveOnCommand();
}

/* The below function driveOnCommand does as illustrated in Task 1-3 of the Lab1 sheet */
void driveOnCommand(){
	do {
		do {
		
		if(t1.is_pressed) {
			waitForPressAndRelease();
		}
		
		M1.run_direct();
		M2.run_direct();
		
	} while(!t1.is_pressed);
	
	M1.stop();
	M2.stop();
	
	} while(!button::enter.pressed());  // Run driveOnCommand infinitely until we use the Centre EV3 Button!!
}

/* This is a waiting function for the Press and release of Touch sensor to run the motors */
void waitForPressAndRelease() {
	touch_sensor t1 = touch_sensor(INPUT_1);
	
	do {

		} while (t1.is_pressed() != 0);
}