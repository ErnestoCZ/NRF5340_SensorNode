#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(NRF5340_SensorNode, LOG_LEVEL_DBG);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
// #define DEVICE_NAME_LENGTH 

static const struct bt_le_adv_param bt_param = BT_LE_ADV_PARAM_INIT(BT_LE_ADV_NCONN,800,800,0); 

static const struct bt_data adv_data[] = {
        BT_DATA_BYTES(BT_DATA_FLAGS,BT_LE_AD_NO_BREDR),
        BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, sizeof(DEVICE_NAME) -1),
};

static unsigned char url_data[] ={0x17,'/','/','a','c','a','d','e','m','y','.',
 'n','o','r','d','i','c','s','e','m','i','.',
 'c','o','m'};

static const struct bt_data scan_res[] = {
        BT_DATA(BT_DATA_URI, url_data, sizeof(url_data)),
};

int main(void) {

  int err;

  err = bt_enable(NULL);
  if (err) {
    LOG_ERR("Failed to enable bluetooth stack!\n");
  } else {
    LOG_INF("Enabled bluetooth stack on device (%s)\n", DEVICE_NAME);
    err = bt_le_adv_start(BT_LE_ADV_NCONN, adv_data, ARRAY_SIZE(adv_data), scan_res, ARRAY_SIZE(scan_res));

    if (err)
    {
        LOG_ERR("Failed to start advertisments/n");
    }
    else
    {
        LOG_INF("Advertisements started");
    }
    
    
  }




  return 0;
}
