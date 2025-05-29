#ifndef GPIO_HELPER_H 
#define GPIO_HELPER_H
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

#define LED_0_NODE GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios)
#define LED_1_NODE GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios)
#define LED_2_NODE GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios)
#define LED_3_NODE GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios)

static const struct gpio_dt_spec led0 = LED_0_NODE;
static const struct gpio_dt_spec led1 = LED_1_NODE;
static const struct gpio_dt_spec led2 = LED_2_NODE;
static const struct gpio_dt_spec led3 = LED_3_NODE;


int init_GPIO();

#endif //GPIO_HELPER_H