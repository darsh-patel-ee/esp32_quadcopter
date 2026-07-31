#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../include/mpu6050.h"
#include "../include/mpu6050_regs.h"

static const char* TAG = "mpu6050";


/**
 * @brief Write data to a specified register.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[in] reg Register to write to.
 * 
 * @param[in] data Data to write.
 * 
 * @return
 *      -ESP_OK success
 */
static esp_err_t write_reg(mpu6050_t *dev, uint8_t reg, uint8_t data)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");

    uint8_t buf[2] = {reg, data};

    ESP_RETURN_ON_ERROR(
        i2c_master_transmit(dev->dev_handle, buf, sizeof(buf), 1000),
        TAG,
        "Failed to write to register."
    );

    return ESP_OK;
}

/**
 * @brief Read data from a specified register.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[in] reg Register to read from.
 * 
 * @param[out] data Reference to where the data should be written.
 * 
 * @param[in] len Size of data being read.
 * 
 * @return
 *      -ESP_OK success
 */
static esp_err_t read_reg(mpu6050_t *dev, uint8_t reg, uint8_t *data, size_t len)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");

    ESP_RETURN_ON_ERROR(
        i2c_master_transmit_receive(dev->dev_handle, &reg, 1, data, len, 1000),
        TAG,
        "Failed to reaad from register."
    );

    return ESP_OK;
}

esp_err_t mpu6050_init(mpu6050_t *dev, i2c_master_bus_handle_t bus_handle, uint8_t i2c_addr)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");
    ESP_RETURN_ON_FALSE(bus_handle != NULL, ESP_ERR_INVALID_ARG, TAG, "'bus_handle' is NULL.");

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = i2c_addr,
        .scl_speed_hz = 400000,
    };

    ESP_RETURN_ON_ERROR(i2c_master_probe(bus_handle, i2c_addr, 1000), TAG, "Failed to probe MPU6050.");

    ESP_RETURN_ON_ERROR(i2c_master_bus_add_device(bus_handle, &dev_config, &dev->dev_handle), TAG, "Failed to add MPU6050 to I2C bus.");
    
    ESP_RETURN_ON_ERROR(mpu6050_set_sleep(dev, 0), TAG, "Failed to wake up device.");
    
    uint8_t who_am_i;
    ESP_RETURN_ON_ERROR(mpu6050_who_am_i(dev, &who_am_i), TAG, "Failed to verify device.");
    ESP_LOGI(TAG, "WHO_AM_I = 0x%02X", who_am_i);
    ESP_RETURN_ON_FALSE((i2c_addr & 0xFE) == who_am_i, ESP_ERR_INVALID_ARG, TAG, "Incorrect I2C address.");

    vTaskDelay(pdMS_TO_TICKS(100));

    ESP_RETURN_ON_ERROR(mpu6050_select_clock_source(dev, MPU6050_CLKSEL_PLL_X), TAG, "Failed to select clock source.");

    vTaskDelay(pdMS_TO_TICKS(100));

    ESP_RETURN_ON_ERROR(mpu6050_dlpf_config(dev, MPU6050_DLPF_CFG_98), TAG, "Failed to configure DLPF.");
    
    ESP_RETURN_ON_ERROR(mpu6050_gyro_config(dev, MPU6050_FS_SEL_2000), TAG, "Failed to configure gyroscope.");

    ESP_RETURN_ON_ERROR(mpu6050_accel_config(dev, MPU6050_AFS_SEL_8, MPU6050_ACCEL_HPF_5), TAG, "Failed to configure accelerometer.");

    return ESP_OK;
}

esp_err_t mpu6050_who_am_i(mpu6050_t *dev, uint8_t *who_am_i)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");
    ESP_RETURN_ON_FALSE(who_am_i != NULL, ESP_ERR_INVALID_ARG, TAG, "'who_am_i' is NULL.");

    ESP_RETURN_ON_ERROR(
        read_reg(dev, MPU6050_REG_WHO_AM_I, who_am_i, sizeof(*who_am_i)),
        TAG,
        "Failed to read from MPU6050_REG_WHO_AM_I"
    );

    return ESP_OK;
}

esp_err_t mpu6050_device_reset(mpu6050_t *dev)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");

    ESP_RETURN_ON_ERROR(
        write_reg(dev, MPU6050_REG_PWR_MGMT_1, MPU6050_PWR_MGMT_1_DEVICE_RESET_MASK),
        TAG,
        "Failed to write to MPU6050_REG_PWR_MGMT_1"
    );

    return ESP_OK;
}

esp_err_t mpu6050_set_sleep(mpu6050_t *dev, bool enable)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");
    
    uint8_t buf[1];

    ESP_RETURN_ON_ERROR(read_reg(dev, MPU6050_REG_PWR_MGMT_1, buf, sizeof(buf)), TAG, "Failed to read from MPU6050_REG_PWR_MGMT_1");

    buf[0] &= ~MPU6050_PWR_MGMT_1_SLEEP_MASK;
    buf[0] |= (uint8_t) enable << MPU6050_PWR_MGMT_1_SLEEP_POS;

    ESP_RETURN_ON_ERROR(write_reg(dev, MPU6050_REG_PWR_MGMT_1, buf[0]), TAG, "Failed to write to MPU6050_REG_PWR_MGMT_1");

    return ESP_OK;
}

esp_err_t mpu6050_select_clock_source(mpu6050_t *dev, mpu6050_clksel_t clksel)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");

    uint8_t buf[1];

    ESP_RETURN_ON_ERROR(read_reg(dev, MPU6050_REG_PWR_MGMT_1, buf, sizeof(buf)), TAG, "Failed to read from MPU6050_REG_PWR_MGMT_1");

    buf[0] &= ~MPU6050_PWR_MGMT_1_CLKSEL_MASK;
    buf[0] |= clksel;

    ESP_RETURN_ON_ERROR(write_reg(dev, MPU6050_REG_PWR_MGMT_1, buf[0]), TAG, "Failed to write to MPU6050_REG_PWR_MGMT_1");

    return ESP_OK;
}

esp_err_t mpu6050_set_sample_rate_divider(mpu6050_t *dev, uint8_t divider)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");

    ESP_RETURN_ON_ERROR(write_reg(dev, MPU6050_REG_SMPLRT_DIV, divider), TAG, "Failed to set sample rate divider.");

    return ESP_OK;
}

esp_err_t mpu6050_dlpf_config(mpu6050_t *dev, mpu6050_dlpf_cfg_t dlpf_cfg)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");

    uint8_t buf[1];
    
    ESP_RETURN_ON_ERROR(read_reg(dev, MPU6050_REG_CONFIG, buf, sizeof(buf)), TAG, "Failed to read from MPU6050_REG_CONFIG");

    buf[0] &= ~MPU6050_CONFIG_DLPF_CFG_MASK;
    buf[0] |= dlpf_cfg;

    ESP_RETURN_ON_ERROR(write_reg(dev, MPU6050_REG_CONFIG, buf[0]), TAG, "Failed to write to MPU6050_REG_CONFIG");

    return ESP_OK;
}

esp_err_t mpu6050_gyro_config(mpu6050_t *dev, mpu6050_fs_sel_t fs_sel)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");

    uint8_t buf[1];

    ESP_RETURN_ON_ERROR(read_reg(dev, MPU6050_REG_GYRO_CONFIG, buf, sizeof(buf)), TAG, "Failed to read from MPU6050_REG_GYRO_CONFIG");
    
    buf[0] &= ~MPU6050_GYRO_CONFIG_FS_SEL_MASK;
    buf[0] |= fs_sel << MPU6050_GYRO_CONFIG_FS_SEL_POS;
    
    ESP_RETURN_ON_ERROR(write_reg(dev, MPU6050_REG_GYRO_CONFIG, buf[0]), TAG, "Failed to write to MPU6050_REG_GYRO_CONFIG");

    return ESP_OK;
}

esp_err_t mpu6050_accel_config(mpu6050_t *dev, mpu6050_afs_sel_t afs_sel, mpu6050_accel_hpf_t hpf_sel)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");

    uint8_t buf[1];

    ESP_RETURN_ON_ERROR(read_reg(dev, MPU6050_REG_ACCEL_CONFIG, buf, sizeof(buf)), TAG, "Failed to read from MPU6050_REG_ACCEL_CONFIG");
    
    buf[0] &= ~(MPU6050_ACCEL_CONFIG_AFS_SEL_MASK | MPU6050_ACCEL_CONFIG_ACCEL_HPF_MASK);
    buf[0] |= (afs_sel << MPU6050_ACCEL_CONFIG_AFS_SEL_POS) | hpf_sel;

    ESP_RETURN_ON_ERROR(write_reg(dev, MPU6050_REG_ACCEL_CONFIG, buf[0]), TAG, "Failed to write to MPU6050_REG_ACCEL_CONFIG");

    return ESP_OK;
}

esp_err_t mpu6050_interrupt_config(mpu6050_t *dev, uint8_t int_en_flags)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");

    ESP_RETURN_ON_ERROR(write_reg(dev, MPU6050_REG_INT_ENABLE, int_en_flags), TAG, "Failed to write to MPU6050_REG_INT_ENABLE");

    return ESP_OK;
}

esp_err_t mpu6050_freefall_detect_config(mpu6050_t *dev, mpu6050_ff_cfg_t *ff_cfg)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");
    ESP_RETURN_ON_FALSE(ff_cfg != NULL, ESP_ERR_INVALID_ARG, TAG, "'ff_cfg' is NULL.");

    ESP_RETURN_ON_ERROR(write_reg(dev, MPU6050_REG_FF_THR, ff_cfg->ff_thr), TAG, "Failed to write to MPU6050_REG_FF_THR");
    ESP_RETURN_ON_ERROR(write_reg(dev, MPU6050_REG_FF_DUR, ff_cfg->ff_dur), TAG, "Failed to write to MPU6050_REG_FF_DUR");

    return ESP_OK;
}

esp_err_t mpu6050_motion_detect_config(mpu6050_t *dev, mpu6050_mot_cfg_t *mot_cfg)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");
    ESP_RETURN_ON_FALSE(mot_cfg != NULL, ESP_ERR_INVALID_ARG, TAG, "'mot_cfg' is NULL.");

    ESP_RETURN_ON_ERROR(write_reg(dev, MPU6050_REG_MOT_THR, mot_cfg->mot_thr), TAG, "Failed to write to MPU6050_REG_MOT_THR");
    ESP_RETURN_ON_ERROR(write_reg(dev, MPU6050_REG_MOT_DUR, mot_cfg->mot_dur), TAG, "Failed to write to MPU6050_REG_MOT_DUR");

    return ESP_OK;
}

esp_err_t mpu6050_zero_motion_detect_config(mpu6050_t *dev, mpu6050_zrmot_cfg_t *zrmot_cfg)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");
    ESP_RETURN_ON_FALSE(zrmot_cfg != NULL, ESP_ERR_INVALID_ARG, TAG, "'zrmot_cfg' is NULL.");

    ESP_RETURN_ON_ERROR(write_reg(dev, MPU6050_REG_ZRMOT_THR, zrmot_cfg->zrmot_thr), TAG, "Failed to write to MPU6050_REG_ZRMOT_THR");
    ESP_RETURN_ON_ERROR(write_reg(dev, MPU6050_REG_ZRMOT_DUR, zrmot_cfg->zrmot_dur), TAG, "Failed to write to MPU6050_REG_ZRMOT_DUR");

    return ESP_OK;
}

esp_err_t mpu6050_read_raw_gyro_data(mpu6050_t *dev, mpu6050_raw_gyro_data_t *raw_gyro)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");
    ESP_RETURN_ON_FALSE(raw_gyro != NULL, ESP_ERR_INVALID_ARG, TAG, "raw_gyro is NULL.");
    
    uint8_t buf[6];
    
    ESP_RETURN_ON_ERROR(read_reg(dev, MPU6050_REG_GYRO_XOUT_H, buf, sizeof(buf)), TAG, "Failed to read gyro output");

    // ESP_LOGI(TAG, "GYRO bytes: %02X %02X %02X %02X %02X %02X",
    //      buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
    
    raw_gyro->gyro_xout = (buf[0] << 8) | buf[1];
    raw_gyro->gyro_yout = (buf[2] << 8) | buf[3];
    raw_gyro->gyro_zout = (buf[4] << 8) | buf[5];

    return ESP_OK;
}

esp_err_t mpu6050_read_raw_accel_data(mpu6050_t *dev, mpu6050_raw_accel_data_t *raw_accel)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");
    ESP_RETURN_ON_FALSE(raw_accel != NULL, ESP_ERR_INVALID_ARG, TAG, "'raw_accel' is NULL.");

    uint8_t buf[6];
    
    ESP_RETURN_ON_ERROR(read_reg(dev, MPU6050_REG_ACCEL_XOUT_H, buf, sizeof(buf)), TAG, "Failed to read from MPU6050_REG_ACCEL_XOUT_H");
    
    // ESP_LOGI(TAG, "ACCEL bytes: %02X %02X %02X %02X %02X %02X",
    //      buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);

    raw_accel->accel_xout = (buf[0] << 8) | buf[1];
    raw_accel->accel_yout = (buf[2] << 8) | buf[3];
    raw_accel->accel_zout = (buf[4] << 8) | buf[5];

    return ESP_OK;
}

esp_err_t mpu6050_read_raw_temp_data(mpu6050_t *dev, mpu6050_raw_temp_data_t *raw_temp)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");
    ESP_RETURN_ON_FALSE(raw_temp != NULL, ESP_ERR_INVALID_ARG, TAG, "'raw_temp' is NULL.");

    uint8_t buf[2];
    
    ESP_RETURN_ON_ERROR(read_reg(dev, MPU6050_REG_TEMP_OUT_H, buf, sizeof(buf)), TAG, "Failed to read from MPU6050_REG_TEMP_OUT_H");
    
    // ESP_LOGI(TAG, "TEMP bytes: %02X %02X", buf[0], buf[1]);

    raw_temp->temp_out = (buf[0] << 8) | buf[1];

    return ESP_OK;
}

esp_err_t mpu6050_read_motion_detect_status(mpu6050_t *dev, mpu6050_mot_detect_t *mot_detect_status)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");
    ESP_RETURN_ON_FALSE(mot_detect_status != NULL, ESP_ERR_INVALID_ARG, TAG, "'mot_detect_status' is NULL.");

    uint8_t buf[1];

    ESP_RETURN_ON_ERROR(
        read_reg(dev, MPU6050_REG_MOT_DETECT_STATUS, buf, sizeof(buf)),
        TAG,
        "Failed to read from MPU6050_REG_MOT_DETECT_STATUS"
    );

    mot_detect_status->mot_xneg = (bool) ((buf[0] >> 7) & 0x01);
    mot_detect_status->mot_xpos = (bool) ((buf[0] >> 6) & 0x01);
    mot_detect_status->mot_yneg = (bool) ((buf[0] >> 5) & 0x01);
    mot_detect_status->mot_ypos = (bool) ((buf[0] >> 4) & 0x01);
    mot_detect_status->mot_zneg = (bool) ((buf[0] >> 3) & 0x01);
    mot_detect_status->mot_zpos = (bool) ((buf[0] >> 2) & 0x01);
    mot_detect_status->mot_zrmot = (bool) (buf[0] & 0x01);

    return ESP_OK;
}

esp_err_t mpu6050_read_interrupt_status(mpu6050_t *dev, mpu6050_int_t *int_status)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");
    ESP_RETURN_ON_FALSE(int_status != NULL, ESP_ERR_INVALID_ARG, TAG, "'int_status' is NULL.");

    uint8_t buf[1];

    ESP_RETURN_ON_ERROR(read_reg(dev, MPU6050_REG_INT_STATUS, buf, sizeof(buf)), TAG, "Failed to read from MPU6050_REG_INT_STATUS");

    int_status->int_ff = (bool) (buf[0] >> 7) & 0x01;
    int_status->int_mot = (bool) (buf[0] >> 6) & 0x01;
    int_status->int_zrmot = (bool) (buf[0] >> 5) & 0x01;
    int_status->int_fifo_oflow = (bool) (buf[0] >> 4) & 0x01;
    int_status->int_i2c_mst = (bool) (buf[0] >> 3) & 0x01;
    int_status->int_data_rdy = (bool) buf[0] & 0x01;
    
    return ESP_OK;
}

esp_err_t mpu6050_signal_path_reset(mpu6050_t *dev, bool gyro_reset, bool accel_reset, bool temp_reset)
{
    ESP_RETURN_ON_FALSE(dev != NULL, ESP_ERR_INVALID_ARG, TAG, "MPU6050 device has not been initialized.");

    uint8_t buf = (uint8_t) gyro_reset << 2 | (uint8_t) accel_reset << 1 | (uint8_t) temp_reset;

    ESP_RETURN_ON_ERROR(write_reg(dev, MPU6050_REG_SIGNAL_PATH_RESET, buf), TAG, "Failed to write to MPU6050_REG_SIGNAL_PATH_RESET");

    return ESP_OK;
}
