/* Note: This is COMPLETED SOURCE CODE of the Exercise 1-6 of Cyber Physical Systems Lab1. 
	1. It is written to find the TARGET distance and TARGET angle of the NEAREST object, and print the values of the same to the console
	2. Once the CORRECT TARGET values are found, next is to find a way to TRACK and FOLLOW the NEAREST object
	3. For TRACKING and FOLLOWING appropriate speed values for both driving motors as a function of the distance and angle, was calculated.
*/

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
void startScan();
void backToHomePosition();
struct ang_dist findObjectCoordinates();
void followObject(struct ang_dist an_di);
void run(float d);
void turnLeft(float a, float d);
void turnRight(float a, float d);
void stop();

/*##################	 --> END <--	 ######################### */

using namespace ev3dev;

int main()
{	
	touch_sensor t1 = touch_sensor(INPUT_1);
	ultrasonic_sensor u1 = ultrasonic_sensor(INPUT_4);
	motor M3 = motor(OUTPUT_D, motor::motor_medium);
	
	M3.set_position(0);		// Set the current position of Motor M3 to 0 degrees
	struct ang_dist target;
	
	/* startScan() below is just to START Motor M3 for and SCAN for 1 rotation, just to get started, and return back to Home position (0 degrees) */
	startScan();	
	backToHomePosition();
	
	do {
		/* Start the CODE with PRESS and RELEASE of TOUCH SENSOR */
		if (t1.is_pressed()==1) {
			waitForRelease();
						
			do {
				target = findObjectCoordinates();	// This finds the coordinates (angle & distance) of the nearest object, during the scan of the Ultrasonic sensor
		
				/*	The tracking of the object is only performed, if the object lies within the Limits (Min = 5 and Max = 90)	*/
				if (target.d >= minDist && target.d <= maxDist) {
					followObject(target);		// Based on the coordinates of the nearest object (TARGET), the Robot tracks and follows the Object
				}
				else {
					std::cout << "TARGET OUT OF REACH!! "<< std::endl;
					std::cout << "TARGET DISTANCE: " << target.d << std::endl;
					std::cout << "TARGET ANGLE: " << target.a << std::endl;
					stop();
				}
			} while(!t1.is_pressed());	//	The loop ends with the press of the Touch sensor and all the motors are stopped

			stop();
			M3.stop();
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

/*	FUNCTION 2: Just getting WARMED UP */
void startScan() {
	ultrasonic_sensor u1 = ultrasonic_sensor(INPUT_4);
	motor M3 = motor(OUTPUT_D, motor::motor_medium);

	/* Rotate LEFT */
	M3.set_position_sp(-45);
	M3.set_duty_cycle_sp(25);
	M3.run_to_abs_pos();
	
	do {	
		
	} while (M3.position() > -45);
	
	/* Rotate RIGHT */
	M3.set_position_sp(45);
	M3.set_duty_cycle_sp(25);
	M3.run_to_abs_pos();
	

	do {	
		
	} while (M3.position() < 45);
}

/*	FUNCTION3: This function makes the MOTOR M3 back to ZERO POSITION */
void backToHomePosition() {
	motor M3 = motor(OUTPUT_D, motor::motor_medium);	
	
	M3.set_position_sp(0);
	M3.set_duty_cycle_sp(25);
	M3.run_to_abs_pos();

	do {
	
	} while(M3.position() < 0);
}

/*	FUNCTION4: Find the coordinates of the NEAREST OBJECT */
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

/*	FUNCTION5: Follow the nearest object with the specified coordinates	*/
void followObject(struct ang_dist an_di) {
	float a = an_di.a;
	float d = an_di.d;

	if (a < 0) {
		turnLeft(a,d);
	}
	else if (a > 0) {
		turnRight(a,d);
	}
	
	else {
		run(d);		
	}
}

/*	FUNCTION6: Run Primary Motors or DRIVE STRAIGHT	*/
void run(float d) {
	motor M1 = motor(OUTPUT_A, motor::motor_large);
	motor M2 = motor(OUTPUT_B, motor::motor_large);
	
	int sp;		// Speed of the Primary motors
	
	if(d > minDist && d < 50) {
		sp = int(2*d);	// A factor of 2 was used to BOOST speed at lower distances (d < 50cm)		
	} else {
			sp = 100;
	}

	M1.set_duty_cycle_sp(sp);
	M2.set_duty_cycle_sp(sp);

	M1.run_direct();
	M2.run_direct();

}

/*	FUNCTION7: Turn Left	*/
void turnLeft(float a, float d) {
	motor M1 = motor(OUTPUT_A, motor::motor_large);
	motor M2 = motor(OUTPUT_B, motor::motor_large);

	ls = int(d);
	int diff = int(-a);		// Difference between speeds of Motors M1 and M2, and is proportional to the angle measured by the Ultrasonic sensor
	rs = diff + ls;

	if(rs > 100) {
		rs = 100;		// Run at fastest possible SPPED for distances greates than or equal to 50cm
		ls = rs - diff;
	}	

	M1.set_duty_cycle_sp(ls);
	M2.set_duty_cycle_sp(rs);

	M1.run_direct();
	M2.run_direct();

}

/*	FUNCTION8: Turn Right	*/
void turnRight(float a, float d) {
	motor M1 = motor(OUTPUT_A, motor::motor_large);
	motor M2 = motor(OUTPUT_B, motor::motor_large);
	 
	rs = int(d);
	int diff = int(a);
	ls = diff + rs;

	if(ls > 100) {
		ls = 100;
		rs = ls - diff;
	}	

	M1.set_duty_cycle_sp(ls);
	M2.set_duty_cycle_sp(rs);

	M1.run_direct();
	M2.run_direct();
}

/*	FUNCTION9: Stop Primary MOTORS	*/
void stop() {
	motor M1 = motor(OUTPUT_A, motor::motor_large);
	motor M2 = motor(OUTPUT_B, motor::motor_large);

	M1.stop();
	M2.stop();
}
