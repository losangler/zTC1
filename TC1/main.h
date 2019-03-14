#ifndef __MAIN_H_
#define __MAIN_H_

#include "mico.h"
#include "MiCOKit_EXT.h"

#define TYPE 1
#define TYPE_NAME "zTC1"

#define ZTC1_NAME "zTC1_%02X%02X"

#define USER_CONFIG_VERSION 1
#define SETTING_MQTT_STRING_LENGTH_MAX  32      //���� 4 �ֽڶ��롣

#define PLUG_NAME_LENGTH 32
#define PLUG_NUM 6              //��������
#define PLUG_TIME_TASK_NUM 5    //ÿ���������5�鶨ʱ����

#define Led         MICO_GPIO_5
#define Button      MICO_GPIO_23

#define Relay_ON     1
#define Relay_OFF     0

#define Relay_0     MICO_GPIO_6
#define Relay_1     MICO_GPIO_7
#define Relay_2     MICO_GPIO_8
#define Relay_3     MICO_GPIO_9
#define Relay_4     MICO_GPIO_10
#define Relay_5     MICO_GPIO_18
#define Relay_NUM   PLUG_NUM

typedef struct
{
    char hour;      //Сʱ
    char minute;    //����
    uint8_t repeat; //bit7:һ��   bit6-0:����-��һ
    char action;    //����
    char on;    //����
} user_plug_task_config_t;

typedef struct
{
    char name[PLUG_NAME_LENGTH];
    char idx;
    char on;    //��¼��ǰ����
    user_plug_task_config_t task[PLUG_TIME_TASK_NUM];

} user_plug_config_t;

//�û���������ṹ��
typedef struct
{
    char mqtt_ip[SETTING_MQTT_STRING_LENGTH_MAX];   //mqtt service ip
    int mqtt_port;        //mqtt service port
    char mqtt_user[SETTING_MQTT_STRING_LENGTH_MAX];     //mqtt service user
    char mqtt_password[SETTING_MQTT_STRING_LENGTH_MAX];     //mqtt service user
//     char mqtt_device_id[SETTING_MQTT_STRING_LENGTH_MAX];        //mqtt service user  device name

    char version;
    char idx;
    user_plug_config_t plug[PLUG_NUM];
} user_config_t;

extern char rtc_init;

extern char strMac[16];
extern system_config_t * sys_config;
extern user_config_t * user_config;

extern mico_gpio_t Relay[Relay_NUM];

#endif