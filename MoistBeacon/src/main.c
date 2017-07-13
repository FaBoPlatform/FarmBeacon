// MoistBeacon for nRF5 SDK 12.2.0

#include <string.h>
#include "bsp.h"
#include "boards.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "app_timer.h"
#include "app_button.h"
#include "app_uart.h"

#include "adc.h"
#include "beacon.h"
#include "blestack.h"


#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE         4                                           /**< Size of timer operation queues. */
#define ADC_TIMER_INTERVAL              1000                                        /**< 1s. */

// ターゲットUUID
static uint8_t uuid[16] = {0xcb,0x86,0xbc,0x31,0x05,0xbd,0x40,0xcc,0x90,0x3d,0x1c,0x9b,0xd1,0x3d,0x96,0x6b};	

// ADCチェックタイマー
APP_TIMER_DEF(adc_timer_id);

// Beacon情報
static beacon_info_t beacon_info;

// エラーハンドラ
__WEAK void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
    switch (id)
    {
        case NRF_FAULT_ID_SDK_ASSERT: {
            NRF_LOG_ERROR("**Assert!**\r\n");
            volatile assert_info_t *assert_info = (assert_info_t*)info;
            NRF_LOG_ERROR("Line:%d\r\n", assert_info->line_num);
            NRF_LOG_ERROR("File:%c\r\n", assert_info->p_file_name[0]);
        } break;
        case NRF_FAULT_ID_SDK_ERROR: {
            NRF_LOG_ERROR("**Error!**\r\n");
            volatile error_info_t *error_info = (error_info_t*)info;
            NRF_LOG_ERROR("Line:%d\r\n", error_info->line_num);
            NRF_LOG_ERROR("File:%s\r\n", nrf_log_push((char*)error_info->p_file_name));
            NRF_LOG_ERROR("Code:%d\r\n", error_info->err_code);
        } break;
    }
    NRF_LOG_FINAL_FLUSH();

    NVIC_SystemReset();
}

// Timerイベント
static void timeout_handler(void * p_context)
{
    if (p_context == adc_timer_id) {
        // Beacon開始
        int16_t val = adc_check();
        NRF_LOG_INFO("*val:%d\r\n", val);
        beacon_info.minor = val;
        beacon_init(beacon_info);
        beacon_start();
    }
}


// BLEイベントハンドラ
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
}


int main()
{
    uint32_t err_code;

    // ログ初期化
    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("**started**\r\n");

    // Timer初期化
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
    err_code = app_timer_create(&adc_timer_id, APP_TIMER_MODE_REPEATED, timeout_handler);
    APP_ERROR_CHECK(err_code);

    // BLEStack初期化
    ble_stack_init(on_ble_evt);

    // DCDCを有効化
#if (NRF_SD_BLE_API_VERSION == 3)
    // NRF_POWER->DCDCEN = 1;
    sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);
#endif

    // BSP初期化
    err_code = bsp_init(BSP_INIT_LED, APP_TIMER_TICKS(100, APP_TIMER_PRESCALER), NULL);
    APP_ERROR_CHECK(err_code);

    // ADC初期化
    adc_init(NRF_SAADC_INPUT_AIN1);
    int16_t val = adc_check();
    NRF_LOG_INFO("*val:%d\r\n", val);

    // Beacon情報初期化
    beacon_info.major = 0x00;
    beacon_info.minor = val;
    beacon_info.interval = 600;
    beacon_info.measured_pow = 0xC4;
    beacon_info.channel_mask.ch_37_off = false;
    beacon_info.channel_mask.ch_38_off = false;
    beacon_info.channel_mask.ch_39_off = false;
    memcpy(beacon_info.uuid, uuid, 16);

    // Timer開始
    err_code = app_timer_start(adc_timer_id, APP_TIMER_TICKS(ADC_TIMER_INTERVAL, 0), (void*)adc_timer_id);
    APP_ERROR_CHECK(err_code);

    // LED点滅
    err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
    APP_ERROR_CHECK(err_code);

    while(true){
        if (NRF_LOG_PROCESS() == false) {
            err_code = sd_app_evt_wait();
            APP_ERROR_CHECK(err_code);
        }
    }

    return 0;
}
