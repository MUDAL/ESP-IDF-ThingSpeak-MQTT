#ifndef BME280_H
#define BME280_H

//Sensor data readout struct
typedef struct bme280
{
  uint8_t temperature; 
  uint8_t humidity;
}bme280_data_t;

/**
 * @brief Initialize BME280 sensor using custom settings 
 * (check the parameters below)
 * 
 * @param i2c_port I2C port for communicating with the sensor.
 * i2c_port could be 0 or 1.
 * @param sda_pin GPIO pin to be configured as SDA line
 * @param scl_pin GPIO pin to be configured as SCL line
 * @param enable_pullup If true, pull-ups are enabled for SDA and SCL
 * @param clk_freq Speed of I2C communication
 */
void bme280_custom_init(uint8_t i2c_port,
                        uint8_t sda_pin,uint8_t scl_pin,
                        bool enable_pullup,uint32_t clk_freq);
/**
 * @brief Initialize BME280 sensor using default settings.
 * Default settings:
 * 1. SDA pin = 18
 * 2. SCL pin = 19
 * 3. CLOCK SPEED = 100kHz
 * 4. No pull-ups on the SDA and SCL lines
 * 5. I2C port 0
 */                        
void bme280_init(void);
/**
 * @brief Get data from BME280 sensor.
 * The calculations in this function were obtained from the sensor's
 * datasheet.
 * @param sensor_data pointer to struct that will receive
 * the processed values of the measured humidity and temperature.
 */
void bme280_get_data(bme280_data_t* sensor_data);

#endif