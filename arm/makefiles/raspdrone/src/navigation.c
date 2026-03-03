#include <navigation.h>
#include <gyroscope.h>
#include <camera.h>

int navigation_compute_destination(int *gpx)
{
	int *img;
	char *gyro_data;

	/* TODO: compute the navigation based on GPS coordinates. */
	gyro_capture(gyro_data);
	img = camera_acquire();

	return 0;
}

struct coord *navigation_get_coord(void)
{
	/* TODO: returns the internal coordinate based on the navigation plan */
	return (struct coord *) 0x1000;
}
