#include <zephyr/bluetooth/addr.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#ifdef __has_include("defines.h")
  #include "defines.h"
#endif

#ifdef CONFIG_GPIO_HELPER
#include "cgpio/gpio_helper.h"
#endif

LOG_MODULE_REGISTER(NRF5340_SensorNode, LOG_LEVEL_DBG);

#if ADD_TEMP_ADC == ON
static uint8_t measured_temperature;
static void sensor_update_thread_handler(struct adc_dt_spec *adc_channel) {

}
static const struct adc_dt_spec adc_ch0 = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));
int initADC();
K_THREAD_DEFINE(sensor_update_thread_id, 1024, sensor_update_thread_handler,
                &adc_ch0, NULL, NULL, 7, K_USER, 0);


#endif


static struct bt_conn *my_conn = NULL;

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
  if (err) {
    LOG_ERR("Connection err %d", err);
    return;
  }
  LOG_INF("Connected");
  my_conn = bt_conn_ref(conn);

  // task should be executed by submitting task to system queue
  gpio_pin_set_dt(&led0, GPIO_ACTIVE_LOW);
}

static void disconnected_cb(struct bt_conn *conn, uint8_t reason) {

  LOG_INF("Device disconnected. Reason %d", reason);

  bt_conn_unref(my_conn);

  // task should be executed by submitting task to system queue
  gpio_pin_set_dt(&led0, GPIO_ACTIVE_HIGH);
}

static void recycled_cb(void) { printk("Recycled_cb\n"); };

BT_CONN_CB_DEFINE(conn_callbacks) = {
    .recycled = recycled_cb,
    .connected = connected_cb,
    .disconnected = disconnected_cb,
};
// ADC

int main(void) {

  int err;
  init_GPIO();


#if ADD_TEMP_ADC == ON
  uint16_t buffer;
  struct adc_sequence adc_seq = {
      .buffer = &buffer,
      .buffer_size = sizeof(buffer),
  };

  err = initADC(&adc_ch0,&adc_seq);
  //TODO create function to read certain ADC

  while (true) {
    // DO something
    err = adc_read_dt(&adc_ch0, &adc_seq);
    if (err < 0) {
      LOG_ERR("Failed to read out sensor through adc.. with error %d ", err);
    }
    int32_t val_mv = buffer;

    err = adc_raw_to_millivolts_dt(&adc_ch0, &val_mv);
    if (err < 0) {
      LOG_WRN(" (value in mV not available)\n");
    } else {
      measured_temperature = (uint8_t)(val_mv / 10);
      LOG_INF("Measured voltage is %d mV and corresponding Temperature in Degrees Celsius %d", val_mv, measured_temperature);
    }
    k_sleep(K_SECONDS(5));
  }
#endif
  // while (true) {

  //   k_msleep(1000);
  // }

  // INIT ADC END

  // err = bt_addr_le_from_str("FF:EE:DD:CC:BB:AA", "random", &addr);
  // if (err) {
  //   printk("Invalid BT address (err %d)\n", err);
  // }

  // err = bt_id_create(&addr, NULL);
  // if (err < 0) {
  //   printk("Creating new ID failed (err %d)\n", err);
  // }
  // err = bt_addr_le_from_str("FF:EE:DD:CC:BB:AA", "random", &addr);
  // if (err) {
  //   printk("Failed to create a random address");
  // }

  // if (bt_enable(NULL)) {
  //   LOG_ERR("Failed to enable bluetooth stack!\n");
  // } else {
  //   LOG_INF("Enabled bluetooth stack on device (%s)\n", DEVICE_NAME);
  // }

  // if (bt_set_appearance(BT_APPEARANCE_SENSOR_MULTI)) {
  //   LOG_ERR("Setting up of the device appearance failed!");
  // }
  // LOG_INF("Bluetooth device appearance is set to %d", bt_get_appearance());

  // bt_le_adv_start(adv_param, adv, ARRAY_SIZE(adv), sr, ARRAY_SIZE(sr));
  // if (err) {
  //   LOG_ERR("Failed to start advertising...");
  // } else {
  //   LOG_INF("Started advertising successfully!");
  // }

  return 0;
}

int initADC(const struct adc_dt_spec * adc_dev, struct adc_sequence * sequence){
  int err;
  if (!adc_is_ready_dt(adc_dev)) {
    LOG_ERR("ADC %s with channel %d is not ready yet", adc_dev->dev->name,
            adc_dev->channel_id);
    return -1;
  }

  err = adc_channel_setup_dt(adc_dev);
  if (err < 0) {
    LOG_ERR("ADC channel setup failed with error %d", err);
    return err;
  } else {
    LOG_INF("ADC channel setup successfull");
  }

  err = adc_sequence_init_dt(adc_dev, sequence);
  if (err < 0) {
    LOG_ERR("FAILED to init adc sequence with error %d", err);
    return 0;
  } else {
    LOG_INF("ADC sequence init successfull");
  }

  return err;

};