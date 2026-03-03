#include <motor.h>
#include <autopilot.h>
#include <camera.h>
#include <gyroscope.h>

int engine_type=0x1234;

void init_system(void)
{
	/* TODO: initialize the system */

	return;
}

int main(int argc, char *argv[])
{
	/* Initializing raspdrone environment... */
	motor_init(MOTOR_ROTOR_1);
	motor_init(MOTOR_ROTOR_2);
	motor_init(MOTOR_ROTOR_3);
	motor_init(MOTOR_ROTOR_4);

	camera_init();
	gyro_init();

	autopilot_init();

	/* Raspdrone ready for taking off in a few seconds... */
	return 0;
}
