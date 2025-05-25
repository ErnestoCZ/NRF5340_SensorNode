#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(NRF5340_SensorNode, LOG_LEVEL_DBG);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
// #define DEVICE_NAME_LENGTH 

int main(void) {

  int err;

  err = bt_enable(NULL);
  if (err) {
    LOG_ERR("Failed to enable bluetooth stack!\n");
  } else {
    LOG_INF("Enabled bluetooth stack on device (%s)\n", DEVICE_NAME);
    
  }




  return 0;
}
