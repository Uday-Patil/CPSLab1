#include "ev3dev.h"
#include <iostream>

using namespace ev3dev;

int main()
{
	std::cout << "Press main button to leave ..." << std::endl;
	// Wait for main button is pressed
	float dist;
	float angle;
	ultrasonic_sensor u1 = ultrasonic_sensor(INPUT_4);
	motor M3 = motor(OUTPUT_D, motor::motor_medium);
	
	
//	M3.stop();	
	M3.set_position(0);
	do {
		
		M3.set_position_sp(-30);
		M3.set_duty_cycle_sp(50);
		M3.run_to_abs_pos();

	} while (M3.position() > -30);

	dist = u1.distance_centimeters();
	std::cout << "Distance of nearest object: " << dist << std::endl;	
	
	angle = M3.position();
	
	std::cout << "Angle of the motor; " << angle << std::endl;
	do {
		
			
	} while ( !button::enter.pressed() );
	return 0;
}
