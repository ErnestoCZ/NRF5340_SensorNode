#include <zephyr/bluetooth/addr.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#ifdef CONFIG_GPIO_HELPER
#include "gpio_helper.h"
#endif

LOG_MODULE_REGISTER(NRF5340_SensorNode, LOG_LEVEL_DBG);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

static struct bt_conn * my_conn = NULL;

// BLE relevant initalizations
static bt_addr_le_t addr;
static const struct bt_le_adv_param adv_param[] = BT_LE_ADV_PARAM(
    BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_USE_IDENTITY, 800, 801, NULL);
static const struct bt_data adv[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static struct bt_data sr[] = {
    BT_DATA_BYTES(
        BT_DATA_UUID128_ALL,
        BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785feabcd123)),
};

static void connected_cb(struct bt_conn *conn, uint8_t err) {
  if(err){
    LOG_ERR("Connection err %d",err);
    return;
  }
  LOG_INF("Connected");
  my_conn = bt_conn_ref(conn);
  
  //task should be executed by submitting task to system queue
  gpio_pin_set_dt(&led0, GPIO_ACTIVE_LOW);

}

static void disconnected_cb(struct bt_conn *conn, uint8_t reason) {

  LOG_INF("Device disconnected. Reason %d", reason);

  bt_conn_unref(my_conn);

  //task should be executed by submitting task to system queue
  gpio_pin_set_dt(&led0, GPIO_ACTIVE_HIGH);
}

static void recycled_cb(void) { printk("Recycled_cb\n"); };

BT_CONN_CB_DEFINE(conn_callbacks) = {
    .recycled = recycled_cb,
    .connected = connected_cb,
    .disconnected = disconnected_cb,
};


int main(void) {

  int err;
  init_GPIO();

  err = bt_addr_le_from_str("FF:EE:DD:CC:BB:AA", "random", &addr);
  if (err) {
    printk("Invalid BT address (err %d)\n", err);
  }

  err = bt_id_create(&addr, NULL);
  if (err < 0) {
    printk("Creating new ID failed (err %d)\n", err);
  }
  err = bt_addr_le_from_str("FF:EE:DD:CC:BB:AA", "random", &addr);
  if (err) {
    printk("Failed to create a random address");
  }

  if (bt_enable(NULL)) {
    LOG_ERR("Failed to enable bluetooth stack!\n");
  } else {
    LOG_INF("Enabled bluetooth stack on device (%s)\n", DEVICE_NAME);
  }

  if (bt_set_appearance(BT_APPEARANCE_SENSOR_MULTI)) {
    LOG_ERR("Setting up of the device appearance failed!");
  }
  LOG_INF("Bluetooth device appearance is set to %d", bt_get_appearance());

  bt_le_adv_start(adv_param, adv, ARRAY_SIZE(adv), sr, ARRAY_SIZE(sr));
  if (err) {
    LOG_ERR("Failed to start advertising...");
  } else {
    LOG_INF("Started advertising successfully!");
  }

  return 0;
}
