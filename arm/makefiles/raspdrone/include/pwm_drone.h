struct pwm_config;

void pwm_init(void);
int pwm_enable(struct pwm_config *cfg);
void pwm_disable(void);
