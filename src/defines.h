#ifndef CUSTOM_DEFINES_H 
#define CUSTOM_DEFINES_H
//Function declaration 

#define ON 1
#define OFF 0

//SensorNode behavior defines
#define ADD_TEMP_ADC OFF




//Bluetooth Low Energy - General defines like device name set into KConfigs
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)


#endif //CUSTOM_DEFINES_H