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
		
	do {
	
	if (t1.is_pressed()==1 && flag==0) {
		do {

		} while (t1.is_pressed() != 0);

		M1.run_direct();
		M2.run_direct();
		flag=1;
	}
	
	if (t1.is_pressed() == 1 && flag==1) {
		do {

		} while (t1.is_pressed() != 0);	 
		
		M1.stop();
		M2.stop();
		flag = 0;
		} 
	} while ( !button::enter.pressed() );
	
	M1.stop();
	M2.stop();
	return 0;
}
