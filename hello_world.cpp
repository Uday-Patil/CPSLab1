#include "ev3dev.h"
#include <iostream>

using namespace ev3dev;

int main()
{
	std::cout << "Hello CPS Course." << std::endl;
	std::cout << "Press main button to leave ..." << std::endl;
	// Wait for main button is pressed
	float dist;
	float angle;
	ultrasonic_sensor u1 = ultrasonic_sensor(INPUT_4);
	motor M3 = motor(OUTPUT_D, motor::motor_medium);


	do {
		dist = u1.distance_centimeters();
		std::cout << "Distance of nearest object: " << dist << std::endl;	
		angle = M3.position();
		std::cout << "Angle of the motor; " << angle << std::endl;
			
	} while ( !button::enter.pressed() );
	return 0;
}
