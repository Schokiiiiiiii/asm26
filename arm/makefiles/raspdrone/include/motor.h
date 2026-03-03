#define MOTOR_ROTOR_1 	0x0
#define MOTOR_ROTOR_2	0x1
#define MOTOR_ROTOR_3   0x2
#define MOTOR_ROTOR_4	0x3

void motor_init(int motor_id);
void motor_set_power(int motor_id, int power_level);
int motor_get_power(int motor_id);

