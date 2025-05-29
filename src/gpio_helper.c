#include "gpio_helper.h"
#include <zephyr/kernel.h>

static int checkGPIO(const struct gpio_dt_spec *current_gpio){
    int err;
    printk("Checking gpio is ready... | PORT : %d  PIN : %d \n", (int)current_gpio->port, (int)current_gpio->pin);
    err = !gpio_is_ready_dt(current_gpio);
    if(err){
        printk("GPIO is not ready...!\n");
    };

    return err;
};

static int config_gpio_as_output(const struct gpio_dt_spec * current_gpio){
    int err;

    err = gpio_pin_configure_dt(current_gpio, GPIO_OUTPUT_ACTIVE);
    if(err != 0){
        printk("Configuration of GPIO : PORT %d  and PIN %d failed with error %d!\n", (int)current_gpio->port, (int)current_gpio->pin , err);
    }

    return err;
}

static void testSequence(const struct gpio_dt_spec * current_gpio){
    gpio_pin_toggle_dt(current_gpio);
    k_msleep(200);
    // gpio_pin_toggle_dt(current_gpio);
};

int init_GPIO(){

    //check for all gpios are ready 
    int err;

    err = checkGPIO(&led0);
    err = checkGPIO(&led1);
    err = checkGPIO(&led2);
    err = checkGPIO(&led3);

    config_gpio_as_output(&led0);
    config_gpio_as_output(&led1);
    config_gpio_as_output(&led2);
    config_gpio_as_output(&led3);


    //test led on boot
    testSequence(&led0);
    testSequence(&led1);
    testSequence(&led2);
    testSequence(&led3);

    return err;



}