#include "ev3dev.h"
#include <iostream>

/*################## BEGIN1: Structure to store ANGLE and DISTANCE under one variable ang_dist ###### */

struct ang_dist {
  float a;
  float d;
};

/*################## BEGIN2: List of GLOBALS used by the program ######################### */

#define minDist 5			// Global MINIMUM DISTANCE
#define maxDist 90			// Global MAXIMUM DISTANCE

/*################## BEGIN3: List of GLOBALS used by the program ######################### */
int ls, rs;		// ls: Speed of the Left motor M1 and rs: Speed of the Right motor M2 respectively

/*################### BEGIN4: List of functions used by the program ###################### */

void waitForRelease();
void backToHomePosition();
struct ang_dist findObjectCoordinates();


/*##################	 --> END <--	 ######################### */
using namespace ev3dev;

int main()
{	
	touch_sensor t1 = touch_sensor(INPUT_1);
	ultrasonic_sensor u1 = ultrasonic_sensor(INPUT_4);
	motor M3 = motor(OUTPUT_D, motor::motor_medium);
	
	M3.set_position(0);		// Set the current position of Motor M3 to 0 degrees
	struct ang_dist target;
	
	do {
		/* Start the CODE with PRESS and RELEASE of TOUCH SENSOR */
		if (t1.is_pressed()==1) {
			waitForRelease();
						
			do {
				target = findObjectCoordinates();	// This finds the coordinates (angle & distance) of the nearest object, during the scan of the Ultrasonic sensor
				
				/* Using PRINT STATEMENTS to DEBUG TARGET DISTANCE and TARGET ANGLE values */
				std::cout << "TARGET DISTANCE: " << target.d << std::endl;
				std::cout << "TARGET ANGLE: " << target.a << std::endl;
				
			} while(!t1.is_pressed());	//	The loop ends with the press of the Touch sensor and all the motors are stopped


			backToHomePosition();
			waitForRelease();
		}
		
	} while ( !button::enter.pressed() );		//	The program is running until the MAIN BUTTON is pressed
	return 0;

}

/*	FUNCTION 1: Wait until Touch sensor is released */
void waitForRelease() {
	touch_sensor t1 = touch_sensor(INPUT_1);
	do {

		} while (t1.is_pressed() != 0);
}

/*	FUNCTION2: This function makes the MOTOR M3 back to ZERO POSITION */
void backToHomePosition() {
	motor M3 = motor(OUTPUT_D, motor::motor_medium);	
	
	M3.set_position_sp(0);
	M3.set_duty_cycle_sp(25);
	M3.run_to_abs_pos();

	do {
	
	} while(M3.position() < 0);
}

/*	FUNCTION3: Find the coordinates of the NEAREST OBJECT */
struct ang_dist findObjectCoordinates() {
	
	struct ang_dist nearest; 		// Struct to return the nearest angle and nearest distance to the object 
	ultrasonic_sensor u1 = ultrasonic_sensor(INPUT_4);
	motor M3 = motor(OUTPUT_D, motor::motor_medium);
	touch_sensor t1 = touch_sensor(INPUT_1);

	nearest.a = M3.position();
	nearest.d = u1.distance_centimeters();
	
	/* Rotate LEFT */
	M3.set_position_sp(-50);
	M3.set_duty_cycle_sp(25);
	M3.run_to_abs_pos();
	
	
	do {	
		if(u1.distance_centimeters() < nearest.d) {
			nearest.d = u1.distance_centimeters();
			nearest.a = M3.position();			
		}
	} while ((M3.position() > -45) && (!t1.is_pressed()));		//	It ensures that the Ultrasonic sensors sweeps upto -45 deg, and breaks if the Touch sensor is pressed
	
	/* Rotate RIGHT */
	M3.set_position_sp(50);
	M3.set_duty_cycle_sp(25);
	M3.run_to_abs_pos();
	

	do {	
		if(u1.distance_centimeters() < nearest.d) {
			nearest.d = u1.distance_centimeters();
			nearest.a = M3.position();			
		}
	} while ((M3.position() < 45) && (!t1.is_pressed()));		//	It ensures that the Ultrasonic sensors sweeps upto -45 deg, and breaks if the Touch sensor is pressed
	
	return nearest;		//	Return the coordinates of the nearest Object
}
