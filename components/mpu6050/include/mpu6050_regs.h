#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MPU6050_REG_AUX_VDDIO           0x01    /*!< Auxiliary I2C Supply Selection Register*/
#define MPU6050_REG_SMPLRT_DIV          0x19    /*!< Sample Rate Divider Register */
#define MPU6050_REG_CONFIG              0x1A    /*!< Configuration Register */
#define MPU6050_REG_GYRO_CONFIG         0x1B    /*!< Gyroscope Configuration Register */
#define MPU6050_REG_ACCEL_CONFIG        0x1C    /*!< Accelerometer Configuration Register */
#define MPU6050_REG_FF_THR              0x1D    /*!< Free Fall Acceleration Threshold Register */
#define MPU6050_REG_FF_DUR              0x1E    /*!< Free Fall Duration Register */
#define MPU6050_REG_MOT_THR             0x1F    /*!< Motion Detection Threshold Register */
#define MPU6050_REG_MOT_DUR             0x20    /*!< Motion Detection Duration Register */
#define MPU6050_REG_ZRMOT_THR           0x21    /*!< Zero Motion Detection Threshold Register */
#define MPU6050_REG_ZRMOT_DUR           0x22    /*!< Zero Motion Detection Duration Register */
#define MPU6050_REG_FIFO_EN             0x23    /*!< FIFO Enable Register */
#define MPU6050_REG_I2C_MST_CTRL        0x24    /*!< I2C Master Control Register */

// I2C Slave 0 Registers
#define MPU6050_REG_I2C_SLV0_ADDR       0x25    /*!< I2C Slave 0 Device Address Register */
#define MPU6050_REG_I2C_SLV0_REG        0x26    /*!< I2C Slave 0 Internal Data Register */
#define MPU6050_REG_I2C_SLV0_CTRL       0x27    /*!< I2C Slave 0 Control Register */

// I2C Slave 1 Registers
#define MPU6050_REG_I2C_SLV1_ADDR       0x28    /*!< I2C Slave 1 Device Address Register */
#define MPU6050_REG_I2C_SLV1_REG        0x29    /*!< I2C Slave 1 Internal Data Register */
#define MPU6050_REG_I2C_SLV1_CTRL       0x2A    /*!< I2C Slave 1 Control Register */

// I2C Slave 2 Registers
#define MPU6050_REG_I2C_SLV2_ADDR       0x2B    /*!< I2C Slave 2 Device Address Register */
#define MPU6050_REG_I2C_SLV2_REG        0x2C    /*!< I2C Slave 2 Internal Data Register */
#define MPU6050_REG_I2C_SLV2_CTRL       0x2D    /*!< I2C Slave 2 Control Register */

// I2C Slave 3 Registers
#define MPU6050_REG_I2C_SLV3_ADDR       0x2E    /*!< I2C Slave 3 Device Address Register */
#define MPU6050_REG_I2C_SLV3_REG        0x2F    /*!< I2C Slave 3 Internal Data Register */
#define MPU6050_REG_I2C_SLV3_CTRL       0x30    /*!< I2C Slave 3 Control Register */

// I2C Slave 4 Registers
#define MPU6050_REG_I2C_SLV4_ADDR       0x31    /*!< I2C Slave 4 Device Address Register */
#define MPU6050_REG_I2C_SLV4_REG        0x32    /*!< I2C Slave 4 Internal Data Register */
#define MPU6050_REG_I2C_SLV4_DO         0x33    /*!< I2C Slave 4 Data Out Register */
#define MPU6050_REG_I2C_SLV4_CTRL       0x34    /*!< I2C Slave 4 Control Register */
#define MPU6050_REG_I2C_SLV4_DI         0x35    /*!< I2C Slave 4 Data In Register */

#define MPU6050_REG_I2C_MST_STATUS      0x36    /*!< I2C Master Status Register */
#define MPU6050_REG_INT_PIN_CFG         0x37    /*!< Interrupt Pin/Bypass Enable Configuration Register */
#define MPU6050_REG_INT_ENABLE          0x38    /*!< Interrupt Enable Register */
#define MPU6050_REG_INT_STATUS          0x3A    /*!< Interrupt Status Register */

// Accelerometer Measurement Registers
#define MPU6050_REG_ACCEL_XOUT_H        0x3B    /*!< Accelerometer X-Axis Output High Byte Register */
#define MPU6050_REG_ACCEL_XOUT_L        0x3C    /*!< Accelerometer X-Axis Output Low Byte Register */
#define MPU6050_REG_ACCEL_YOUT_H        0x3D    /*!< Accelerometer Y-Axis Output High Byte Register */
#define MPU6050_REG_ACCEL_YOUT_L        0x3E    /*!< Accelerometer Y-Axis Output Low Byte Register */
#define MPU6050_REG_ACCEL_ZOUT_H        0x3F    /*!< Accelerometer Z-Axis Output High Byte Register */
#define MPU6050_REG_ACCEL_ZOUT_L        0x40    /*!< Accelerometer Z-Axis Output Low Byte Register */

// Internal Temperature Measurement Register
#define MPU6050_REG_TEMP_OUT_H          0x41    /*!< Internal Temperature Output High Byte Register */
#define MPU6050_REG_TEMP_OUT_L          0x42    /*!< Internal Temperature Output Low Byte Register */

// Gyroscope Measurement Registers
#define MPU6050_REG_GYRO_XOUT_H         0x43    /*!< Gyroscope X-Axis Output High Byte Register */
#define MPU6050_REG_GYRO_XOUT_L         0x44    /*!< Gyroscope X-Axis Output Low Byte Register */
#define MPU6050_REG_GYRO_YOUT_H         0x45    /*!< Gyroscope Y-Axis Output High Byte Register */
#define MPU6050_REG_GYRO_YOUT_L         0x46    /*!< Gyroscope Y-Axis Output Low Byte Register */
#define MPU6050_REG_GYRO_ZOUT_H         0x47    /*!< Gyroscope Z-Axis Output High Byte Register */
#define MPU6050_REG_GYRO_ZOUT_L         0x48    /*!< Gyroscope Z-Axis Output Low Byte Register */

// External Sensor Data Registers
#define MPU6050_REG_EXT_SENS_DATA_00    0x49    /*!< External Sensor Data 00 Register */
#define MPU6050_REG_EXT_SENS_DATA_01    0x4A    /*!< External Sensor Data 01 Register */
#define MPU6050_REG_EXT_SENS_DATA_02    0x4B    /*!< External Sensor Data 02 Register */
#define MPU6050_REG_EXT_SENS_DATA_03    0x4C    /*!< External Sensor Data 03 Register */
#define MPU6050_REG_EXT_SENS_DATA_04    0x4D    /*!< External Sensor Data 04 Register */
#define MPU6050_REG_EXT_SENS_DATA_05    0x4E    /*!< External Sensor Data 05 Register */
#define MPU6050_REG_EXT_SENS_DATA_06    0x4F    /*!< External Sensor Data 06 Register */
#define MPU6050_REG_EXT_SENS_DATA_07    0x50    /*!< External Sensor Data 07 Register */
#define MPU6050_REG_EXT_SENS_DATA_08    0x51    /*!< External Sensor Data 08 Register */
#define MPU6050_REG_EXT_SENS_DATA_09    0x52    /*!< External Sensor Data 09 Register */
#define MPU6050_REG_EXT_SENS_DATA_10    0x53    /*!< External Sensor Data 10 Register */
#define MPU6050_REG_EXT_SENS_DATA_11    0x54    /*!< External Sensor Data 11 Register */
#define MPU6050_REG_EXT_SENS_DATA_12    0x55    /*!< External Sensor Data 12 Register */
#define MPU6050_REG_EXT_SENS_DATA_13    0x56    /*!< External Sensor Data 13 Register */
#define MPU6050_REG_EXT_SENS_DATA_14    0x57    /*!< External Sensor Data 14 Register */
#define MPU6050_REG_EXT_SENS_DATA_15    0x58    /*!< External Sensor Data 15 Register */
#define MPU6050_REG_EXT_SENS_DATA_16    0x59    /*!< External Sensor Data 16 Register */
#define MPU6050_REG_EXT_SENS_DATA_17    0x5A    /*!< External Sensor Data 17 Register */
#define MPU6050_REG_EXT_SENS_DATA_18    0x5B    /*!< External Sensor Data 18 Register */
#define MPU6050_REG_EXT_SENS_DATA_19    0x5C    /*!< External Sensor Data 19 Register */
#define MPU6050_REG_EXT_SENS_DATA_20    0x5D    /*!< External Sensor Data 20 Register */
#define MPU6050_REG_EXT_SENS_DATA_21    0x5E    /*!< External Sensor Data 21 Register */
#define MPU6050_REG_EXT_SENS_DATA_22    0x5F    /*!< External Sensor Data 22 Register */
#define MPU6050_REG_EXT_SENS_DATA_23    0x60    /*!< External Sensor Data 23 Register */

#define MPU6050_REG_MOT_DETECT_STATUS   0x61    /*!< Motion Detection Status Register */
#define MPU6050_REG_I2C_SLV0_DO         0x63    /*!< I2C Slave 0 Data Out Register */
#define MPU6050_REG_I2C_SLV1_DO         0x64    /*!< I2C Slave 1 Data Out Register */
#define MPU6050_REG_I2C_SLV2_DO         0x65    /*!< I2C Slave 2 Data Out Register */
#define MPU6050_REG_I2C_SLV3_DO         0x66    /*!< I2C Slave 3 Data Out Register */
#define MPU6050_REG_I2C_MST_DELAY_CTRL  0x67    /*!< I2C Master Delay Control Register */
#define MPU6050_REG_SIGNAL_PATH_RESET   0x68    /*!< Signal Path Reset Register */
#define MPU6050_REG_MOT_DETECT_CTRL     0x69    /*!< Motion Detection Control Register */
#define MPU6050_REG_USER_CTRL           0x6A    /*!< User Control Register */
#define MPU6050_REG_PWR_MGMT_1          0x6B    /*!< Power Management 1 Register */
#define MPU6050_REG_PWR_MGMT_2          0x6C    /*!< Power Management 2 Register */

// FIFO Count Registers
#define MPU6050_REG_FIFO_COUNT_H        0x72    /*!< FIFO Count Output High Byte Register */
#define MPU6050_REG_FIFO_COUNT_L        0x73    /*!< FIFO Count Output Low Byte Register */

#define MPU6050_REG_FIFO_R_W            0x74    /*!< FIFO READ/WRITE Register */
#define MPU6050_REG_WHO_AM_I            0x75    /*!< Who Am I Register */

// CONFIG Masks
#define MPU6050_CONFIG_EXT_SYNC_SET_MASK        0x38
#define MPU6050_CONFIG_EXT_SYNC_SET_POS         3
#define MPU6050_CONFIG_DLPF_CFG_MASK            0x07

// GYRO_CONFIG Mask 
#define MPU6050_GYRO_CONFIG_FS_SEL_MASK         0x18
#define MPU6050_GYRO_CONFIG_FS_SEL_POS          3

// ACCEL_CONFIG Masks   
#define MPU6050_ACCEL_CONFIG_AFS_SEL_MASK       0x18
#define MPU6050_ACCEL_CONFIG_AFS_SEL_POS        3
#define MPU6050_ACCEL_CONFIG_ACCEL_HPF_MASK     0x07

// PWR_MGMT_1 Mask
#define MPU6050_PWR_MGMT_1_DEVICE_RESET_MASK    0x80
#define MPU6050_PWR_MGMT_1_DEVICE_RESET_POS     7
#define MPU6050_PWR_MGMT_1_SLEEP_MASK           0x40
#define MPU6050_PWR_MGMT_1_SLEEP_POS            6
#define MPU6050_PWR_MGMT_1_CYCLE_MASK           0x20
#define MPU6050_PWR_MGMT_1_CYCLE_POS            5
#define MPU6050_PWR_MGMT_1_TEMP_DIS_MASK        0x08
#define MPU6050_PWR_MGMT_1_TEMP_DIS_POS         3
#define MPU6050_PWR_MGMT_1_CLKSEL_MASK          0x07

#ifdef __cplusplus
}
#endif
