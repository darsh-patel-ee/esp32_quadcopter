# MPU6050 Driver

A basic MPU6050 driver component for ESP-IDF that reads raw accelerometer, gyroscope, and temperature data over I2C and supports basic device configuration and motion detection.

## Features

- I2C communication
- WHO_AM_I detection
- Sleep/wake control
- Register configuration
- Raw accel/gyro/temp reads
- Motion detection

## Hardware Requirements

- MPU6050 breakout
- ESP32 board
- I2C pins
- 3.3V supply

## File Layout

```text
components/mpu6050/
├── docs/
│   ├── MPU-6050_Product_Specification.pdf
│   └── MPU-6050_Register_Map_and_Descriptions.pdf
├── include/
│   ├── mpu6050_regs.h
│   └── mpu6050.h
├── src/
│   └── mpu6050.c
├── CMakeLists.txt
└── README.md
```

## API Reference / Usage

Example initialization:

```c
mpu6050_t mpu6050;

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
```

## Known Limitations

- No FIFO buffer support yet.
- No auxiliary slave support yet.
- No data filtering or attitude estimation yet.

## Version History

### [v0.1.0] - 2026-07-31
- Added initial functional implementation for MPU6050 sensor data reading.
- Added basic I2C communication setup and raw accelerometer/gyroscope output.

## Future Work

- Portable core driver
- ESP-IDF wrapper
- Attitude estimation
- PID integration

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
