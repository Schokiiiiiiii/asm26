
#ifndef LIBLEDS_H
#define LIBLEDS_H

#include <stdint.h>

// Permet d'initaliser le registre des leds
extern void     init_reg_leds(void);
// Permet de lire la valeur du registre des leds
extern uint32_t get_reg_leds(void);
// Permet d'écrire la valeur du registre des leds
extern void     set_reg_leds(uint32_t val);

#endif /* LIBLEDS_H */
