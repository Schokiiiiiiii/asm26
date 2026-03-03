#include <autopilot.h>
#include <gyroscope.h>
#include <navigation.h>

void autopilot_init(void)
{
	/* TODO: initialize the autopilot system */

	gyro_init();
}

void autopilot_activate(void)
{
	int *gpx;
	struct coord *sys_coord;
	char *gyro_data;

	/* TODO: activate the autopilot system */
	gyro_calibrate();
	gyro_capture(gyro_data);

	navigation_compute_destination(gpx);

	sys_coord = navigation_get_coord();

}

void autopilot_deactivate(void)
{
	/* TODO: deactivate the autopilot system */
}
