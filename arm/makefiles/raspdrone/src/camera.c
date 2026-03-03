#include <camera.h>

void camera_init(void)
{
	/* TODO: initialize the camera */
}

void camera_on(int resolution)
{
	/* TODO: enable the camera with a certain resolution */
}

int *camera_acquire(void)
{
	/* TODO: retrieve a pointer to an image. */
	return (int *) 0xa10000;
}
