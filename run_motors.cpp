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
	
	M1.set_duty_cycle_sp(50);
	M2.set_duty_cycle_sp(50);

	
	do {

		M1.run_direct();
		M2.run_direct();		

	} while ( !button::enter.pressed() );
	
	M1.stop();
	M2.stop();
	
	return 0;
}
