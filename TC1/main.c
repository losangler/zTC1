#include "main.h"

#include "user_gpio.h"
#include "user_wifi.h"
#include "user_rtc.h"
#include "user_udp.h"
#include "user_mqtt_client.h"

#define os_log(format, ...)  custom_log("TC1", format, ##__VA_ARGS__)

char rtc_init = 0;    //sntpУʱ�ɹ���־λ

char strMac[16] = { 0 };

system_config_t * sys_config;
user_config_t * user_config;

mico_gpio_t Relay[Relay_NUM] = { Relay_0, Relay_1, Relay_2, Relay_3, Relay_4, Relay_5 };

/* MICO system callback: Restore default configuration provided by application */
void appRestoreDefault_callback( void * const user_config_data, uint32_t size )
{
    int i, j;
    UNUSED_PARAMETER( size );

    if ( strMac[0] == 0 )
    {
        IPStatusTypedef para;
        micoWlanGetIPStatus( &para, Station );
        strcpy( strMac, para.mac);
    }

    unsigned char mac1,mac2;
    mac1=strtohex(strMac[8],strMac[9]);
    mac2=strtohex(strMac[10],strMac[11]);


    sprintf( mico_system_context_get( )->micoSystemConfig.name, ZTC1_NAME,mac1,mac2 );
    user_config_t* userConfigDefault = user_config_data;

    userConfigDefault->mqtt_ip[0] = 0;
    userConfigDefault->mqtt_port = 0;
    userConfigDefault->mqtt_user[0] = 0;
    userConfigDefault->mqtt_password[0] = 0;

    userConfigDefault->idx = -1;
    userConfigDefault->version = USER_CONFIG_VERSION;
    for ( i = 0; i < PLUG_NUM; i++ )
    {
        userConfigDefault->plug[i].idx = -1;
        sprintf( userConfigDefault->plug[i].name, "����%d", i );
        for ( j = 0; j < PLUG_TIME_TASK_NUM; j++ )
        {
            userConfigDefault->plug[i].task[j].hour = 0;
            userConfigDefault->plug[i].task[j].minute = 0;
            userConfigDefault->plug[i].task[j].repeat = 0x80;
            userConfigDefault->plug[i].task[j].on = 0;
            userConfigDefault->plug[i].task[j].action = 1;
        }
    }

//    mico_system_context_update( sys_config );

}

int application_start( void )
{
    int i, j;
    os_log( "Start" );

    OSStatus err = kNoErr;

    for ( i = 0; i < Relay_NUM; i++ )
    {
        MicoGpioInitialize( Relay[i], OUTPUT_PUSH_PULL );
        //MicoGpioOutputHigh(Relay[i]);
    }
    /* Create mico system context and read application's config data from flash */
    sys_config = mico_system_context_init( sizeof(user_config_t) );
    user_config = ((system_context_t *) sys_config)->user_config_data;
    require_action( user_config, exit, err = kNoMemoryErr );

    err = mico_system_init( sys_config );
    require_noerr( err, exit );



    MicoGpioInitialize( (mico_gpio_t) Button, INPUT_PULL_UP );
    if ( !MicoGpioInputGet( Button ) )
    {   //����ʱ��ť״̬
        os_log( "wifi_start_easylink" );
        wifi_status = WIFI_STATE_NOEASYLINK;  //wifi_init������easylink
    }

    MicoGpioInitialize( (mico_gpio_t) MICO_GPIO_5, OUTPUT_PUSH_PULL );
    user_led_set( 0 );

    if ( user_config->version != USER_CONFIG_VERSION || user_config->plug[0].task[0].hour < 0 || user_config->plug[0].task[0].hour > 23 )
    {
        os_log( "WARNGIN: user params restored!" );
        err = mico_system_context_restore( sys_config );
        require_noerr( err, exit );
    }

    os_log( "mqtt_ip:%s",user_config->mqtt_ip );
    os_log( "mqtt_port:%d",user_config->mqtt_port );
    os_log( "mqtt_user:%s",user_config->mqtt_user );
    os_log( "mqtt_password:%s",user_config->mqtt_password );

    os_log( "version:%d",user_config->version );
    os_log( "idx:%d",user_config->idx );
//    for ( i = 0; i < PLUG_NUM; i++ )
//    {
//        os_log("plug_%d:",i);
//        os_log("\tname:%s:",user_config->plug[i].name);
//        os_log("\tidx:%d:",user_config->plug[i].idx);
//        for ( j = 0; j < PLUG_TIME_TASK_NUM; j++ )
//        {
//            os_log("\t\ton:%d\t %02d:%02d repeat:0x%X",user_config->plug[i].task[j].on,
//                user_config->plug[i].task[j].hour,user_config->plug[i].task[j].minute,
//                user_config->plug[i].task[j].repeat);
//        }
//    }

    wifi_init( );
    key_init( );
    err = user_mqtt_init( );
    require_noerr( err, exit );
    err = user_rtc_init( );
    require_noerr( err, exit );

    err = user_udp_init( );
    while ( 1 )
    {
//        mico_thread_msleep(500);
//        MicoGpioOutputTrigger(MICO_GPIO_5);
//        mico_gpio_output_toggle( MICO_SYS_LED );
//        mico_rtos_delay_milliseconds(1000);
    }
    exit:
    os_log("application_start ERROR!");
    return 0;
}
