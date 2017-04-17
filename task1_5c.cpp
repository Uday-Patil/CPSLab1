#include "ev3dev.h"
#include <iostream>

using namespace ev3dev;

int main()
{
	std::cout << "Press main button to leave ..." << std::endl;
	// Wait for main button is pressed
	float dist;
	int l_angle[];
	int r_angle[];
	dec = 0;
	inc = 0;
	
	float measure_angle;
	ultrasonic_sensor u1 = ultrasonic_sensor(INPUT_4);
	motor M3 = motor(OUTPUT_D, motor::motor_medium);
	
	float nd = u1.distance_centimeters();
	float na = M3.position;
	
	M3.set_position(0);		// Set the current angle of the motor to 0
	do {
		do {
		
			if(t1.is_pressed) {
				waitForPressAndRelease(); // Wait for press and release for the first time
			}
		
		/* Initalisation of LEFT and RIGHT angles for rotation of ULTRASONIC SENSOR MOTOR */
			for(i=0; i<30; i++) 
			{
				l_angle[i] = dec - 5;
				dec = l_angle[i];
				r_angle[i] = inc + 5;
				inc = r_angle[i];
			}
	
		/* Start rotating CCW and keep a note of the nearest object */
			for(i=0; i<30; i++) 
			{
				dist = u1.distance_centimeters();
				M3.set_position_sp(l_angle[i]);
				M3.set_duty_cycle_sp(30);
				M3.run_to_abs_pos();
		
				if(dist < nearest) {
					na = l_angle[i];
					nd = dist;
				}
			}
	
		/* Start rotating Clk wise and keep a note of the nearest object */
			for(i=0; i<30; i++) 
			{
				dist = u1.distance_centimeters();
				M3.set_position_sp(r_angle[i]);
				M3.set_duty_cycle_sp(30);
				M3.run_to_abs_pos();
		
				if(dist < nearest) {
					na = l_angle[i];
					nd = dist;
				}
			}
		} while(!t1.is_pressed);
	
		M3.stop();
		
		std::cout << "Distance of nearest object: " << nd << std::endl;		// Print the distance to the nearest object	
		std::cout << "Angle to the nearest object " << na << std::endl;		// Print the angle to the nearest object
	
	} while(!button::enter.pressed());  // Run driveOnCommand infinitely until we use the Centre EV3 Button!!
}

void waitForPressAndRelease() {
	touch_sensor t1 = touch_sensor(INPUT_1);
	
	do {

		} while (t1.is_pressed() != 0);
}