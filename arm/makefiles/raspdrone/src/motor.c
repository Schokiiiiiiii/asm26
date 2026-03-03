#include <motor.h>

void motor_init(int motor_id)
{
	/* TODO: Initialize the motor corresponding
	 * to the <motor_id>
	 */
	motor_set_power(motor_id, 0);
}

void motor_set_power(int motor_id, int power_level)
{
	/* TODO: set the power level of the motor */

}

int motor_get_power(int motor_id)
{
	/* TODO: return the power level for this motor */
	return 0;
}
