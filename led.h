#ifndef _LED_H
#define _LED_H

#define LED_1	(4)	/* D10 */
#define LED_2	(5)	/* D11 */
#define LED_4	(6)	/* D12 */

extern void led_init(void);
extern void turn_on_led(int led_idx);
extern void turn_off_led(int led_idx);


#endif

