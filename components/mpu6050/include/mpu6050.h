#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"
#include "driver/i2c_master.h"
#include "mpu6050_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DLPF configuration.
 */
typedef enum {
    MPU6050_DLPF_CFG_256 = 0,   /*!< Gyroscope: 256 Hz, Accelerometer: 260 Hz */
    MPU6050_DLPF_CFG_188 = 1,   /*!< Gyroscope: 188 Hz, Accelerometer: 184 Hz */
    MPU6050_DLPF_CFG_98  = 2,   /*!< Gyroscope: 98 Hz, Accelerometer: 94 Hz */
    MPU6050_DLPF_CFG_42  = 3,   /*!< Gyroscope: 42 Hz, Accelerometer: 44 Hz */
    MPU6050_DLPF_CFG_20  = 4,   /*!< Gyroscope: 20 Hz, Accelerometer: 21 Hz */
    MPU6050_DLPF_CFG_10  = 5,   /*!< Gyroscope: 10 Hz, Accelerometer: 10 Hz */
    MPU6050_DLPF_CFG_5   = 6    /*!< Gyroscope: 5 Hz, Accelerometer: 5 Hz */
} mpu6050_dlpf_cfg_t;

/**
 * @brief Gyroscope full scale range selection.
 */
typedef enum {
    MPU6050_FS_SEL_250  = 0,
    MPU6050_FS_SEL_500  = 1,
    MPU6050_FS_SEL_1000 = 2,
    MPU6050_FS_SEL_2000 = 3
} mpu6050_fs_sel_t;

/**
 * @brief Accelerometer full scale range selection.
 */
typedef enum {
    MPU6050_AFS_SEL_2  = 0,
    MPU6050_AFS_SEL_4  = 1,
    MPU6050_AFS_SEL_8  = 2,
    MPU6050_AFS_SEL_16 = 3
} mpu6050_afs_sel_t;

/**
 * @brief Accelerometer HPF configuration.
 */
typedef enum {
    MPU6050_ACCEL_HPF_RESET = 0,    /*!< Effectively disables the DHPF */
    MPU6050_ACCEL_HPF_5     = 1,    /*!< Cuts off frequencies below 5Hz */
    MPU6050_ACCEL_HPF_2_5   = 2,    /*!< Cuts off frequencies below 2.5Hz */
    MPU6050_ACCEL_HPF_1_25  = 3,    /*!< Cuts off frequencies below 1.25Hz */
    MPU6050_ACCEL_HPF_0_63  = 4,    /*!< Cuts off frequencies below 0.63Hz */
    MPU6050_ACCEL_HPF_HOLD  = 7     /*!< Holds current sample and outputs difference between input and held sample */
} mpu6050_accel_hpf_t;

/**
 * @brief Interrupt enable configuration.
 */
typedef enum {
    MPU6050_INT_EN_FF         = 1 << 7,
    MPU6050_INT_EN_MOT        = 1 << 6,
    MPU6050_INT_EN_ZMOT       = 1 << 5,
    MPU6050_INT_EN_FIFO_OFLOW = 1 << 4,
    MPU6050_INT_EN_I2C_MST    = 1 << 3,
    MPU6050_INT_EN_DATA_RDY   = 1,
} mpu6050_int_en_t;

/**
 * @brief Clock source selection.
 */
typedef enum {
    MPU6050_CLKSEL_8MHZ  = 0,   /*!< Internal 8MHz oscillator */
    MPU6050_CLKSEL_PLL_X = 1,   /*!< Phase-locked loop with x-axis gyroscope reference */
    MPU6050_CLKSEL_PLL_Y = 2,   /*!< Phase-locked loop with y-axis gyroscope reference */
    MPU6050_CLKSEL_PLL_Z = 3,   /*!< Phase-locked loop with z-axis gyroscope reference */
    MPU6050_CLKSEL_RESET = 7    /*!< Stops the clock and keeps the timing generator in reset */
} mpu6050_clksel_t;

/**
 * @brief MPU6050 device information.
 */
typedef struct {
    i2c_master_dev_handle_t dev_handle;
    uint8_t i2c_addr;
} mpu6050_t;
 
/**
 * @brief Raw gyroscope output.
 */
typedef struct {
    int16_t gyro_xout;
    int16_t gyro_yout;
    int16_t gyro_zout;
} mpu6050_raw_gyro_data_t;

/**
 * @brief Raw accelerometer output.
 */
typedef struct {
    int16_t accel_xout;
    int16_t accel_yout;
    int16_t accel_zout;
} mpu6050_raw_accel_data_t;

/**
 * @brief Raw internal temperature output.
 */
typedef struct {
    int16_t temp_out;
} mpu6050_raw_temp_data_t;

/**
 * @brief Motion detection status.
 */
typedef struct {
    bool mot_xneg;
    bool mot_xpos;
    bool mot_yneg;
    bool mot_ypos;
    bool mot_zneg;
    bool mot_zpos;
    bool mot_zrmot;
} mpu6050_mot_detect_t;

/**
 * @brief Interrupt status.
 */
typedef struct {
    bool int_ff;
    bool int_mot;
    bool int_zrmot;
    bool int_fifo_oflow;
    bool int_i2c_mst;
    bool int_data_rdy;
} mpu6050_int_t;

/**
 * @brief Free fall detection configuration.
 */
typedef struct {
    uint8_t ff_thr;
    uint8_t ff_dur;
} mpu6050_ff_cfg_t;

/**
 * @brief Motion detection configuration.
 */
typedef struct {
    uint8_t mot_thr;
    uint8_t mot_dur;
} mpu6050_mot_cfg_t;

/**
 * @brief Zero-motion detection configuration.
 */
typedef struct {
    uint8_t zrmot_thr;
    uint8_t zrmot_dur;
} mpu6050_zrmot_cfg_t;

/**
 * @brief Initialize MPU6050.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[in] bus_handle I2C master bus handle.
 * 
 * @param[in] i2c_addr MPU6050 I2C device address.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_init(mpu6050_t *dev, i2c_master_bus_handle_t bus_handle, uint8_t i2c_addr);

/**
 * @brief Verify device identity.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[out] who_am_i Device I2C address.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_who_am_i(mpu6050_t *dev, uint8_t *who_am_i);

/**
 * @brief Reset MPU6050.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_device_reset(mpu6050_t *dev);

/**
 * @brief Configure sample rate divider.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[in] enable Enable sleep bit.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_set_sleep(mpu6050_t *dev, bool enable);

/**
 * @brief Select clock source.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[in] clksel Clock source selection.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_select_clock_source(mpu6050_t *dev, mpu6050_clksel_t clksel);

/**
 * @brief Configure sample rate divider.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[in] divider Sample rate divider.
 * 
 * Sample Rate = Gyroscope Output Rate / (1 + divider)
 * where Gyroscope Output Rate = 8 kHz when DLPF is disabled
 * and 1 kHz when DLPF is enabled.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_set_sample_rate_divider(mpu6050_t *dev, uint8_t divider);

/**
 * @brief Configure DLPF.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[in] dlpf_cfg Digital LPF configuration.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_dlpf_config(mpu6050_t *dev, mpu6050_dlpf_cfg_t dlpf_cfg);

/**
 * @brief Configure gyroscope.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[in] fs_sel Selection for gyroscope's full scale range.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_gyro_config(mpu6050_t *dev, mpu6050_fs_sel_t fs_sel);

/**
 * @brief Configure accelerometer.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[in] afs_sel Selection of accelerometer's full scale range.
 * 
 * @param[in] hpf_sel Selection of accelerometer's HPF.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_accel_config(mpu6050_t *dev, mpu6050_afs_sel_t afs_sel, mpu6050_accel_hpf_t hpf_sel);

/**
 * @brief Configure interrupt.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[in] int_en_flags Interrupt enable flag configuration.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_interrupt_config(mpu6050_t *dev, uint8_t int_en_flags);

/**
 * @brief Configure free fall detection.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[in] ff_cfg Free fall detection configuration.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_freefall_detect_config(mpu6050_t *dev, mpu6050_ff_cfg_t *ff_cfg);

/**
 * @brief Configure motion detection.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[in] mot_cfg Motion detection configuration.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_motion_detect_config(mpu6050_t *dev, mpu6050_mot_cfg_t *mot_cfg);

/**
 * @brief Configure zero-motion detection.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[in] zrmot_cfg Zero-motion detection configuration.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_zero_motion_detect_config(mpu6050_t *dev, mpu6050_zrmot_cfg_t *zrmot_cfg);

/**
 * @brief Read raw gyroscope data.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[out] raw_gyro Raw gyroscope data.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_read_raw_gyro_data(mpu6050_t *dev, mpu6050_raw_gyro_data_t *raw_gyro);

/**
 * @brief Read raw accelerometer data.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[out] raw_accel Raw accelerometer data.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_read_raw_accel_data(mpu6050_t *dev, mpu6050_raw_accel_data_t *raw_accel);

/**
 * @brief Read raw internal temperature data.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[out] raw_temp Raw internal temperature data.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_read_raw_temp_data(mpu6050_t *dev, mpu6050_raw_temp_data_t *raw_temp);

/**
 * @brief Read motion detection status.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[out] mot_detect_status Motion detection statuses.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_read_motion_detect_status(mpu6050_t *dev, mpu6050_mot_detect_t *mot_detect_status);

/**
 * @brief Read interrupt status.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[out] int_status Interrupt statuses.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_read_interrupt_status(mpu6050_t *dev, mpu6050_int_t *int_status);

/**
 * @brief Reset signal paths.
 * 
 * @param[in] dev MPU6050 device reference.
 * 
 * @param[in] gyro Reset gyroscope path.
 * 
 * @param[in] accel Reset accelerometer path.
 * 
 * @param[in] temp Reset internal temperature path.
 * 
 * @return
 *      - ESP_OK: I2C master transmit-receive success.
 *      - ESP_ERR_INVALID_RESPONSE: I2C master transmit-receive receives NACK.
 *      - ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
 *      - ESP_ERR_TIMEOUT: Operation timeout (larger than xfer_timeout_ms) because the bus is busy or hardware crash.
 */
esp_err_t mpu6050_signal_path_reset(mpu6050_t *dev, bool gyro, bool accel, bool temp);

#ifdef __cplusplus
}
#endif
