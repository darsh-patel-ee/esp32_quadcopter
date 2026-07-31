#include <stdio.h>
#include <stdbool.h>
#include "esp_log.h"
#include "esp_err.h"
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mpu6050.h"

static const char* TAG = "main";

static mpu6050_t mpu6050;
static mpu6050_raw_gyro_data_t raw_gyro;
static mpu6050_raw_accel_data_t raw_accel;
static mpu6050_raw_temp_data_t raw_temp;

TaskHandle_t mpu6050_read_gyro_task_handle = NULL;
TaskHandle_t mpu6050_read_accel_task_handle = NULL;
TaskHandle_t mpu6050_read_temp_task_handle = NULL;

void mpu6050_read_gryo_task(void *parameters)
{
    for (;;) {
        esp_err_t err = mpu6050_read_raw_gyro_data(&mpu6050, &raw_gyro);
        if (err == ESP_OK) {
            printf("gx=%d \t gy=%d \t gz=%d\n", raw_gyro.gyro_xout, raw_gyro.gyro_yout, raw_gyro.gyro_zout);
        } else {
            ESP_LOGE(TAG, "Gyro read failed: %s", esp_err_to_name(err));
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void mpu6050_read_accel_task(void *parameters)
{
    for (;;) {
        esp_err_t err = mpu6050_read_raw_accel_data(&mpu6050, &raw_accel);
        if (err == ESP_OK) {
            printf("ax=%d \t ay=%d \t az=%d\n", raw_accel.accel_xout, raw_accel.accel_yout, raw_accel.accel_zout);
        } else {
            ESP_LOGE(TAG, "Accel read failed: %s", esp_err_to_name(err));
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void mpu6050_read_temp_task(void *parameters)
{
    for (;;) {
        esp_err_t err = mpu6050_read_raw_temp_data(&mpu6050, &raw_temp);
        if (err == ESP_OK) {
            printf("temp=%d\n", raw_temp.temp_out);
        } else {
            ESP_LOGE(TAG, "Temp read failed: %s", esp_err_to_name(err));
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    i2c_master_bus_config_t i2c_mst_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .intr_priority = 0,
        .trans_queue_depth = 0,
        .flags.enable_internal_pullup = false,
        .flags.allow_pd = false,
    };

    i2c_master_bus_handle_t bus_handle;
    
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));
    ESP_ERROR_CHECK(mpu6050_init(&mpu6050, bus_handle, 0x68));

    BaseType_t result = xTaskCreate(
        mpu6050_read_gryo_task,
        "MPU6050 read gryo task",
        4096,
        NULL,
        1,
        &mpu6050_read_gyro_task_handle
    );

    if (result != pdPASS) {
        ESP_LOGE(TAG, "Failed to create gyro task");
    }

    result = xTaskCreate(
        mpu6050_read_accel_task,
        "MPU6050 read accel task",
        4096,
        NULL,
        1,
        &mpu6050_read_accel_task_handle
    );
    if (result != pdPASS) {
        ESP_LOGE(TAG, "Failed to create accel task");
    }

    result = xTaskCreate(
        mpu6050_read_temp_task,
        "MPU6050 read temp task",
        4096,
        NULL,
        1,
        &mpu6050_read_temp_task_handle
    );
    if (result != pdPASS) {
        ESP_LOGE(TAG, "Failed to create temp task");
    }

    while (1) {
        vTaskDelay(portMAX_DELAY);
    }
}
